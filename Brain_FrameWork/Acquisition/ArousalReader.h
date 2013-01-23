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


/**
* The arousal reader class is the main part of the api designed to help
* reading and processing data from the Emotiv headset
*/
class ArousalReader {

	private :
		
		//The data beeing read, format : [sampleIndex][channelIndex]
		std::vector<std::vector<double> > _rawData;
		//The last data read, format : [sampleIndex][channelIndex]
		std::vector<std::vector<double> > _lastRawData;
		//The last calculated frequencies
		std::vector<std::vector<double> > _lastFrequencies;

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

		//Validation data
		unsigned int _lastCounter;

	public:

		/**
		* The sampling rate at witch data are aquired ie : 128 sample every second
		*/
		static const unsigned int samplingRate = 128; //Hz

		/**
		* Constructor with postProcessing and optional preProcessing algorithm
		* \param postProcess : the post-processing that will be used on the data (typically, FFT like algorithms)
		* \param preProcess : the pre-processing that will be used to process the data before they go through the post processing algotihm
		*/
		explicit ArousalReader(Algorithm* postProcess, Algorithm* preProcess=NULL);

		/**
		* Constructor with postProcessing, preProcessing, and normalization algorithm
		* \param postProcess : the post-processing that will be used on the data (typically, FFT like algorithms)
		* \param preProcess : the pre-processing that will be used to process the data before they go through the post processing algotihm
		* \param normalize : the normalization or final processing algorithm, data will be processed through this algorithm after 
		* they have been through the post-processing one
		*/
		explicit ArousalReader(Algorithm* postProcess, Algorithm* preProcess, Algorithm* normalize);

		/**
		* Connect to the headset or the emoComposer and initialize the reading variables
		*/
		void initialiseReading();

		/**
		* Clean the reading variables and disconnect from the headset
		*/
		void endReading();

		/**
		* Read the next piece of data from the headset
		* \return if enough data (128 sample) have been acquired return true, else, return false;
		*/
		bool readNextFrequencies();

		/**
		* Print the array into a file named string (the values will be added at the end of the file)
		* \param file : the path (relative or absolute) to the file to read
		* \param array : the array from wich the data will be printed into the file
		* \size : the size of the array to print
		*/
		void printArrayToFile(std::string file, double* array, int size);

		/**
		* Get a vector of data from a previously saved file (format .cvs), throws WrongFileFormatException 
		* when the file is not readable
		* \return the vector filed with the data read from the file
		*/
		std::vector<double> getVectorFromFile(std::string file);

		/**
		* Get all the frequencies corresponding to the beta waves for the specified channel
		* the post processing algorithm is used to perform the rawdata, frequencies conversion
		* \param channelIndex : the index of the channel
		* \return a vector filled with the frequencies corresponding to the beta waves for this channel
		*/
		std::vector<double> getBetaWavesFromChannel(int channelIndex);
		
		/**
		* Get all the frequencies corresponding to the alpha waves for the specified channel
		* the post processing algorithm is used to perform the rawdata, frequencies conversion
		* \param channelIndex : the index of the channel
		* \return a vector filled with the frequencies corresponding to the alpha waves for this channel
		*/
		std::vector<double> getAlphaWavesFromChannel(int channelIndex);
		
		/**
		* Get all the frequencies for one channel, the post processing algorithm
		* is used to perform the rawdata, frequencies conversion
		* \param channelIndex : the index of the channel
		* \return a vector filled with the frequencies for this channel
		*/
		std::vector<double> getFrequenciesFromChannel(int channelIndex);
		
		/**
		* Get the last 128 acquired sample
		* \param channelIndex : the index of the channel
		* \return a vector filled with the raw data for this channel
		*/
		std::vector<double> getRawDataFromChannel(int channelIndex);
		
		/**
		* Get all the frequencies from the begin (included) to end (excluded) parameters for the specified channel
		* the post processing algorithm is used to perform the rawdata, frequencies conversion
		* \param begin : the start frequency of the range
		* \param end the frequency following the range end
		* \param channelIndex : the index of the channel
		* \return a vector filled with the frequencies corresponding to the wanted range for this channel
		*/
		std::vector<double> getFrequenciesRangedFromChannel(int begin, int end, int channelIndex);

		/**
		* Get the map containing the name of each channel with the corresponding channel index
		* \return the map containing the channel and their indexes
		*/
		static std::map<int, std::string> getChannelMap();
		
		/**
		* Initialize the channel list
		*/
		static void initialiseChannelList();
		
		/**
		* Object destructor, delete the acquisition vectors
		*/
		~ArousalReader();

		/**
		* Thrown when the application can't connect to the Emotiv Engine
		*/
		class EmotivConnectException{};
		
		/**
		* Thrown when trying to get data before the first full second have bee read
		*/
		class NoDataReadException{};

		/**
		* Thrown when a packet is lost
		*/
		class PacketLostException{};

		/**
		* Thrown when trying to read an array
		* from a file with a wrong extension
		*/
		class WrongFileFormatException{};
};

#endif
