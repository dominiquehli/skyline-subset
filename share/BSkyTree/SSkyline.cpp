#include "SSkyline.h"

void ExecuteSSkyline(vector<int>& AttList, vector<Point>& PointList, vector<Point>& Skyline)
{
	bool bDominate, bDominated;
	
	int nCurAtt, nNumAtt = AttList.size();
	int nCurPos, nHead = 0, nTail = PointList.size() - 1;
	Point HeadPoint, CurPoint;

	while (nHead < nTail)
	{
		nCurPos = nHead + 1;
		HeadPoint = PointList[nHead];
		while (nCurPos <= nTail)
		{
			bDominate = false;
			bDominated = false;
			CurPoint = PointList[nCurPos];

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

			// If the object in nHead dominates another object in nCurPos, exchange the current object into the last object in nTail.
			if (bDominate && !bDominated)
			{
				Point Temp = PointList[nCurPos];
				PointList[nCurPos] = PointList[nTail];
				PointList[nTail] = Temp;
				
				//PointList.pop_back();
				nTail--;
			}
			// If the object in nHead is dominated by another object in nCurPos, exchange the head object, the current object, and the last object each other.
			else if (!bDominate && bDominated)
			{
				Point Temp = PointList[nHead];
				PointList[nHead] = PointList[nCurPos];
				PointList[nCurPos] = PointList[nTail];
				PointList[nTail] = Temp;
				HeadPoint = PointList[nHead];

				//HeadPoint = PointList[nHead] = PointList[nCurPos];
				//PointList[nCurPos] = PointList[nTail];
				//PointList.pop_back();
				nTail--;

				nCurPos = nHead + 1;
			}
			else
				nCurPos++;
		}

		nHead++;
	}

	for (int nPnt = 0;  nPnt <= nTail; nPnt++)
		Skyline.push_back(PointList[nPnt]);
}




