#pragma once
// clang-format off
// For use by Pepper clients

#include "Pepper/Core/Application.hpp"
#include "Pepper/Layers/Layer.hpp"
#include "Pepper/Core/Log.hpp"
#include "Pepper/Core/TimeStep.hpp"

#include "Pepper/OrthoCameraController.hpp"

// --Input------------------------
#include "Pepper/Input/Input.hpp"
#include "Pepper/Input/KeyCodes.hpp"
#include "Pepper/Input/MouseButtonCodes.hpp"
// -------------------------------------

// --Renderer------------------------
#include "Pepper/Renderer/OrthoCamera.hpp"
#include "Pepper/Renderer/RenderCommand.hpp"
#include "Pepper/Renderer/Renderer.hpp"
#include "Pepper/Renderer/Shader.hpp"
#include "Pepper/Renderer/ShaderLibrary.hpp"
#include "Pepper/Renderer/Texture.hpp"
// -------------------------------------

// --Entry Point------------------------
#include "Pepper/Core/EntryPoint.hpp"
// -------------------------------------
// clang-format on