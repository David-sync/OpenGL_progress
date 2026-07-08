#pragma once
#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>

GLuint LoadShaders(const char* vertex_file_path, const char* fragment_file_path);

#endif