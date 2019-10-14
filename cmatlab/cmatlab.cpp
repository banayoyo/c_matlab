// cmatlab.cpp : 定义控制台应用程序的入口点。
//
#include "stdafx.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "engine.h"		// add header file

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
	int Nsample = 50;
	const double PI = 3.1415926;
	double *t = new double[Nsample];

	for (int i = 0; i < Nsample; i++)
	{
		t[i] = i * 2 * PI / Nsample;
	}

	mxArray *T = NULL, *result = NULL;
	T = mxCreateDoubleMatrix(1, Nsample, mxREAL);
	memcpy((void *)mxGetPr(T), (void *)t, Nsample * sizeof(t[0]));

	engPutVariable(ep, "T", T);			// put data to engine

										// execute matlab operations
	engEvalString(ep, "Y=sin(T);");
	engEvalString(ep, "plot(T,Y);");
	engEvalString(ep, "title('y=sin(t)');");
	engEvalString(ep, "xlabel('t');");
	engEvalString(ep, "ylabel('y');");

	printf("Hit return to continue\n");
	fgetc(stdin);

	// clean operation(don't forget!!!)
	mxDestroyArray(T);
	engEvalString(ep, "close;");

	// close engine
	engClose(ep);

	return EXIT_SUCCESS;
}