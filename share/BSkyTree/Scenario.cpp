#include "Scenario.h"

void TestDataGeneration()
{
    int nNumPnt = 10;
    int nNumAtt = 3;

	srand(2);
	vector<Point> PointList = GenerateIndependent(nNumPnt, nNumAtt, true);

	PrintPointList(nNumAtt, PointList);
	ClearPointList(PointList);
}


void TestSkyline()
{
	bool bCorrect = true;
	int nNumPnt = 200000;
	int nNumAtt = 6;

	int nRepeat = 0;
	int nTotalRepeat = 100;
	unsigned long long nMeasure1 = 0, nMeasure2 = 0;

	clock_t start_time, finish_time;

	srand(1);
	//srand((unsigned int)time(NULL));

	while (bCorrect && nRepeat < nTotalRepeat)
	{
		//vector<Point> PointList = GenerateAntiCorrelated(nNumPnt, nNumAtt, false);
		vector<Point> PointList = GenerateIndependent(nNumPnt, nNumAtt, false);

		vector<Point> PointList2, PointList3;
		CopyPointList(nNumAtt, PointList, PointList2);
		CopyPointList(nNumAtt, PointList, PointList3);

		cout << "Generated Data: " << PointList[0][1] << endl;

		vector<Point> Skyline1, Skyline2, Skyline3;
		vector<int> AttList;
		for (int nAttID = 1; nAttID <= nNumAtt; nAttID++)
			AttList.push_back(nAttID);

		start_time = clock();
		nGMeasure = 0;
		ExecuteBSkyTreeP(AttList, PointList, Skyline1, DIST);
		nMeasure1 = nGMeasure;
		finish_time = clock();
		cout << "ExecuteBSkyTreeP: " << finish_time - start_time << endl;

		start_time = clock();
		nGMeasure = 0;
		//ExecuteBSkyTreeP(AttList, PointList2, Skyline2, COS);
		ExecuteSSkyline(AttList, PointList2, Skyline2);
		nMeasure2 = nGMeasure;
		finish_time = clock();
		cout << "ExecuteSSkyline: " << finish_time - start_time << endl;

		cout << "Iteration: " << nRepeat++ << "\tSkylineSize: " << Skyline1.size() << "\tSSkyline: " << nMeasure1 << "\tBSkyTree: " << nMeasure2 << endl << endl;

		if (!CompareSkyline(Skyline1, Skyline2))
		{
			bCorrect = false;
			PrintSkyline(Skyline1);
			PrintSkyline(Skyline2);
			PrintPointList(nNumAtt, PointList3);
			ExecuteBSkyTreeS(AttList, PointList3, Skyline3, DOM);
		}

		ClearPointList(PointList);
		ClearPointList(PointList2);
		ClearPointList(PointList3);
	}

	cout << bCorrect << endl;
}



void TestInsertSkyline()
{
	bool bCorrect = true;
	int nNumPnt = 500000;
	int nNumPnt2 = 1000;
	int nNumAtt = 8;

	int nRepeat = 0;
	int nTotalRepeat = 5;
	clock_t start_time, finish_time;

	srand(1);
	//srand((unsigned int)time(NULL));

	while (bCorrect && nRepeat < nTotalRepeat)
	{
		vector<Point> PointList = GenerateAntiCorrelated(nNumPnt, nNumAtt, false);
		//vector<Point> PointList = GenerateIndependent(nNumPnt, nNumAtt, false);

		vector<Point> PointList2, PointList3;
		CopyPointList(nNumAtt, PointList, PointList2);
		CopyPointList(nNumAtt, PointList, PointList3);

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		vector<Point> InsertPointList = GenerateAntiCorrelated(nNumPnt2, nNumAtt, false);

		for (int nPnt = 0; nPnt < nNumPnt2; nPnt++)
			InsertPointList[nPnt][0] = nNumPnt + nPnt;
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		cout << "Generated Data: " << PointList[0][1] << endl;

		vector<Point> Skyline1, Skyline2, Skyline3;
		SNode SkyTree1, SkyTree2, SkyTree3;

		vector<int> AttList;
		for (int nAttID = 1; nAttID <= nNumAtt; nAttID++)
			AttList.push_back(nAttID);

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		ExecuteBSkyTreeP(AttList, PointList, SkyTree1, DIST);
		start_time = clock();
		for (int nPnt = 0; nPnt < nNumPnt2; nPnt++)
			InsertSkyTree(AttList, InsertPointList[nPnt], SkyTree1);
		finish_time = clock();
		InsertSkyline(Skyline1, SkyTree1);
		cout << "InsertSkyTree: " << finish_time - start_time << endl;
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		ExecuteBSkyTreeP(AttList, PointList2, SkyTree2, DIST);
		start_time = clock();
		for (int nPnt = 0; nPnt < nNumPnt2; nPnt++)
			InsertNaive(AttList, InsertPointList[nPnt], SkyTree2);
		finish_time = clock();
		InsertSkyline(Skyline2, SkyTree2);
		cout << "InsertNaive: " << finish_time - start_time << endl;
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		cout << "Iteration: " << nRepeat++ << "\tSkylineSize: " << Skyline1.size() << endl << endl;

		if (!CompareSkyline(Skyline1, Skyline2))
		{
			bCorrect = false;
			PrintSkyline(Skyline1);
			PrintSkyline(Skyline2);
			PrintPointList(nNumAtt, PointList2);

			ExecuteBSkyTreeP(AttList, PointList3, SkyTree3, DOM);

			PrintSkyTree(0, SkyTree3);
			for (int nPnt = 0; nPnt < nNumPnt2; nPnt++)
			{
				InsertSkyTree(AttList, InsertPointList[nPnt], SkyTree3);
				PrintSkyTree(0, SkyTree3);
				cout << endl;
			}
		}

		ClearSkyTree(SkyTree1);
		ClearSkyTree(SkyTree2);

		ClearPointList(PointList);
		ClearPointList(PointList2);
		ClearPointList(PointList3);
	}

	cout << bCorrect << endl;
}


void TestDeleteSkyline()
{
	bool bCorrect = true;
	int nNumPnt = 100000;
	int nNumPnt2 = 1;
	int nNumAtt = 10;

	int nRepeat = 0;
	int nTotalRepeat = 1000;
	clock_t start_time, finish_time;

	srand(1);
	//srand((unsigned int)time(NULL));

	while (bCorrect && nRepeat < nTotalRepeat)
	{
		vector<Point> PointList = GenerateAntiCorrelated(nNumPnt, nNumAtt, false);
		//vector<Point> PointList = GenerateIndependent(nNumPnt, nNumAtt, false);

		vector<Point> PointList2, PointList3;
		CopyPointList(nNumAtt, PointList, PointList2);
		CopyPointList(nNumAtt, PointList, PointList3);

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		vector<Point> DeletePointList;
		for (int nPnt = 0; nPnt < nNumPnt2; nPnt++)
			DeletePointList.push_back(PointList[nPnt]);
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		cout << "Generated Data: " << PointList[0][1] << endl;

		vector<Point> Skyline1, Skyline2, Skyline3;
		SNode SkyTree1, SkyTree2, SkyTree3;

		vector<int> AttList;
		for (int nAttID = 1; nAttID <= nNumAtt; nAttID++)
			AttList.push_back(nAttID);

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		ExecuteBSkyTreeP(AttList, PointList, SkyTree1, DIST);
		start_time = clock();
		for (int nPnt = 0; nPnt < nNumPnt2; nPnt++)
		{
			vector<Point>::iterator it = PointList.begin();
			PointList.erase(it);

			DeleteSkyTree(AttList, PointList, DeletePointList[nPnt], SkyTree1);
		}
		finish_time = clock();
		InsertSkyline(Skyline1, SkyTree1);
		cout << "DeleteSkyline: " << finish_time - start_time << endl;
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		ExecuteBSkyTreeP(AttList, PointList2, SkyTree2, DIST);
		start_time = clock();
		for (int nPnt = 0; nPnt < nNumPnt2; nPnt++)
		{
			vector<Point>::iterator it = PointList2.begin();
			PointList2.erase(it);

			DeleteNaive(AttList, PointList2, DeletePointList[nPnt], SkyTree2);
		}
		finish_time = clock();
		InsertSkyline(Skyline2, SkyTree2);
		cout << "Naive: " << finish_time - start_time << endl;
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		cout << "Iteration: " << nRepeat++ << "\tSkylineSize: " << Skyline1.size() << endl << endl;

		if (!CompareSkyline(Skyline1, Skyline2))
		{
			bCorrect = false;
			PrintSkyline(Skyline1);
			PrintSkyline(Skyline2);
			PrintPointList(nNumAtt, PointList3);

			ExecuteBSkyTreeP(AttList, PointList3, SkyTree3, DIST);

			PrintSkyTree(0, SkyTree3);
			for (int nPnt = 0; nPnt < nNumPnt2; nPnt++)
			{
				vector<Point>::iterator it = PointList3.begin();
				PointList3.erase(it);

				DeleteSkyTree(AttList, PointList3, DeletePointList[nPnt], SkyTree3);
				PrintSkyTree(0, SkyTree3);
				cout << endl;
			}
		}

		ClearSkyTree(SkyTree1);
		ClearSkyTree(SkyTree2);

		ClearPointList(PointList);
		ClearPointList(PointList2);
		ClearPointList(PointList3);
	}

	cout << bCorrect << endl;
}


void TestSkyBand()
{
	int nNumPnt = 10000;
	int nNumAtt = 4;

	int nRepeat = 0;
	int nTotalRepeat = 5;

	clock_t start_time, finish_time;

	srand(1);
	while (nRepeat < nTotalRepeat)
	{
		vector<Point> PointList = GenerateIndependent(nNumPnt, nNumAtt, false);

		vector<SBPoint> SkyBand;
		vector<int> AttList;
		for (int nAttID = 1; nAttID <= nNumAtt; nAttID++)
			AttList.push_back(nAttID);

		start_time = clock();
		ExecuteSkyBand(AttList, PointList, SkyBand);
		finish_time = clock();

		cout << "ExecuteSkyBand: " << finish_time - start_time << endl;
		cout << "Iteration: " << nRepeat++ << "\tSkylineSize: " << SkyBand.size() << endl << endl;

		//int nNumSkyBand = SkyBand.size();
		//for (double nDom = nNumPnt*0.05; nDom <= TOP_K; nDom += nNumPnt*0.05)
		//{
		//	int nCount = 0;
		//	for (int nSky = 0; nSky < nNumSkyBand; nSky++)
		//	{
		//		if (SkyBand[nSky].nNumDom <= nDom)
		//			nCount++;
		//	}
		//	cout << nCount << endl;
		//}
		//cout << endl;
		//nRepeat++;

		ClearPointList(PointList);
		SkyBand.clear();
	}
}


