#include "ArousalReader.h"

EE_DataChannel_t targetChannelList[] = {
		ED_COUNTER,
		ED_AF3, ED_F7, ED_F3, ED_FC5, ED_T7, 
		ED_P7, ED_O1, ED_O2, ED_P8, ED_T8, 
		ED_FC6, ED_F4, ED_F8, ED_AF4, ED_GYROX, ED_GYROY, ED_TIMESTAMP, 
		ED_FUNC_ID, ED_FUNC_VALUE, ED_MARKER, ED_SYNC_SIGNAL
	};

ArousalReader::ArousalReader(){
	_fftSum= new double[10];
}

void ArousalReader::initialiseReading(){

	EmoEngineEventHandle eEvent			= EE_EmoEngineEventCreate();
	EmoStateHandle eState				= EE_EmoStateCreate();

	if (EE_EngineConnect() != EDK_OK) {
        throw ArousalReader::EmotivConnectException();
	}

	float secs=0.0f;

    DataHandle hData = EE_DataCreate();
	EE_DataSetBufferSizeInSec(secs);
}

void ArousalReader::endReading(){
	EE_DataFree(hData);
	EE_EngineDisconnect();
	EE_EmoStateFree(eState);
	EE_EmoEngineEventFree(eEvent);
}

double* ArousalReader::readNextFrequencies(){

	bool readytocollect;

	if(EE_EngineGetNextEvent(eEvent)==EDK_OK){

		EE_Event_t eventType = EE_EmoEngineEventGetType(eEvent);
		EE_EmoEngineEventGetUserId(eEvent, &userID);

		// Log the EmoState if it has been updated
		if (eventType == EE_UserAdded) {
			EE_DataAcquisitionEnable(userID,true);
			readytocollect = true;
		}
	}

	if(readytocollect){
		EE_DataUpdateHandle(0, hData);

		unsigned int nSamplesTaken=0;
		float secs1=0;
		EE_DataGetNumberOfSample(hData,&nSamplesTaken);
		EE_DataSetBufferSizeInSec(secs1);


		if (nSamplesTaken != 0) {

			double* currentSample = new double[nSamplesTaken];
			double* imaginaryStuff;
			//reinit the _fftSum
			cleanFftSum(nSamplesTaken);

			for (int i = _indexFirstChannel ; i<= _indexLastChannel ; i++) {//pour chaque capteur

				EE_DataGet(hData, targetChannelList[i], currentSample, nSamplesTaken);
				processData(currentSample);
			}

			int newSize=FFT::closestTwoPower(nSamplesTaken);
			imaginaryStuff=new double[newSize];
			//currentSample+newSize=NULL;
			FFT fourrier(newSize);
			fourrier.fft(currentSample, imaginaryStuff);
			//TODO stuff : using the arrays
			delete[] currentSample;
		} else {
			throw ArousalReader::NoSampleFoundException();
		}

		return _fftSum;
	}
	return NULL;
}

void ArousalReader::processData( double* channelInput ){
	for(int i=0; i<sizeof(channelInput)/sizeof(double);++i){
		_fftSum[i]+=channelInput[i];
	}
}

void ArousalReader::cleanFftSum( int nbSample ){
	_fftSum = new double[nbSample];
}

ArousalReader::~ArousalReader(){
	delete[] _fftSum;
}