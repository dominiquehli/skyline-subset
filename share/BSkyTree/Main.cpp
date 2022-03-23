#include "Scenario.h"
#include "Evaluation.h"
#include "UpdateEvaluation.h"
#ifdef MEMORYLEAK
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#endif

void Evaluation()
{
	const int nNumAlg = 4;
	string strAlgName[nNumAlg] = { "BSkyTreeS_DOM", "BSkyTreeS_RAND", "BSkyTreeS_DIST", "BSkyTreeS_COS"};
	//string strAlgName[nNumAlg] = { "SFS", "SaLSa"};

	for (int i = 0; i < nNumAlg; i++)
		EvaluateDimSize(strAlgName[i]);

	for (int i = 0; i < nNumAlg; i++)
		EvaluateDataSize(strAlgName[i]);

	//for (int i = 0; i < nNumAlg; i++)
	//	EvaluateRealData(strAlgName[i]);
}

void UpdateEvaluation()
{
	const int nNumAlg = 2;
	string strAlgName[nNumAlg] = { "InsertSkyTree", "DeleteSkyTree"};
	//string strAlgName[nNumAlg] = { "InsertSkyTree", "DeleteSkyTree", "NaiveInsert", "NaiveDelete"};

	for (int i = 0; i < nNumAlg; i++)
		EvaluateUpdateDimSize(strAlgName[i]);

	for (int i = 0; i < nNumAlg; i++)
		EvaluateUpdateDataSize(strAlgName[i]);
}


int main(int argc, char **argv)
{
	//TestSkyline();
	//UpdateEvaluation();
	Evaluation();
	//TestInsertSkyline();
	//TestDeleteSkyline();

#ifdef MEMORYLEAK
	_CrtDumpMemoryLeaks();
#endif

	return 0;
}