#include "Component.hpp"


Sprite2D::Sprite2D() {
    init_buffers(vertices, sizeof(vertices), indices, sizeof(indices), 3, 3, 2);
    update_vertices();
}

void Sprite2D::update_vertices() {
    float u0 = flipped_h ? 1.0f : 0.0f;
    float u1 = flipped_h ? 0.0f : 1.0f;
    float new_vertices[20] = {
        // positions        // texcoords
         0.5f,  0.5f, 0.0f, u1, 1.0f, // top right
         0.5f, -0.5f, 0.0f, u1, 0.0f, // bottom right
        -0.5f, -0.5f, 0.0f, u0, 0.0f, // bottom left
        -0.5f,  0.5f, 0.0f, u0, 1.0f  // top left
    };
    // Copy positions and texcoords
    for (int i = 0; i < 20; ++i) vertices[i] = new_vertices[i];
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(new_vertices), new_vertices);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Sprite2D::setShader(const std::string& vertexPath, const std::string& fragmentPath) {
    _set_shader(vertexPath, fragmentPath, &this->shaderProgram);
}

void Sprite2D::draw() {
    glActiveTexture(GL_TEXTURE0);
    if (!textureIDs.empty()) {
        glBindTexture(GL_TEXTURE_2D, textureIDs[current_texture_index]);
    } else {
        glBindTexture(GL_TEXTURE_2D, textureID); // fallback
    }
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

Sprite2D::~Sprite2D() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    if (shaderProgram) glDeleteProgram(shaderProgram);
}

GLuint Sprite2D::getShaderProgram() const {
    return shaderProgram;
}

void Sprite2D::addTexture(const std::string& path) {
    GLuint texID = 0;
    glGenTextures(1, &texID);
    glBindTexture(GL_TEXTURE_2D, texID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char *data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
    if (data) {
        GLenum format = (nrChannels == 4) ? GL_RGBA : GL_RGB;
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        textureIDs.push_back(texID);
    } else {
        std::cerr << "Failed to load texture: " << path << std::endl;
        glDeleteTextures(1, &texID);
    }
    stbi_image_free(data);
}

void Sprite2D::setTextureIndex(int idx) {
    if (idx >= 0 && idx < (int)textureIDs.size()) {
        current_texture_index = idx;
    }
}

int Sprite2D::lua_sprite2d_add_texture(lua_State* L) {
    Sprite2D* sprite = *(Sprite2D**)luaL_checkudata(L, 1, "Sprite2DMeta");
    const char* path = luaL_checkstring(L, 2);
    sprite->addTexture(path);
    return 0;
}

int Sprite2D::lua_sprite2d_set_texture_index(lua_State* L) {
    Sprite2D* sprite = *(Sprite2D**)luaL_checkudata(L, 1, "Sprite2DMeta");
    int idx = luaL_checkinteger(L, 2) - 1; // Lua 1-based
    sprite->setTextureIndex(idx);
    return 0;
}
