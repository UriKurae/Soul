#pragma once

#include <glm/glm.hpp>

namespace Soul
{
	struct TransformComponent
	{
		glm::mat4 transform{ 1.0f };

		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default;
		TransformComponent(const glm::mat4& trans)
			: transform(trans) {}

		operator glm::mat4& () { return transform; }
		operator const glm::mat4& () const { return transform; }
	};
}