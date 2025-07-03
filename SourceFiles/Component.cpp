#include "Component.hpp"
#include <memory>


MentalResult mentalComponentCreate(MentalComponentInfo* info, MentalComponent* component) {
    if (component == nullptr || info == nullptr ) return MENTAL_ERROR;
    component->info = *info;
    component->L = luaL_newstate();
    luaL_openlibs(component->L);
    
    // Register component type in Lua
    switch (info->cType) {
        case MENTAL_COMPONENT_SPRITE2D:
            break;
        case MENTAL_COMPONENT_TRIANGLE:
            _mentalCreateTriangle(component);
            break;
        case MENTAL_COMPONENT_CAMERA:
            break;
        default:
            return MENTAL_ERROR; // Unsupported component type
    }
    return MENTAL_OK;
}
