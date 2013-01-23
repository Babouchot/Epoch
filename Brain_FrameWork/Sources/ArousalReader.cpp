#include "ArousalReader.h"
#include <unistd.h>
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

////////////////////functions////////////////////

unsigned int channelSize(){
	return sizeof(targetChannelList)/sizeof(EE_DataChannel_t);
}

/////////////////member functions//////////////////

ArousalReader::ArousalReader(Algorithm* postProcess, Algorithm* preProcess) : 
						_postProcessingAlgorithm(postProcess),
						_preProcessingAlgorithm(preProcess),
						_normalizationAlgorithm(NULL),
						_readyToCollect(false),
						secs(0), userID(0), _lastCounter(0) {
	if(_channelList.size()==0){
		initialiseChannelList();
	}
}

ArousalReader::ArousalReader(Algorithm* postProcess, Algorithm* preProcess, Algorithm* normalize) : 
						_postProcessingAlgorithm(postProcess),
						_preProcessingAlgorithm(preProcess),
						_normalizationAlgorithm(normalize),
						_readyToCollect(false),
						secs(0), userID(0), _lastCounter(0)  {
	if(_channelList.size()==0){
		initialiseChannelList();
	}
}

ArousalReader::~ArousalReader(){
	_rawData.clear();
	_lastRawData.clear();
	//delete &_rawData;
	//delete &_lastRawData;
}

void ArousalReader::initialiseReading(){

	initialiseChannelList();
	eEvent = EE_EmoEngineEventCreate();
	eState = EE_EmoStateCreate();

	if (EE_EngineConnect() != EDK_OK) {
		cout <<"Emotiv Engine start up failed"<<endl;
		throw ArousalReader::EmotivConnectException();
	}
	else
	{
		cout<<"Emotiv Engine start up sucssessful."<<endl;
	}

    hData = EE_DataCreate();
	EE_DataSetBufferSizeInSec(secs);

	cout<<"reading initialise"<<endl;
}

void ArousalReader::endReading(){
	if(_rawData.size()!=0){
		EE_DataFree(hData);
		EE_EngineDisconnect();
		EE_EmoStateFree(eState);
		EE_EmoEngineEventFree(eEvent);
	}
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
		vector<double> result;
		if (nSamplesTaken != 0) {

			double* currentSample = new double[nSamplesTaken];
			//Go through each acquired sample
			for(unsigned int i=0;i<nSamplesTaken;++i){
				//Go through each channel
				for (unsigned int chan = 0; chan<channelSize(); chan++) {//pour chaque capteur
					//Save the data for the current sample and the current channel
					EE_DataGet(hData, targetChannelList[chan], currentSample, nSamplesTaken);
					result.push_back(currentSample[i]);
				}
				_rawData.push_back(result);
				result.clear();
			}

			//If we have enough data (one second worth of sample)
			if(_rawData.size()>=ArousalReader::samplingRate){
				
				if(_lastCounter!=0 && _rawData[_rawData.size()-1][0]!=_lastCounter-1) {
					//packet loss treatment
					throw PacketLostException();
				}

				_lastCounter=_rawData[_rawData.size()-1][0];

				cout<<"time stamp "<<_rawData[_rawData.size()-1][17]<<"\n";
				
				//set the new _lastRawData
				_lastRawData.clear();
				_lastRawData=_rawData;

				//Applay pre processing
				if( _preProcessingAlgorithm!=NULL){
					_lastRawData=_preProcessingAlgorithm->process(_lastRawData);
				}

				//Apply post processing (fft or other similar algorithm)
				_lastFrequencies =_postProcessingAlgorithm->process(_lastRawData);

				//Apply normalization or post fft treatment algorithm
				if(_normalizationAlgorithm!=NULL){
					_lastFrequencies = _normalizationAlgorithm->process(_lastRawData);
				}

				_rawData.clear();
				delete[] currentSample;
				return true;
			}

			delete[] currentSample;
		}
	}

	usleep(1000/150);

	return false;
}

void ArousalReader::printArrayToFile(string file, double* array, int size){
	ofstream ofs(file.c_str(),ios::app);
	for(int i=0; i<size; ++i){
		ofs<<array[i]<<",";
	}
	ofs<<endl;
	ofs.close();
}

vector<double> ArousalReader::getBetaWavesFromChannel(int channelIndex){

	vector<double> waveVector=getFrequenciesRangedFromChannel(startBeta, endBeta, channelIndex);
	return waveVector;
}

vector<double> ArousalReader::getAlphaWavesFromChannel(int channelIndex){

	vector<double> waveVector=getFrequenciesRangedFromChannel(startAlpha, endAlpha, channelIndex);
	return waveVector;
}

vector<double> ArousalReader::getFrequenciesFromChannel(int channelIndex){

	if(_lastRawData.size()!=samplingRate){
		throw ArousalReader::NoDataReadException();
	}

	vector<double> freq;

	for(unsigned int i=0; i<_lastFrequencies.size(); ++i){
		freq.push_back(_lastFrequencies[i][channelIndex]);
	}

	return freq;
}

vector<double> ArousalReader::getRawDataFromChannel(int channelIndex){

	if(_lastRawData.size()==0){
		throw ArousalReader::NoDataReadException();
	}

	vector<double> data;
	for(unsigned int i=0; i<_lastRawData.size(); ++i){
		data.push_back(_lastRawData[i][channelIndex]);
	}

	return data;
}

vector<double> ArousalReader::getFrequenciesRangedFromChannel(int begin, int end, int channelIndex){

	vector<double> freqRange;
	vector<double> freq = getFrequenciesFromChannel(channelIndex);
	freqRange.assign(freq.begin()+begin, freq.begin()+end);

	freq.clear();
	return freqRange;
}

map<int, string> ArousalReader::getChannelMap(){
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
