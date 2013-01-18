#ifndef Arousal_READER_H
#define Arousal_READER_H

#include "EmoStateDLL.h"
#include "edk.h"
#include "edkErrorCode.h"
#include "fft.h"
#include <string>
#include <iostream>
#include <vector>
#include <map>
#include "Algorithm.h"

class ArousalReader {

	private :

		std::vector<std::vector<double>> _rawData;
		std::vector<std::vector<double>> _lastRawData;
		EmoEngineEventHandle eEvent;
		EmoStateHandle eState;
		DataHandle hData;
		float secs;
		unsigned int userID;
		bool _readyToCollect;
		Algorithm* _postProcessingAlgorithm;
		Algorithm* _preProcessingAlgorithm;
		static std::map<int, std::string> _channelList;
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
		void normalize(double* array);

	public:

		static const unsigned int _samplingRate = 128; //Hz
		static const unsigned int _indexFirstChannel=1;
		static const unsigned int _indexLastChannel=14;

		ArousalReader(Algorithm* postProcess, Algorithm* preProcess=NULL);
		void initialiseReading(int option);
		void endReading();
		bool readNextFrequencies();
		void printArrayToFile(std::string file, double* array);
		std::vector<double> getBetaWavesFromChannel(int channelIndex);
		std::vector<double> getAlphaWavesFromChannel(int channelIndex);
		std::vector<double> getFrequenciesFromChannel(int channelIndex);
		std::vector<double> getRawDataFromChannel(int channelIndex);
		std::vector<double> getFrequenciesRangeFromChannel(int begin, int end, int channelIndex);
		std::map<int, std::string> getChannelList();

		~ArousalReader();

		class EmotivConnectException{};
		class NoSampleFoundException{};
		class InitialisationFailureException{};
};

#endif