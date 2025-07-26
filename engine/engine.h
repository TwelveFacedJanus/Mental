#ifndef MENTAL_ENGINE_H
#define MENTAL_ENGINE_H

#include "../core/mental.h"
#include "windowmanager/wm.h"
#include "backends/mental_vk_support.h"

typedef struct MentalEngineInfo {
    MentalStructureTypes sType;
    MentalWindowManagerInfo wmInfo;
} MentalEngineInfo;

typedef struct MentalEngine {
    MentalStructureTypes sType;
    MentalWindowManager* wm;
    MentalVK* mvk;
} MentalEngine;

MentalResult mentalEngineInitialize(MentalEngineInfo info, MentalEngine* engine);
MentalResult mentalEngineRun(MentalEngine* engine);
MentalResult mentalEngineDestroy(MentalEngine* engine);
#endif // MENTAL_ENGINE_H