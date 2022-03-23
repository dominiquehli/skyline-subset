#include "Utility.h"

bool CompareSkyline(vector<Point>& Skyline1, vector<Point>& Skyline2)
{
	bool bSame = true;

	if (Skyline1.size() == Skyline2.size())
	{
		sort(Skyline1.begin(), Skyline1.end(), ComparePointID);
		sort(Skyline2.begin(), Skyline2.end(), ComparePointID);

		int nNumSky = (int)Skyline1.size();
		for (int nSky = 0; nSky < nNumSky; nSky++)
		{
			if (Skyline1[nSky][0] != Skyline2[nSky][0])
			{
				bSame = false;
				break;
			}
		}
	}
	else
		bSame = false;

	return bSame;
}


void CopyPointList(int nNumAtt, vector<Point>& PointList, vector<Point>& CPointList)
{
	int nNumPnt = (int)PointList.size();
	for (int nPntID = 0; nPntID < nNumPnt; nPntID++)
	{
		Point NewPoint = new double[nNumAtt+1];
		for (int nAttID = 0; nAttID <= nNumAtt; nAttID++)
			NewPoint[nAttID] = PointList[nPntID][nAttID];

		CPointList.push_back(NewPoint);
	}
}


void ClearPointList(vector<Point>& PointList)
{
	int nNumPnt = (int)PointList.size();
	for (int nPntID = 0; nPntID < nNumPnt; nPntID++)
		delete[] PointList[nPntID];
	
	PointList.clear();
}


void PrintPointList(int nNumAtt, vector<Point>& PointList)
{
	int nNumPnt = (int)PointList.size();
	for (int nPntID = 0; nPntID < nNumPnt; nPntID++)
	{
		cout << PointList[nPntID][0];
		for (int nAttID = 1; nAttID <= nNumAtt; nAttID++)
			cout << "\t" << PointList[nPntID][nAttID];
		cout << endl;
	}
	cout << endl;
}


void PrintSkyline(vector<Point>& Skyline)
{
	sort(Skyline.begin(), Skyline.end(), ComparePointID);
	for (vector<Point>::iterator it = Skyline.begin(); it != Skyline.end(); it++)
		cout << (*it)[0] << " ";
	cout << endl;
}

