#ifndef INSERT_SKYTREE_H
#define INSERT_SKYTREE_H

#include "Common.h"
#include "BSkyTreeP.h"

void InsertSkyTree(vector<int>& AttList, Point NewPoint, SNode& SkyTree);

bool CheckDominance(vector<int>& AttList, Point NewPoint, SNode& SkyTree);

void TraverseSkyTree(bool& bUpdate, vector<int>& AttList, int nPLattice, Point NewPoint, SNode& SkyTree);
void InsertSkyNode(int nLattice, Point NewPoint, int nPos, SNode& SkyTree);

void UpdateSkyTree(vector<int>& AttList, Point NewPoint, SNode& SkyTree);
void UpdateSkyTree(vector<int>& AttList, Point NewPoint, int& nPos, SNode& SkyTree);

int DominanceTest(vector<int>& AttList, Point CPoint, Point SkyPoint);
void DominanceTest(vector<int>& AttList, vector<Point>& CPointList, Point SkyPoint);

#endif