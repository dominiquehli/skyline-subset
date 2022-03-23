#ifndef _UTILITY_H
#define _UTILITY_H

#include "Common.h"

bool CompareSkyline(vector<Point>& Skyline1, vector<Point>& Skyline2);

void CopyPointList(int nNumAtt, vector<Point>& PointList, vector<Point>& CPointList);
void ClearPointList(vector<Point>& PointList);

void PrintPointList(int nNumAtt, vector<Point>& PointList);
void PrintSkyline(vector<Point>& Skyline);

#endif
