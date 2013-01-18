#include "ArousalReader.h"
#include <cmath>
#include <iostream>
#include <sstream>
#include <fstream>

using namespace std;

EE_DataChannel_t targetChannelList[] = {
		ED_COUNTER,
		ED_AF3, ED_F7, ED_F3, ED_FC5, ED_T7, 
		ED_P7, ED_O1, ED_O2, ED_P8, ED_T8, 
		ED_FC6, ED_F4, ED_F8, ED_AF4, ED_GYROX, ED_GYROY, ED_TIMESTAMP, 
		ED_FUNC_ID, ED_FUNC_VALUE, ED_MARKER, ED_SYNC_SIGNAL
	};

ArousalReader::ArousalReader() : secs(0){
	_fftSum= *new vector<double>;
	readytocollect=false;
}

void ArousalReader::initialiseReading(){

	eEvent = EE_EmoEngineEventCreate();
	eState = EE_EmoStateCreate();
	const unsigned short composerPort	= 1726;
	int option=1;
	switch (option) {
		case 1:
		{
			if (EE_EngineConnect() != EDK_OK) {
				cout <<"Emotiv Engine start up failed"<<endl;
				throw ArousalReader::EmotivConnectException();
			}
			else
			{
				cout<<"Emotiv Engine start up sucssessful."<<endl;
			}
			break;
		}
		case 2:
		{
			cout << "Target IP of EmoComposer? [127.0.0.1] "<<endl;
			string input("127.0.0.1");

			if (EE_EngineRemoteConnect(input.c_str(), composerPort) != EDK_OK) {
				string errMsg = "Cannot connect to EmoComposer on [" + input + "]";
				cout<< errMsg.c_str();
			}
			break;
		}
		default:
			cout<< "Invalid option..."<<endl;
			break;
	}


    hData = EE_DataCreate();
	EE_DataSetBufferSizeInSec(secs);

	cout<<"reading initialise"<<endl;
}

void ArousalReader::endReading(){
	EE_DataFree(hData);
	EE_EngineDisconnect();
	EE_EmoStateFree(eState);
	EE_EmoEngineEventFree(eEvent);
}

bool ArousalReader::readNextFrequencies(){

	//cout<<"Starting to read";
	if(EE_EngineGetNextEvent(eEvent)==EDK_OK){

		//cout<<"EDK OK"<<endl;
		EE_Event_t eventType = EE_EmoEngineEventGetType(eEvent);
		EE_EmoEngineEventGetUserId(eEvent, &userID);

		// Log the EmoState if it has been updated
		if (eventType == EE_UserAdded) {
			EE_DataAcquisitionEnable(userID,true);
			readytocollect = true;
			cout<<"Ready to collect set to true"<<endl;
		}
	}

	if(readytocollect){
		//cout<<"in the if"<<endl;
		EE_DataUpdateHandle(0, hData);
		//cout<<"test"<<endl;
		unsigned int nSamplesTaken=0;
		EE_DataGetNumberOfSample(hData,&nSamplesTaken);
		EE_DataSetBufferSizeInSec(secs);

		//cout<<"Buffer size in sec "<<secs<<endl;

		//cout<<"Collecting started"<<endl;

		if (nSamplesTaken != 0) {

			//cout<<"Reading "<<nSamplesTaken<<" samples"<<endl;

			double* currentSample = new double[nSamplesTaken];
			

			double* result=new double[nSamplesTaken];
			initialiseArray(result, nSamplesTaken);

			for (int i = _indexFirstChannel ; i<= _indexLastChannel ; i++) {//pour chaque capteur

				EE_DataGet(hData, targetChannelList[i], currentSample, nSamplesTaken);
				processData(currentSample, result, nSamplesTaken);
			}

			//push the new samples into the fftSum
			for(int i=0;i<nSamplesTaken;++i){
				_fftSum.push_back(result[i]);
			}

			if(_fftSum.size()>=_samplingRate){

				/*int newSize=FFT::closestTwoPower(_fftSum.size());
				cout<<"newSize "<<newSize<<endl;
				imaginaryStuff=new double[newSize];
				cout<<"Fourrier starting"<<endl;
				FFT fourrier(newSize);*/
				//*** &_fftSum[0]=>double array from the vector ***//
				/*fourrier.fft(&_fftSum[0], imaginaryStuff);
				cout<<"Fourrier done"<<endl;

				printArray(&_fftSum[0], newSize);
				printArray(imaginaryStuff, newSize);

				delete[] imaginaryStuff;*/
				return lookForWaves();
			}

			delete[] currentSample;
			delete[] result;
		} else {
			throw ArousalReader::NoSampleFoundException();
		}
	}
	return false;
}

void ArousalReader::processData( double* channelInput, double* result, int size ){
	for(int i=0; i<size;++i){
		result[i]+=channelInput[i];
	}
}

void ArousalReader::cleanFftSum(){
	_fftSum.clear();
	_fftSum = *new vector<double>();
}

bool ArousalReader::lookForWaves(){

	double* imaginaryStuff;
	int betaFirstIndex=15;
	int betaLastIndex=29;
	int alpahFirstIndex=7;
	int alphaLastIndex=14;
	double betaSumRe=0;
	double betaSumIr=0;

	double alphaSumRe=0;
	double alphaSumIr=0;

	int newSize=FFT::closestTwoPower(_fftSum.size());

	cout<<"newSize "<<newSize<<endl;
	imaginaryStuff=new double[newSize];
	initialiseArray(imaginaryStuff, newSize);

	cout<<"Fourrier starting"<<endl;
	FFT fourrier(newSize);
	fourrier.fft(&_fftSum[0], imaginaryStuff);

	cout<<"Fourrier done"<<endl;

	for(int i=betaFirstIndex; i<=betaLastIndex; ++i){
		betaSumRe+=abs(_fftSum[i]);
		betaSumIr+=abs(imaginaryStuff[i]);
	}

	for(int i=betaFirstIndex; i<=betaLastIndex; ++i){
		alphaSumRe+=abs(_fftSum[i]);
		alphaSumIr+=abs(imaginaryStuff[i]);
	}

	cout<<"Beta SumRe "<<betaSumRe<<endl;
	cout<<"Beta SumIr "<<betaSumIr<<endl;
	cout<<"Alpha SumRe "<<alphaSumRe<<endl;
	cout<<"Alpha SumIr "<<alphaSumIr<<endl;
	ofstream ofs("res.cvs", ios::app);
	ofs<<betaSumRe<<","<<betaSumIr<<","<<alphaSumRe<<","<<alphaSumIr<<endl;
	ofs.close();
	delete[] imaginaryStuff;
	cleanFftSum();

	return false;
}

ArousalReader::~ArousalReader(){
	delete &_fftSum;
}

void ArousalReader::printArray(double* array, int size){
	cout<<"[";
	for(int i=0;i<size;++i){
		cout<<array[i]<<", ";
	}
	cout<<"]"<<endl;
}

void ArousalReader::initialiseArray(double* array, int size){
		for(int i=0;i<size;++i){
		array[i]=0.0;
	}
}