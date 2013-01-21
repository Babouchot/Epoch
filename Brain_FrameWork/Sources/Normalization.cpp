#include "Normalization.h"

using namespace std;

Normalization::Normalization(int nMin, int nMax) : newMin(nMin), newMax(nMax) {

}

vector<vector<double> > Normalization::process(const vector<vector<double> >& data){
	double min=100000;
	double max=-100000;
	vector<vector<double> > resultData(data);
	double* temp = new double[data.size()];

	for(int chan=0; chan<data[0].size(); ++chan){

		for(int i=0; i<data.size(); ++i){
			if(data[i][chan]<min){
				min=data[i][chan];
			}

			if(data[i][chan]>max){
				max=data[i][chan];
			}
		}

		for(int i=0; i<data.size(); ++i){
			resultData[i][chan]=newMin+(data[i][chan]-min)*(newMax-newMin)/(max-min);
		}

	}

	return resultData;
}



