#ifndef PIVOT_SELECTION_H
#define PIVOT_SELECTION_H

#include "Common.h"

void SelectPivotPoint(vector<int>& AttList, vector<Point>& PointList, vector<double>& MinList, vector<double>& MaxList, ALG nType);

void SelectDomPivotPoint(vector<int>& AttList, vector<Point>& PointList);
void SelectRandPivotPoint(vector<int>& AttList, vector<Point>& PointList);
void SelectBalPivotPoint(vector<int>& AttList, vector<Point>& PointList, vector<double>& MinList, vector<double>& MaxList, ALG nType);

double ComputeCosine(vector<int>& AttList, Point CPoint, vector<double>& MinList, vector<double>& NormalList);
double ComputeDistance(vector<int>& AttList, Point CPoint, vector<double>& MinList, vector<double>& NormalList);
bool EvaluatePoint(vector<int>& AttList, int nCPos, vector<Point>& PointList);

#endif
