#ifndef _FFT_H_
#define _FFT_H_

#include <stdio.h>
#include <iostream>
#include <string>
#include "Algorithm.h"

/*
* Class implementing the fft algorithm ported from the colombia fft algorithm
*/
class FFT : public Algorithm{
  private:
    int n, m;
     
    // Lookup tables.  Only need to recompute when size of FFT changes.
    double* cosinus ;
    double* sinus ;

  public:
    FFT();
    void printReIm(double* re, double* im);
    void beforeAfter(FFT fft, double* re, double* im);
    void fft(double* x, double* y);
    void computeModulus(double* x, double* y, double* result);
    void setSize(int nb);
    virtual std::vector<std::vector<double> > process(const std::vector<std::vector<double> >& data);
    static int closestTwoPower(int n);
};

#endif
	
