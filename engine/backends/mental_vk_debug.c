#include "mental_vk_support.h"

const char* validationLayers[] = { "VK_LAYER_KHRONOS_validation" };
const unsigned int validationLayerCount = sizeof(validationLayers) / sizeof(validationLayers[0]);
#ifdef NDEBUG
const bool enableValidationLayers = false;
#else
const bool enableValidationLayers = true;
#endif


bool checkValidationLayerSupport()
{
    fprintf(stderr, "[DEBUG] checkValidationLayerSupport()\n");
    uint32_t layerCount;
    vkEnumerateInstanceLayerProperties(&layerCount, NULL);

    VkLayerProperties* availableLayers = malloc(sizeof(VkLayerProperties) * layerCount);
    if (!availableLayers) {
        fprintf(stderr, "Failed to allocate memory for VkLayerProperties.\n");
        return false;
    }
    vkEnumerateInstanceLayerProperties(&layerCount, availableLayers);

    for (uint32_t i = 0; i < sizeof(validationLayers) / sizeof(validationLayers[0]); i++) {
        bool layerFound = false;
        for (uint32_t j = 0; j < layerCount; j++) {
            if (strcmp(validationLayers[i], availableLayers[j].layerName) == 0) {
                layerFound = true;
                break;
            }
        }
        if (!layerFound) {
            fprintf(stderr, "[DEBUG] Validation layer not found: %s\n", validationLayers[i]);
            free(availableLayers);
            return false;
        }
    }

    free(availableLayers);
    fprintf(stderr, "[DEBUG] All requested validation layers are available\n");
    return true;
}

void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT* createInfo)
{
    fprintf(stderr, "[DEBUG] populateDebugMessengerCreateInfo()\n");
    createInfo->sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    createInfo->messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
                                  VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
                                  VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    createInfo->messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
                              VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
                              VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    createInfo->pfnUserCallback = debugCallback;
    createInfo->pUserData = NULL;
    createInfo->flags = 0;
}

void setupDebugMessenger(MentalVK* mvk)
{
    fprintf(stderr, "[DEBUG] setupDebugMessenger()\n");
    if (!enableValidationLayers) {
        fprintf(stderr, "[DEBUG] Validation layers disabled, skipping debug messenger setup\n");
        return;
    }

    VkDebugUtilsMessengerCreateInfoEXT createInfo;
    populateDebugMessengerCreateInfo(&createInfo);

    PFN_vkCreateDebugUtilsMessengerEXT func = (PFN_vkCreateDebugUtilsMessengerEXT)
        vkGetInstanceProcAddr(mvk->instance, "vkCreateDebugUtilsMessengerEXT");
    if (func != NULL) {
        VkResult res = func(mvk->instance, &createInfo, NULL, &mvk->debugMessenger);
        if (res != VK_SUCCESS) {
            fprintf(stderr, "Failed to set up debug messenger! VkResult: %d\n", res);
            return;
        }
        fprintf(stderr, "[DEBUG] Debug messenger created successfully\n");
    } else {
        fprintf(stderr, "[DEBUG] vkCreateDebugUtilsMessengerEXT not found\n");
    }
}

void DestroyDebugUtilsMessengerEXT(
    VkInstance instance,
    VkDebugUtilsMessengerEXT messenger,
    const VkAllocationCallbacks* pAllocator
) {
    fprintf(stderr, "[DEBUG] DestroyDebugUtilsMessengerEXT()\n");
    PFN_vkDestroyDebugUtilsMessengerEXT func = (PFN_vkDestroyDebugUtilsMessengerEXT)
        vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
    if (func != NULL) {
        func(instance, messenger, pAllocator);
        fprintf(stderr, "[DEBUG] Debug messenger destroyed\n");
    } else {
        fprintf(stderr, "[DEBUG] vkDestroyDebugUtilsMessengerEXT not found\n");
    }
}