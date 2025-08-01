/*
    ==------------------------------------------------==
            This file is part of mental engine.
    ==------------------------------------------------==
    @file       project.h
    @brief      Project for Mental Engine.
    @details    In Mental Engine we have project.
                Project contains engine and wm.

    @author     TwelveFacedJanus
    @date       31.07.2025
    ==------------------------------------------------==
*/

#ifndef mental_project_h
#define mental_project_h

#include "mental.h"
#include "wm.h"
#include "engine.h"

/*
    ==-------------------------------------------------==
        @brief      MentalProjectInfo
        @details    Information about project

        @example    ```c
                    MentalProjectInfo projectInfo = {
                        .sType = MENTAL_STRUCTURE_TYPE_PROJECT_INFO,
                        .pProjectName = "Mental Project",
                        .projectVersion = 101,
                        .pWindowInfo = &windowInfo,
                        .pEngineInfo = &engineInfo,
                    };
                    ```
    ==-------------------------------------------------==
*/
typedef struct MentalProjectInfo {
    MentalStructureType      sType;          /** MENTAL_STURCTURE_TYPE_PROJECT_INFO */
    const char              *pProjectName;   /** Pointer to project name */
    uint32_t                 projectVersion; /** Version of project. eg 100 or 201 */

    MentalWindowManagerInfo *pWindowInfo;    /** Pointer to WindowManagerInfo */
    MentalEngineInfo        *pEngineInfo;    /** Pointer to MentalEngineInfo */
} MentalProjectInfo;


/*
    ==-------------------------------------------------==
        @brief      MentalProject core structure.
        @details    MentalProject is a core structure for
                    project.

        @example    ```c
                    MentalProject project = {
                        .sType = MENTAL_STRUCTURE_TYPE_PROJECT,
                    };
    ==-------------------------------------------------==
*/
typedef struct MentalProject {
    MentalStructureType     sType;           /** MENTAL_STRUCTURE_TYPE_PROJECT */
    MentalProjectInfo       *pInfo;          /** Pointer to MentalProjectInfo  */

    MentalEngine            *pEngine;        /** Pointer to Engine */
    MentalWindowManager     *pWindowManager; /** Pointer to WindowManager */
} MentalProject;

MentalResult mentalCreateProject(MentalProjectInfo *pInfo, MentalProject *pProject);
MentalResult mentalRunProject(MentalProject *pProject);
MentalResult mentalDestroyProject(MentalProject *pProject);

#endif // mental_project_h