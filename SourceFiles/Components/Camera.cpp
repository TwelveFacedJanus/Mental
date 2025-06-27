#include "Component.hpp"


Camera::Camera( glm::vec3 pos, glm::vec3 tgt, glm::vec3 upv, float fov_, float aspect_, float near_, float far_)
: position(pos), target(tgt), up(upv), fov(fov_), aspect(aspect_), nearPlane(near_), farPlane(far_) {
    interp_position = pos;
    interp_target = tgt;
    update_matrices();
}

void Camera::set_position(float x, float y, float z) {
    position = glm::vec3(x, y, z);
    update_matrices();
}
void Camera::look_at(float x, float y, float z) {
    target = glm::vec3(x, y, z);
    update_matrices();
}
void Camera::set_perspective(float fov_, float aspect_, float near_, float far_) {
    fov = fov_; aspect = aspect_; nearPlane = near_; farPlane = far_;
    update_matrices();
}
void Camera::update_matrices() {
    view = glm::lookAt(position, target, up);
    projection = glm::perspective(glm::radians(fov), aspect, nearPlane, farPlane);
}
void Camera::update_interpolated(float alpha, const glm::vec3& new_pos, const glm::vec3& new_target) {
    interp_position = glm::mix(interp_position, new_pos, alpha);
    interp_target = glm::mix(interp_target, new_target, alpha);
    view = glm::lookAt(interp_position, interp_target, up);
    projection = glm::perspective(glm::radians(fov), aspect, nearPlane, farPlane);
}
glm::vec3 Camera::getInterpPosition() const { return interp_position; }
glm::vec3 Camera::getInterpTarget() const { return interp_target; }
glm::mat4 Camera::getView() const { return view; }
glm::mat4 Camera::getProjection() const { return projection; }
GLuint Camera::getShaderProgram() const { return 0; }

