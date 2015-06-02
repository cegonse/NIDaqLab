%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% daqInputExample.m
%
% This file is an example of a MATLAB script using the NIDaqLab library
%
% The files "daqAdquireData.mexw32" and "daqDeviceProperties.mexw32" must
% be in the same directory as this script file.
%
% Use the tool "daqDeviceList.mexw32" to list the avaible devices on your
% system. The National Instruments DAQmx software must be installed for
% this script to run.
%
% César Gonzalez Segura
% Script created 16 / 5 / 2012
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% This library is free software; you can redistribute it and/or
% modify it under the terms of the GNU Lesser General Public
% License as published by the Free Software Foundation; either
% version 3.0 of the License, or (at your option) any later version.

% This library is distributed in the hope that it will be useful,
% but WITHOUT ANY WARRANTY; without even the implied warranty of
% MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
% Lesser General Public License for more details.

% You should have received a copy of the GNU Lesser General Public
% License along with this library.

Range = 3.0; % Voltage range we want to measure
Device = 'Dev2'; % Change this value for the name of the device installed
                 % in your system.
Channel = 'Dev2/ai0'; % Same as above
Rate = 1000; % Sampling rate in samples per second
Samples = 500; % Number of samples to adquire
Type = 'Voltage'; % We want to measure voltage

Data = daqAdquireData(Rate, Channel, Range, Type, Samples, Device); % We do the measuring
Spectrum = fft(Data); % Then obtain the frequency components of the adquired signal

t = 0:length(Data) - 1;
f = 0:length(Spectrum) - 1;

SpectrumNorm = Spectrum./max(Spectrum);
SpectrumDB = 20*log(SpectrumNorm);

% And then show it
figure(1);
plot(t, Data);
title('Adquired data');
xlabel('Time (samples)');
ylabel('Amplitude (Volts)');

figure(2);
plot(f, SpectrumDB);
title('Spectrum of the adquired data');
xlabel('Frequency');
ylabel('Amplitude (dB)');