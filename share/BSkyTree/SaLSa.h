#ifndef _SALSA_H
#define _SALSA_H

#include "Common.h"

struct SPoint2
{
	double dMin;
	double dSum;
	Point Value;

	SPoint2() { }
	SPoint2 (double dMin, double dSum, Point Value) : dMin(dMin), dSum(dSum), Value(Value) { }
};


void ExecuteSaLSa(vector<int>& AttList, vector<Point>& PointList, vector<Point>& Skyline);


vector<SPoint2> SortSPoint2List(vector<int>& AttList, vector<Point>& PointList);
int CompareSPoint2(SPoint2 FirPoint, SPoint2 SecPoint);

#endif