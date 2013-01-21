#include <math.h>
#include <sys/time.h>
#include <stdlib.h>
#include "fft.h"

#define PI  3.141592653

using namespace std;


FFT::FFT()  {

}

void FFT::setSize(int nb){

  n=nb;
  m = (int)(log(n) / log(2));

  cosinus = (double*) malloc(n * sizeof(double));
  sinus = (double*) malloc(n * sizeof(double));
   
    // Make sure n is a power of 2
  if(n != (1<<m)) 
      cerr << "n doit être une puissance de 2 "<< endl;

  for(int i=0; i< (n/2); i++) {
      cosinus[i] = cos(-2*PI*i/n);
      sinus[i] = sin(-2*PI*i/n);
  }
}

void FFT::beforeAfter(FFT fft, double re[], double im[]) {
  cout << "Before: ";
  printReIm(re, im);
  fft.fft(re, im);
  cout << "After: ";
  printReIm(re, im);
}

void FFT::printReIm(double re[], double im[]) {
  cout << "Re: [";
  for(int i=0; i<n/2; i++)
    cout << ((int)(re[i]*1000)/1000.0) << " ";
 
  cout << "]\nIm: [";
  for(int i=0; i<n/2; i++)
    cout << ((int)(im[i]*1000)/1000.0) << " ";
 
  cout << "]"<<endl;
}

/***************************************************************
 * fft.c
 * Douglas L. Jones 
 * University of Illinois at Urbana-Champaign 
 * January 19, 1992 
 * http://cnx.rice.edu/content/m12016/latest/
 * 
 *   fft: in-place radix-2 DIT DFT of a complex input 
 * 
 *   input: 
 * n: length of FFT: must be a power of two 
 * m: n = 2**m 
 *   input/output 
 * x: double array of length n with real part of data 
 * y: double array of length n with imag part of data 
 * 
 *   Permission to copy and use this program is granted 
 *   as long as this header is included. 
 ****************************************************************/
void FFT::fft(double x[], double y[]) {
  int i,j,k,n1,n2,a;
  double c,s,e,t1,t2;
   
   
  // Bit-reverse
  j = 0;
  n2 = n/2;
  for (i=1; i < n - 1; i++) {
    n1 = n2;
    while ( j >= n1 ) {
      j = j - n1;
      n1 = n1/2;
    }
    j = j + n1;
     
    if (i < j) {
      t1 = x[i];
      x[i] = x[j];
      x[j] = t1;
      t1 = y[i];
      y[i] = y[j];
      y[j] = t1;
    }
  }
 
  // FFT
  n1 = 0;
  n2 = 1;
   
  for (i=0; i < m; i++) {
    n1 = n2;
    n2 = n2 + n2;
    a = 0;
     
    for (j=0; j < n1; j++) {
      c = cosinus[a];
      s = sinus[a];
      a +=  1 << (m-i-1);
 
      for (k=j; k < n; k=k+n2) {
	t1 = c*x[k+n1] - s*y[k+n1];
	t2 = s*x[k+n1] + c*y[k+n1];
	x[k+n1] = x[k] - t1;
	y[k+n1] = y[k] - t2;
	x[k] = x[k] + t1;
	y[k] = y[k] + t2;
      }
    }
  }
}

void FFT::process(double* realArray, double* result, int size){
  setSize(size);
  double* imaginaryArray=new double[size];
  for(int i=0; i<size; ++i){
    imaginaryArray[i]=0;
  }
  fft(realArray, imaginaryArray);
  computeModulus(realArray, imaginaryArray, result);

  delete[] imaginaryArray;
  delete[] sinus;
  delete[] cosinus;
}

void FFT::computeModulus(double* x, double* y, double* result){
  for(int i=0; i<n; ++i){
    result[i]=sqrt(pow(x[i],2) + pow(y[i], 2));
  }
}

int FFT::closestTwoPower(int n){
  int i=0;
  int res=0;
  while(res<=n){
    ++i;
    res=pow(2,i);
  }
  return pow(2,i-1);
}

