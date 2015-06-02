/*************************************************************/
// daqAdquireData.cpp
//
// Reads the specified number of samples attending the passed
// parameters
//
//                       ------ ARGUMENTS ------
//
// [AdquiredData (f)] = daqAdquireData(SamplingPeriod (n), ChannelName (s),
//     InputRange (f), AdquisitionType (s), NumberOfSamples (n), Device (s))
//
// - n denotes a natural value
// - f denotes a positive real number
// - s denotes a string
//
//  - SamplingPeriod: sampling period for the data adquisition. Must not
//                    exceed the maximum sampling period supported by
//                    by the device
//
//     - ChannelName: name of the physical channel that is going to be used
//                    for the adquisition. Check the channel names avaible
//                    using the function "daqDeviceProperties"
//
//      - InputRange: maximum amplitude expected to adquire. Must not exceed
//                    the maximum ranges supported by the device
//
//  - AdqusitionType: specifies the kind of data to adquire. It must be
//                    "Voltage", "VoltageRMS", "Current", "CurrentRMS",
//                    "Resistance". Must be a mode supported by the device.
//                    Check the avaible modes using the function "daqDeviceProperties"
//
// - NumberOfSamples: number of samples to adquire
//
//          - Device: name of the device that will be used for adquiring the data.
//                    check the installed devices on the system using the function
//                    "daqDeviceList"
//
//    - AdquiredData: a row vector with the data adquired by the device with length
//                    equal to the number of samples told to adquire
//
// Created 15/5/2012
// Cesar Gonzalez Segura
/*************************************************************/
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 3.0 of the License, or (at your option) any later version.

// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.

// You should have received a copy of the GNU Lesser General Public
// License along with this library.

#include "NIDAQmx.h"
#include "mex.h"
#include "string.h"
#pragma comment(lib, "NIDAQmx.lib")

void outMexError(int nError)
{
    if (nError != 0)
    {
        int nSize = DAQmxGetErrorString(nError, NULL, 0);
        char *lpError = (char*) mxMalloc(nSize);
        DAQmxGetErrorString(nError, lpError, nSize);
        char lpOutput[256];
        
        sprintf(lpOutput, "DAQmx Error %d: %s.", nError, lpError);
        mxFree(lpError);
        
        mexErrMsgTxt(lpOutput);
    }
}

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    if (nrhs > 6)
    {
        mexErrMsgTxt("Too many input arguments.");
    }
    else if (nrhs < 6)
    {
        mexErrMsgTxt("Too few input arguments.");
    }
    else if (nlhs > 1)
    {
        mexErrMsgTxt("Too many output arguments.");
    }
    else
    {
        if ((mxIsChar(prhs[1]) != 1) || (mxIsChar(prhs[3]) != 1) || (mxIsChar(prhs[5]) != 1))
        {
            mexErrMsgTxt("Input arguments 2, 4 and 6 must be strings.");
        }
        else if (mxGetM(prhs[1])!= 1 || mxGetM(prhs[3]) != 1 || mxGetM(prhs[5]) != 1)
        {
            mexErrMsgTxt("Input arguments 2, 4 and 6 must be row vectors.");
        }
        else if (mxIsNumeric(prhs[0]) != 1 || mxIsNumeric(prhs[4]) != 1 || mxIsNumeric(prhs[2]) != 1)
        {
            mexErrMsgTxt("Input arguments 1, 3 and 5 must be numeric values.");
        }
        else
        {
            TaskHandle hTask = NULL;
            mwSize 
                    nSizeArg1 = (mxGetM(prhs[1]) * mxGetN(prhs[1])) + 1,
                    nSizeArg3 = (mxGetM(prhs[3]) * mxGetN(prhs[3])) + 1,
                    nSizeArg5 = (mxGetM(prhs[5]) * mxGetN(prhs[5])) + 1;
            char
                    *lpArg1 = mxArrayToString(prhs[1]),
                    *lpArg3 = mxArrayToString(prhs[3]),
                    *lpArg5 = mxArrayToString(prhs[5]),
                    *lpDevice, *lpChannel, *lpType;
            
            float64
                    *nArg2 = (float64*) mxGetPr(prhs[2]),
                    *nArg4 = (float64*) mxGetPr(prhs[4]),
                    *nArg0 = (float64*) mxGetPr(prhs[0]),
                    fMaxVolts = *nArg2,
                    nSamplingPeriod = *nArg0,
                    nSamples = *nArg4;
            
            lpDevice = (char*) mxCalloc(strlen(lpArg5) + 1, sizeof(char));
            lpChannel = (char*) mxCalloc(strlen(lpArg1) + 1, sizeof(char));
            lpType = (char*) mxCalloc(strlen(lpArg3) + 1, sizeof(char));
            
            strcpy(lpChannel, lpArg1); mxFree(lpArg1);
            strcpy(lpType, lpArg3); mxFree(lpArg3);
            strcpy(lpDevice, lpArg5); mxFree(lpArg5);
            
            int32
                   nAdquisitionType,
                   nResult,
                  *nSamplesRead = (int32*) mxMalloc(sizeof(int32));
            
            if (!strcmp(lpType, "Voltage"))
            {
                nAdquisitionType = DAQmx_Val_Voltage;
            }
            else if (!strcmp(lpType, "VoltageRMS"))
            {
                nAdquisitionType = DAQmx_Val_VoltageRMS;
            }
            else if (!strcmp(lpType, "Current"))
            {
                nAdquisitionType = DAQmx_Val_Current;
            }
            else if (!strcmp(lpType, "CurrentRMS"))
            {
                nAdquisitionType = DAQmx_Val_CurrentRMS;
            }
            else if (!strcmp(lpType, "Resistance"))
            {
                nAdquisitionType = DAQmx_Val_Resistance;
            }
            else
            {
                mxFree(lpDevice);
                mxFree(lpType);
                mxFree(lpChannel);
                mxFree(nSamplesRead);
                
                mexPrintf("\n\nAdqusition type is not valid. Use one of the following values:\n");
                mexPrintf(" - 'Voltage'\n - 'VoltageRMS'\n - 'Current'\n - 'CurrentRMS'\n - 'Resistance'\n");
                mexErrMsgTxt("Input parameter not valid.");
            }
            
            mxFree(lpType);
            nResult = DAQmxCreateTask("Task1", &hTask);
            
            if (nResult)
            {
                mxFree(lpChannel);
                mxFree(lpDevice);
                mxFree(nSamplesRead);
                #ifdef __DAQ_DEBUG
                        mexPrintf("LINE 169\n");
                #endif
                outMexError(nResult);
            }
            
            if (nAdquisitionType == DAQmx_Val_Voltage)
            {
                nResult = DAQmxCreateAIVoltageChan(hTask, lpChannel, "", DAQmx_Val_Diff, -fMaxVolts, fMaxVolts, DAQmx_Val_Volts, NULL);
            }
            else
            {
                mxFree(lpChannel);
                mxFree(lpDevice);
                mxFree(nSamplesRead);
                DAQmxClearTask(hTask);
                
                mexErrMsgTxt("Only voltage adquisition is implemented.");
            }
            
            if (nResult)
            {
                mxFree(lpChannel);
                mxFree(lpDevice);
                mxFree(nSamplesRead);
                #ifdef __DAQ_DEBUG
                        mexPrintf("LINE 182\n");
                #endif
                DAQmxClearTask(hTask);
                outMexError(nResult);
            }
            
            nResult = DAQmxCfgSampClkTiming(hTask, NULL, nSamplingPeriod, DAQmx_Val_Rising, DAQmx_Val_FiniteSamps, nSamples);
            
            if (nResult)
            {
                mxFree(lpChannel);
                mxFree(lpDevice);
                mxFree(nSamplesRead);
                #ifdef __DAQ_DEBUG
                        mexPrintf("LINE 195\n");
                #endif
                DAQmxClearTask(hTask);
                outMexError(nResult);
            }
            
            nResult = DAQmxStartTask(hTask);
            
            if (nResult)
            {
                mxFree(lpChannel);
                mxFree(lpDevice);
                mxFree(nSamplesRead);
                #ifdef __DAQ_DEBUG
                        mexPrintf("LINE 208\n");
                #endif
                DAQmxClearTask(hTask);
                outMexError(nResult);
            }
            
            float64 *fReadArray = (float64*) mxCalloc(nSamples, sizeof(float64));
            
            nResult = DAQmxReadAnalogF64(hTask, nSamples, 10, DAQmx_Val_GroupByChannel, fReadArray, nSamples, nSamplesRead, NULL);
            
            if (nResult)
            {
                mxFree(lpChannel);
                mxFree(lpDevice);
                mxFree(fReadArray);
                mxFree(nSamplesRead);
                outMexError(nResult);
            }
            
            DAQmxStopTask(hTask);
            
            plhs[0] = mxCreateNumericMatrix(1, *nSamplesRead, mxDOUBLE_CLASS, mxREAL);
            
            double *ptrOutput = mxGetPr(plhs[0]);
            
            for (int i = 0; i < (int) *nSamplesRead; i++)
            {
                ptrOutput[i] = fReadArray[i];
            }
            
            mxFree(fReadArray);
            mxFree(lpChannel);
            mxFree(lpDevice);
            mxFree(nSamplesRead);
            
            DAQmxClearTask(hTask);
        }
    }
    
    return;
}








