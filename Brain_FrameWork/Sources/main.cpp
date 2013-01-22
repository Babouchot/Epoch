#include <iostream>
#include <unistd.h>
#include "ArousalReader.h"
#include <sstream>

using namespace std;

int main(){
	cout<<"Main application starting\n";

	FFT fft;
	ArousalReader ar(&fft);
	ar.initialiseReading();
	vector<double> beta;
	vector<double> raw;
	int cpt=0;
	while(true){
		try {
			if(ar.readNextFrequencies()){
			cout<<cpt<<endl;
			++cpt;
			for(int i=0; i<ar.getChannelMap().size(); ++i){
				beta=ar.getBetaWavesFromChannel(i);
			    ostringstream oss;
			    oss <<"beta_"<< i <<".cvs";
			    ar.printArrayToFile(oss.str(), &beta[0], beta.size());
			    raw=ar.getRawDataFromChannel(i);
			    ostringstream oss2;
			    oss2 <<"raw_"<< i <<".cvs";
			    ar.printArrayToFile(oss2.str(), &raw[0], raw.size());
			}
		}
		} catch (ArousalReader::PacketLostException loss) {
			cout<<"Packet Lost \n";
		}

	}
	ar.endReading();
	return 0;
}
