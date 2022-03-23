#include "Common.h"

int nGlobalAtt;
unsigned long long nGMeasure;
vector<int> GlobalAttList;

bool ComparePointID(Point FirPoint, Point SecPoint)
{
	if (FirPoint[0] < SecPoint[0])
		return true;
	else
		return false;
}


bool CompareAtt(Point FirPoint, Point SecPoint)
{
	if (FirPoint[nGlobalAtt] - SecPoint[nGlobalAtt] < 0)
		return true;
	else if (FirPoint[nGlobalAtt] == SecPoint[nGlobalAtt] && FirPoint[0] < SecPoint[0])
		return true;
	else
		return false;
}

