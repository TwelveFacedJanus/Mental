#include "wm.h"
#include <stdio.h>
#include <GLFW/glfw3.h>
#include <stdlib.h>

MentalResult mentalInitializeGLFW()
{
    if (glfwInit() != GLFW_TRUE)
    {
        return MENTAL_FATAL;
    }
    return MENTAL_OK;
}
MentalResult mentalGLFWSetWindowHints()
{
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    return MENTAL_OK;
}

MentalResult mentalGLFWCreateWindow(MentalWindowManagerInfo info, MentalWindowManager* manager)
{
    manager->window = glfwCreateWindow(info.initialSizes[0], info.initialSizes[1], info.windowTitle, NULL, NULL);
    if (manager->window == NULL)
    {
        return MENTAL_FATAL;
    }
    return MENTAL_OK;
}

MentalResult mentalWindowManagerInitialize(MentalWindowManagerInfo info, MentalWindowManager* manager)
{
    if (manager == NULL)
    {
        fprintf(stderr, "Failed to initialize MentalWindowManager. Manager is NULL.\n");
    }
    if (mentalInitializeGLFW() != MENTAL_OK)
    {
        fprintf(stderr, "Failed to initialize GLFW library.\n");
        return MENTAL_FATAL;
    }
    mentalGLFWSetWindowHints();
    if (mentalGLFWCreateWindow(info, manager) != MENTAL_OK)
    {
        glfwTerminate();
        fprintf(stderr, "Failed to create GLFW window.\n");
        return MENTAL_FATAL; 
    }

    printf("MentalWindowManagerInitialize OK.\n");
    return MENTAL_OK;
}

MentalResult mentalWindowManagerDestroy(MentalWindowManager* mwm)
{
    if (mwm == NULL) return MENTAL_OK;
    
    if (mwm->window) {
        glfwDestroyWindow(mwm->window);
    }
    glfwTerminate();
    free(mwm);
    return MENTAL_OK;
}

MentalResult mentalWindowManagerRun(MentalWindowManager* mwm)
{
    if (mwm == NULL)
    {
        return MENTAL_FATAL;
    }

    while (!glfwWindowShouldClose(mwm->window))
    {
        glfwPollEvents();
        glfwSwapBuffers(mwm->window);
    }
    return MENTAL_OK;
}