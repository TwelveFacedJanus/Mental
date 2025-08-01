/*
    ==------------------------------------------------==
            This file is part of mental engine.
    ==------------------------------------------------==
    @file       wm.h
    @brief      Window manager for mental engine.
    @details    This file contains all data for mental engine wm.
                You can use this file another engine.

                GLFW already included for this file by default.

    @author     TwelveFacedJanus
    @date       31.07.2025
    ==------------------------------------------------==
*/

#ifndef mental_wm_h
#define mental_wm_h

#include <GLFW/glfw3.h>
#include "mental.h"
#include "engine.h"

/*
    ==---------------------------------------------------==
        @brief      Window manager information structure.
        @details    Structure with info for creating
                    window manager using @ref mentalCreateWindowManager
                    function.

                    This typedef is part of @ref MentalProject and also
                    part of @ref MentalWindowManager.

        
        @example    ```c
                    MentalWindowManager info = {
                        .sType = MENTAL_STRUCTURE_TYPE_WM_INFO,
                        .windowSize = { 800, 600 },
                        .pWindowTitle = "Mental Opengl Window Manager."
                    };
                    ```

                    Also, you can not implement .sType and .pNext fields
                    if you want to use only window manager.
    ==----------------------------------------------------==
*/
typedef struct MentalWindowManagerInfo {
    MentalStructureType     sType;          /** Structure type. Use only MENTAL_STRUCTURE_TYPE_WM_INFO for this. */
    int                     windowSize[2];  /** Int array with window sizes (width, height) */
    const char*             pWindowTitle;   /** Window title. */
} MentalWindowManagerInfo;

/*
    ==-----------------------------------------------------==
        @brief      MentalWindowManager structure.
        @details    This struct will fill automatically using
                    @ref mentalCreateWindowManager function.

        @example    ```c
                    MentalWindowManager wm = {
                        .sType = MENTAL_STRUCTURE_TYPE_WM,
                    };
                    ```

                    And after creating `wm` u need to pass it
                    into @ref mentalCreateWindowManager
                    function.
    ==------------------------------------------------------==
*/
typedef struct MentalWindowManager {
    MentalStructureType          sType;   /** Use MENTAL_STRUCTURE_TYPE_WM by default */
    MentalWindowManagerInfo     *pInfo;   /** Pointer to MentalWindowManagerInfo struct */
    GLFWwindow                  *pWindow; /** Pointer to GLFW window */
    const void                  *pNext;   /** Pointer to next structure. */        
} MentalWindowManager;

MentalResult mentalCreateWindowManager(MentalWindowManagerInfo* pInfo, MentalWindowManager* pManager);
MentalResult mentalRunWindowManager(MentalWindowManager* pManager, MentalEngine *pEngine);
MentalResult mentalDestroyWindowManager(MentalWindowManager* pManager);

#endif // mental_wm_h