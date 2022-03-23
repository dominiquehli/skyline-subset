#include "SFS.h"

void ExecuteSFS(vector<int>& AttList, vector<Point>& PointList, vector<Point>& Skyline)
{
	int nNumPnt = (int)PointList.size(), nNumSky = 0;
	int nAttID, nCurAtt, nNumAtt = (int)AttList.size();
	vector<Point> SPointList = SortSPointList(AttList, PointList);

	for (int nPnt = 0; nPnt < nNumPnt; nPnt++)
	{
		Point CPoint = SPointList[nPnt];

		bool bSkyline = true;
		for (int nSky = 0; nSky < nNumSky; nSky++)
		{
			Point SkyPoint = Skyline[nSky];

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
			else 	if (nAttID == nNumAtt)
			{
				bSkyline = false;
				break;
			}
		}

		if (bSkyline)
		{
			Skyline.push_back(SPointList[nPnt]);
			nNumSky++;
		}
	}
}


vector<Point> SortSPointList(vector<int>& AttList, vector<Point>& PointList)
{
	int nNumAtt = (int)AttList.size(), nNumPnt = (int)PointList.size();
	vector<SPoint> SPointList;

	for (int nPnt = 0; nPnt < nNumPnt; nPnt++)
	{
		Point CPoint = PointList[nPnt];
		
		double dSum = CPoint[AttList[0]];
		for (int nAttID = 1; nAttID < nNumAtt; nAttID++)
			dSum += CPoint[AttList[nAttID]];

		SPointList.push_back(SPoint(dSum, CPoint));
	}

	sort(SPointList.begin(), SPointList.end(), CompareSPoint);

	vector<Point> CPointList;
	for (int nPnt = 0; nPnt < nNumPnt; nPnt++)
		CPointList.push_back(SPointList[nPnt].Value);

	return CPointList;
}



int CompareSPoint(SPoint FirPoint, SPoint SecPoint)
{
	if (FirPoint.dSum < SecPoint.dSum)
		return true;
	else
		return false;
}
