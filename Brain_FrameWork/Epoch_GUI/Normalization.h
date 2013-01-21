#ifndef NORMALIZATION_H
#define NORMALIZATION_H

#include "Algorithm.h"


class Normalization
{
	private : 

		int newMin;
		int newMax;

	public:

		Normalization(int nMin, int nMax);
		virtual void process(double* realArray, double* result, int size);

};