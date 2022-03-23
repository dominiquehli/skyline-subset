#ifndef _SCENARIO_H
#define _SCENARIO_H

#include "DataGenerator//DataGenerator.h"
#include "Common.h"
#include "Utility.h"
#include "SSkyline.h"
#include "SFS.h"
#include "SaLSa.h"
#include "BSkyTreeS.h"
#include "BSkyTreeP.h"
#include "InsertSkyTree.h"
#include "DeleteSkyTree.h"
#include "NaiveUpdate.h"
#include "SkyBand.h"

void TestDataGeneration();

void TestSkyline();
void TestInsertSkyline();
void TestDeleteSkyline();

void TestSkyBand();

#endif