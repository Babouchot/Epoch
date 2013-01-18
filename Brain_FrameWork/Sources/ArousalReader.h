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
		//[sampleNb][channel]
		std::vector<std::vector<double> > _rawData;
		std::vector<std::vector<double> > _lastRawData;
		EmoEngineEventHandle eEvent;
		EmoStateHandle eState;
		DataHandle hData;
		float secs;
		unsigned int userID;
		bool _readyToCollect;
		Algorithm* _postProcessingAlgorithm;
		Algorithm* _preProcessingAlgorithm;
		static std::map<int, std::string> _channelList;
		static const unsigned int _indexFirstRelevantChannel=1;
		static const unsigned int _indexLastRelevantChannel=14;

		void printArray(double* array, int size);
		void initialiseArray(double* array, int size);
		void normalize(double* array, int size);

	public:

		static const unsigned int samplingRate = 128; //Hz

		ArousalReader(Algorithm* postProcess, Algorithm* preProcess=NULL);
		void initialiseReading(int option);
		void endReading();
		bool readNextFrequencies();
		void printArrayToFile(std::string file, double* array, int size);
		std::vector<double> getBetaWavesFromChannel(int channelIndex);
		std::vector<double> getAlphaWavesFromChannel(int channelIndex);
		std::vector<double> getFrequenciesFromChannel(int channelIndex);
		std::vector<double> getRawDataFromChannel(int channelIndex);
		std::vector<double> getFrequenciesRangedFromChannel(int begin, int end, int channelIndex);
		static std::map<int, std::string> getChannelList();
		static void initialiseChannelList();

		~ArousalReader();

		class EmotivConnectException{};
		class NoSampleFoundException{};
		class InitialisationFailureException{};
		class NoDataReadException{};
};

#endif