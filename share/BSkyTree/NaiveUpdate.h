#ifndef NAIVE_UPDATE_H
#define NAIVE_UPDATE_H

#include "Common.h"
#include "InsertSkyTree.h"
#include "DeleteSkyTree.h"

void InsertNaive(vector<int>& AttList, Point NewPoint, SNode& SkyTree);

void DeleteNaive(vector<int>& AttList, vector<Point>& PointList, Point DelPoint, SNode& SkyTree);
bool CheckExistence(vector<int>& AttList, Point DelPoint, SNode& SkyTree);

#endif