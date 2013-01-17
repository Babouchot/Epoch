#ifndef _FFT_H_
#define _FFT_H_

#include <stdio.h>
#include <iostream>
#include <string>

class FFT {
 private:
  int n, m;
   
  // Lookup tables.  Only need to recompute when size of FFT changes.
  double* cosinus ;
  double* sinus ;

 public:
  FFT(int n);
  void printReIm(double* re, double* im);
  void beforeAfter(FFT fft, double* re, double* im);
  void fft(double* x, double* y);
  static int closestTwoPower(int n);
};

#endif
	
