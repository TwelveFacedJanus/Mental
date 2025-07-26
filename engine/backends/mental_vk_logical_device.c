#include "mental_vk_support.h"

void getUniqueQueueFamilies(const QueueFamilyIndicies* indices, uint32_t* uniqueFamilies, uint32_t* count) {
    *count = 0;
    
    // Add graphics family if it exists
    if (indices->has_value) {
        uniqueFamilies[(*count)++] = indices->graphicsFamily;
    }
    
    // Add present family if it exists and is different from graphics family
    if (indices->present_has_value && 
        (!indices->has_value || indices->presentFamily != indices->graphicsFamily)) {
        uniqueFamilies[(*count)++] = indices->presentFamily;
    }
}

void createLogicalDevice(MentalVK* mvk) {
    QueueFamilyIndicies indices = findQueueFamilies(mvk, mvk->physicalDevice);
    
    // Queue creation info
    float queuePriority = 1.0f;
    
    
    uint32_t uniqueFamilies[2];
    uint32_t count;
    getUniqueQueueFamilies(&indices, uniqueFamilies, &count);

    VkDeviceQueueCreateInfo* queueCreateInfos = malloc(count * sizeof(VkDeviceQueueCreateInfo));
    for (uint32_t i = 0; i < count; i++)
    {
        VkDeviceQueueCreateInfo queueCreateInfo = {};
        queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueCreateInfo.queueFamilyIndex = uniqueFamilies[i];
        queueCreateInfo.queueCount = 1;
        queueCreateInfo.pQueuePriorities = &queuePriority;
        queueCreateInfos[i] = queueCreateInfo;
    }
    
    // Device extensions (as an array)
    const char* deviceExtensions[] = {
        VK_KHR_SWAPCHAIN_EXTENSION_NAME,
        "VK_KHR_portability_subset"  // or VK_KHR_PORTABILITY_SUBSET_EXTENSION_NAME if defined
    };
    const uint32_t deviceExtensionCount = sizeof(deviceExtensions) / sizeof(deviceExtensions[0]);

    // Device features
    VkPhysicalDeviceFeatures deviceFeatures = {};

    // Device create info
    VkDeviceCreateInfo createInfo = {
        .sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
        .pQueueCreateInfos = queueCreateInfos,
        .queueCreateInfoCount = count,
        .pEnabledFeatures = &deviceFeatures,
        .enabledExtensionCount = deviceExtensionCount,
        .ppEnabledExtensionNames = deviceExtensions
    };

    // Validation layers
    if (enableValidationLayers) {
        createInfo.enabledLayerCount = validationLayerCount;
        createInfo.ppEnabledLayerNames = validationLayers;
    } else {
        createInfo.enabledLayerCount = 0;
    }

    // Create the device
    if (vkCreateDevice(mvk->physicalDevice, &createInfo, NULL, &mvk->device) != VK_SUCCESS) {
        fprintf(stderr, "Failed to create logical device.\n");
        return;
    }
    vkGetDeviceQueue(mvk->device, indices.graphicsFamily, 0, &mvk->graphicsQueue);
    vkGetDeviceQueue(mvk->device, indices.presentFamily, 0, &mvk->presentQueue);
    free(queueCreateInfos);
}