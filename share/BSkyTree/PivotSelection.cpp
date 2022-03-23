#include "PivotSelection.h"


void SelectPivotPoint(vector<int>& AttList, vector<Point>& PointList, vector<double>& MinList, vector<double>& MaxList, ALG nType)
{
	switch (nType)
	{
	case DOM:
			SelectDomPivotPoint(AttList, PointList);
			break;

	case RAND:
			SelectRandPivotPoint(AttList, PointList);
			break;

	case DIST:
	case COS:
			SelectBalPivotPoint(AttList, PointList, MinList, MaxList, nType);
			break;
	}
}


void SelectDomPivotPoint(vector<int>& AttList, vector<Point>& PointList)
{
	int nPivotPos, nNumAtt = AttList.size(), nNumPnt = PointList.size();
	double dSum, dMinSum = nNumAtt;

	Point CPoint;
	for (int nPnt = 0; nPnt < nNumPnt; nPnt++)
	{
		CPoint = PointList[nPnt];

		dSum = CPoint[AttList[0]];
		for (int nAttID = 1; nAttID < nNumAtt; nAttID++)
			dSum += CPoint[AttList[nAttID]];

		if (dSum < dMinSum)
		{
			dMinSum = dSum;
			nPivotPos = nPnt;
		}
	}

	Point Temp = PointList[0];
	PointList[0] = PointList[nPivotPos];
	PointList[nPivotPos] = Temp;
}


void SelectRandPivotPoint(vector<int>& AttList, vector<Point>& PointList)
{
	int nPivotPos, nNumAtt = AttList.size(), nNumPnt = PointList.size();
	double dSum, dMinSum = nNumAtt;

	vector<double> Weight(nNumAtt);
	for (int nAttID = 0; nAttID < nNumAtt; nAttID++)
		Weight[nAttID] = (double)rand() / RAND_MAX;

	Point CPoint;
	for (int nPnt = 0; nPnt < nNumPnt; nPnt++)
	{
		CPoint = PointList[nPnt];

		dSum = CPoint[AttList[0]] * Weight[0];
		for (int nAttID = 1; nAttID < nNumAtt; nAttID++)
			dSum += CPoint[AttList[nAttID]]  * Weight[nAttID];

		if (dSum < dMinSum)
		{
			dMinSum = dSum;
			nPivotPos = nPnt;
		}
	}

	Point Temp = PointList[0];
	PointList[0] = PointList[nPivotPos];
	PointList[nPivotPos] = Temp;
}


void SelectBalPivotPoint(vector<int>& AttList, vector<Point>& PointList, vector<double>& MinList, vector<double>& MaxList, ALG nType)
{
	int nHead = 0, nTail = (int)PointList.size() - 1, nCPos = 1;
	int nCurAtt, nNumAtt = (int)AttList.size();

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	vector<double> NormalList(nNumAtt);
	for (int nAttID = 0; nAttID < nNumAtt; nAttID++)
		NormalList[nAttID] = MaxList[nAttID] - MinList[nAttID];
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	Point CPoint, HPoint = PointList[nHead], Temp;
	double dCurDist, dMinDist;
	
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	if (nType == DIST)
		dMinDist = ComputeDistance(AttList, HPoint, MinList, NormalList);
	else if (nType == COS)
		dMinDist = ComputeCosine(AttList, HPoint, MinList, NormalList);
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	while (nCPos <= nTail)
	{
		CPoint = PointList[nCPos];

#ifdef MEASURE
		nGMeasure++;
#endif

		bool bDominate = false, bDominated = false;
		for (int nAttID = 0; nAttID < nNumAtt; nAttID++)
		{
			nCurAtt = AttList[nAttID];
			if (HPoint[nCurAtt] < CPoint[nCurAtt])
				bDominate = true;
			else if (HPoint[nCurAtt] > CPoint[nCurAtt])
				bDominated = true;

			if (bDominate && bDominated)
				break;
		}

		if (bDominate && !bDominated)
		{
			PointList[nCPos] = PointList[nTail];
			PointList.pop_back();
			nTail--;
		}
		else if (!bDominate && bDominated)
		{
			PointList[nHead] = PointList[nCPos];
			PointList[nCPos] = PointList[nTail];
			PointList.pop_back();
			nTail--;

			HPoint = PointList[nHead];

			///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			if (nType == DIST)
				dMinDist = ComputeDistance(AttList, HPoint, MinList, NormalList);
			else if (nType == COS)
				dMinDist = ComputeCosine(AttList, HPoint, MinList, NormalList);
			///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		}
		else
		{
			///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			if (nType == DIST)
				dCurDist = ComputeDistance(AttList, CPoint, MinList, NormalList);
			else if (nType == COS)
				dCurDist = ComputeCosine(AttList, CPoint, MinList, NormalList);
			///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

			if (dCurDist < dMinDist)
			{
				if (EvaluatePoint(AttList, nCPos, PointList))
				{
					Temp = PointList[nHead];
					PointList[nHead] = PointList[nCPos];
					PointList[nCPos] = Temp;

					HPoint = PointList[nHead];
					dMinDist = dCurDist;
					nCPos++;
				}
				else
				{
					PointList[nCPos] = PointList[nTail];
					PointList.pop_back();
					nTail--;
				}
			}
			else
				nCPos++;
		}
	}
}


double ComputeCosine(vector<int>& AttList, Point CPoint, vector<double>& MinList, vector<double>& NormalList)
{
	int nNumAtt = (int)AttList.size();
	double dCValue, dSum, dDeno;

	dSum = dCValue = (CPoint[AttList[0]] - MinList[0]) / NormalList[0];
	dDeno = pow(dCValue, 2);
	for (int nAttID = 1; nAttID < nNumAtt; nAttID++)
	{
		dCValue = (CPoint[AttList[nAttID]] - MinList[nAttID]) / NormalList[nAttID];
		dSum += dCValue;
		dDeno += pow(dCValue, 2);
	}

	return - dSum / sqrt(dDeno);
}


double ComputeDistance(vector<int>& AttList, Point CPoint, vector<double>& MinList, vector<double>& NormalList)
{
	int nCurAtt, nNumAtt = (int)AttList.size();
	double dMax, dMin;

	dMax = dMin = (CPoint[AttList[0]] - MinList[0]) / NormalList[0];
	for (int nAttID = 1; nAttID < nNumAtt; nAttID++)
	{
		nCurAtt = AttList[nAttID];
		if (dMax < CPoint[nCurAtt])
			dMax = (CPoint[nCurAtt]  - MinList[nAttID]) / NormalList[nAttID];
		else if (dMin > CPoint[nCurAtt])
			dMin = (CPoint[nCurAtt] - MinList[nAttID]) / NormalList[nAttID];
	}

	return dMax - dMin;
}


bool EvaluatePoint(vector<int>& AttList, int nCPos, vector<Point>& PointList)
{
	bool bSkyline = true;
	int nAttID, nCurAtt, nNumAtt = (int)AttList.size();

	Point CPoint = PointList[nCPos], SPoint;
	for (int nPnt = 1; nPnt < nCPos; nPnt++)
	{
#ifdef MEASURE
		nGMeasure++;
#endif

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		SPoint = PointList[nPnt];
		for (nAttID = 0; nAttID < nNumAtt; nAttID++)
		{
			nCurAtt = AttList[nAttID];
			if (CPoint[nCurAtt] < SPoint[nCurAtt])
				break;
		}
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		if (nAttID == nNumAtt)
		{
			bSkyline = false;
			break;
		}
	}

	return bSkyline;
}




