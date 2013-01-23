#ifndef NORMALIZATION_H
#define NORMALIZATION_H

#include "Algorithm.h"

/**
* exemple of an algorithm implementation
* Normalization class, doesn't really give goo results right now because
* it only normalize one second a the time and not the whole data set
*/
class Normalization : public Algorithm {
	private : 

		int newMin;
		int newMax;

	public:

		Normalization(int nMin, int nMax);
		virtual std::vector<std::vector<double> > process(const std::vector<std::vector<double> >& data);

};

#endif
