#include "mental_vk_support.h"
#include <stdlib.h>

VkSurfaceFormatKHR chooseSwapSurfaceFormat(const VkSurfaceFormatKHR* availableFormats) {
    uint32_t availableFormats_count = sizeof(availableFormats) / sizeof(availableFormats[0]);
    for (uint32_t i = 0; i < availableFormats_count; i++)
    {
        if (availableFormats[i].format == VK_FORMAT_B8G8R8A8_SRGB && availableFormats[i].colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
        {
            return availableFormats[i];
        }
    }
    return availableFormats[0];
}
VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR* capabilities, GLFWwindow* window) {
    if (capabilities->currentExtent.width != UINT32_MAX) {
        return capabilities->currentExtent;
    }
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);

    VkExtent2D actualExtent = { width, height };


    actualExtent.width = (actualExtent.width < capabilities->minImageExtent.width) ? capabilities->minImageExtent.width : 
                     (actualExtent.width > capabilities->maxImageExtent.width) ? capabilities->maxImageExtent.width : 
                     actualExtent.width;
    actualExtent.height = (actualExtent.height < capabilities->minImageExtent.height) ? capabilities->minImageExtent.height : 
                      (actualExtent.height > capabilities->maxImageExtent.height) ? capabilities->maxImageExtent.height : 
                      actualExtent.height;

    
    return actualExtent;
}


void mentalVKCreateSwapchain(MentalVK* mvk) {
    SwapChainSupportDetails swapChainSupport = querySwapChainSupport(mvk->physicalDevice);
    VkSurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(swapChainSupport.formats);
    VkPresentModeKHR presentMode = chooseSwapPresentMode(swapChainSupport.presentModes);
    VkExtent2D extent = chooseSwapExtent(swapChainSupport.capabilities);

    uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;
    if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount) {
        imageCount = swapChainSupport.capabilities.maxImageCount;
    }

    VkSwapchainCreateInfoKHR createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    createInfo.surface = mvk->surface;

    createInfo.minImageCount = imageCount;
    createInfo.imageFormat = surfaceFormat.format;
    createInfo.imageColorSpace = surfaceFormat.colorSpace;
    createInfo.imageExtent = extent;
    createInfo.imageArrayLayers = 1;
    createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

    QueueFamilyIndicies indices = findQueueFamilies(mvk, mvk->physicalDevice);
    uint32_t queueFamilyIndices[] = {indices.graphicsFamily, indices.presentFamily};

    if (indices.graphicsFamily != indices.presentFamily) {
        createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        createInfo.queueFamilyIndexCount = 2;
        createInfo.pQueueFamilyIndices = queueFamilyIndices;
    } else {
        createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
    }

    createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
    createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    createInfo.presentMode = presentMode;
    createInfo.clipped = VK_TRUE;

    createInfo.oldSwapchain = VK_NULL_HANDLE;

    if (vkCreateSwapchainKHR(mvk->device, &createInfo, NULL, &mvk->swapChain) != VK_SUCCESS) {
        throw std::runtime_error("failed to create swap chain!");
    }

    vkGetSwapchainImagesKHR(device, swapChain, &imageCount, nullptr);
    swapChainImages.resize(imageCount);
    vkGetSwapchainImagesKHR(device, swapChain, &imageCount, swapChainImages.data());

    swapChainImageFormat = surfaceFormat.format;
    swapChainExtent = extent;
}

SwapChainSupportDetails querySwapChainSupport(MentalVK* mvk, VkPhysicalDevice device) {
    SwapChainSupportDetails details;
    details.formats = NULL;
    details.presentModes = NULL;
    details.formats_count = 0;
    details.present_modes_count = 0;

    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, mvk->surface, &details.capabilities);

    uint32_t formatCount = 0;
    vkGetPhysicalDeviceSurfaceFormatsKHR(device, mvk->surface, &formatCount, NULL);

    if (formatCount != 0) {
        details.formats = malloc(sizeof(VkSurfaceFormatKHR) * formatCount);
        vkGetPhysicalDeviceSurfaceFormatsKHR(device, mvk->surface, &formatCount, details.formats);
        details.formats_count = formatCount;
    }

    uint32_t presentModeCount = 0;
    vkGetPhysicalDeviceSurfacePresentModesKHR(device, mvk->surface, &presentModeCount, NULL);

    if (presentModeCount != 0) {
        details.presentModes = malloc(sizeof(VkPresentModeKHR) * presentModeCount);
        vkGetPhysicalDeviceSurfacePresentModesKHR(device, mvk->surface, &presentModeCount, details.presentModes);
        details.present_modes_count = presentModeCount;
    }

    return details;
}