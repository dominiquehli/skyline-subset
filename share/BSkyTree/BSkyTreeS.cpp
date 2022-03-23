#include "BSkyTreeS.h"


void ExecuteBSkyTreeS(vector<int>& AttList, vector<Point>& PointList, vector<Point>& Skyline, ALG nType)
{
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	int nCurAtt, nNumAtt = AttList.size();
	vector<double> MinList(nNumAtt), MaxList(nNumAtt);
	for (int nAttID = 0; nAttID < nNumAtt; nAttID++)
		MinList[nAttID] = 0.0, MaxList[nAttID] = 1.0;
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	SelectPivotPoint(AttList, PointList, MinList, MaxList, nType);		// Pivot selection

	vector<LPoint> LPointList;
	MapPointToRegion(AttList, PointList, LPointList);			// Map points to binary vectors representing subregions.

	bool bDominate, bDominated;
	int nHead = 0, nCurPos, nTail = LPointList.size() - 1;
	int nHeadID, nCurID;
	Point HeadPoint, CurPoint;

	while (nHead < nTail)
	{
		if (LPointList[nHead].nLatticeID == 0)
		{
			nHead++;
			continue;
		}

		HeadPoint = LPointList[nHead].Value;
		nHeadID = LPointList[nHead].nLatticeID;

		nCurPos = nHead + 1;
		while (nCurPos <= nTail)
		{
			nCurID = LPointList[nCurPos].nLatticeID;
			if (((nCurID & nHeadID) == nHeadID) || ((nCurID & nHeadID) == nCurID))
			{
				bDominate = false;
				bDominated = false;

				CurPoint = LPointList[nCurPos].Value;

#ifdef MEASURE
				nGMeasure++;
#endif

				///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
				for (int nAttID = 0; nAttID < nNumAtt; nAttID++)
				{
					nCurAtt = AttList[nAttID];		// Set the current attribute from the attribute list.
					if (HeadPoint[nCurAtt] < CurPoint[nCurAtt])
						bDominate = true;
					else if (HeadPoint[nCurAtt] > CurPoint[nCurAtt])
						bDominated = true;

					if (bDominate && bDominated)
						break;
				}
				///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

				if (bDominate && !bDominated)
				{
					LPointList[nCurPos] = LPointList[nTail];
					LPointList.pop_back();
					nTail--;
				}
				else if (!bDominate && bDominated)
				{
					LPointList[nHead] = LPointList[nCurPos];
					LPointList[nCurPos] = LPointList[nTail];
					LPointList.pop_back();
					nTail--;

					HeadPoint = LPointList[nHead].Value;
					nHeadID = LPointList[nHead].nLatticeID;
					nCurPos = nHead + 1;
				}
				else
					nCurPos++;
			}
			else
				nCurPos++;
		}

		nHead++;
	}

	for (int nPnt = 0;  nPnt <= nTail; nPnt++)
		Skyline.push_back(LPointList[nPnt].Value);
}


void MapPointToRegion(vector<int>& AttList, vector<Point>& PointList, vector<LPoint>& LPointList)
{
	int nCurAtt, nNumAtt = (int)AttList.size();
	int nNumPnt = (int)PointList.size();
	int nLattice, nEqlLattice, nPruned = (1 << nNumAtt)  - 1;

	Point CPoint, BasisPoint = PointList[0];
	for (int nPnt = 0; nPnt < nNumPnt; nPnt++)
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
			LPointList.push_back(LPoint(CPoint, nLattice));
		else if (nEqlLattice == 0)
			LPointList.push_back(LPoint(CPoint, 0));
	}
}
