#include "FIR_LP_Rectangular_filter_header.h"
#include <stdio.h>
#include <stdlib.h>
#include <tgmath.h>

//Prototype of get_coeff function to calculate
//window coefficients
double *get_coeff(int filter_order, double f_cutoff, double f_sampling);

//Prototype of get_freq_resp to compute the
//frequency response
void get_freq_resp(double *h_d, int filter_order, double df, double f_sampling, mxComplexDouble *freq_resp);


void MATLAB_main(mxComplexDouble *freq_resp, double *filt_data, double *input_data, size_t sizeSignal, double f_passband, double f_stopband, double f_sampling, double ripple_stopband) {

	//Declaring variables
	double f_cutoff = 0;
	int filter_order = 0;
	double f_passband_norm = 0;
	double f_stopband_norm = 0;


	//Verifying if Rectangular Window can be used based
	//on the given stopband ripple
	if ((- 20 * log10(ripple_stopband)) < 23) {
		printf("Peak stopband attenuation is %0.1f dB, Rectangular Window can be used.\n", -20 * log10(ripple_stopband));
	}
	else
	{
		printf("Peak stopband attenuation is lower than -23 dB, Rectangular Window cannot be used.\n");
		return;
	}

	//Calculating normalized passband and stopband frequencies
	f_passband_norm = (2 * acos(-1) * f_passband) / f_sampling;
	f_stopband_norm = (2 * acos(-1) * f_stopband) / f_sampling;

	//Calculating normalized cutoff frequency
	f_cutoff = (f_passband_norm + f_stopband_norm) / 2;

	//Calculating filter_order
	filter_order = (4 * acos(-1)) / (f_stopband_norm - f_passband_norm);
	if ((filter_order % 2) != 0) {
		filter_order += 1;
	}
	printf("The filter order based on the given specifications is %d.\n", filter_order);


	int k = 1 + log2(filter_order);
	int sizeBuffer = pow(2, k);
	double df = (f_sampling / 2) / 512;	//Frequency delta
	

	//Obtaining the low_pass coefficients
	double* h_d = get_coeff(filter_order, f_cutoff, f_sampling);

	//Multiplying the window coefficients
	//with the low pass coefficients, in this
	//case all rectangular window's coefficients
	//are set to 1
	for (int i = 0; i < (filter_order + 1); i++) {
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
		
		for (int j = 0; j <= filter_order; j++) {
			sum += h_d[j] * buffer[(i - j) % (unsigned int)sizeBuffer];
		}

		//Saving data to output
		filt_data[i] = sum;
	}
	
	//Getting the frequency response
	get_freq_resp(h_d, filter_order, df, f_sampling, freq_resp);

	//Releasing memory used by
	//window coefficients
	free(h_d);
}




//Implementation of get_coeff function
double *get_coeff(int filter_order, double f_cutoff, double f_sampling) {
	
	//The size of the window corresponds
	//to the filter's order + 1 
	double* h_d;
	h_d = (double*)calloc(filter_order + 1, sizeof(double));

	// if memory cannot be allocated
	if (h_d == NULL) {
		printf("Error! memory not allocated.");
		exit(0);
	}

	//Index of middle coefficient
	int m = filter_order / 2;

	//Initializing all elements of the
	//window to the corresponding coefficient
	for (int i = 0; i < (filter_order + 1); i++) {
		if (m != i) {
			h_d[i] = sin(f_cutoff*(i - m))/(acos(-1)*(i - m));
		}
		else {
			h_d[i] = f_cutoff / acos(-1);
		}
	}

	return h_d;
}



//Implementation of get_freq_resp function
void get_freq_resp(double* h_d, int filter_order, double df, double f_sampling, mxComplexDouble* freq_resp) {
	//Generating the frequency response
	for (int i = 0; i <= 512; i++) {
		complex double freq_resp_comp = 0 + 0 * I;
		double f = i * df;

		//Base complex exponential in rectangular form
		complex double H_z = cos(2 * acos(-1) * (f / f_sampling)) - sin(2 * acos(-1) * (f / f_sampling)) * I;

		//Sum of the multiplication of filter
		//coefficients and complex exponential
		for (int j = 0; j <= filter_order; j++) {
			freq_resp_comp += h_d[j] * cpow(H_z, j);
		}

		//Saving data to output
		freq_resp[i].real = creal(freq_resp_comp);
		freq_resp[i].imag = cimag(freq_resp_comp);
	}
}