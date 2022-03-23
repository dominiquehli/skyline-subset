#ifndef _EVALUATION_H
#define _EVALUATION_H

#include "Scenario.h"

extern string InputPath;
extern string OutputPath;
extern string strDist[2];

extern string InputRealDataPath;
extern string OutputRealDataPath;
extern int RealDataPnt[NUM_OF_REAL];
extern int RealDataAtt[NUM_OF_REAL];
extern string RealData[2];


void EvaluateDimSize(string strAlgName);
void EvaluateDataSize(string strAlgName);
void EvaluateRealData(string strAlgName);

unsigned long long ExecuteAlgorithm(int nNumAtt, vector<Point>& PointList, vector<Point>& Skyline, string strAlgName);
void WriteEvaluationResult(int nNumAtt, int nNumPnt, string strDist, string strAlgName, unsigned long long* TotalMeasureList, int nTotalRepeat, string strFileName);

vector<Point> ReadPointList(int nNumPnt, int nNumAtt, string strFileName);
vector<Point> ReadTextPointList(int nNumPnt, int nNumAtt, string strFileName);

void GenerateSyntheticPointLists();
void WritePointList(int nNumAtt, int nNumPnt, string strDist, int nRepeat);

#endif