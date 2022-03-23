#include "SkyBand.h"


void ExecuteSkyBand(vector<int>& AttList, vector<Point>& PointList, vector<SBPoint>& SkyBand)
{
	SBNode SkyTree;
	SkyTree.nLatticeID = 0;

	vector<SBPoint> SBPointList = InitializePointList(SortSPointList(AttList, PointList));

	ComputeSubSkyBand(AttList, SBPointList, SkyTree);
	InsertSkyline(SkyBand, SkyTree);
	ClearSkyTree(SkyTree);
}


void ComputeSubSkyBand(vector<int>& AttList, vector<SBPoint>& PointList, SBNode& SkyTree)
{
	SelectPivotPoint(AttList, PointList);

	map<int, vector<SBPoint>> PointMap;
	MapPointToRegion(AttList, PointList, PointMap, SkyTree);		// Map points to binary vectors representing subregions.

	if (!PointMap.empty())
		SkyTree.ChildNode = new SBNode[PointMap.size()];

	int nNumChild = 0;
	for (map<int, vector<SBPoint>>::iterator it = PointMap.begin(); it != PointMap.end(); it++)
	{
		int nLatticeID = (*it).first;
		vector<SBPoint> CPointList = (*it).second;

		if (nNumChild > 0)
		{
			SkyTree.nNumChild = nNumChild;
			PartialDominance(AttList, nLatticeID, CPointList, SkyTree);			// Partial dominance check
		}

		if (CPointList.size() == 1)
		{
			SkyTree.ChildNode[nNumChild].nLatticeID = nLatticeID;
			SkyTree.ChildNode[nNumChild].NodePointList.push_back(CPointList[0]);
			SkyTree.ChildNode[nNumChild++].nNumChild = 0;
		}
		else if (CPointList.size() > 1)
		{
			SkyTree.ChildNode[nNumChild].nLatticeID = nLatticeID;
			ComputeSubSkyBand(AttList, CPointList, SkyTree.ChildNode[nNumChild++]);	// Recursive call.
		}
	}

	SkyTree.nNumChild = nNumChild;
}


void SelectPivotPoint(vector<int>& AttList, vector<SBPoint>& PointList)
{
	int nPivotPos, nNumAtt = AttList.size(), nNumPnt = PointList.size();
	double dSum, dMinSum = nNumAtt;

	for (int nPnt = 0; nPnt < nNumPnt; nPnt++)
	{
		SBPoint CPoint = PointList[nPnt];

		dSum = CPoint.Value[AttList[0]];
		for (int nAttID = 1; nAttID < nNumAtt; nAttID++)
			dSum += CPoint.Value[AttList[nAttID]];

		if (dSum < dMinSum)
		{
			dMinSum = dSum;
			nPivotPos = nPnt;
		}
	}

	SBPoint Temp = PointList[0];
	PointList[0] = PointList[nPivotPos];
	PointList[nPivotPos] = Temp;
}


void MapPointToRegion(vector<int>& AttList, vector<SBPoint>& PointList, map<int, vector<SBPoint>>& PointMap, SBNode& SkyTree)
{
	int nCurAtt, nNumAtt = (int)AttList.size();
	int nNumPnt = (int)PointList.size();
	int nLattice, nEqlLattice, nPruned = (1 << nNumAtt)  - 1;

	Point CPoint, BasisPoint = PointList[0].Value;
	SkyTree.NodePointList.push_back(PointList[0]);

	for (int nPnt = 1; nPnt < nNumPnt; nPnt++)
	{
#ifdef MEASURE
		nGMeasure++;
#endif

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		nLattice = 0, nEqlLattice = nPruned;
		CPoint = PointList[nPnt].Value;
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

		if (nEqlLattice == 0)
			SkyTree.NodePointList.push_back(PointList[nPnt]);
		else 
		{
			///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			if (nLattice == nPruned)
		 		PointList[nPnt].nNumDom++;
			///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

			if (PointList[nPnt].nNumDom <= TOP_K)
			{
				if (PointMap.find(nLattice) != PointMap.end())
					PointMap[nLattice].push_back(PointList[nPnt]);
				else
				{
					vector<SBPoint> CPointList;
					CPointList.push_back(PointList[nPnt]);
					PointMap.insert(pair<int, vector<SBPoint>>(nLattice, CPointList));
				}
			}
		}
	}
}


void PartialDominance(vector<int>& AttList, int nBase, vector<SBPoint>& PointList, SBNode& SkyTree)
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




bool FilterPoint(SBPoint& CPoint, vector<int>& AttList, SBNode& SkyNode)
{
	int nCurAtt, nNumAtt = AttList.size();
	Point SkyPoint = SkyNode.NodePointList[0].Value;

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	int nLattice = 0, nPruned = (1 << nNumAtt) - 1;
	for (int nAttID = 0; nAttID < nNumAtt; nAttID++)
	{
		nCurAtt = AttList[nAttID];
		if (SkyPoint[nCurAtt] <= CPoint.Value[nCurAtt])
			nLattice |= 1 << nAttID;
	}
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef MEASURE
	nGMeasure++;
#endif

	if (nLattice == nPruned)
	{
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		CPoint.nNumDom++;
		if (CPoint.nNumDom > TOP_K)
			return false;
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	}

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


vector<SBPoint> InitializePointList(vector<Point>& PointList)
{
	vector<SBPoint> CPointList;
	for (vector<Point>::iterator it = PointList.begin(); it != PointList.end(); it++)
		CPointList.push_back(SBPoint(*it, 0));

	return CPointList;
}


void InsertSkyline(vector<SBPoint>& SkylineList, SBNode& SkyNode)
{
	int nNumChild = (int)SkyNode.nNumChild;
	if (nNumChild > 0)
	{
		int nNumPnt = (int)SkyNode.NodePointList.size();
		for (int nPnt = 0; nPnt < nNumPnt; nPnt++)
			SkylineList.push_back(SkyNode.NodePointList[nPnt]);

		for (int nChild = 0; nChild < nNumChild; nChild++)
			InsertSkyline(SkylineList, SkyNode.ChildNode[nChild]);
	}
	else
	{
		int nNumPnt = (int)SkyNode.NodePointList.size();
		for (int nPnt = 0; nPnt < nNumPnt; nPnt++)
			SkylineList.push_back(SkyNode.NodePointList[nPnt]);
	}
}


void ClearSkyTree(SBNode& SkyTree)
{
	stack<SBNode> Stack;
	PushStack(Stack, SkyTree);

	while (!Stack.empty())
	{
		delete[] Stack.top().ChildNode;
		Stack.pop();
	}
}


void PushStack(stack<SBNode>& Stack, SBNode& SkyNode)
{
	int nNumChild = (int)SkyNode.nNumChild;
	if (nNumChild > 0)
	{
		Stack.push(SkyNode);
		for (int nChild=0; nChild<nNumChild; nChild++)
			PushStack(Stack, SkyNode.ChildNode[nChild]);
	}
}
