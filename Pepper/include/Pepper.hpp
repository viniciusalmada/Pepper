#pragma once
// clang-format off
// For use by Pepper clients

#include "Pepper/Core/Application.hpp"
#include "Pepper/Layers/Layer.hpp"
#include "Pepper/Core/Log.hpp"
#include "Pepper/Core/Colors.hpp"
#include "Pepper/Core/TimeStep.hpp"
#include "Pepper/Core/OrthoCameraController.hpp"
#include "Pepper/Core/Utils.hpp"

// --Utilities-----------------------
#include "Pepper/Utilities/Random.hpp"

// --Events-----------------------
#include "Pepper/Events/Event.hpp"
#include "Pepper/Events/EventDispatcher.hpp"

// --Input------------------------
#include "Pepper/Input/Input.hpp"
#include "Pepper/Input/KeyCodes.hpp"
#include "Pepper/Input/MouseButtonCodes.hpp"
// -------------------------------------

// --Renderer------------------------
#include "Pepper/Renderer/OrthoCamera.hpp"
#include "Pepper/Renderer/RenderCommand.hpp"
#include "Pepper/Renderer/Renderer.hpp"
#include "Pepper/Renderer/Renderer2D.hpp"
#include "Pepper/Renderer/Shader.hpp"
#include "Pepper/Renderer/ShaderLibrary.hpp"
#include "Pepper/Renderer/Texture.hpp"
// -------------------------------------
// clang-format on