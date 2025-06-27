#include "Component.hpp"

Rect::Rect() {
    init_buffers(vertices, sizeof(vertices), indices, sizeof(indices), 3);
}
void Rect::setShader(const std::string& vertexPath, const std::string& fragmentPath) {
    _set_shader(vertexPath, fragmentPath, &this->shaderProgram);
}
void Rect::draw() {
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}
Rect::~Rect() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    if (shaderProgram) glDeleteProgram(shaderProgram);
}
GLuint Rect::getShaderProgram() const {
    return shaderProgram;
}