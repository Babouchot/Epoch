#ifndef ALGORITHM_H
#define ALGORITHM_H

#include <vector>

class Algorithm
{
	public:
		Algorithm(){}
		virtual std::vector<std::vector<double> > process(const std::vector<std::vector<double> >& data) = 0;
		~Algorithm(){}
};

#endif
