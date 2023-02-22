#include "slpch.h"
#include "EditorCamera.h"

#include "Soul/Core/Input.h"
#include "Soul/Core/KeyCodes.h"
#include "Soul/Core/MouseButtonCodes.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"
#include <Soul/Utils/Model.h>

#include <glfw/glfw3.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/intersect.hpp>
#include "glad/glad.h"

namespace Soul
{
	EditorCamera::EditorCamera(float fov, float aspectRatio, float nearClip, float farClip)
		: m_Fov(fov), m_AspectRatio(aspectRatio), m_NearClip(nearClip), m_FarClip(farClip), Camera(glm::perspective(glm::radians(fov), aspectRatio, nearClip, farClip))
	{
		UpdateView();
	}

	void EditorCamera::UpdateProjection()
	{
		m_AspectRatio = m_ViewportWidth / m_ViewportHeight;
		m_Projection = glm::perspective(glm::radians(m_Fov), m_AspectRatio, m_NearClip, m_FarClip);
	}

	void EditorCamera::UpdateView()
	{
		// m_Yaw = m_Pitch = 0.0f; // Lock the camera's rotation
		m_Position = CalculatePosition();

		glm::quat orientation = GetOrientation();
		m_ViewMatrix = glm::translate(glm::mat4(1.0f), m_Position) * glm::toMat4(orientation);
		m_ViewMatrix = glm::inverse(m_ViewMatrix);
	}

	std::pair<float, float> EditorCamera::PanSpeed() const
	{
		float x = std::min(m_ViewportWidth / 1000.0f, 2.4f); // max = 2.4f
		float xFactor = 0.0366f * (x * x) - 0.1778f * x + 0.3021f;

		float y = std::min(m_ViewportHeight / 1000.0f, 2.4f); // max = 2.4f
		float yFactor = 0.0366f * (y * y) - 0.1778f * y + 0.3021f;

		return { xFactor, yFactor };
	}

	float EditorCamera::RotationSpeed() const
	{
		return 0.8f;
	}

	float EditorCamera::ZoomSpeed() const
	{
		float distance = m_Distance * 0.2f;
		distance = std::max(distance, 0.0f);
		float speed = distance * distance;
		speed = std::min(speed, 100.0f); // max speed = 100
		return speed;
	}

	void EditorCamera::OnUpdate(Timestep ts)
	{
		if (Input::IsKeyPressed(Key::LeftAlt))
		{
			const glm::vec2& mouse{ Input::GetMouseX(), Input::GetMouseY() };
			glm::vec2 delta = (mouse - m_InitialMousePosition) * 0.003f;
			m_InitialMousePosition = mouse;

			if (Input::IsMouseButtonPressed(Mouse::ButtonMiddle))
				MousePan(delta);
			else if (Input::IsMouseButtonPressed(Mouse::ButtonLeft))
				MouseRotate(delta);
			else if (Input::IsMouseButtonPressed(Mouse::ButtonRight))
				MouseZoom(delta.y);
		}
		
		UpdateView();

		
	}

	void EditorCamera::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseScrolledEvent>(SL_BIND_EVENT_FN(EditorCamera::OnMouseScroll));
	}

	bool EditorCamera::OnMouseScroll(MouseScrolledEvent& e)
	{
		float delta = e.GetYOffset() * 0.1f;
		MouseZoom(delta);
		UpdateView();
		return false;
	}

	void EditorCamera::MousePan(const glm::vec2& delta)
	{
		auto [xSpeed, ySpeed] = PanSpeed();
		m_FocalPoint += -GetRight() * delta.x * xSpeed * m_Distance;
		m_FocalPoint += GetUp() * delta.y * ySpeed * m_Distance;
	}

	void EditorCamera::MouseRotate(const glm::vec2& delta)
	{
		float yawSign = GetUp().y < 0 ? -1.0f : 1.0f;
		m_Yaw += yawSign * delta.x * RotationSpeed();
		m_Pitch += delta.y * RotationSpeed();
	}

	void EditorCamera::MouseZoom(float delta)
	{
		m_Distance -= delta * ZoomSpeed();
		if (m_Distance < 1.0f)
		{
			m_FocalPoint += GetForward();
			m_Distance = 1.0f;
		}
	}

	glm::vec3 EditorCamera::GetUp() const
	{
		return glm::rotate(GetOrientation(), glm::vec3(0.0f, 1.0f, 0.0f));
	}

	glm::vec3 EditorCamera::GetRight() const
	{
		return glm::rotate(GetOrientation(), glm::vec3(1.0f, 0.0f, 0.0f));
	}

	glm::vec3 EditorCamera::GetForward() const
	{
		return glm::rotate(GetOrientation(), glm::vec3(0.0f, 0.0f, -1.0f));
	}

	glm::vec3 EditorCamera::RayCastLine(glm::vec2 mousePos, glm::vec2 viewPortSize)
	{
		float mouseX = (2.0f * mousePos.x) / viewPortSize.x - 1.0f;
		float mouseY = 1.0f - (2.0f * mousePos.y) / viewPortSize.y;
		float mouseZ = 1.0f;
		glm::vec3 rayNDC = glm::vec3(mouseX, mouseY, mouseZ);

		glm::vec4 ray = glm::vec4(rayNDC.x, rayNDC.y, -mouseZ, 1.0);
		glm::vec4 eyeRay = glm::inverse(GetProjectionMatrix()) * ray;

		eyeRay = glm::vec4(eyeRay.x, eyeRay.y, -1.0, 0.0);

		glm::vec3 worldRay = (glm::inverse(GetViewMatrix()) * eyeRay);
		glm::vec3 normWorldRay = glm::normalize(worldRay);
		glm::vec3 finalRay = { normWorldRay.x, -normWorldRay.y, -1.0f };
		
		return finalRay;
	}

	bool EditorCamera::RayToMeshes(std::vector<Triangle> triangles, float distance, glm::vec2 mousePos, glm::vec2 viewPortSize, glm::vec3& intersectionPoint)
	{
		glm::vec3 origin = GetPosition();
		glm::vec3 direction = glm::unProject(glm::vec3(mousePos.x, mousePos.y, 1.0f), GetViewMatrix(), GetProjectionMatrix(), glm::vec4(0.0f, 0.0f, viewPortSize.x, viewPortSize.y));
		//glm::vec3 ra = RayCastLine(mousePos, glm::vec2(viewPortSize.x, viewPortSize.y));
		SL_CORE_WARN("dir {0}, {1}, {2}", direction.x, direction.y, direction.z);


		bool hit = false;
		bool trujit = false;
		std::vector<int> trianglesHit = {};
		float closestDistance = 5000.0f;
		Triangle closestTriangle = {};
		for (uint32_t i = 0; i < triangles.size(); ++i)
		{
			glm::vec3 hitPoint;
			hit = glm::intersectLineTriangle(origin, direction, triangles[i].a, triangles[i].b, triangles[i].c, hitPoint);
			if (hit)
			{
				float distance = 0.0f;
				
				distance = glm::distance(hitPoint, origin);
				if (distance < closestDistance)
				{
					closestDistance = distance;
					closestTriangle = triangles[i];
					trujit = true;
				}
				trianglesHit.push_back(i);
				hit = false;
			}
		}

		
		if (trujit)
		{
			intersectionPoint.x = (closestTriangle.a.x + closestTriangle.b.x + closestTriangle.c.x) / 3;
			intersectionPoint.y = (closestTriangle.a.y + closestTriangle.b.y + closestTriangle.c.y) / 3;
			intersectionPoint.z = (closestTriangle.a.z + closestTriangle.b.z + closestTriangle.c.z) / 3;
		}


		/*if (!trianglesHit.empty())
		{
			glm::vec3 closestTriangle;
			closestTriangle.x = (triangles[trianglesHit[0]].a.x + triangles[trianglesHit[0]].b.x + triangles[trianglesHit[0]].c.x) / 3;
			closestTriangle.y = (triangles[trianglesHit[0]].a.y + triangles[trianglesHit[0]].b.y + triangles[trianglesHit[0]].c.y) / 3;
			closestTriangle.z = (triangles[trianglesHit[0]].a.z + triangles[trianglesHit[0]].b.z + triangles[trianglesHit[0]].c.z) / 3;

			float lowestDistance = 0.0f;
			lowestDistance = (closestTriangle - origin).length();
			intersectionPoint = closestTriangle;
			for (uint32_t i = 0; i < trianglesHit.size(); ++i)
			{
				glm::vec3 newTriangle = {};
				newTriangle.x = (triangles[trianglesHit[i]].a.x + triangles[trianglesHit[i]].b.x + triangles[trianglesHit[i]].c.x) / 3;
				newTriangle.y = (triangles[trianglesHit[i]].a.y + triangles[trianglesHit[i]].b.y + triangles[trianglesHit[i]].c.y) / 3;
				newTriangle.z = (triangles[trianglesHit[i]].a.z + triangles[trianglesHit[i]].b.z + triangles[trianglesHit[i]].c.z) / 3;

				if ((newTriangle - origin).length() < lowestDistance)
				{
					closestTriangle = newTriangle;
					intersectionPoint = closestTriangle;
				}
			}
		}*/
		
		return hit;
	}

	glm::vec3 EditorCamera::CalculatePosition() const
	{
		return m_FocalPoint - GetForward() * m_Distance;
	}

	glm::quat EditorCamera::GetOrientation() const
	{
		return glm::quat(glm::vec3(-m_Pitch, -m_Yaw, 0.0f));
	}
}