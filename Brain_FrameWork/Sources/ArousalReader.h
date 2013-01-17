#ifndef Arousal_READER_H
#define Arousal_READER_H

#include "EmoStateDLL.h"
#include "edk.h"
#include "edkErrorCode.h"
#include "fft.h"

class ArousalReader {

	private :

		double* _fftSum;
		EmoEngineEventHandle eEvent;
		EmoStateHandle eState;
		DataHandle hData;
		unsigned int userID;
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

	public:

		ArousalReader();
		void initialiseReading();
		void endReading();

		/*
		* Read the next piece of data for each channel, process it and return the fftSum or NULL if there is nothing to return
		* raise a NoSampleFoundException if no sample are to be read
		*/
		double* readNextFrequencies();

		/*
		* FFT the data and add it to the fftSum
		*/
		void processData( double* channelInput);
		void cleanFftSum(int nbSample);
		~ArousalReader();

		class EmotivConnectException{};
		class NoSampleFoundException{};
};

#endif