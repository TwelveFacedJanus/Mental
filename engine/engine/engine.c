#include "engine.h"
#include "wm.h"

MentalResult mentalCreateEngine(MentalEngineInfo *pInfo, MentalEngine *pEngine)
{
    if (pInfo == NULL || pEngine == NULL) return MENTAL_POINTER_IS_NULL;
    return MENTAL_OK;
}

MentalResult mentalRunEngine(MentalEngine *pEngine, MentalWindowManager *pManager) {
    if (pEngine == NULL || pManager == NULL) return MENTAL_POINTER_IS_NULL;
    return MENTAL_OK;
}

MentalResult mentalDestroyEngine(MentalEngine *pEngine) {
    if (pEngine == NULL) return MENTAL_OK;
    return MENTAL_OK;
}