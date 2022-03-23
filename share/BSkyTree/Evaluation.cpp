#include "Evaluation.h"

string InputPath = "D:\\BSkyTreeEXP\\Data\\";
string OutputPath = "D:\\BSkyTreeEXP\\Results\\";
string strDist[2] = { "Ind", "Anti" };

string InputRealDataPath = "D:\\BSkyTreeEXP\\RealData\\";
string OutputRealDataPath = "D:\\BSkyTreeEXP\\RealData\\Results\\";
int RealDataPnt[NUM_OF_REAL] = {17264, 127931};
int RealDataAtt[NUM_OF_REAL] = {8, 6};
string RealData[2] = {"NBA", "House"};


void EvaluateDimSize(string strAlgName)
{
	int nNumPnt = DEFAULT_CAR;
	int nNumAtt;
	int nNumDim = NUM_OF_DIM;
	int nTotalRepeat = TOTAL_REPEAT;
	int nNumDist = NUM_OF_DIST;

	string strOutputPath = OutputPath + strAlgName + "\\";
	string strInputFileName, strFileName1, strFileName2, strFileName3;

	list<int> SkylineList;

	// DimSize : 4, 6, 8, 10, 12, 14, 16, 18, 20, 22
	int DimSizeList[NUM_OF_DIM];
	for (int i = 0; i < nNumDim; i++)
		DimSizeList[i] = INITIAL_DIM + (i * 2);

	unsigned long long TotalDTList[TOTAL_REPEAT];
	unsigned long long TotalTimeList[TOTAL_REPEAT];
	unsigned long long TotalSkylineList[TOTAL_REPEAT];

	for (int nDist = 0; nDist < nNumDist; nDist++)
	{
		strFileName1 = strOutputPath + "DimDT(" + strDist[nDist] + ").txt";
		strFileName2 = strOutputPath + "DimTime(" + strDist[nDist] + ").txt";
		strFileName3 = strOutputPath + "DimSkyline(" + strDist[nDist] + ").txt";

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

				vector<Point> Skyline;

				cout << "Exeucte Algorithm: " << strAlgName << endl;
				nGMeasure = 0;
				TotalTimeList[nRep] = ExecuteAlgorithm(nNumAtt, PointList, Skyline, strAlgName);
				TotalDTList[nRep] = nGMeasure;

				TotalSkylineList[nRep] = (unsigned long long)Skyline.size();

				cout << strAlgName << " Time: " << TotalTimeList[nRep] << "\tComparison: " << TotalDTList[nRep] << "\tSkyline Size: " << TotalSkylineList[nRep] << endl;
				cout << "Dist: " << strDist[nDist] << " DimSize: " << nNumAtt << " Repeat: " << nRep + 1 << endl << endl;

				ClearPointList(PointList);
			}

			WriteEvaluationResult(nNumAtt, nNumPnt, strDist[nDist], strAlgName, TotalDTList, nTotalRepeat, strFileName1);
			WriteEvaluationResult(nNumAtt, nNumPnt, strDist[nDist], strAlgName, TotalTimeList, nTotalRepeat, strFileName2);
			WriteEvaluationResult(nNumAtt, nNumPnt, strDist[nDist], strAlgName, TotalSkylineList, nTotalRepeat, strFileName3);
		}
	}
}



void EvaluateDataSize(string strAlgName)
{
	int nNumPnt;
	int nNumAtt = DEFAULT_DIM;
	int nNumData = NUM_OF_DATA;
	int nTotalRepeat = TOTAL_REPEAT;
	int nNumDist = NUM_OF_DIST;

	string strOutputPath = OutputPath + strAlgName + "\\";
	string strInputFileName, strFileName1, strFileName2, strFileName3;

	// DataSize : 100K, 200K, 300K, 400K, 500K
	int DataSizeList[NUM_OF_DATA];
	for (int i = 0; i < nNumData; i++)
		DataSizeList[i] = INITIAL_CAR + INITIAL_CAR * i;

	unsigned long long TotalDTList[TOTAL_REPEAT];
	unsigned long long TotalTimeList[TOTAL_REPEAT];
	unsigned long long TotalSkylineList[TOTAL_REPEAT];

	for (int nDist = 0; nDist <nNumDist; nDist++)
	{
		strFileName1 = strOutputPath + "DataDT(" + strDist[nDist] + ").txt";
		strFileName2 = strOutputPath + "DataTime(" + strDist[nDist] + ").txt";
		strFileName3 = strOutputPath + "DataSkyline(" + strDist[nDist] + ").txt";

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

				vector<Point> Skyline;

				cout << "Exeucte Algorithm: " << strAlgName << endl;
				nGMeasure = 0;
				TotalTimeList[nRep] = ExecuteAlgorithm(nNumAtt, PointList, Skyline, strAlgName);
				TotalDTList[nRep] = nGMeasure;

				TotalSkylineList[nRep] = (unsigned long long)Skyline.size();

				cout << strAlgName << " Time: " << TotalTimeList[nRep] << "\tComparison: " << TotalDTList[nRep] << "\tSkyline Size: " << TotalSkylineList[nRep] << endl;
				cout << "Dist: " << strDist[nDist] << " DataSize: " << nNumPnt << " Repeat: " << nRep + 1 << endl << endl;

				ClearPointList(PointList);
			}

			WriteEvaluationResult(nNumAtt, nNumPnt, strDist[nDist], strAlgName, TotalDTList, nTotalRepeat, strFileName1);
			WriteEvaluationResult(nNumAtt, nNumPnt, strDist[nDist], strAlgName, TotalTimeList, nTotalRepeat, strFileName2);
			WriteEvaluationResult(nNumAtt, nNumPnt, strDist[nDist], strAlgName, TotalSkylineList, nTotalRepeat, strFileName3);
		}
	}
}



void EvaluateRealData(string strAlgName)
{
	int nTotalRepeat = TOTAL_REPEAT;
	int nNumReal = NUM_OF_REAL;

	string strOutputPath = OutputRealDataPath + strAlgName + "\\";
	string strInputFileName, strFileName1, strFileName2, strFileName3;

	unsigned long long TotalDTList[TOTAL_REPEAT];
	unsigned long long TotalTimeList[TOTAL_REPEAT];
	unsigned long long TotalSkylineList[TOTAL_REPEAT];

	for (int i = 0; i < nNumReal; i++)
	{
		strFileName1 = strOutputPath + "DT(" + RealData[i] + ").txt";
		strFileName2 = strOutputPath + "Time(" + RealData[i] + ").txt";
		strFileName3 = strOutputPath + "Skyline(" + RealData[i] + ").txt";

		for (int nRep = 0; nRep < nTotalRepeat; nRep++)
		{
			stringstream ss;
			ss << RealData[i];
			strInputFileName = InputRealDataPath + ss.str() + ".txt";
			ss.flush();

			vector<Point> PointList = ReadTextPointList(RealDataPnt[i], RealDataAtt[i], strInputFileName);
			cout << "Read Real Data Done." << endl;

			vector<Point> Skyline;

			cout << "Exeucte Algorithm: " << strAlgName << endl;
			nGMeasure = 0;
			TotalTimeList[nRep] = ExecuteAlgorithm(RealDataAtt[i], PointList, Skyline, strAlgName);
			TotalDTList[nRep] = nGMeasure;

			TotalSkylineList[nRep] = (unsigned long long)Skyline.size();

			cout << strAlgName << " Time: " << TotalTimeList[nRep] << "\tComparison: " << TotalDTList[nRep] << "\tSkyline Size: " << TotalSkylineList[nRep] << endl;
			cout << "RealData: " << RealData[i] << " Repeat: " << nRep + 1 << endl << endl;

			ClearPointList(PointList);
		}

		WriteEvaluationResult(RealDataAtt[i], RealDataPnt[i], RealData[i], strAlgName, TotalDTList, nTotalRepeat, strFileName1);
		WriteEvaluationResult(RealDataAtt[i], RealDataPnt[i], RealData[i], strAlgName, TotalTimeList, nTotalRepeat, strFileName2);
		WriteEvaluationResult(RealDataAtt[i], RealDataPnt[i], RealData[i], strAlgName, TotalSkylineList, nTotalRepeat, strFileName3);
	}
}

unsigned long long ExecuteAlgorithm(int nNumAtt, vector<Point>& PointList, vector<Point>& Skyline, string strAlgName)
{
	clock_t nPointtTime, nFinTime;

	vector<int> AttList;
	for (int nAttID = 0; nAttID < nNumAtt; nAttID++)
		AttList.push_back(nAttID + 1);

	if (strAlgName == "SFS")
	{
		nPointtTime = clock();
		ExecuteSFS(AttList, PointList, Skyline);
		nFinTime = clock();
	}
	else if (strAlgName == "SaLSa")
	{
		nPointtTime = clock();
		ExecuteSaLSa(AttList, PointList, Skyline);
		nFinTime = clock();
	}
	else if (strAlgName == "SSkyline")
	{
		nPointtTime = clock();
		ExecuteSSkyline(AttList, PointList, Skyline);
		nFinTime = clock();
	}
	else if (strAlgName == "BSkyTreeS_DOM")
	{
		nPointtTime = clock();
		ExecuteBSkyTreeS(AttList, PointList, Skyline, DOM);
		nFinTime = clock();
	}
	else if (strAlgName == "BSkyTreeS_RAND")
	{
		nPointtTime = clock();
		ExecuteBSkyTreeS(AttList, PointList, Skyline, RAND);
		nFinTime = clock();
	}
	else if (strAlgName == "BSkyTreeS_DIST")
	{
		nPointtTime = clock();
		ExecuteBSkyTreeS(AttList, PointList, Skyline, DIST);
		nFinTime = clock();
	}
	else if (strAlgName == "BSkyTreeS_COS")
	{
		nPointtTime = clock();
		ExecuteBSkyTreeS(AttList, PointList, Skyline, COS);
		nFinTime = clock();
	}
	else if (strAlgName == "BSkyTreeP_DIST")
	{
		nPointtTime = clock();
		ExecuteBSkyTreeP(AttList, PointList, Skyline, DIST);
		nFinTime = clock();
	}
	else if (strAlgName == "BSkyTreeP_DOM")
	{
		nPointtTime = clock();
		ExecuteBSkyTreeP(AttList, PointList, Skyline, DOM);
		nFinTime = clock();
	}
	else if (strAlgName == "BSkyTreeP_RAND")
	{
		nPointtTime = clock();
		ExecuteBSkyTreeP(AttList, PointList, Skyline, RAND);
		nFinTime = clock();
	}
	else if (strAlgName == "BSkyTreeP_COS")
	{
		nPointtTime = clock();
		ExecuteBSkyTreeP(AttList, PointList, Skyline, COS);
		nFinTime = clock();
	}

	return nFinTime - nPointtTime;
}


void WriteEvaluationResult(int nNumAtt, int nNumPnt, string strDist, string strAlgName, unsigned long long* TotalMeasureList, int nTotalRepeat, string strFileName)
{
	ofstream outFile;
	outFile.open(strFileName.c_str(), ios::out | ios::app);
	outFile << nNumPnt / 1000 << "\t" << nNumAtt << "\t" << strDist << "\t" << strAlgName;

	unsigned long long nAvgMeasure = 0;
    for (int nRep = 0; nRep < nTotalRepeat; nRep++)
	{
		nAvgMeasure += TotalMeasureList[nRep];
		outFile << "\t" << TotalMeasureList[nRep];
	}

	outFile << "\t" << nAvgMeasure / nTotalRepeat << endl;
	outFile.close();
}




vector<Point> ReadPointList(int nNumPnt, int nNumAtt, string strFileName)
{
	FILE *fp;
	vector<Point> PointList;

	if ((fp = fopen(strFileName.c_str(), "rb")) != NULL)
	{
		for (int nPnt = 0; nPnt < nNumPnt; nPnt++)
		{
			Point Value = new double[nNumAtt+1];
			fread(Value, sizeof(double), nNumAtt+1, fp);

			PointList.push_back(Value);
		}
		fclose(fp);
	}
	else
		cout << "There is no File." << endl;

	return PointList;
}


vector<Point> ReadTextPointList(int nNumPnt, int nNumAtt, string strFileName)
{
	vector<Point> PointList;

	string strLine;
	ifstream inFile (strFileName.c_str());
	stringstream sin (stringstream::in | stringstream::out);

	if (inFile.is_open())
	{
		while (!inFile.eof())
		{
			getline (inFile, strLine);
			sin << strLine;
			
			Point Value = new double[nNumAtt+1];
			for (int nAttID = 0; nAttID <= nNumAtt; nAttID++)
				sin >> Value[nAttID];
			sin.clear();

			PointList.push_back(Value);
		}
	}
	else
		cout << "There is no File." << endl;

	inFile.close();

	return PointList;
}


void GenerateSyntheticPointLists()
{
	// DataSize : 100K, 200K, 300K, 400K, 500K
	int DataSizeList[NUM_OF_DATA];
	for (int i = 0; i < NUM_OF_DATA; i++)
		DataSizeList[i] = INITIAL_CAR + INITIAL_CAR * i;

	// DimSize : 4, 6, 8, 10, 12, 14, 16, 18, 20, 22
	int DimSizeList[NUM_OF_DIM];
	for (int i = 0; i < NUM_OF_DIM; i++)
		DimSizeList[i] = INITIAL_DIM + (i * 2);

	srand ((unsigned int)time(NULL));
	for (int nDist = 0; nDist < NUM_OF_DIST; nDist++)
		for (int nAtt = 0; nAtt < NUM_OF_DIM; nAtt++)
			for (int nRep = 1; nRep <= TOTAL_REPEAT; nRep++)
				WritePointList(DimSizeList[nAtt], DEFAULT_CAR, strDist[nDist], nRep);

	for (int nDist = 0; nDist < NUM_OF_DIST; nDist++)
		for (int nData=0; nData<NUM_OF_DATA; nData++)
			for (int nRep = 1; nRep <= TOTAL_REPEAT; nRep++)
				WritePointList(DEFAULT_DIM, DataSizeList[nData], strDist[nDist], nRep);
}


void WritePointList(int nNumAtt, int nNumPnt, string strDist, int nRepeat)
{
	vector<Point> PointList;

	stringstream ss;
	ss << strDist << (int)(nNumPnt / 1000) << "k" << nNumAtt << "d" << nRepeat << "p";
    
	if (strDist == "Ind")
		PointList = GenerateIndependent(nNumPnt, nNumAtt, true);
	else if (strDist == "Cor")
		PointList = GenerateCorrelated(nNumPnt, nNumAtt, true);
	else if (strDist == "Anti")
		PointList = GenerateAntiCorrelated(nNumPnt, nNumAtt, true);

	if (strDist == "Ind" || strDist == "Cor" || strDist == "Anti")
	{
		string strDataFile = InputPath + ss.str() + ".dat";
		
		FILE *fp;
		if ((fp = fopen(strDataFile.c_str(), "wb")) != NULL)
		{
			for (int nPnt = 0; nPnt < nNumPnt; nPnt++)
				fwrite(PointList[nPnt], sizeof(double), nNumAtt + 1, fp);
			fclose(fp);
		}
		else
			cout << "The file is not opened." << endl;

		cout << "Dist: " << strDist << "\tNumPnt: " << nNumPnt << "\tAtt: " << nNumAtt << "\tRepeat: " << nRepeat << endl;
		ClearPointList(PointList);
	}
}
