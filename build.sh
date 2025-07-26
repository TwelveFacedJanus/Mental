#!/bin/bash

export VK_LAYER_PATH=/opt/homebrew/share/vulkan/explicit_layer.d
export DYLD_LIBRARY_PATH=/opt/homebrew/lib:$DYLD_LIBRARY_PATH

clang main.c \
    engine/engine.c \
    engine/windowmanager/wm.c \
    engine/backends/mental_vk_support.c \
    engine/backends/mental_vk_instance.c \
    engine/backends/mental_vk_debug.c \
    engine/backends/mental_vk_queue_families.c \
    engine/backends/mental_vk_physical_device.c \
    engine/backends/mental_vk_logical_device.c \
    engine/backends/mental_vk_surface.c \
    -L/opt/homebrew/lib \
    -I/opt/homebrew/include \
    -I/opt/homebrew/opt/molten-vk/include \
    -L/opt/homebrew/opt/molten-vk/lib \
    -framework OpenGL \
    -lglfw -lvulkan -lMoltenVK \
    -o Mental

# Для работы Vulkan на macOS через MoltenVK:
# export VK_ICD_FILENAMES=$VULKAN_SDK/share/vulkan/icd.d/MoltenVK_icd.json