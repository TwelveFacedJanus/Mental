#include "mental_vk_support.h"



bool QFIisComplete(QueueFamilyIndicies qfi)
{
    return qfi.has_value && qfi.present_has_value;
}

QueueFamilyIndicies findQueueFamilies(MentalVK* mvk, VkPhysicalDevice device)
{
    QueueFamilyIndicies indices;
    indices.has_value = false;
    
    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, NULL);
    VkQueueFamilyProperties* queueFamilies = malloc(sizeof(VkQueueFamilyProperties) * queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies);

    int i = 0;
    for (uint32_t index = 0; index < queueFamilyCount; i++)
    {
        if (queueFamilies[index].queueFlags & VK_QUEUE_GRAPHICS_BIT)
        {
            indices.graphicsFamily = i;
            indices.has_value = true;
        }
        VkBool32 presentSupport = false;
        vkGetPhysicalDeviceSurfaceSupportKHR(device, i, mvk->surface, &presentSupport);
        if (presentSupport)
        {
            indices.presentFamily = i;
            indices.present_has_value = true;
        }
        if (indices.has_value) {
            break;
        }
        i++;
    }

    return indices;
}