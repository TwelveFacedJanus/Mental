#ifndef MENTAL_WM_H
#define MENTAL_WM_H

#include "../../core/mental.h"

typedef struct MentalWindowManagerInfo {
    MentalStructureTypes sType;
    int initialSizes[2];
    char* windowTitle;
} MentalWindowManagerInfo;

typedef struct MentalWindowManager {
    MentalStructureTypes sType;
    GLFWwindow* window;
} MentalWindowManager;

MentalResult mentalGLFWCreateWindow(MentalWindowManagerInfo info, MentalWindowManager* manager);
MentalResult mentalGLFWSetWindowHints();
MentalResult mentalInitializeGLFW();
MentalResult mentalWindowManagerInitialize(MentalWindowManagerInfo info, MentalWindowManager* manager);
MentalResult mentalWindowManagerDestroy(MentalWindowManager* mwm);
MentalResult mentalWindowManagerRun(MentalWindowManager* mwm);

#endif // MENTAL_WM_H