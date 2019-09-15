#pragma once
#include <cstdio>
#include <Eigen/Core>
#ifdef _WINDOWS
	#include <windows.h>		// Header File For Windows
#endif

inline void Log(const char* format, ...) {
	char msgbuf[1024];
	va_list _ArgList;
	va_start(_ArgList, format);
	vsnprintf(msgbuf, 1024, format, _ArgList);
	va_end(_ArgList);  
	sprintf_s(msgbuf, "%s\n", msgbuf);
#ifdef _WINDOWS
	OutputDebugString(msgbuf);
#endif
}
char msgbuf[1024];

inline const char* DebugString(Eigen::Matrix4f matrix) {

	float* matrixArray = new float[16];
	Eigen::Map<Eigen::Matrix4f>(matrixArray, matrix.rows(), matrix.cols()) = matrix;
	sprintf_s(msgbuf,"%f %f %f %f\n%f %f %f %f\n%f %f %f %f\n%f %f %f %f\n",
		matrixArray[0], matrixArray[1], matrixArray[2], matrixArray[3],
		matrixArray[4], matrixArray[5], matrixArray[6], matrixArray[7],
		matrixArray[8], matrixArray[9], matrixArray[10], matrixArray[11],
		matrixArray[12], matrixArray[13], matrixArray[14], matrixArray[15]
	);
	return msgbuf;
}


inline const char* DebugString(float* matrixArray) {

	sprintf_s(msgbuf, "%f %f %f %f\n%f %f %f %f\n%f %f %f %f\n%f %f %f %f\n",
		matrixArray[0], matrixArray[1], matrixArray[2], matrixArray[3],
		matrixArray[4], matrixArray[5], matrixArray[6], matrixArray[7],
		matrixArray[8], matrixArray[9], matrixArray[10], matrixArray[11],
		matrixArray[12], matrixArray[13], matrixArray[14], matrixArray[15]
	);
	return msgbuf;
}


