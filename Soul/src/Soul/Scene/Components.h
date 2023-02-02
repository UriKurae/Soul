#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Soul/Renderer/Camera.h"
#include "Soul/Utils/Model.h"

namespace Soul
{
	struct TagComponent
	{
		std::string tag;

		TagComponent() = default;
		TagComponent(const TagComponent&) = default;
		TagComponent(const std::string& tagName)
			: tag(tagName) {}
	};

	struct TransformComponent
	{
		glm::vec3 translation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 rotation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 scale = { 1.0f, 1.0f, 1.0f };


		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default;
		TransformComponent(const glm::vec3& trans)
			: translation(trans) {}

		glm::mat4 GetTransform() const
		{
			glm::mat4 rot = glm::rotate(glm::mat4(1.0f), rotation.x, {1.0f, 0.0f, 0.0f}) * 
				glm::rotate(glm::mat4(1.0f), rotation.y, { 0.0f, 1.0f, 0.0f }) *
				glm::rotate(glm::mat4(1.0f), rotation.z, { 0.0f, 0.0f, 1.0f });

			return glm::translate(glm::mat4(1.0f), translation) *
				rot *
				glm::scale(glm::mat4(1.0f), scale);
		}
	};

	struct CameraComponent
	{
		Soul::Camera camera;
		bool primary = true;

		CameraComponent() = default;
		CameraComponent(const CameraComponent&) = default;
		CameraComponent(const glm::mat4& projection)
			: camera(projection) {}
	};

	struct MeshComponent
	{
		MeshComponent() = default;
		MeshComponent(const MeshComponent&) = default;
	
		std::shared_ptr<Model> model;
	};
}