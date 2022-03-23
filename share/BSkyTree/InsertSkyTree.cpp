#include "InsertSkyTree.h"


void InsertSkyTree(vector<int>& AttList, Point NewPoint, SNode& SkyTree)
{
	if (!CheckDominance(AttList, NewPoint, SkyTree))	// Check if the new point is dominated by existing skyline points.
		return;

	int nLattice = DominanceTest(AttList, NewPoint, SkyTree.NodePointList[0]);
	if (nLattice == 0)
		UpdateSkyTree(AttList, NewPoint, SkyTree);	// If the node is a root node, reconstruct the skytree.
	else	// Otherwise, update the skytree partially.
	{
		bool bUpdate = true;
		TraverseSkyTree(bUpdate, AttList, nLattice, NewPoint, SkyTree);
	}
}



bool CheckDominance(vector<int>& AttList, Point NewPoint, SNode& SkyTree)
{
	int nPruned = (1 << AttList.size()) - 1;
	int nLattice = DominanceTest(AttList, NewPoint, SkyTree.NodePointList[0]);

	if (nLattice == 0)
		return true;
	else if (nLattice < nPruned)
	{
		if (SkyTree.nNumChild > 0)
		{
			int nCLattice, nNumChild = SkyTree.nNumChild;
			for (int nChild = 0; nChild < nNumChild; nChild++)
			{
				nCLattice = SkyTree.ChildNode[nChild].nLatticeID;
				if (nCLattice <= nLattice)
				{
					if ((nCLattice & nLattice) == nCLattice)
					{
						if (!CheckDominance(AttList, NewPoint, SkyTree.ChildNode[nChild]))
							return false;
					}
				}
				else
					break;
			}
		}

		return true;
	}
	else
		return false;
}


void TraverseSkyTree(bool& bUpdate, vector<int>& AttList, int nPLattice, Point NewPoint, SNode& SkyTree)
{
	for (int nChild = 0; nChild < SkyTree.nNumChild; nChild++)
	{
		int nCLattice = SkyTree.ChildNode[nChild].nLatticeID;
		if (nCLattice == nPLattice)
		{
			Point SkyPoint = SkyTree.ChildNode[nChild].NodePointList[0];
			int nLattice = DominanceTest(AttList, NewPoint, SkyPoint);

			if (nLattice == 0)
			{
				if (bUpdate)
				{
					bUpdate = false;
					UpdateSkyTree(AttList, NewPoint, SkyTree.ChildNode[nChild]);
				}
				else
					UpdateSkyTree(AttList, NewPoint, nChild, SkyTree);
			}
			else	
				TraverseSkyTree(bUpdate, AttList, nLattice, NewPoint, SkyTree.ChildNode[nChild]);
		}
		else if (nCLattice > nPLattice)
		{
			if (bUpdate)
			{
				///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
				bUpdate = false;
				InsertSkyNode(nPLattice, NewPoint, nChild, SkyTree);
				nChild++;
				///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			}

			if((nCLattice & nPLattice) == nPLattice)
			{
				Point SkyPoint = SkyTree.ChildNode[nChild].NodePointList[0];
				int nLattice = DominanceTest(AttList, NewPoint, SkyPoint);

				if (nLattice == 0)
				{
					if (bUpdate)
					{
						bUpdate = false;
						UpdateSkyTree(AttList, NewPoint, SkyTree.ChildNode[nChild]);
					}
					else
						UpdateSkyTree(AttList, NewPoint, nChild, SkyTree);
				}
				else
					TraverseSkyTree(bUpdate, AttList, nLattice, NewPoint, SkyTree.ChildNode[nChild]);
			}
		}
	}

	if (bUpdate)
	{
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		bUpdate = false;
		InsertSkyNode(nPLattice, NewPoint, SkyTree.nNumChild, SkyTree);
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	}
}



void InsertSkyNode(int nLattice, Point NewPoint, int nPos, SNode& SkyTree)
{
	// Make a new node.
	SNode NewNode;
	NewNode.NodePointList.push_back(NewPoint);
	NewNode.nLatticeID = nLattice;
	NewNode.nNumChild = 0;

	SkyTree.nNumChild++;
	SNode* TempNode = SkyTree.ChildNode;
	SkyTree.ChildNode = new SNode[SkyTree.nNumChild];

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	for (int nChild = 0; nChild < nPos; nChild++)
		SkyTree.ChildNode[nChild] = TempNode[nChild];

	// Add a new node in the location of the pos.
	SkyTree.ChildNode[nPos] = NewNode;

	for (int nChild = nPos + 1; nChild < SkyTree.nNumChild; nChild++)
		SkyTree.ChildNode[nChild] = TempNode[nChild - 1];
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	if (SkyTree.nNumChild > 1)
		delete[] TempNode;
}


void UpdateSkyTree(vector<int>& AttList, Point NewPoint, SNode& SkyTree)
{
	int nPrevLattice = SkyTree.nLatticeID;

	vector<Point> PointList;
	PointList.push_back(NewPoint);
	InsertSkyline(PointList, SkyTree);
	ClearSkyTree(SkyTree);

	ExecuteBSkyTreeP(AttList, PointList, SkyTree, DIST);
	SkyTree.nLatticeID = nPrevLattice;
}



void UpdateSkyTree(vector<int>& AttList, Point NewPoint, int& nPos, SNode& SkyTree)
{
	int nPrevLattice = SkyTree.ChildNode[nPos].nLatticeID;

	vector<Point> CPointList;
	InsertSkyline(CPointList, SkyTree.ChildNode[nPos]);
	ClearSkyTree(SkyTree.ChildNode[nPos]);

	DominanceTest(AttList, CPointList, NewPoint);
	if (CPointList.size() > 0)
	{
		ExecuteBSkyTreeP(AttList, CPointList, SkyTree.ChildNode[nPos], DIST);
		SkyTree.ChildNode[nPos].nLatticeID = nPrevLattice;
	}
	else
	{
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		for (int nChild = nPos; nChild < SkyTree.nNumChild - 1; nChild++)
			SkyTree.ChildNode[nChild] = SkyTree.ChildNode[nChild + 1];
		
		SkyTree.nNumChild--, nPos--;
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	}
}


int DominanceTest(vector<int>& AttList, Point CPoint, Point SkyPoint)
{
	int nCurAtt, nNumAtt = AttList.size();
	int nPruned = (1 << nNumAtt) - 1;
	int nLattice = 0, nEqlLattice = nPruned;

	for (int nAttID = 0; nAttID < nNumAtt; nAttID++)
	{
		nCurAtt = AttList[nAttID];
		if (SkyPoint[nCurAtt] < CPoint[nCurAtt])
			nLattice |= 1 << nAttID;
		else if (SkyPoint[nCurAtt] == CPoint[nCurAtt])
		{
			nLattice |= 1 << nAttID;
			nEqlLattice ^= 1 << nAttID;
		}
	}

	if ((nLattice & nEqlLattice) == 0)
		nLattice = 0;

	return nLattice;
}


void DominanceTest(vector<int>& AttList, vector<Point>& CPointList, Point SkyPoint)
{
	int nNumPnt = CPointList.size();
	int nAttID, nCurAtt, nNumAtt = AttList.size();

	for (int nPnt = 0; nPnt < nNumPnt; nPnt++)
	{
		Point CPoint = CPointList[nPnt];
		for (nAttID = 0; nAttID < nNumAtt; nAttID++)
		{
			nCurAtt = AttList[nAttID];
			if (CPoint[nCurAtt] < SkyPoint[nCurAtt])
				break;
		}

		if (nAttID == nNumAtt)
		{
			CPointList[nPnt] = CPointList[nNumPnt - 1];
			CPointList.pop_back();
			nNumPnt--, nPnt--;
		}
	}
}