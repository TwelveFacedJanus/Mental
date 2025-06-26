#include "Component.hpp"

AnimatedSprite2D::AnimatedSprite2D(int rows_, int cols_, float frame_time_) {
    this->rows = rows_;
    this->cols = cols_;
    this->total_frames = rows * cols;
    this->animations["default"] = {0, total_frames-1, frame_time_, true};
    this->current_anim = "default";
    this->set_frame(0);
    this->frame_duration = frame_time_;
    this->tileset_path = "";
}

void AnimatedSprite2D::set_frame(int frame) {
    current_frame = frame % total_frames;
    int row = current_frame / cols;
    int col = current_frame % cols;
    float u0 = (float)col / cols;
    float v0 = (float)row / rows;
    float u1 = (float)(col+1) / cols;
    float v1 = (float)(row+1) / rows;
    float custom_vertices[20] = {
        0.5f,  0.5f, 0.0f, u1, v1, // top right
        0.5f, -0.5f, 0.0f, u1, v0, // bottom right
        -0.5f, -0.5f, 0.0f, u0, v0, // bottom left
        -0.5f,  0.5f, 0.0f, u0, v1  // top left
    };
    for (int i = 0; i < 20; ++i) this->vertices[i] = custom_vertices[i];
    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(custom_vertices), custom_vertices);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void AnimatedSprite2D::update_animation(float delta) {
    if (!playing || animations.empty() || animations.count(current_anim) == 0) return;
    Animation& anim = animations[current_anim];
    time_accum += delta;
    if (time_accum >= anim.frame_time) {
        time_accum -= anim.frame_time;
        int next = current_frame + 1;
        if (next > anim.end_frame) {
            if (anim.loop) next = anim.start_frame;
            else { next = anim.end_frame; playing = false; }
        }
        set_frame(next);
    }
}

void AnimatedSprite2D::play(const std::string& anim_name) {
    if (!anim_name.empty() && animations.count(anim_name)) {
        current_anim = anim_name;
        set_frame(animations[current_anim].start_frame);
    }
    playing = true;
}

void AnimatedSprite2D::pause() {
    playing = false;
}
void AnimatedSprite2D::stop() {
    playing = false;
    if (animations.count(current_anim))
        set_frame(animations[current_anim].start_frame);
}
void AnimatedSprite2D::add_animation(const std::string& name, int start, int end, float frame_time, bool loop) {
    animations[name] = {start, end, frame_time, loop};
}
void AnimatedSprite2D::remove_animation(const std::string& name) {
    animations.erase(name);
    if (current_anim == name && !animations.empty()) current_anim = animations.begin()->first;
}
void AnimatedSprite2D::set_animation_params(const std::string& name, int start, int end, float frame_time, bool loop) {
    if (animations.count(name)) animations[name] = {start, end, frame_time, loop};
}