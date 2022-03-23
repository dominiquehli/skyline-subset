#ifndef  SKYBAND_H
#define SKYBAND_H

#include "Common.h"
#include "SFS.h"

struct SBPoint
{
	Point Value;
	int nNumDom;

	SBPoint() {};
	SBPoint(Point Value, int nNumDom) : Value(Value), nNumDom(nNumDom) {};
};


struct SBNode
{
	int nLatticeID;
	vector<SBPoint> NodePointList;

	int nNumChild;
	SBNode* ChildNode;

	SBNode() { }
	SBNode (int nLatticeID) : nLatticeID(nLatticeID) { }
};

void ExecuteSkyBand(vector<int>& AttList, vector<Point>& PointList, vector<SBPoint>& SkyBand);
void ComputeSubSkyBand(vector<int>& AttList, vector<SBPoint>& PointList, SBNode& SkyTree);

void SelectPivotPoint(vector<int>& AttList, vector<SBPoint>& PointList);
void MapPointToRegion(vector<int>& AttList, vector<SBPoint>& PointList, map<int, vector<SBPoint>>& PointMap, SBNode& SkyTree);
void PartialDominance(vector<int>& AttList, int nBase, vector<SBPoint>& PointList, SBNode& SkyTree);
bool FilterPoint(SBPoint& CPoint, vector<int>& AttList, SBNode& SkyNode);

vector<SBPoint> InitializePointList(vector<Point>& PointList);


void InsertSkyline(vector<SBPoint>& SkylineList, SBNode& SkyNode);
void ClearSkyTree(SBNode& SkyTree);
void PushStack(stack<SBNode>& Stack, SBNode& SkyNode);

#endif