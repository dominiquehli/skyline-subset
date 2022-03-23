#include "UpdateEvaluation.h"

void EvaluateUpdateDimSize(string strAlgName)
{
	int nNumPnt = DEFAULT_CAR;
	int nNumAtt;
	int nNumDim = NUM_OF_DIM;
	int nTotalRepeat = TOTAL_REPEAT;
	int nNumDist = NUM_OF_DIST;

	string strOutputPath = OutputPath + strAlgName + "\\";
	string strInputFileName, strFileName1, strFileName2;

	list<int> SkylineList;

	// DimSize : 4, 6, 8, 10, 12, 14, 16, 18, 20, 22
	int DimSizeList[NUM_OF_DIM];
	for (int i = 0; i < nNumDim; i++)
		DimSizeList[i] = INITIAL_DIM + (i * 2);

	unsigned long long TotalTimeList[TOTAL_REPEAT];
	unsigned long long TotalSkylineList[TOTAL_REPEAT];

	for (int nDist = 0; nDist < nNumDist; nDist++)
	{
		strFileName1 = strOutputPath + "DimTime(" + strDist[nDist] + ").txt";
		strFileName2 = strOutputPath + "DimSkyline(" + strDist[nDist] + ").txt";

		for (int nDim = 0; nDim < nNumDim; nDim++)
		{
			nNumAtt = DimSizeList[nDim];

			for (int nRep = 0; nRep < nTotalRepeat; nRep++)
			{
				srand(nNumAtt * RAND_PARAMETER + nRep);		// Parameter setting

				stringstream ss;
				ss << strDist[nDist] << (int)(nNumPnt / 1000) << "k" << nNumAtt << "d" << nRep + 1<< "p";
				strInputFileName = InputPath + ss.str() + ".dat";
				ss.flush();

				vector<Point> PointList;
				if (strDist[nDist] == "Anti")
					PointList = GenerateAntiCorrelated(nNumPnt, nNumAtt, false);
				else if (strDist[nDist] == "Ind")
					PointList = GenerateIndependent(nNumPnt, nNumAtt, false);

				//vector<Point> PointList = ReadPointList(nNumPnt, nNumAtt, strInputFileName);
				cout << "Read Data Done." << endl;

				///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
				cout << "Exeucte Algorithm: " << strAlgName << endl;

				vector<Point> Skyline;
				TotalTimeList[nRep] = ExecuteAlgorithm(strDist[nDist], nNumAtt, PointList, Skyline, strAlgName);
				TotalSkylineList[nRep] = (unsigned long long)Skyline.size();
				///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

				cout << strAlgName << " Time: " << TotalTimeList[nRep] << "\tSkyline Size: " << TotalSkylineList[nRep] << endl;
				cout << "Dist: " << strDist[nDist] << " DimSize: " << nNumAtt << " Repeat: " << nRep + 1 << endl << endl;

				ClearPointList(PointList);
			}

			WriteEvaluationResult(nNumAtt, nNumPnt, strDist[nDist], strAlgName, TotalTimeList, nTotalRepeat, strFileName1);
			WriteEvaluationResult(nNumAtt, nNumPnt, strDist[nDist], strAlgName, TotalSkylineList, nTotalRepeat, strFileName2);
		}
	}
}


void EvaluateUpdateDataSize(string strAlgName)
{
	int nNumPnt;
	int nNumAtt = DEFAULT_DIM;
	int nNumData = NUM_OF_DATA;
	int nTotalRepeat = TOTAL_REPEAT;
	int nNumDist = NUM_OF_DIST;

	string strOutputPath = OutputPath + strAlgName + "\\";
	string strInputFileName, strFileName1, strFileName2;

	// DataSize : 100K, 200K, 300K, 400K, 500K
	int DataSizeList[NUM_OF_DATA];
	for (int i = 0; i < nNumData; i++)
		DataSizeList[i] = INITIAL_CAR + INITIAL_CAR * i;

	unsigned long long TotalDTList[TOTAL_REPEAT];
	unsigned long long TotalTimeList[TOTAL_REPEAT];
	unsigned long long TotalSkylineList[TOTAL_REPEAT];

	for (int nDist = 0; nDist <nNumDist; nDist++)
	{
		strFileName1 = strOutputPath + "DataTime(" + strDist[nDist] + ").txt";
		strFileName2 = strOutputPath + "DataSkyline(" + strDist[nDist] + ").txt";

		for (int j = 0; j < nNumData; j++)
		{
			nNumPnt = DataSizeList[j];

			for (int nRep = 0; nRep < nTotalRepeat; nRep++)
			{
				srand(nNumPnt / 1000 * RAND_PARAMETER + nRep);		// Parameter setting

				stringstream ss;
				ss << strDist[nDist] << (int)(nNumPnt / 1000) << "k" << nNumAtt << "d" << nRep + 1<< "p";
				strInputFileName = InputPath + ss.str() + ".dat";
				ss.flush();

				vector<Point> PointList;
				if (strDist[nDist] == "Anti")
					PointList = GenerateAntiCorrelated(nNumPnt, nNumAtt, false);
				else if (strDist[nDist] == "Ind")
					PointList = GenerateIndependent(nNumPnt, nNumAtt, false);

				//vector<Point> PointList = ReadPointList(nNumPnt, nNumAtt, strInputFileName);
				cout << "Read Data Done." << endl;

				///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
				cout << "Exeucte Algorithm: " << strAlgName << endl;

				vector<Point> Skyline;
				TotalTimeList[nRep] = ExecuteAlgorithm(strDist[nDist], nNumAtt, PointList, Skyline, strAlgName);
				TotalSkylineList[nRep] = (unsigned long long)Skyline.size();
				///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

				cout << strAlgName << " Time: " << TotalTimeList[nRep] << "\tComparison: " << TotalDTList[nRep] << "\tSkyline Size: " << TotalSkylineList[nRep] << endl;
				cout << "Dist: " << strDist[nDist] << " DataSize: " << nNumPnt << " Repeat: " << nRep + 1 << endl << endl;

				ClearPointList(PointList);
			}

			WriteEvaluationResult(nNumAtt, nNumPnt, strDist[nDist], strAlgName, TotalTimeList, nTotalRepeat, strFileName1);
			WriteEvaluationResult(nNumAtt, nNumPnt, strDist[nDist], strAlgName, TotalSkylineList, nTotalRepeat, strFileName2);
		}
	}
}



unsigned long long ExecuteAlgorithm(string strDist, int nNumAtt, vector<Point>& PointList, vector<Point>& Skyline, string strAlgName)
{
	clock_t nPointtTime, nFinTime;

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	int nNumInsertion = 10000;
	int nNumDeletion = 100;
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	vector<int> AttList;
	for (int nAttID = 0; nAttID < nNumAtt; nAttID++)
		AttList.push_back(nAttID + 1);

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	vector<Point> InsertPointList;
	if (strDist == "Anti")
		InsertPointList = GenerateAntiCorrelated(nNumInsertion, nNumAtt, false);
	else if (strDist == "Ind")
		InsertPointList = GenerateIndependent(nNumInsertion, nNumAtt, false);

	int nNumPnt = PointList.size();
	for (int nPnt = 0; nPnt < nNumInsertion; nPnt++)
		InsertPointList[nPnt][0] = nNumPnt + nPnt;
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	vector<Point> DeletePointList;
	for (int nPnt = 0; nPnt < nNumDeletion; nPnt++)
		DeletePointList.push_back(PointList[nPnt]);
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	SNode SkyTree;
	ExecuteBSkyTreeP(AttList, PointList, SkyTree, DIST);
	cout << "SkyTree Construction is completed!" << endl;

	if (strAlgName == "InsertSkyTree")
	{
		nPointtTime = clock();
		for (int nPnt = 0; nPnt < nNumInsertion; nPnt++)
			InsertSkyTree(AttList, InsertPointList[nPnt], SkyTree);
		nFinTime = clock();
	}
	else if (strAlgName == "InsertNaive")
	{
		nPointtTime = clock();
		for (int nPnt = 0; nPnt < nNumInsertion; nPnt++)
			InsertNaive(AttList, InsertPointList[nPnt], SkyTree);
		nFinTime = clock();
	}
	else if (strAlgName == "DeleteSkyTree")
	{
		nPointtTime = clock();
		for (int nPnt = 0; nPnt < nNumInsertion; nPnt++)
		{
			vector<Point>::iterator it = PointList.begin();
			PointList.erase(it);

			DeleteSkyTree(AttList, PointList, DeletePointList[nPnt], SkyTree);
		}
		nFinTime = clock();
	}
	else if (strAlgName == "DeleteNaive")
	{
		nPointtTime = clock();
		for (int nPnt = 0; nPnt < nNumInsertion; nPnt++)
		{
			vector<Point>::iterator it = PointList.begin();
			PointList.erase(it);

			DeleteNaive(AttList, PointList, DeletePointList[nPnt], SkyTree);
		}
		nFinTime = clock();
	}

	InsertSkyline(Skyline, SkyTree);
	ClearSkyTree(SkyTree);

	return nFinTime - nPointtTime;
}

