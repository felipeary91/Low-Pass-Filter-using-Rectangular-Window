# Low-Pass-Filter-using-Rectangular-Window
A low-pass filter allows to pass signals with a frequency lower than that of a determined cutoff frequency, while attenuating signals with a frequency higher than this cutoff frequency. This low-pass filter has been designed using the "Rectangular Window" method, which uses a
set of N coefficients all with value of 1. Though being easy to implement, the Rectangular Window method is not really used for digital filter design, due to its low attenuation of the stopband and producing filters characterized by extreme values (e.g. sudden jump from zero at the initial sample).

Output samples of a FIR low-pass filter are produced based on:

```math
y(n) = \sum_{j=0}^{N} b_k \times x[n - j]
```

While its frequency response is given by:

```math
H(w) = \sum_{j=0}^{N} b_k \times e^{-jwn}
```


In order to test the time response of the filter you need to create the mex function in MATLAB using the command `mex -R2018a FIR_LP_Rectangular.c`. When calling the function you need to do it passing the parameters shown below:

```hs
FIR_LP_Rectangular(input_signal, filter_order, cutoff_frequency, sampling_frequency);
```

Overall, the following MATLAB code helps plotting the time response:

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

Here, a signal with a frequency of 5KHz through the filter, depending on the cutoff frequency declared when creating the filter, the signal might be allowed or attenuated. In this case, the cutoff frequency is 2.5KHz, so the signal will be attenuated.

On the other hand, the frequency response is also quite easy to plot:

```hs
f = 0:10000/512:10000;
plot(f, 20*log10(abs(freq_response)))
```
