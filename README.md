# Low-Pass-Filter-using-Rectangular-Window
A low-pass filter allows to pass signals with a frequency lower than that of a determined cutoff frequency, while attenuating signals with a frequency higher than this cutoff frequency. This low-pass filter has been designed using the "Rectangular Window" method, which uses a
set of N coefficients all with value of 1. Though being easy to implement, the Rectangular Window method is not really used for digital filter design, due to its low attenuation of the stopband and producing filters characterized by extreme values (e.g. sudden jump from zero at the initial sample).

Output samples of a FIR low-pass filter are produced based on:

```math
y(n) = \sum_{j=0}^{N} b_k \times x[n - j]
```

While its frequency response is given by:

```math
H(w) = \sum_{j=0}^{N} b_k \times exp^{-jwn}
```


In order to test the time response of the filter you can create the mex function using the command `mex -R2018a FIR_LP_Rectangular.c`. After this, the following commands can be run in MATLAB:

```hs
fs = 20000;
dt = 1/fs;
F = 5000; 
t = 0:dt:0.0025;
x = sin(2*pi*F*t);
plot(x, 'red')
[filt_data, freq_response] = FIR_LP_Rectangular(x, 20, 2500, 20000);
hold on
plot(filt_data, 'blue')
```

Here, we are passing a signal with a frequency of 5KHz to our filter, depending on the cutoff frequency declared when creating the filter, the signal might be allowed or attenuated. In this case, the cutoff frequency is 2.5KHz, so the signal will be attenuated. 
