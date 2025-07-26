#include "mental_vk_support.h"

void createSurface(MentalVK* mvk, GLFWwindow* window) {
    if (glfwCreateWindowSurface(mvk->instance, window, NULL, &mvk->surface) != VK_SUCCESS) {
        fprintf(stderr, "Failed to create Window surface.\n");
        return;
    }
}