#include <iostream>
#include "ArousalReader.h"
#include <sstream>
#include <time.h>

using namespace std;

int main(){
	cout<<"Main application starting\n";

	FFT fft;
	ArousalReader ar(&fft);
	ar.initialiseReading();
	vector<double> beta;
	vector<double> raw;
	int cpt=0;
  	time_t now;
 	clock_t clock_now;

	while(true){
		try {
			if(ar.readNextFrequencies()){
			cout<<cpt<<endl;
			time(&now);
			clock_now=clock();
			double cl=clock_now/CLOCKS_PER_SEC;
			cout<<"per secs "<<CLOCKS_PER_SEC<< " , nbclock " <<clock_now<<endl;
			cout<<"other time : "<<cl<<endl;
			cout<<"time is : "<<ctime(&now)<<endl;
			++cpt;
			for(unsigned int i=0; i<ar.getChannelMap().size(); ++i){
				beta=ar.getBetaWavesFromChannel(i);
			    /*ostringstream oss;
			    oss <<"beta_"<< i <<".cvs";
			    ar.printArrayToFile(oss.str(), &beta[0], beta.size());*/
			    raw=ar.getRawDataFromChannel(i);
			    /*ostringstream oss2;
			    oss2 <<"raw_"<< i <<".cvs";
			    ar.printArrayToFile(oss2.str(), &raw[0], raw.size());*/
			}
			cout<<"////////////////////////////////\n";
		}
		} catch (ArousalReader::PacketLostException loss) {
			cout<<"Packet Lost \n";
		} catch (ArousalReader::NoDataReadException noData) {
			cout<<"No Data Read \n";
		}
	}
	ar.endReading();
	return 0;
}
