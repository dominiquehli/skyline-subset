#ifndef _SFS_H
#define _SFS_H

#include "Common.h"

struct SPoint
{
	double dSum;
	Point Value;

	SPoint() { }
	SPoint (double dSum, Point Value) : dSum(dSum), Value(Value) { }
};


void ExecuteSFS(vector<int>& AttList, vector<Point>& PointList, vector<Point>& Skyline);


vector<Point> SortSPointList(vector<int>& AttList, vector<Point>& PointList);
int CompareSPoint(SPoint FirPoint, SPoint SecPoint);

#endif