#include "FIR_LP_Rectangular_filter_header.h"
#include <stdio.h>
#include <stdlib.h>
#include <tgmath.h>

//Prototype of get_coeff function to calculate
//a window coefficient
double *get_coeff(int order, double freq_fc, double freq_fs);

void MATLAB_main(mxComplexDouble *freq_resp, double *filt_data, double *input_data, size_t sizeSignal, double order, double freq_fc, double freq_fs) {

	//General variables
	int k = 1 + log2(order);
	int sizeBuffer = pow(2, k);
	double df = (freq_fs/2)/512;	//Frequency delta
	
	//Obtaining the low_pass coefficients
	double* h_d = get_coeff(order, freq_fc, freq_fs);

	//Multiplying the window coefficients
	//with the low pass coefficients, in this
	//case all rectangular window's coefficients
	//are set to 1
	for (int i = 0; i < (order + 1); i++) {
		h_d[i] *= 1;	//h_n
	}
	
	//Creating the buffer with length 2^k
	double* buffer;
	buffer = (double*)calloc(pow(2, k), sizeof(double));
	//Initializing buffer to zero
	for (int i = 0; i < sizeBuffer; i++) {
		buffer[i] = 0;
	}

	//Generating filter output
	for (int i = 0; i < sizeSignal; i++) {
		double sum = 0;
		
		//Assigning the incoming data to the index
		//given by the modulus (circular buffer)
		buffer[i%sizeBuffer] = input_data[i];
		
		for (int j = 0; j <= order; j++) {
			sum += h_d[j] * buffer[(i - j) % (unsigned int)sizeBuffer];
		}

		//Saving data to output
		filt_data[i] = sum;
	}
	
	//Generating the frequency response
	for (int i = 0; i <= 512; i++) {
		complex double freq_resp_comp = 0 + 0*I;
		double f = i * df;
		
		//Base complex exponential in rectangular form
		complex double H_z = cos(2 * acos(-1) * (f/freq_fs)) - sin(2 * acos(-1) * (f/freq_fs)) * I;
		
		//Sum of the multiplication of filter
		//coefficients and complex exponential
		for (int j = 0; j <= order; j++) {
			freq_resp_comp += h_d[j] * cpow(H_z, j);
		}

		//Saving data to output
		freq_resp[i].real = creal(freq_resp_comp);
		freq_resp[i].imag = cimag(freq_resp_comp);
	}

	//Releasing memory used by
	//window coefficients
	free(h_d);
}

//Implementation of the get_coeff function
double *get_coeff(int order, double freq_fc, double freq_fs) {
	
	//The size of the window corresponds
	//to the filter's order + 1 
	double* h_d;
	h_d = (double*)calloc(order + 1, sizeof(double));

	// if memory cannot be allocated
	if (h_d == NULL) {
		printf("Error! memory not allocated.");
		exit(0);
	}

	//Index of middle coefficient
	int m = order / 2;
	//Normalized frequency
	double w_c = (2 * acos(-1) * freq_fc) / freq_fs;

	//Initializing all elements of the
	//window to the corresponding coefficient
	for (int i = 0; i < (order + 1); i++) {
		if (m != i) {
			h_d[i] = sin(w_c*(i - m))/(acos(-1)*(i - m));
		}
		else {
			h_d[i] = w_c / acos(-1);
		}
	}

	return h_d;
}