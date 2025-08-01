#ifndef mental_vulkan_support_h
#define mental_vulkan_support_h

#include "engine.h"

MentalResult mentalVKS
MentalResult mentalVKSCreateLogicalDevice(MentalEngine *pEngine);
MentalResult mentalVKSPickPhysicalDevice(MentalEngine *pEngine);
MentalResult mentalVKSCreateInstance(MentalEngine *pEngine);
MentalResult mentalVKSDrawFrame(MentalEngine *pEngine);

#endif // mental_vulkan_support_h