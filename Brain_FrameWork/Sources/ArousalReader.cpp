#include "ArousalReader.h"
#include <cmath>
#include <iostream>
#include <sstream>
#include <fstream>

using namespace std;

map<int, string> ArousalReader::_channelList;

EE_DataChannel_t targetChannelList[] = {
		ED_COUNTER,
		ED_AF3, ED_F7, ED_F3, ED_FC5, ED_T7, 
		ED_P7, ED_O1, ED_O2, ED_P8, ED_T8, 
		ED_FC6, ED_F4, ED_F8, ED_AF4, ED_GYROX, ED_GYROY, ED_TIMESTAMP, 
		ED_FUNC_ID, ED_FUNC_VALUE, ED_MARKER, ED_SYNC_SIGNAL
	};

ArousalReader::ArousalReader(Algorithm* postProcess, Algorithm* preProcess) : 
						secs(0), _postProcessingAlgorithm(postProcess),
						_preProcessingAlgorithm(preProcess) {
	_readyToCollect=false;
}

void ArousalReader::initialiseReading(int option){

	initialiseChannelList();
	eEvent = EE_EmoEngineEventCreate();
	eState = EE_EmoStateCreate();
	const unsigned short composerPort	= 1726;
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
			throw ArousalReader::InitialisationFailureException();
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

	if(EE_EngineGetNextEvent(eEvent)==EDK_OK){

		EE_Event_t eventType = EE_EmoEngineEventGetType(eEvent);
		EE_EmoEngineEventGetUserId(eEvent, &userID);

		// Log the EmoState if it has been updated
		if (eventType == EE_UserAdded) {
			EE_DataAcquisitionEnable(userID,true);
			_readyToCollect = true;
			cout<<"Ready to collect set to true"<<endl;
		}
	}

	if(_readyToCollect){

		EE_DataUpdateHandle(0, hData);
		unsigned int nSamplesTaken=0;
		EE_DataGetNumberOfSample(hData,&nSamplesTaken);
		EE_DataSetBufferSizeInSec(secs);

		if (nSamplesTaken != 0) {

			double* currentSample = new double[nSamplesTaken];

			for(int i=0;i<nSamplesTaken;++i){
				vector<double> result;
				for (int chan = 0; chan< sizeof(targetChannelList)/sizeof(EE_DataChannel_t); chan++) {//pour chaque capteur

					EE_DataGet(hData, targetChannelList[chan], currentSample, nSamplesTaken);
					result.push_back(currentSample[i]);
				}
				_rawData.push_back(result);
			}

			if(_rawData.size()>=ArousalReader::samplingRate){
				_lastRawData.clear();
				_lastRawData=*new vector< vector<double> >(_rawData);
				_rawData.clear();
				return true;
			}

			delete[] currentSample;
		} else {
			throw ArousalReader::NoSampleFoundException();
		}
	}
	return false;
}

ArousalReader::~ArousalReader(){
	delete &_rawData;
	delete &_lastRawData;
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
		array[i]=0.0f;
	}
}

void ArousalReader::normalize(double* array, int size){
	double med = 0;
	for(int i=0; i<size; ++i){
		med+=array[i];
	}
	med=med/size;
	for(int i=0; i<size; ++i){
		array[i]-=med;
	}
}

void ArousalReader::printArrayToFile(string file, double* array, int size){
	ofstream ofs(file.c_str(),ios::app);
	for(int i=0; i<size; ++i){
		ofs<<array[i]<<",";
	}
	ofs<<endl;
}

vector<double> ArousalReader::getBetaWavesFromChannel(int channelIndex){

	int startBeta=14;//15Hz
	int endBeta=30;//30Hz
	vector<double> waveVector=getFrequenciesRangedFromChannel(startBeta, endBeta, channelIndex);
	normalize(&waveVector[0],endBeta - startBeta);
	return waveVector;
}

vector<double> ArousalReader::getAlphaWavesFromChannel(int channelIndex){

	int startAlpha=7;//8Hz
	int endAlpha=13;//13Hz
	vector<double> waveVector=getFrequenciesRangedFromChannel(startAlpha, endAlpha, channelIndex);
	normalize(&waveVector[0],endAlpha - startAlpha);
	return waveVector;
}

vector<double> ArousalReader::getFrequenciesFromChannel(int channelIndex){

	if(_lastRawData.size()!=samplingRate){
		throw ArousalReader::NoDataReadException();
	}

	vector<double> temp;

	double * imaginaryArray=new double[samplingRate];
	double* freq=new double[samplingRate];

	initialiseArray(imaginaryArray, samplingRate);
	initialiseArray(freq, samplingRate);

	for(int i=0; i<_lastRawData.size(); ++i){
		temp.push_back(_lastRawData[i][channelIndex]);
	}
	_postProcessingAlgorithm->process(&temp[0],imaginaryArray, freq, samplingRate);
	cout<<"OMG, it's spiderman !";
	temp.assign(freq, freq+samplingRate);

	normalize(&temp[0], temp.size());

	delete[] imaginaryArray;
	delete[] freq;
	return temp;
}

vector<double> ArousalReader::getRawDataFromChannel(int channelIndex){

	if(_lastRawData.size()==0){
		throw ArousalReader::NoDataReadException();
	}

	vector<double> data;
	for(int i=0; i<_lastRawData.size(); ++i){
		data.push_back(_lastRawData[i][channelIndex]);
	}

	return data;
}

vector<double> ArousalReader::getFrequenciesRangedFromChannel(int begin, int end, int channelIndex){

	vector<double> freqRange;
	vector<double> freq = getFrequenciesFromChannel(channelIndex);
	freqRange.assign(freq.begin()+begin, freq.begin()+end);

	normalize(&freqRange[0], freqRange.size());

	//delete &freq;
	return freqRange;
}

map<int, string> ArousalReader::getChannelList(){
	return _channelList;
}

void ArousalReader::initialiseChannelList(){
	_channelList[0]= "ED_COUNTER";
	_channelList[1]= "ED_AF3";
	_channelList[2]= "ED_F7";
	_channelList[3]= "ED_F3";
	_channelList[4]= "ED_FC5";
	_channelList[5]= "ED_T7";
	_channelList[6]= "ED_P7";
	_channelList[7]= "ED_O1";
	_channelList[8]= "ED_O2";
	_channelList[9]= "ED_P8";
	_channelList[10]= "ED_T8"; 
	_channelList[11]= "ED_FC6";
	_channelList[12]= "ED_F4";
	_channelList[13]= "ED_F8";
	_channelList[14]= "ED_AF4";
	_channelList[15]= "ED_GYROX";
	_channelList[16]= "ED_GYROY";
	_channelList[17]= "ED_TIMESTAMP";
	_channelList[18]= "ED_FUNC_ID";
	_channelList[19]= "ED_FUNC_VALUE";
	_channelList[20]= "ED_MARKER";
	_channelList[21]= "ED_SYNC_SIGNAL";
}