#include "project.h"

MentalResult mentalCreateProject(MentalProjectInfo *pInfo, MentalProject *pProject)
{
    if (pInfo == NULL || pProject == NULL) return MENTAL_POINTER_IS_NULL;
    pProject->pInfo = pInfo;

    MentalResult result = 0;
    
    //==-------- Allocate memory for window manager. ------------==//
    pProject->pWindowManager = (MentalWindowManager*)malloc(sizeof(MentalWindowManager));
    if (pProject->pWindowManager == NULL) return MENTAL_FAILED_TO_ALLOCATE_MEMORY_FOR_MENTAL_WM;
    
    //==--------- Allocate memory for engine.       -------------==//
    pProject->pEngine = (MentalEngine*)malloc(sizeof(MentalEngine));
    if (pProject->pEngine == NULL) return MENTAL_FAILED_TO_ALLOCATE_MEMORY_FOR_ENGINE;

    //==---------- Creating window manager.         --------------==//
    result = mentalCreateWindowManager(pProject->pInfo->pWindowInfo, pProject->pWindowManager);
    if (result != MENTAL_OK) return MENTAL_UNIMPLEMENTED_ERROR;

    //==---------- Creating engine.         --------------==//
    result = mentalCreateEngine(pProject->pInfo->pEngineInfo, pProject->pEngine);
    if (result != MENTAL_OK) return MENTAL_UNIMPLEMENTED_ERROR;

    return MENTAL_OK;
}

MentalResult mentalRunProject(MentalProject *pProject)
{
    if (pProject == NULL) return MENTAL_POINTER_IS_NULL;

    return MENTAL_OK;
}

MentalResult mentalDestroyProject(MentalProject *pProject)
{
    if (pProject == NULL) return MENTAL_OK;
    
    if (pProject->pWindowManager)
    {
        mentalDestroyWindowManager(pProject->pWindowManager);
        free(pProject->pWindowManager);
    }
    if (pProject->pEngine)
    {
        mentalDestroyEngine(pProject->pEngine);
        free(pProject->pEngine);
    }
    
    return MENTAL_OK;
}