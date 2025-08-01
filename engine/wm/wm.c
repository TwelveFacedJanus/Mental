/*
    ==-------------------------------------------==
            This file is part of Mental Engine.
    ==-------------------------------------------==
        @file       wm.c
        @brief      MentalWindowManager realization
        @details    This file contains realization of
                    @ref wm.h file.

        @author     TwelveFacedJanus
        @date       31.07.2025
    ==--------------------------------------------==
*/

#include "wm.h"

MentalResult mentalCreateWindowManager(MentalWindowManagerInfo *pInfo, MentalWindowManager *pManager)
{
    if (pInfo == NULL || pManager == NULL) return MENTAL_POINTER_IS_NULL;
    
    pManager->pInfo = pInfo;

    return MENTAL_OK;
}

MentalResult mentalRunWindowManager(MentalWindowManager *pManager, MentalEngine* pEngine) {
    if (pManager == NULL || pEngine == NULL) return MENTAL_POINTER_IS_NULL;
    
    while (!glfwWindowShouldClose(pManager->pWindow))
    {
        mentalRunEngine(pEngine, pManager);
    }
    return MENTAL_OK;
}

MentalResult mentalDestroyWindowManager(MentalWindowManager *pManager)
{
    if (pManager == NULL) return MENTAL_POINTER_IS_NULL;
    return MENTAL_OK;
}