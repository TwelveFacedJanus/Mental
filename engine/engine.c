#include "engine.h"
#include <stdio.h>
#include <stdlib.h>

MentalResult mentalEngineInitialize(MentalEngineInfo info, MentalEngine* engine)
{
    if (engine == NULL) {
        fprintf(stderr, "Failed to initialize MentalEngine. Engine is NULL.\n");
        return MENTAL_FATAL;
    }

    engine->wm = (MentalWindowManager*)malloc(sizeof(MentalWindowManager));
    if (engine->wm == NULL)
    {
        mentalEngineDestroy(engine);
        fprintf(stderr, "Failed to mallocate memory for MentalWindowManager.\n");
        return MENTAL_FATAL;
    }

    engine->mvk = (MentalVK*)malloc(sizeof(MentalVK));
    if (engine->mvk == NULL)
    {
        mentalEngineDestroy(engine);
        fprintf(stderr, "Failed to mallocate memory for MentalVK.\n");
        return MENTAL_FATAL;
    }

    if (mentalWindowManagerInitialize(info.wmInfo, engine->wm) != MENTAL_OK)
    {
        mentalEngineDestroy(engine);
        fprintf(stderr, "Some error while initializing GLFW.\n");
        return MENTAL_FATAL;
    }

    if (mentalVKInitialize(engine->mvk, engine->wm->window) != MENTAL_OK)
    {
        mentalEngineDestroy(engine);
        fprintf(stderr, "Failed to initialize vulkan instance.\n");
        return MENTAL_FATAL;
    }
    return MENTAL_OK;
}

MentalResult mentalEngineRun(MentalEngine* engine)
{
    if (engine == NULL)
    {
        fprintf(stderr, "Failed to run MentalEngine. Engine is NULL.\n");
        return MENTAL_FATAL;
    }
    mentalWindowManagerRun(engine->wm);
    return MENTAL_OK;
}

MentalResult mentalEngineDestroy(MentalEngine* engine)
{
    if (engine == NULL) return MENTAL_OK;

    if (engine->mvk != NULL) {
        mentalVKDestroy(engine->mvk);
    }

    if (engine->wm != NULL) {
        mentalWindowManagerDestroy(engine->wm);
    }
    return MENTAL_OK;
}