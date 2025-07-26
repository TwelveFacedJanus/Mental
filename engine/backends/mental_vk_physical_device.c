#include "mental_vk_support.h"
#include <string.h>
#include <stdbool.h>

bool checkDeviceExtensionSupport(VkPhysicalDevice device, const char** deviceExtensions, uint32_t deviceExtensionCount) {
    uint32_t extensionCount;
    vkEnumerateDeviceExtensionProperties(device, NULL, &extensionCount, NULL);

    VkExtensionProperties* availableExtensions = malloc(extensionCount * sizeof(VkExtensionProperties));
    if (!availableExtensions) {
        fprintf(stderr, "Failed to allocate memory for extension properties\n");
        return false;
    }

    vkEnumerateDeviceExtensionProperties(device, NULL, &extensionCount, availableExtensions);

    // Create a copy of required extensions to track which ones we've found
    bool* foundExtensions = malloc(deviceExtensionCount * sizeof(bool));
    if (!foundExtensions) {
        fprintf(stderr, "Failed to allocate memory for extension tracking\n");
        free(availableExtensions);
        return false;
    }
    memset(foundExtensions, 0, deviceExtensionCount * sizeof(bool));

    // Check each available extension against our requirements
    for (uint32_t i = 0; i < extensionCount; i++) {
        for (uint32_t j = 0; j < deviceExtensionCount; j++) {
            if (strcmp(availableExtensions[i].extensionName, deviceExtensions[j]) == 0) {
                foundExtensions[j] = true;
                break;
            }
        }
    }

    // Verify all required extensions were found
    bool allFound = true;
    for (uint32_t i = 0; i < deviceExtensionCount; i++) {
        if (!foundExtensions[i]) {
            allFound = false;
            break;
        }
    }

    free(availableExtensions);
    free(foundExtensions);
    return allFound;
}

bool isDeviceSuitable(MentalVK* mvk, VkPhysicalDevice device) {
    QueueFamilyIndicies indices = findQueueFamilies(mvk, device);
    
    // Define required extensions
    const char* deviceExtensions[] = {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME
    };
    uint32_t deviceExtensionCount = sizeof(deviceExtensions) / sizeof(deviceExtensions[0]);
    
    bool extensionsSupported = checkDeviceExtensionSupport(device, deviceExtensions, deviceExtensionCount);
    
    // Check if swap chain is adequate if we need it
    bool swapChainAdequate = false;
    if (extensionsSupported) {
        // You would need to implement querying swap chain support here
        // swapChainAdequate = querySwapChainSupport(device);
        swapChainAdequate = true; // Temporary for compilation
    }
    
    VkPhysicalDeviceFeatures supportedFeatures;
    vkGetPhysicalDeviceFeatures(device, &supportedFeatures);
    
    return indices.has_value && 
           indices.present_has_value && 
           extensionsSupported && 
           swapChainAdequate &&
           supportedFeatures.samplerAnisotropy; // Example feature check
}

void pickPhysicalDevice(MentalVK* mvk) {
    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(mvk->instance, &deviceCount, NULL);
    if (deviceCount == 0) {
        fprintf(stderr, "Failed to find GPUs with Vulkan support!\n");
        return;
    }
    
    VkPhysicalDevice* devices = malloc(sizeof(VkPhysicalDevice) * deviceCount);
    if (!devices) {
        fprintf(stderr, "Failed to allocate memory for physical devices\n");
        return;
    }
    
    vkEnumeratePhysicalDevices(mvk->instance, &deviceCount, devices);

    for (uint32_t i = 0; i < deviceCount; i++) {
        if (isDeviceSuitable(mvk, devices[i])) {
            mvk->physicalDevice = devices[i];
            break;
        }
    }
    
    free(devices);
    
    if (mvk->physicalDevice == VK_NULL_HANDLE) {
        fprintf(stderr, "Failed to find a suitable GPU.\n");
        return;
    }
    
    // Optional: Print selected device name
    VkPhysicalDeviceProperties deviceProperties;
    vkGetPhysicalDeviceProperties(mvk->physicalDevice, &deviceProperties);
    fprintf(stderr, "Selected physical device: %s\n", deviceProperties.deviceName);
}