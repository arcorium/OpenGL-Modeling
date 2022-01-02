#pragma once

// WINDOWS
#include <Windows.h>

// STD
#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>
#include <unordered_map>
#include <vector>
#include <set>
#include <map>


// OEPNGL
// Loader
#include <glad/glad.h>
// Window and Input
#include <GLFW/glfw3.h>
// Math
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// ASSIMP
#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>


// IMGUI
#include <imgui.h>
#include "imgui_impl_opengl3.h"
#include "imgui_impl_glfw.h"

// Static Local
#include "Util.h"
#include "Texture.h"
#include "Vertex.h"
#include "Logger.h"