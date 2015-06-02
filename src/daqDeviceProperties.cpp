/*************************************************************/
// deviceproperties.cpp
//
// Outputs the information for the specified DAQmx device
// Created 15/5/2012
// Cesar Gonzalez Segura
/*************************************************************/
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
    if (nrhs != 1)
    {
        mexErrMsgTxt("One input required.");
    }
    else if (nlhs > 1)
    {
        mexErrMsgTxt("Too many output arguments.");
    }
    else
    {
        if (mxIsChar(prhs[0]) != 1)
        {
            mexErrMsgTxt("Input must be a string.");
        }
        else if (mxGetM(prhs[0]) !=1)
        {
            mexErrMsgTxt("Input must be a row vector.");
        }
        else
        {
            mexPrintf("\n\n");
            
            /************************************************/
            /************************************************/
            /************************************************/
            mwSize nInLen = (mxGetM(prhs[0]) * mxGetN(prhs[0])) + 1;
            char *lpDevName, *lpChannelsIn, *lpChannelsOut, *lpInput, *lpDevice, *lpHard;
            
            lpInput = mxArrayToString(prhs[0]);
            lpDevice = (char*) mxCalloc(strlen(lpInput) + 1, sizeof(char));
            strcpy(lpDevice, lpInput);
            
            mexPrintf("\nDevice '%s':\n", lpDevice);
            mxFree(lpInput);
            
            /************************************************/
            /***            DEVICE PROPERTIES             ***/
            /************************************************/
            
            // Device hardware name
            nInLen = DAQmxGetDevProductType(lpDevice, NULL, 0);
            lpHard = (char*) mxCalloc(nInLen, sizeof(char));
            DAQmxGetDevProductType(lpDevice, lpHard, nInLen);
            
            mexPrintf(" - ProductName: '%s'\n", lpHard);
            mxFree(lpHard);
            
            /************************************************/
            /***            INPUT PROPERTIES              ***/
            /************************************************/
            
            mexPrintf("\n  --- INPUT PROPERTIES ---\n\n");
            
            // Avaible analog input channels
            nInLen = DAQmxGetDevAIPhysicalChans(lpDevice, NULL, 0);
            lpChannelsIn = (char*) mxCalloc(nInLen, sizeof(char));
            DAQmxGetDevAIPhysicalChans(lpDevice, lpChannelsIn, nInLen);
            
            for (int i = 0; i < strlen(lpChannelsIn); i++)
            {
                if (lpChannelsIn[i] == ',')
                {
                    lpChannelsIn[i] = '\n';
                }
            }
            
            mexPrintf(" - Analog input physical channels: \n %s\n", lpChannelsIn);
            mxFree(lpChannelsIn);
            
            // Avaible input modes
            nInLen = (int) DAQmxGetDevAISupportedMeasTypes(lpDevice, NULL, 0);
            int32 *nSupportedMeasureIn = (int32*) mxCalloc(nInLen, sizeof(int32));
            DAQmxGetDevAISupportedMeasTypes(lpDevice, nSupportedMeasureIn, nInLen);
            
            mexPrintf(" - Supported input modes:\n");
            
            for (int i = 0; i < nInLen; i++)
            {
                switch (nSupportedMeasureIn[i])
                {
                    case DAQmx_Val_Voltage:
                        mexPrintf("Voltage measurement\n");
                        break;
                    case DAQmx_Val_VoltageRMS:
                        mexPrintf("RMS Voltage measurement\n");
                        break;
                    case DAQmx_Val_Current:
                        mexPrintf("Current measurement\n");
                        break;
                    case DAQmx_Val_CurrentRMS:
                        mexPrintf("RMS Current measurement\n");
                        break;
                    case DAQmx_Val_Resistance:
                        mexPrintf("Resistance measurement\n");
                        break;
                }
            }
            
            mxFree(nSupportedMeasureIn);
            
            // Maximum sample rate in single channel mode
            
            float64 *fMaxSChanRate = (float64*) mxMalloc(sizeof(float64));
            DAQmxGetDevAIMaxSingleChanRate(lpDevice, fMaxSChanRate);
            mexPrintf(" - Maximum single-channel sample rate: %f kS/s\n", *fMaxSChanRate);
            mxFree(fMaxSChanRate);
            
            // Maximum sample rate in multi channel mode
            
            float64 *fMaxMChanRate = (float64*) mxMalloc(sizeof(float64));
            DAQmxGetDevAIMaxMultiChanRate(lpDevice, fMaxMChanRate);
            mexPrintf(" - Maximum multiple-channel sample rate: %f kS/s\n", *fMaxMChanRate);
            mxFree(fMaxMChanRate);
            
            // Sampling modes supported
            
            mexPrintf(" - Input sampling supported properties:\n");
            
            nInLen = DAQmxGetDevAISampModes(lpDevice, NULL, 0);
            int32 *nSampleModesIn = (int32*) mxCalloc(nInLen, sizeof(int32));
            DAQmxGetDevAISampModes(lpDevice, nSampleModesIn, nInLen);
            
            for (int i = 0; i < nInLen; i++)
            {
                switch (nSampleModesIn[i])
                {
                    case DAQmx_Val_FiniteSamps:
                        mexPrintf("Adquiring a finite number of samples\n");
                        break;
                    case DAQmx_Val_ContSamps:
                        mexPrintf("Adquiring samples continiously until stop\n");
                        break;
                }
            }
            
            mxFree(nSampleModesIn);
            
            bool32 *bMultiInSupport = (bool32*) mxMalloc(sizeof(bool32));
            DAQmxGetDevAISimultaneousSamplingSupported(lpDevice, bMultiInSupport);
            
            if (*bMultiInSupport > 0)
            {
                mexPrintf("Adquiring samples from multiple channels simultaneously supported\n");
            }
            
            mxFree(bMultiInSupport);
            
            /************************************************/
            /***            OUTPUT PROPERTIES             ***/
            /************************************************/
            
            mexPrintf("\n  --- OUTPUT PROPERTIES ---\n\n");
            
            // Avaible analog output channels
            nInLen = DAQmxGetDevAOPhysicalChans(lpDevice, NULL, 0);
            lpChannelsOut = (char*) mxCalloc(nInLen, sizeof(char));
            DAQmxGetDevAOPhysicalChans(lpDevice, lpChannelsOut, nInLen);
            
            for (int i = 0; i < strlen(lpChannelsOut); i++)
            {
                if (lpChannelsOut[i] == ',')
                {
                    lpChannelsOut[i] = '\n';
                }
            }
            
            mexPrintf(" - Analog output physical channels: \n %s\n", lpChannelsOut);
            mxFree(lpChannelsOut);
            
            // Avaible output modes
            nInLen = (int) DAQmxGetDevAOSupportedOutputTypes(lpDevice, NULL, 0);
            int32 *nSupportedMeasureOut = (int32*) mxCalloc(nInLen, sizeof(int32));
            DAQmxGetDevAOSupportedOutputTypes(lpDevice, nSupportedMeasureOut, nInLen);
            
            mexPrintf(" - Supported output modes:\n");
            
            for (int i = 0; i < nInLen; i++)
            {
                switch (nSupportedMeasureOut[i])
                {
                    case DAQmx_Val_Voltage:
                        mexPrintf("Voltage output\n");
                        break;
                    case DAQmx_Val_Current:
                        mexPrintf("Current output\n");
                        break;
                    case DAQmx_Val_FuncGen:
                        mexPrintf("In-device function generation output\n");
                        break;
                }
            }
            
            mxFree(nSupportedMeasureOut);
            
            // Device supports using a sample clock
            bool32* bSampleClk = (bool32*) mxMalloc(sizeof(bool32));
            DAQmxGetDevAOSampClkSupported(lpDevice, bSampleClk);
            
            if (*bSampleClk > 0)
            {
                mexPrintf(" - Device supports using an output sample clock\n");
            }
            
            mxFree(bSampleClk);
            
            // Supported output sampling modes
            nInLen = DAQmxGetDevAOSampModes(lpDevice, NULL, 0);
            int32 *nSampleModesOut = (int32*) mxCalloc(nInLen, sizeof(int32));
            DAQmxGetDevAOSampModes(lpDevice, nSampleModesOut, nInLen);
            
            mexPrintf(" - Device supported output sampling modes:\n");
            
            for (int i = 0; i < nInLen; i++)
            {
                switch (nSampleModesOut[i])
                {
                    case DAQmx_Val_FiniteSamps:
                        mexPrintf("Outputting a finite number of samples\n");
                        break;
                    case DAQmx_Val_ContSamps:
                        mexPrintf("Outputting continiously samples until stop\n");
                        break;
                }
            }
            
            mxFree(nSampleModesOut);
            
            // Maximum output sample rate
            
            float64 *fMaxOutRate = (float64*) mxMalloc(sizeof(float64));
            DAQmxGetDevAOMaxRate(lpDevice, fMaxOutRate);
            
            mexPrintf(" - Maximum output sample rate: %f kS/s\n", *fMaxOutRate);
            mxFree(fMaxOutRate);
            
            /************************************************/
            /************************************************/
            /************************************************/
            
            // Cleaning out
            mxFree(lpDevice);
            mexPrintf("\n\n");
        }
    }
    
    return;
}









