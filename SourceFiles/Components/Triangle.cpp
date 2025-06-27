#include "Component.hpp"

Triangle::Triangle() {
    init_buffers(vertices, sizeof(vertices), nullptr, 0, 3, 3, 2);
}

void Triangle::apply_shader() {
    _apply_shader(&this->shaderProgram);
    glUniform1i(glGetUniformLocation(this->shaderProgram, "ourTexture"), 0);
}

void Triangle::setShader(const std::string& vertexPath, const std::string& fragmentPath) {
    _set_shader(vertexPath, fragmentPath, &this->shaderProgram);
}

void Triangle::draw() {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindVertexArray(0);
}

Triangle::~Triangle() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    if (shaderProgram) glDeleteProgram(shaderProgram);
}
GLuint Triangle::getShaderProgram() const {
    return shaderProgram;
}
