#ifndef MENTAL_VK_SUPPORT_H
#define MENTAL_VK_SUPPORT_H

#include "../../core/mental.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <GLFW/glfw3.h>

//==--------------------------------------------------------------------------==//
//                          MENTAL_VK_SUPPORT                                 //
//==--------------------------------------------------------------------------==//
typedef struct MentalVK {
    VkInstance instance;
    VkDebugUtilsMessengerEXT debugMessenger;

    VkPhysicalDevice physicalDevice;
    VkDevice device;

    VkSurfaceKHR surface;

    VkQueue graphicsQueue;
    VkQueue presentQueue;

    VkSwapchainKHR swapChain;
} MentalVK;

typedef struct QueueFamilyIndicies {
    uint32_t graphicsFamily;
    uint32_t presentFamily;
    bool has_value;
    bool present_has_value;
} QueueFamilyIndicies;

typedef struct SwapChainSupportDetails {
    VkSurfaceCapabilitiesKHR capabilities;
    VkSurfaceFormatKHR* formats;
    VkPresentModeKHR* presentModes;

    uint32_t formats_count;
    uint32_t present_modes_count;
} SwapChainSupportDetails;

//==--------------------------------------------------------------------------==//
//                       MENTAL_VK_DEBUG                                        //
//==--------------------------------------------------------------------------==//
extern const char* validationLayers[];
extern const unsigned int validationLayerCount;
extern const bool enableValidationLayers;

static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
                                                    VkDebugUtilsMessageTypeFlagsEXT messageType,
                                                    const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData) {
    fprintf(stderr, "[DEBUG] debugCallback called\n");
    if (pCallbackData && pCallbackData->pMessage) {
        fprintf(stderr, "Validation layer: %s\n", pCallbackData->pMessage);
    } else {
        fprintf(stderr, "Validation layer: (null message)\n");
    }
    return VK_FALSE;
}

bool checkValidationLayerSupport();
void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT* createInfo);
void setupDebugMessenger(MentalVK* mvk);
void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT messenger,
                                   const VkAllocationCallbacks* pAllocator);

//==--------------------------------------------------------------------------==//
//                          MENTAL_LOGICAL_DEVICE                               //
//==--------------------------------------------------------------------------==//
void getUniqueQueueFamilies(const QueueFamilyIndicies* indices, uint32_t* uniqueFamilies, uint32_t* count);
void createLogicalDevice(MentalVK* mvk);

//==--------------------------------------------------------------------------==//
//                          MENTAL_PHYSICAL_DEVICE                              //
//==--------------------------------------------------------------------------==//
bool checkDeviceExtensionSupport(VkPhysicalDevice device, const char** deviceExtensions, uint32_t deviceExtensionCount);
bool isDeviceSuitable(MentalVK* mvk, VkPhysicalDevice device);
void pickPhysicalDevice(MentalVK* mvk);

//==--------------------------------------------------------------------------==//
//                          MENTAL_VK_INSTANCE                                  //
//==--------------------------------------------------------------------------==//
MentalResult mentalVKCreateInstance(MentalVK* mvk);
MentalResult mentalVKInitialize(MentalVK* mvk, GLFWwindow* window);
MentalResult mentalVKDestroy(MentalVK* mvk);

//==--------------------------------------------------------------------------==//
//                          MENTAL_QUEUE_FAMILIES                               //
//==--------------------------------------------------------------------------==//
bool QFIisComplete(QueueFamilyIndicies qfi);
QueueFamilyIndicies findQueueFamilies(MentalVK* mvk, VkPhysicalDevice device);

//==--------------------------------------------------------------------------==//
//                          MENTAL_SURFACE                                      //
//==--------------------------------------------------------------------------==//
void createSurface(MentalVK* mvk, GLFWwindow* window);

#endif // MENTAL_VK_SUPPORT_H