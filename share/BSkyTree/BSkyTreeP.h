#ifndef  _BSKYTREEP_H
#define _BSKYTREEP_H

#include "Common.h"
#include "SkyTreeUtil.h"
#include "PivotSelection.h"

void ExecuteBSkyTreeP(vector<int>& AttList, vector<Point>& PointList, SNode& SkyTree, ALG nType);
void ExecuteBSkyTreeP(vector<int>& AttList, vector<Point>& PointList, vector<Point>& Skyline, ALG nType);

void ComputeSubBSkyTree(vector<int>& AttList, vector<Point>& PointList, vector<double>& MinList, vector<double>& MaxList, SNode& SkyTree, ALG nType);

void MapPointToRegion(vector<int>& AttList, vector<Point>& PointList, map<int, vector<Point>>& PointMap, SNode& SkyTree);
void PartialDominance(vector<int>& AttList, int nBase, vector<Point>& PointList, SNode& SkyTree);
bool FilterPoint(Point& CPoint, vector<int>& AttList, SNode& SkyNode);

#endif