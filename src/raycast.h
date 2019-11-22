#pragma once

#include <vector>
#include "geometry_data.h"
#include "vector3f.h"
#include "renderer.h"

namespace core {

bool Raycast(const std::vector<core::Box>& entites, const math::Vector3f& origin, const math::Vector3f& direction, renderer::Renderer& renderer);

}
