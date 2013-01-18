#ifndef ALGORITHM_H
#define ALGORITHM_H

class Algorithm
{
	public:
		Algorithm();
		virtual double* process(double* realArray, double* imaginaryArray) = 0;
		~Algorithm();
};

#endif