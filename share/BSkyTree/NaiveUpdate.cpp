#include "NaiveUpdate.h"

void InsertNaive(vector<int>& AttList, Point NewPoint, SNode& SkyTree)
{
	if (!CheckDominance(AttList, NewPoint, SkyTree))
		return;

	vector<Point> PointList;
	PointList.push_back(NewPoint);
	InsertSkyline(PointList, SkyTree);
	ClearSkyTree(SkyTree);

	ExecuteBSkyTreeP(AttList, PointList, SkyTree, DIST);
}


void DeleteNaive(vector<int>& AttList, vector<Point>& PointList, Point DelPoint, SNode& SkyTree)
{
	if (!CheckExistence(AttList, DelPoint, SkyTree))
		return;

	vector<Point> CPointList;
	InsertSkyline(CPointList, SkyTree);
	ClearSkyTree(SkyTree);

	// Delete DelPoint from CPointList.
	int nNumPnt = CPointList.size();
	for (int nPnt = 0; nPnt < nNumPnt; nPnt++)
	{
		if (CPointList[nPnt][0] == DelPoint[0])
		{
			CPointList[nPnt] = CPointList[nNumPnt - 1];
			CPointList.pop_back();
			break;
		}
	}
	
	// Add DPointList to CPointList.
	vector<Point> DPointList = FindDPointList(AttList, PointList, DelPoint);
	vector<Point>::iterator it = CPointList.end();
	CPointList.insert(it, DPointList.begin(), DPointList.end());

	ExecuteBSkyTreeP(AttList, CPointList, SkyTree, DIST);
}


bool CheckExistence(vector<int>& AttList, Point DelPoint, SNode& SkyTree)
{
	int nLattice = DominanceTest(AttList, DelPoint, SkyTree.NodePointList[0]);
	int nPruned = (1 << AttList.size()) - 1;

	if (nLattice == 0)
		return true;
	else if (nLattice == nPruned)
		return false;
	else
	{
		for (int nChild = 0; nChild < SkyTree.nNumChild; nChild++)
		{
			int nCLattice = SkyTree.ChildNode[nChild].nLatticeID;
			if (nCLattice <= nLattice)
			{
				if ((nCLattice & nLattice) == nCLattice)
					return CheckExistence(AttList, DelPoint, SkyTree.ChildNode[nChild]);
			}
		}

		return true;
	}
}

