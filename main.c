#include "mental.h"
#include "wm.h"
#include "engine.h"
#include "project.h"

int main(int argc, const char* argv[])
{
    MentalEngineInfo mei = {
        .sType = MENTAL_STRUCTURE_TYPE_ENGINE_INFO,
    };

    MentalWindowManagerInfo mwmi = {
        .sType = MENTAL_STRUCTURE_TYPE_WM_INFO,
        .pWindowTitle = "MentalEngine",
        .windowSize = {800, 600},
    };

    MentalProjectInfo mpi = {
        .sType = MENTAL_STRUCTURE_TYPE_PROJECT_INFO,
        .pProjectName = "Mental",
        .projectVersion = 100,
        .pEngineInfo = &mei,
        .pWindowInfo = &mwmi,
    };

    MentalProject project = {
        .sType = MENTAL_STRUCTURE_TYPE_PROJECT,
    };

    mentalCreateProject(&mpi, &project);
    mentalRunProject(&project);
    mentalDestroyProject(&project);
}