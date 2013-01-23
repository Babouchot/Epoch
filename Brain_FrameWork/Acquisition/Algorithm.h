#ifndef ALGORITHM_H
#define ALGORITHM_H

/*!
 * \file Algorithm.h
 * \brief Class representing an algorithm
 * \author Arnaud Ancelin, Aubert Gwenn, Maugard Matthieu, Maureille Bastien
 * \version 1.0
 */
#include <vector>

/*! 
 * \class Algorithm
 * \brief The abstract class you should extends if you want to provide your own algorithms
 */
class Algorithm
{
	public:
		Algorithm(){}

		/*!
		 * \fn virtual std::vector<std::vector<double> > process(const std::vector<std::vector<double> >& data) = 0
		 * Process the data trough the algorithm, this function should not be directly called, call will me made
		 * by the ArousalReader class
		 * \param data a vector of vector of double corresponding to the following format : [sample][channel]
		 * \return a vector of vector (created along the same format as the data parameter) filled with the processed data
		 */
		virtual std::vector<std::vector<double> > process(const std::vector<std::vector<double> >& data) = 0;
		~Algorithm(){}
};

#endif
