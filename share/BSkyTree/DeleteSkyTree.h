#ifndef DELETE_SKYTREE_H
#define DELETE_SKYTREE_H

#include "Common.h"
#include "BSkyTreeP.h"
#include "InsertSkyTree.h"

void DeleteSkyTree(vector<int>& AttList, vector<Point>& PointList, Point DelPoint, SNode& SkyTree);

void TraverseSkyTree(vector<int>& AttList, int nPLattice, Point DelPoint, SNode& SkyTree);
bool DeleteSkyNode(vector<int>& AttList, Point DelPoint, SNode& SkyTree);

void UpdateSkyTree(vector<int>& AttList, vector<Point>& PointList, Point DelPoint, SNode& SkyTree);
vector<Point> FindDPointList(vector<int>& AttList, vector<Point>& PointList, Point DelPoint);

#endif

