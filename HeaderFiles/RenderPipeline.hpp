#include <vector>
#include <functional>
#include <algorithm>
#include "Component.hpp"

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