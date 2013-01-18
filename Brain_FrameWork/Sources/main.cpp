#include <string>
#include <iostream>
#include <unistd.h>
#include "ArousalReader.h"

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
	ArousalReader ar;
	ar.initialiseReading();
	int i=0;
	while(true){
		try{
			if(ar.readNextFrequencies()){
				//Do stuff
			}
		} catch (ArousalReader::NoSampleFoundException e){
			//cout<<"No sample read"<<endl;
		}
		++i;
		//cout<<i<<endl;
	}
	ar.endReading();
	return 0;
}