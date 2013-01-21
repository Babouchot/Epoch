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
		
		//The data beeing read, format : [sampleIndex][channelIndex]
		std::vector<std::vector<double> > _rawData;
		//The last data read, format : [sampleIndex][channelIndex]
		std::vector<std::vector<double> > _lastRawData;

		//Emotiv required data
		EmoEngineEventHandle eEvent;
		EmoStateHandle eState;
		DataHandle hData;
		float secs;
		unsigned int userID;
		bool _readyToCollect;

		//Processing algorithm
		Algorithm* _postProcessingAlgorithm;
		Algorithm* _preProcessingAlgorithm;
		Algorithm* _normalizationAlgorithm;

		//Static toolkit
		static std::map<int, std::string> _channelList;
		static const unsigned int _indexFirstRelevantChannel=1;
		static const unsigned int _indexLastRelevantChannel=14;
		static const int startBeta=14;//15Hz
		static const int endBeta=30;//30Hz
		static const int startAlpha=7;//8Hz
		static const int endAlpha=13;//13Hz

		void printArray(double* array, int size);
		void initialiseArray(double* array, int size);
		void applyPreProcessing();

	public:

		/*
		* The sampling rate at witch data are aquired ie : 128 sample every second
		*/
		static const unsigned int samplingRate = 128; //Hz

		/*
		* Constructor with postProcessing and optional preProcessing algorithm
		*/
		explicit ArousalReader(Algorithm* postProcess, Algorithm* preProcess=NULL);

		/*
		* Constructor with postProcessing, preProcessing, and normalization algorithm
		*/
		explicit ArousalReader(Algorithm* postProcess, Algorithm* preProcess, Algorithm* normalize);

		/*
		* Connect to the headset or the emoComposer and initialize the reading variables
		* The parameter option should be 0 for a connection to the headset and 1 to connect to the 
		* emoComposer
		*/
		void initialiseReading(int option);

		/*
		* Clean the reading variables and disconnect from the headset
		*/
		void endReading();

		/*
		* Read the next piece of data from the headset, if enough data (128 sample)
		* have been acquired return true, else, return false;
		*/
		bool readNextFrequencies();

		/*
		* Print the array o=into a file named string (the values will be added at the end of the file)
		*/
		void printArrayToFile(std::string file, double* array, int size);

		/*
		* Get all the frequencies corresponding to the beta waves for the specified channel
		* the post processing algorithm is used to perform the rawdata, frequencies conversion
		*/
		std::vector<double> getBetaWavesFromChannel(int channelIndex);
		
		/*
		* Get all the frequencies corresponding to the alpha waves for the specified channel
		* the post processing algorithm is used to perform the rawdata, frequencies conversion
		*/
		std::vector<double> getAlphaWavesFromChannel(int channelIndex);
		
		/*
		* Get all the frequencies for one channel, the post processing algorithm
		* is used to perform the rawdata, frequencies conversion
		*/
		std::vector<double> getFrequenciesFromChannel(int channelIndex);
		
		/*
		* Get the last 128 acquired sample
		*/
		std::vector<double> getRawDataFromChannel(int channelIndex);
		
		/*
		* Get all the frequencies from the begin to end parameters for the specified channel
		* the post processing algorithm is used to perform the rawdata, frequencies conversion
		*/
		std::vector<double> getFrequenciesRangedFromChannel(int begin, int end, int channelIndex);
		
		/*
		* Return the map containing the name of each channel with the corresponding channel index
		*/
		static std::map<int, std::string> getChannelMap();
		
		/*
		* Initialize the channel list
		*/
		static void initialiseChannelList();
		
		/*
		* Object destructor, delete the acquisition vectors
		*/
		~ArousalReader();

		/*
		* Thrown when the application can't connect to the Emotiv Engine
		*/
		class EmotivConnectException{};
		
		/*
		* Thrown when a wrong initialization option is given
		*/
		class WrongInitializationException{};
		
		/*
		* Thrown when trying to get data before the first full second have bee read
		*/
		class NoDataReadException{};
};

#endif