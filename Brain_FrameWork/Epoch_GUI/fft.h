#ifndef _FFT_H_
#define _FFT_H_

#include <stdio.h>
#include <iostream>
#include <string>
#include "Algorithm.h"

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
    virtual void process(double* realArray, double* result, int size);
    static int closestTwoPower(int n);
};

#endif
	
