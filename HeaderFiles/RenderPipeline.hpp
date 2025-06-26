#include <vector>
#include <functional>

class RenderPipeline {
    std::vector<std::function<void()>> pipeline;
public:
    RenderPipeline() {}
    void add(const std::function<void()>& func) {
        pipeline.push_back(func);
    }
    void run() {
        for (auto& func : pipeline) func();
    }
};