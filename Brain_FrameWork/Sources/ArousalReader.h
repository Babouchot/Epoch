#ifndef Arousal_READER_H
#define Arousal_READER_H

#include "EmoStateDLL.h"
#include "edk.h"
#include "edkErrorCode.h"
#include "fft.h"
#include <string>
#include <iostream>
#include <vector>

class ArousalReader {

	private :

		std::vector<double> _fftSum;
		EmoEngineEventHandle eEvent;
		EmoStateHandle eState;
		DataHandle hData;
		float secs;
		unsigned int userID;
		bool readytocollect;
		static const unsigned int _samplingRate = 128; //Hz
		static const unsigned int _indexFirstChannel=1;
		static const unsigned int _indexLastChannel=14;
//	AF3,F7,F3, FC5, T7, P7, O1, O2,P8, T8, FC6, F4,F8, AF4 are the wanted channels
// Available channels are :
/*		ED_COUNTER,
		ED_AF3, ED_F7, ED_F3, ED_FC5, ED_T7, 
		ED_P7, ED_O1, ED_O2, ED_P8, ED_T8, 
		ED_FC6, ED_F4, ED_F8, ED_AF4, ED_GYROX, ED_GYROY, ED_TIMESTAMP, 
		ED_FUNC_ID, ED_FUNC_VALUE, ED_MARKER, ED_SYNC_SIGNAL
*/
		void printArray(double* array, int size);
		void initialiseArray(double* array, int size);

	public:

		ArousalReader();
		void initialiseReading();
		void endReading();

		bool readNextFrequencies();

		void processData( double* channelInput, double* result);
		void cleanFftSum();
		bool lookForWaves();
		~ArousalReader();

		class EmotivConnectException{};
		class NoSampleFoundException{};
};

#endif