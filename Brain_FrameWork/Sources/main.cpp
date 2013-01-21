#include <iostream>
#include <unistd.h>
#include "ArousalReader.h"
#include <sstream>

using namespace std;

int main(){
	cout<<"gogoggo\n";
	/*
	cout<<FFT::closestTwoPower(4)<<endl;
	cout<<FFT::closestTwoPower(8)<<endl;
	cout<<FFT::closestTwoPower(7)<<endl;
	cout<<FFT::closestTwoPower(9)<<endl;
	cout<<FFT::closestTwoPower(42)<<endl;
	*/
	FFT fft;
	ArousalReader ar(&fft);
	ar.initialiseReading();
	int cpt=0;
	while(true){

		if(ar.readNextFrequencies()){
			cout<<cpt<<endl;
			for(int i=0; i<ar.getChannelMap().size(); ++i){
				vector<double> beta=ar.getBetaWavesFromChannel(i);
			    ostringstream oss;
			    oss <<"beta_"<< i <<".cvs";
			    ar.printArrayToFile(oss.str(), &beta[0], beta.size());
			}
		}

		++cpt;
		//cout<<i<<endl;
	}
	ar.endReading();
	return 0;
}