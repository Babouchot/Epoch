#ifndef NORMALIZATION_H
#define NORMALIZATION_H


/*!
 * \file Normalization.h
 * \brief Normalization algorithm
 * \author Arnaud Ancelin, Aubert Gwenn, Maugard Matthieu, Maureille Bastien
 * \version 1.0
 */

#include "Algorithm.h"

/*!
 * \class Normalization
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
