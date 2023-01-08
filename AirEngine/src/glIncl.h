#pragma once

#include <stdio.h>
#include <functional>

#include "third-party/imgui/imgui.h"
#include "third-party/imgui/imgui_impl_glfw.h"
#include "third-party/imgui/imgui_impl_opengl3.h"

#ifdef AIR_WEB
	#define GLFW_INCLUDE_ES3
	#include <emscripten/emscripten.h>
	#include <emscripten/html5.h>
#else
	#include <GL/glew.h>
#endif

#include <GLFW/glfw3.h>

#define GLM_FORCE_PURE
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <gtx/transform.hpp> 
#include <glm.hpp>

#include <SOIL.h>

#include <entt.hpp>

#define AIR_LAYER_UI 31
#define AIR_LAYER_UI_BITMASK 2147483648

