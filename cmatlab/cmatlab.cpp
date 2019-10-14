// cmatlab.cpp : 定义控制台应用程序的入口点。
//
#include "stdafx.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "engine.h" // add header file

// import necessary lib
#pragma comment( lib, "libeng.lib")
#pragma comment( lib, "libmx.lib")
#pragma comment( lib, "libmat.lib")

int main(void)
{
    Engine *ep;

    // open engine
    if (!(ep = engOpen(NULL)))
    {
        fprintf(stderr, "\nCan't start MATLAB engine\n");
        return EXIT_FAILURE;
    }

    // generate variables
    int Nsample = 2048;
    const double PI = 3.1415926;
    double *t = new double[Nsample];

    for (int i = 0; i < Nsample; i++)
    {
        t[i] = i * 2 * PI / Nsample;
    }
    //-----------matlab variable initial----------------/
    mxArray *T = NULL, *result = NULL;
    T = mxCreateDoubleMatrix(1, Nsample, mxREAL);
    result = mxCreateDoubleMatrix(1, Nsample, mxREAL);
    //another way is <mxSetPr><mxGetPr>
    memcpy((void *)mxGetPr(T), (void *)t, Nsample * sizeof(t[0]));

    engEvalString(ep, "cd ..\\..\\MATLAB\\SSP\\VC"); // change to .m file dir
    engPutVariable(ep, "T", T); // push data to engine

    //-----------execute matlab commands----------------/
    engEvalString(ep, "sin_val = sin(T);");
    engEvalString(ep, "Y=filter_lp(sin_val);");
    engEvalString(ep, "plot(sin_val,'*-b');");
    engEvalString(ep, "hold on;");
    engEvalString(ep, "plot(Y,'.-r');");
    engEvalString(ep, "title('y=sin(t)');");
    engEvalString(ep, "xlabel('t');");
    engEvalString(ep, "ylabel('y');");

    result = engGetVariable(ep, "Y"); // pop data from engine

    //-----------hold on to show pics----------------/
    printf("Hit return to continue= %f\n",*(mxGetPr(result)+100));
    fgetc(stdin);

    //-----------post process----------------/
    // clean operation(don't forget!!!)
    mxDestroyArray(T);
    mxDestroyArray(result);
    engEvalString(ep, "close;");
    // close engine
    engClose(ep);

    return EXIT_SUCCESS;
}