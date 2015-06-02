/*************************************************************/
// deviceinfo.cpp
//
// Outputs the information for every installed DAQmx adapter
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
#pragma comment(lib, "NIDAQmx.lib")

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
   if ( nrhs > 0)
   {
       mexErrMsgTxt("Too many input arguments.");
   }
   else
   {
       mexPrintf("\n\n");
       int nSize = DAQmxGetSysDevNames(NULL, 0);
       char *lpTemp = (char*) mxMalloc(nSize);
       DAQmxGetSysDevNames(lpTemp, nSize);
       
       if (lpTemp == NULL)
       {
           mexErrMsgTxt("There are no installed devices.");
       }
       else
       {
           mexPrintf("  - InstalledDevices: '%s'\n", lpTemp);
       }
       
       mxFree(lpTemp);
       mexPrintf("\n\n");
   }
   
   return;
}