#pragma once

#ifdef __EMSCRIPTEN__
#include <GLES3/gl3.h>
#include <emscripten.h>
#else
#include <glad/gles2.h>
#endif