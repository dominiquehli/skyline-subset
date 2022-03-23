#include "SaLSa.h"

void ExecuteSaLSa(vector<int>& AttList, vector<Point>& PointList, vector<Point>& Skyline)
{
	int nNumPnt = PointList.size();
	int nAttID, nCurAtt, nNumAtt = AttList.size();
	double dStop = 1.0, dSum;

	vector<SPoint2> SPointList = SortSPoint2List(AttList, PointList);
	vector<SPoint2> SSkyline;

	Point CPoint, SkyPoint;
	for (int nPnt = 0; nPnt < nNumPnt; nPnt++)
	{
		if (dStop < SPointList[nPnt].dMin)				// Stop condition.
			break;

		CPoint = SPointList[nPnt].Value;
		dSum = SPointList[nPnt].dSum;

		bool bSkyline = true;
		vector<SPoint2>::iterator it;
		for (it = SSkyline.begin(); it != SSkyline.end(); it++)
		{
			if (dSum < it->dSum)
				break;

			SkyPoint = it->Value;

#ifdef MEASURE
			nGMeasure++;
#endif
				
			int nEqual = 0; 
			for (nAttID = 0; nAttID < nNumAtt; nAttID++)
			{
				nCurAtt = AttList[nAttID];
				if (CPoint[nCurAtt] < SkyPoint[nCurAtt])
					break;
				else if (CPoint[nCurAtt] == SkyPoint[nCurAtt])
					nEqual++;
			}

			if (nEqual == nNumAtt)
				continue;
			else if (nAttID == nNumAtt)
			{
				bSkyline = false;
				break;
			}
		}

		if (bSkyline)
		{
			SSkyline.insert(it, SPointList[nPnt]);

			double dMax = CPoint[AttList[0]];
			for (nAttID = 1; nAttID < nNumAtt; nAttID++)
			{
				if (dMax < CPoint[AttList[nAttID]])
					dMax = CPoint[AttList[nAttID]];
			}

			if(dMax < dStop)
				dStop = dMax;
		}
	}

	for (vector<SPoint2>::iterator it = SSkyline.begin(); it != SSkyline.end(); it++)
		Skyline.push_back(it->Value);
}



vector<SPoint2> SortSPoint2List(vector<int>& AttList, vector<Point>& PointList)
{
	int nNumAtt = (int)AttList.size(), nNumPnt = (int)PointList.size();
	double dMin, dSum;

	vector<SPoint2> SPointList;
	for (int nPnt = 0; nPnt < nNumPnt; nPnt++)
	{
		Point CPoint = PointList[nPnt];
		
		dMin = dSum = CPoint[AttList[0]];
		for (int nAttID = 1; nAttID < nNumAtt; nAttID++)
		{
			dSum += CPoint[AttList[nAttID]];
			if (CPoint[AttList[nAttID]] < dMin)
				dMin = CPoint[AttList[nAttID]];
		}

		SPointList.push_back(SPoint2(dMin, dSum, CPoint));
	}

	sort(SPointList.begin(), SPointList.end(), CompareSPoint2);

	return SPointList;
}



int CompareSPoint2(SPoint2 FirPoint, SPoint2 SecPoint)
{
	if (FirPoint.dMin < SecPoint.dMin)
		return true;
	if (FirPoint.dMin == SecPoint.dMin && FirPoint.dSum < SecPoint.dSum)
		return true;
	else
		return false;
}
