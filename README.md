# NIDaqLab

NIDaqLab is an open-source library for MATLAB to acquire data from a National Instruments
device without having to install the MATLAB Data Acquisition Toolbox.

The tool can only obtain voltage data from the device, using a single channel
at the same time and in blocking mode (the thread is paused while the
acquisition is being done).

## Usage ##

The library has the following three functions:

 - daqDeviceList (Input parameters: none, Output parameters: none): This function lists
 available devices for acquisition on the MATLAB command prompt if NI-DAQmx is installed.
 
 - daqDeviceProperties (Input parameters: device name (string), Output parameters: none):
 This function lists several parameters of the passed device which might be useful when doing a script
 using the acquisition tool. Reefer to the device documentation for any parameter not shown by this
 tool. The information is displayed on the MATLAB command prompt.
 
 - daqAdquireData (Input paramters: Sample rate (numeric), physical channel (string),
 adquisition range (numeric), adquisition type (string), samples to aquire (numeric),
 device name (string), Output paramters: adquired data (1 x Number of samples row matrix (row vector))):
 This function retrieves the specified number of samples with the specified sample rate using the specified
 channel on the specified device. The input is automatically scaled by the DAQmx driver to which type
 is specified (Voltage only supported) and on the range specified (+-Range volts).

## Building ##

The NI-DAQmx software and drivers must be installed to build and use the library. The binaries
have been compiled using Microsoft Visual C++ 2008, so there might be problems running the
tool on Windows XP or older if the Microsoft C++ Redistributable 9.0 or newer is not
installed on the system.

National Instruments does not offer an ANSI C interface for UNIX or Mac OS systems so it
can only be compiled on the Windows platform.

## Binaries ##

If you want to download a compiled version of the library, you can do so at:

> http://cesar.jumbledevs.net/bin/nidaqlab.zip

## Contributing ##

The application is licensed under the Lesser GNU public license. Feel free to use any part of the application in your projects as long as you quote the original project somewhere. Feel also free to make a pull request or comment about any bug or doubt.


