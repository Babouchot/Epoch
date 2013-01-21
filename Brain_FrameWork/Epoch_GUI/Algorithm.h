#ifndef ALGORITHM_H
#define ALGORITHM_H

class Algorithm
{
	public:
		Algorithm(){}
		virtual void process(double* realArray, double* result, int size) = 0;
		~Algorithm(){}
};

#endif