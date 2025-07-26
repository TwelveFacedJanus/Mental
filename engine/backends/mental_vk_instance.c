#include "mental_vk_support.h"

MentalResult mentalVKCreateInstance(MentalVK* mvk) {
    fprintf(stderr, "[DEBUG] mentalVKInitialize()\n");
    if (mvk == NULL) {
        fprintf(stderr, "Failed to initialize MentalVK. mvk is NULL.\n");
        return MENTAL_FATAL;
    }
    VkApplicationInfo appInfo = {};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "Mental";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "Mental";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_4;

    VkInstanceCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;
    createInfo.flags = VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;

    uint32_t glfwExtensionCount = 0;
    const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
    fprintf(stderr, "[DEBUG] glfwExtensionCount: %u\n", glfwExtensionCount);

    uint32_t extraExtensions = 1; // VK_KHR_portability_enumeration
    if (enableValidationLayers) {
        extraExtensions += 1; // VK_EXT_debug_utils
    }
    const uint32_t extensionCount = glfwExtensionCount + extraExtensions;
    const char** requiredExtensions = malloc(sizeof(char*) * extensionCount);
    if (!requiredExtensions) {
        fprintf(stderr, "Failed to allocate memory for requiredExtensions.\n");
        return MENTAL_FATAL;
    }

    for (uint32_t i = 0; i < glfwExtensionCount; i++) {
        requiredExtensions[i] = glfwExtensions[i];
        fprintf(stderr, "[DEBUG] Required extension[%u]: %s\n", i, glfwExtensions[i]);
    }
    uint32_t extIndex = glfwExtensionCount;
    requiredExtensions[extIndex++] = VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME;
    fprintf(stderr, "[DEBUG] Added extension: %s\n", VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME);
    if (enableValidationLayers) {
        requiredExtensions[extIndex++] = VK_EXT_DEBUG_UTILS_EXTENSION_NAME;
        fprintf(stderr, "[DEBUG] Added extension: %s\n", VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    }

    createInfo.enabledExtensionCount = extensionCount;
    createInfo.ppEnabledExtensionNames = requiredExtensions;

    if (enableValidationLayers && !checkValidationLayerSupport()) {
        fprintf(stderr, "Validation layers requested, but not available.\n");
        free(requiredExtensions);
        return MENTAL_FATAL;
    }

    VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo = {};

    if (enableValidationLayers) {
        createInfo.enabledLayerCount = validationLayerCount;
        createInfo.ppEnabledLayerNames = validationLayers;
        populateDebugMessengerCreateInfo(&debugCreateInfo);
        createInfo.pNext = &debugCreateInfo;
        fprintf(stderr, "[DEBUG] Validation layers enabled\n");
    } else {
        createInfo.enabledLayerCount = 0;
        createInfo.pNext = NULL;
        fprintf(stderr, "[DEBUG] Validation layers disabled\n");
    }

    fprintf(stderr, "[DEBUG] Creating Vulkan instance...\n");
    VkResult result = vkCreateInstance(&createInfo, NULL, &mvk->instance);
    free(requiredExtensions);
    if (result != VK_SUCCESS) {
        fprintf(stderr, "Failed to create VK instance. VkResult: %d\n", result);
        return MENTAL_FATAL;
    }
    fprintf(stderr, "[DEBUG] Vulkan instance created successfully\n");

    setupDebugMessenger(mvk);
    return MENTAL_OK;
}