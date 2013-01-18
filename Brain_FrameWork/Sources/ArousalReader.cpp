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

ArousalReader::ArousalReader(Algorithm* postProcess, Algorithm* preProcess) : 
						secs(0), _postProcessingAlgorithm(postProcess),
						_postProcessingAlgorithm(preProcess) {
	_fftSum= *new vector<double>;
	___readyToCollect=false;
}

void ArousalReader::initialiseReading(int option){

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
			___readyToCollect = true;
			cout<<"Ready to collect set to true"<<endl;
		}
	}

	if(__readyToCollect){

		EE_DataUpdateHandle(0, hData);
		unsigned int nSamplesTaken=0;
		EE_DataGetNumberOfSample(hData,&nSamplesTaken);
		EE_DataSetBufferSizeInSec(secs);

		if (nSamplesTaken != 0) {

			double* currentSample = new double[nSamplesTaken];

			for(int i=0;i<nSamplesTaken;++i){
				vector<double> result;
				for (int chan = _indexFirstChannel ; chan<= _indexLastChannel ; chan++) {//pour chaque capteur

					EE_DataGet(hData, targetChannelList[chan], currentSample, nSamplesTaken);
					result.push_back(currentSample[i]);
				}
				_rawData.push_back(result);
			}

			if(_rawData.size()>=_samplingRate){
				_lastRawData = *new vector<vector<double>>(_rawData);
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

		void normalize(double* array);
		void printArrayToFile(std::string file, double* array);
		std::vector<double> getBetaWavesFromChannel(int channelIndex);
		std::vector<double> getAlphaWavesFromChannel(int channelIndex);
		std::vector<double> getFrequenciesFromChannel(int channelIndex);
		std::vector<double> getRawDataFromChannel(int channelIndex);
		std::vector<double> getFrequenciesRangeFromChannel(int begin, int end, int channelIndex);
		std::map<int, std::string> getChannelList();