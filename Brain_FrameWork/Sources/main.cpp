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
	int cpt=0;
	while(true){
		try {
			if(ar.readNextFrequencies()){
			cout<<cpt<<endl;
			++cpt;
			for(int i=0; i<ar.getChannelMap().size(); ++i){
				vector<double> beta=ar.getBetaWavesFromChannel(i);
			    ostringstream oss;
			    oss <<"beta_"<< i <<".cvs";
			    ar.printArrayToFile(oss.str(), &beta[0], beta.size());
			}
		}
		} catch (ArousalReader::PacketLostException loss) {
			cout<<"Packet Lost \n";
		}

	}
	ar.endReading();
	return 0;
}
