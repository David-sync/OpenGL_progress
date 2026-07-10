#pragma once
#ifndef LOADINGBMP_H
#define LOADINGBMP_H

#include <glad/glad.h>
#include <cstdio>
#include <cstdlib>

#define FOURCC_DXT1 0x31545844 // "DXT1"
#define FOURCC_DXT3 0x33545844 // "DXT3"
#define FOURCC_DXT5 0x35545844 // "DXT5"


// Extension bị thiếu của GLAD
#define GL_COMPRESSED_RGBA_S3TC_DXT1_EXT 0x83F1
#define GL_COMPRESSED_RGBA_S3TC_DXT3_EXT 0x83F2
#define GL_COMPRESSED_RGBA_S3TC_DXT5_EXT 0x83F3

GLuint loadBMP_custom(const char* imagepath);
GLuint loadDDS(const char* imagepath);

#endif