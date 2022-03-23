#ifndef  _BSKYTREES_H
#define _BSKYTREES_H

#include "Common.h"
#include "PivotSelection.h"

struct LPoint
{
	Point Value;
	int nLatticeID;

	LPoint() { }
	LPoint (Point Value, int nLatticeID) : Value(Value), nLatticeID(nLatticeID) { }
};


void ExecuteBSkyTreeS(vector<int>& AttList, vector<Point>& PointList, vector<Point>& Skyline, ALG nType);
void MapPointToRegion(vector<int>& AttList, vector<Point>& PointList, vector<LPoint>& LPointList);

#endif