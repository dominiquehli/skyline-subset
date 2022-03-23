#ifndef _COMMON_H
#define _COMMON_H

#define BITSET								24
#define RAND_PARAMETER			2
#define TOP_K								100
//#define MEASURE


// Synthetic dataset parameters
#define NUM_OF_DATA					5
#define NUM_OF_DIM					7
#define NUM_OF_DIST					2
#define INITIAL_CAR						200000
#define INITIAL_DIM						4
#define DEFAULT_CAR					500000
#define DEFAULT_DIM					12

#define TOTAL_REPEAT				3
#define NUM_OF_REAL					2

enum ALG { DOM, RAND, DIST, COS };

#include <ctime>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <ctime>
#include <algorithm>
#include <cmath>
#include <vector>
#include <list>
#include <bitset>
#include <stack>
#include <queue>
#include <map>
#include <set>

using namespace std;

typedef double* Point;

extern int nGlobalAtt;
extern unsigned long long nGMeasure;
extern vector<int> GlobalAttList;

bool ComparePointID(Point FirPoint, Point SecPoint);
bool CompareAtt(Point FirPoint, Point SecPoint);


#endif

