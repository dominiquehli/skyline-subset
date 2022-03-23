#include "DeleteSkyTree.h"


void DeleteSkyTree(vector<int>& AttList, vector<Point>& PointList, Point DelPoint, SNode& SkyTree)
{
	int nLattice = DominanceTest(AttList, DelPoint, SkyTree.NodePointList[0]);
	int nPruned = (1 << AttList.size()) - 1;

	// If the root node is a DelPoint, reconstruct the skytree.
	if (nLattice == 0)
		DeleteSkyNode(AttList, DelPoint, SkyTree);
	else if (nLattice < nPruned)		// Otherwise, traverse the skytree.
		TraverseSkyTree(AttList, nLattice, DelPoint, SkyTree);

	// Append new skyline points to the skytree.
	UpdateSkyTree(AttList, PointList, DelPoint, SkyTree);
}


void TraverseSkyTree(vector<int>& AttList, int nPLattice, Point DelPoint, SNode& SkyTree)
{
	for (int nChild = 0; nChild < SkyTree.nNumChild; nChild++)
	{
		int nCLattice = SkyTree.ChildNode[nChild].nLatticeID;
		if (nCLattice <= nPLattice)
		{
			if ((nCLattice & nPLattice) == nCLattice)
			{
				Point CPoint = SkyTree.ChildNode[nChild].NodePointList[0];
				int nLattice = DominanceTest(AttList, DelPoint, CPoint);
				int nPruned = (1 << AttList.size()) - 1;

				if (nLattice == 0)
				{
					// If the deleted node is a leaf node, update the skytree.
					if (DeleteSkyNode(AttList, DelPoint, SkyTree.ChildNode[nChild]))
					{
						///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
						for (int nChild2 = nChild; nChild2 < SkyTree.nNumChild - 1; nChild2++)
							SkyTree.ChildNode[nChild2] = SkyTree.ChildNode[nChild2 + 1];

						SkyTree.nNumChild--;
						///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
					}

					return;
				}
				else if(nLattice == nPruned)
					return;
				else
					TraverseSkyTree(AttList, nLattice, DelPoint, SkyTree.ChildNode[nChild]);
			}
		}
	}
}


bool DeleteSkyNode(vector<int>& AttList, Point DelPoint, SNode& SkyTree)
{
	vector<Point> CPointList;

	int nNumPnt = SkyTree.NodePointList.size();
	for (int nPnt = 0; nPnt < nNumPnt; nPnt++)
	{
		if (SkyTree.NodePointList[nPnt][0] == DelPoint[0])
		{
			SkyTree.NodePointList[nPnt] = SkyTree.NodePointList[nNumPnt - 1];
			SkyTree.NodePointList.pop_back();

			nNumPnt--, nPnt--;
		}
	}

	bool bLeafNode = false;
	if (SkyTree.NodePointList.size() == 0)
	{
		InsertSkyline(CPointList, SkyTree);
		ClearSkyTree(SkyTree);

		if (CPointList.size() == 0)
			bLeafNode = true;
		else
			ExecuteBSkyTreeP(AttList, CPointList, SkyTree, DIST);
	}

	return bLeafNode;
}


void UpdateSkyTree(vector<int>& AttList, vector<Point>& PointList, Point DelPoint, SNode& SkyTree)
{
	// Insert a set of dominated points with respect to DelPoint.
	vector<Point> DPointList = FindDPointList(AttList, PointList, DelPoint);
	if (DPointList.size() > 0)
	{
		vector<Point> NewSkyline;
		ExecuteBSkyTreeP(AttList, DPointList, NewSkyline, DIST);

		for (vector<Point>::iterator it = NewSkyline.begin(); it != NewSkyline.end(); it++)
			InsertSkyTree(AttList, *it, SkyTree);
	}
}


vector<Point> FindDPointList(vector<int>& AttList, vector<Point>& PointList, Point DelPoint)
{
	vector<Point> DPointList;

	int nAttID, nCurAtt, nNumAtt = AttList.size();
	int nNumPnt = PointList.size();
	for (int nPnt = 0; nPnt < nNumPnt; nPnt++)
	{
		Point CPoint = PointList[nPnt];
		
		int nEqual = 0;
		for (nAttID = 0; nAttID < nNumAtt; nAttID++)
		{
			nCurAtt = AttList[nAttID];
			if (CPoint[nCurAtt] < DelPoint[nCurAtt])
				break;
			else if (CPoint[nCurAtt] == DelPoint[nCurAtt])
				nEqual++;
		}

		if (nEqual == nNumAtt)
			continue;

		if (nAttID == nNumAtt)
			DPointList.push_back(CPoint);
	}

	return DPointList;
}