#include <stdio.h>
#include <stdlib.h>

#include "engine/engine.h"


int main(int argc, const char* argv[])
{
    MentalWindowManagerInfo mwmInfo = {
        .sType = MENTAL_STRUCTURE_TYPE_WINDOW_INFO,
        .windowTitle = "Mental Engine.",
        .initialSizes = {800, 600},
    };
    MentalEngineInfo engineInfo = {
        .sType = MENTAL_STRUCTURE_TYPE_ENGINE_INFO,
        .wmInfo = mwmInfo,
    };
    MentalEngine engine = {};
    if (mentalEngineInitialize(engineInfo, &engine) != MENTAL_OK)
    {
        return MENTAL_FATAL;
    }
    mentalEngineRun(&engine);
    mentalEngineDestroy(&engine);
    
    return 0;
}