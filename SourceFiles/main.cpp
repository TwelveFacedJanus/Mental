#include "Mental.hpp"


int main()
{
    MentalEngineCore mec;
    mentalEngineInitialize(&mec);
    mentalEngineMainLoop(&mec);
    return 0;
}