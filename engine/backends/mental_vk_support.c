#include "mental_vk_support.h"

MentalResult mentalVKInitialize(MentalVK* mvk, GLFWwindow* window)
{
    if (mvk == NULL) return MENTAL_FATAL;
    
    mentalVKCreateInstance(mvk);
    createSurface(mvk, window);
    pickPhysicalDevice(mvk);
    createLogicalDevice(mvk);
}

MentalResult mentalVKDestroy(MentalVK* mvk)
{
    fprintf(stderr, "[DEBUG] mentalVKDestroy()\n");
    if (mvk == NULL) return MENTAL_OK;
    
    if (enableValidationLayers && mvk->debugMessenger)
        DestroyDebugUtilsMessengerEXT(mvk->instance, mvk->debugMessenger, NULL);
   
    vkDestroySurfaceKHR(mvk->instance, mvk->surface, NULL);
    vkDestroyDevice(mvk->device, NULL);
    fprintf(stderr, "[DEBUG] Destroying Vulkan instance...\n");
    vkDestroyInstance(mvk->instance, NULL);
    fprintf(stderr, "[DEBUG] Freeing MentalVK struct...\n");
    free(mvk);

    return MENTAL_OK;
}