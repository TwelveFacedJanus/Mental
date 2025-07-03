#include <vector>
#include <functional>
#include <algorithm>
#include "Component.hpp"
#include "Mental.hpp"

typedef struct MentalRenderPipeline {
    std::vector<std::pair<MentalComponent*, std::function<void()>>> pipeline;
} MentalRenderPipeline;

MentalResult mentalRenderPipelineInitialize(MentalRenderPipeline* pipeline) {
    if (pipeline == nullptr) return MENTAL_ERROR;
    pipeline->pipeline.clear();
    return MENTAL_OK;
}
MentalResult mentalRenderPipelineAdd(MentalRenderPipeline* pipeline, MentalComponent* component, const std::function<void()>& func) {
    if (pipeline == nullptr || component == nullptr) return MENTAL_ERROR;
    pipeline->pipeline.emplace_back(component, func);
    return MENTAL_OK;
}

class RenderPipeline {
    std::vector<std::pair<Component*, std::function<void()>>> pipeline;
public:
    RenderPipeline() {}
    void add(Component* comp, const std::function<void()>& func) {
        pipeline.emplace_back(comp, func);
    }
    void run() {
        for (auto& pair : pipeline) pair.second();
    }
    // Удаляет все функции, связанные с данным компонентом (по указателю)
    void remove_for_component(Component* comp) {
        pipeline.erase(
            std::remove_if(pipeline.begin(), pipeline.end(),
                [comp](const auto& pair) { return pair.first == comp; }),
            pipeline.end()
        );
    }
};