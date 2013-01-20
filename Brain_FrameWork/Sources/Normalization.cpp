#include "Normalization.h"

Normalization::Normalization() : newMin(nMin), newMax(nMax) {

}

void Normalization::process(double* realArray, double* result, int size){
	double min=100000;
	double max=-100000;
	for(int i=0; i<size; ++i){
		if(realArray[i]>max){
			max=realArray[i];
		}

		if(realArray[i]<min){
			min=realArray[i];
		}
	}

	for(int i=0; i<size; ++i){
		result[i]=newMin+(realArray[i]-min)*(newMax-newMin)/(max-min);
	}
}