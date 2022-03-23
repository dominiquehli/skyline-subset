#include "BSkyTreeP.h"



void ExecuteBSkyTreeP(vector<int>& AttList, vector<Point>& PointList, SNode& SkyTree, ALG nType)
{
	SkyTree.nLatticeID = 0;
	SkyTree.NodePointList.clear();

	vector<Point> CPointList = PointList;

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	int nNumAtt = (int)AttList.size();
	vector<double> MinList(nNumAtt), MaxList(nNumAtt);
	for (int nAttID = 0; nAttID < nNumAtt; nAttID++)
		MinList[nAttID] = 0.0, MaxList[nAttID] = 1.0;
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	ComputeSubBSkyTree(AttList, CPointList, MinList, MaxList, SkyTree, nType);
}



void ExecuteBSkyTreeP(vector<int>& AttList, vector<Point>& PointList, vector<Point>& Skyline, ALG nType)
{
	SNode SkyTree;
	SkyTree.nLatticeID = 0;
	SkyTree.NodePointList.clear();

	vector<Point> CPointList = PointList;

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	int nNumAtt = (int)AttList.size();
	vector<double> MinList(nNumAtt), MaxList(nNumAtt);
	for (int nAttID = 0; nAttID < nNumAtt; nAttID++)
		MinList[nAttID] = 0.0, MaxList[nAttID] = 1.0;
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	ComputeSubBSkyTree(AttList, CPointList, MinList, MaxList, SkyTree,  nType);
	InsertSkyline(Skyline, SkyTree);
	ClearSkyTree(SkyTree);
}




void ComputeSubBSkyTree(vector<int>& AttList, vector<Point>& PointList, vector<double>& MinList, vector<double>& MaxList, SNode& SkyTree, ALG nType)
{
	SelectPivotPoint(AttList, PointList, MinList, MaxList, nType);					// Pivot selection

	map<int, vector<Point>> PointMap;
	MapPointToRegion(AttList, PointList, PointMap, SkyTree);		// Map points to binary vectors representing subregions.

	if (!PointMap.empty())
		SkyTree.ChildNode = new SNode[PointMap.size()];

	int nNumChild = 0;
	for (map<int, vector<Point>>::iterator it = PointMap.begin(); it != PointMap.end(); it++)
	{
		int nLatticeID = (*it).first;
		vector<Point> CPointList = (*it).second;

		if (nNumChild > 0)
		{
			SkyTree.nNumChild = nNumChild;
			PartialDominance(AttList, nLatticeID, CPointList, SkyTree);			// Partial dominance check
		}

		if (CPointList.size() == 1)
		{
			SkyTree.ChildNode[nNumChild].nLatticeID = nLatticeID;
			SkyTree.ChildNode[nNumChild].NodePointList = CPointList;
			SkyTree.ChildNode[nNumChild++].nNumChild = 0;
		}
		else if (CPointList.size() > 1)
		{
			SkyTree.ChildNode[nNumChild].nLatticeID = nLatticeID;

			///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			int nNumAtt = AttList.size();
			vector<double> NewMinList(nNumAtt), NewMaxList(nNumAtt);
			bitset<BITSET> LatticeBit = nLatticeID;
			for (int nAttID = 0; nAttID < nNumAtt; nAttID++)
			{
				if (LatticeBit[nAttID])
					NewMinList[nAttID] = PointList[0][AttList[nAttID]], NewMaxList[nAttID] = MaxList[nAttID];
				else
					NewMinList[nAttID] = MinList[nAttID], NewMaxList[nAttID] = PointList[0][AttList[nAttID]];
			}
			///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

			ComputeSubBSkyTree(AttList, CPointList, NewMinList, NewMaxList, SkyTree.ChildNode[nNumChild++], nType);	// Recursive call.
		}
	}

	SkyTree.nNumChild = nNumChild;
}



void MapPointToRegion(vector<int>& AttList, vector<Point>& PointList, map<int, vector<Point>>& PointMap, SNode& SkyTree)
{
	int nCurAtt, nNumAtt = (int)AttList.size();
	int nNumPnt = (int)PointList.size();
	int nLattice, nEqlLattice, nPruned = (1 << nNumAtt)  - 1;

	Point CPoint, BasisPoint = PointList[0];
	SkyTree.NodePointList.push_back(PointList[0]);

	for (int nPnt = 1; nPnt < nNumPnt; nPnt++)
	{
#ifdef MEASURE
		nGMeasure++;
#endif

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		nLattice = 0, nEqlLattice = nPruned;
		CPoint = PointList[nPnt];
		for (int nAttID = 0; nAttID < nNumAtt; nAttID++)
		{
			nCurAtt = AttList[nAttID];
			if (BasisPoint[nCurAtt] < CPoint[nCurAtt])
				nLattice |= 1 << nAttID;
			else if (BasisPoint[nCurAtt] == CPoint[nCurAtt])
			{
				nLattice |= 1 << nAttID;
				nEqlLattice ^= 1 << nAttID;
			}
		}
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		if (nLattice < nPruned)
		{
			if (PointMap.find(nLattice) != PointMap.end())
				PointMap[nLattice].push_back(PointList[nPnt]);
			else
			{
				vector<Point> CPointList;
				CPointList.push_back(PointList[nPnt]);
				PointMap.insert( pair<int, vector<Point>>(nLattice, CPointList));
			}
		}
		else if (nEqlLattice == 0)
			SkyTree.NodePointList.push_back(PointList[nPnt]);
	}
}



void PartialDominance(vector<int>& AttList, int nBase, vector<Point>& PointList, SNode& SkyTree)
{
	int nCLattice, nNumAtt = (int)AttList.size();
	int nNumPnt, nNumChild = SkyTree.nNumChild;

	for (int nChild = 0; nChild < nNumChild; nChild++)
	{
		nCLattice = SkyTree.ChildNode[nChild].nLatticeID;
		if (nCLattice <= nBase)
		{
			if ((nCLattice & nBase) == nCLattice)
			{
				nNumPnt = (int)PointList.size();
				for (int nPnt = 0; nPnt < nNumPnt; nPnt++)
				{
					if (!FilterPoint(PointList[nPnt], AttList, SkyTree.ChildNode[nChild]))
					{
						PointList[nPnt] = PointList[nNumPnt-1];
						PointList.pop_back();

						nPnt--, nNumPnt--;
					}
				}

				if (PointList.size() == 0)
					break;
			}
		}
		else
			break;
	}
}




bool FilterPoint(Point& CPoint, vector<int>& AttList, SNode& SkyNode)
{
	int nCurAtt, nNumAtt = AttList.size();
	Point SPoint = SkyNode.NodePointList[0];

#ifdef MEASURE
	nGMeasure++;
#endif

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	int nLattice = 0, nPruned = (1 << nNumAtt) - 1;
	for (int nAttID = 0; nAttID < nNumAtt; nAttID++)
	{
		nCurAtt = AttList[nAttID];
		if (SPoint[nCurAtt] <= CPoint[nCurAtt])
			nLattice |= 1 << nAttID;
	}
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	if (nLattice == nPruned)
		return false;
	else
	{
		if (SkyNode.nNumChild > 0)
		{
			int nCLattice, nNumChild = SkyNode.nNumChild;
			for (int nChild = 0; nChild < nNumChild; nChild++)
			{
				nCLattice = SkyNode.ChildNode[nChild].nLatticeID;
				if (nCLattice <= nLattice)
				{
					if ((nCLattice & nLattice) == nCLattice)
					{
						if (!FilterPoint(CPoint, AttList, SkyNode.ChildNode[nChild]))
							return false;
					}
				}
				else
					break;
			}
		}

		return true;
	}
}



