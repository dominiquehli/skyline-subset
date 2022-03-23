#ifndef UPDATE_EVALUATION_H
#define UPDATE_ELVAUATION_H

#include "Scenario.h"
#include "Evaluation.h"

void EvaluateUpdateDimSize(string strAlgName);
void EvaluateUpdateDataSize(string strAlgName);

unsigned long long ExecuteAlgorithm(string strDist, int nNumAtt, vector<Point>& PointList, vector<Point>& Skyline, string strAlgName);

#endif