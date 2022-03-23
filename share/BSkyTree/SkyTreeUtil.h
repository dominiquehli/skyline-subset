#ifndef  _SKYTREEUTIL_H
#define _SKYTREEUTIL_H

#include "Common.h"

struct SNode
{
	int nLatticeID;
	vector<Point> NodePointList;

	int nNumChild;
	SNode* ChildNode;

	SNode() { }
	SNode (int nLatticeID) : nLatticeID(nLatticeID) { }
};


void InsertSkyline(vector<Point>& SkylineList, SNode& SkyNode);
void ClearSkyTree(SNode& SkyTree);
void PushStack(stack<SNode>& Stack, SNode& SkyNode);

void PrintSkyTree(int nHeight, SNode& SkyNode);

#endif