#ifndef mental_engine_h
#define mental_engine_h

#include "mental.h"

typedef struct MentalEngineInfo {
    MentalStructureType sType;          /** MENTAL_STRUCTURE_TYPE_ */
} MentalEngineInfo;

typedef struct MentalEngine {
    MentalStructureType sType;
    MentalEngineInfo    *pInfo;
} MentalEngine;

MentalResult mentalCreateEngine(MentalEngineInfo *pInfo, MentalEngine *pEngine);
MentalResult mentalRunEngine(MentalEngine *pEngine, MentalWindowManager *pManager);
MentalResult mentalDestroyEngine(MentalEngine *pEngine);

#endif // mental_engine_h