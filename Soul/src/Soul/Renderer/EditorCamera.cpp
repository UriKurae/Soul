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
		glm::vec3 finalRay = { normWorldRay.x, -normWorldRay.y, normWorldRay.z };
		
		
		return finalRay;
	}

	bool EditorCamera::RayToMeshes(std::shared_ptr<Model> model, float distance, glm::vec2 mousePos, glm::vec2 viewPortSize, glm::vec3& intersectionPoint, glm::vec2& uvCoords)
	{
		glm::vec3 origin = GetPosition();
		//glm::vec3 direction = glm::unProject(glm::vec3(mousePos.x, mousePos.y, 1.0f), GetViewMatrix(), GetProjectionMatrix(), glm::vec4(0.0f, 0.0f, viewPortSize.x, viewPortSize.y));
		glm::vec3 ra = RayCastLine(mousePos, glm::vec2(viewPortSize.x, viewPortSize.y));
		
	/*	glBegin(GL_LINES);
		glVertex3f(origin.x, origin.y, origin.z);
		glVertex3f(ra.x, ra.y, ra.z);
		glEnd();*/

		// Booleans to know if we hit
		bool hit = false;
		bool hitOnce = false;

		// Start al variables
		std::vector<Mesh>& meshes = model->GetMeshes();
		std::vector<Triangle> trianglesHit = {};
		Triangle finalTriangle = {};

		std::vector<Triangle> triangles = {};
		for (size_t i = 0; i < meshes.size(); i++)
		{
			triangles = meshes[i].GetTriangles();
			float closestDistance = 5000.0f;
			for (uint32_t i = 0; i < triangles.size(); ++i)
			{
				glm::vec3 hitPoint;
				hit = glm::intersectLineTriangle(origin, ra, triangles[i].a, triangles[i].b, triangles[i].c, hitPoint);
				if (hit)
				{
					SL_CORE_WARN("hitPoint is {0}, {1}, {2}", hitPoint.x, hitPoint.y, hitPoint.z);
					trianglesHit.push_back(triangles[i]);
					hit = false;
					hitOnce = true;
				}
			}
			if (!trianglesHit.empty())
			{
				glm::vec3 closestTriangleCenter;
				closestTriangleCenter.x = (triangles[0].a.x + triangles[0].b.x + triangles[0].c.x) / 3;
				closestTriangleCenter.y = (triangles[0].a.y + triangles[0].b.y + triangles[0].c.y) / 3;
				closestTriangleCenter.z = (triangles[0].a.z + triangles[0].b.z + triangles[0].c.z) / 3;

				float lowestDistance = 0.0f;
				lowestDistance = glm::distance(closestTriangleCenter, origin);
		
				for (uint32_t i = 0; i < trianglesHit.size(); ++i)
				{
					glm::vec3 newTriangleCenter = {};
					newTriangleCenter.x = (trianglesHit[i].a.x + trianglesHit[i].b.x + trianglesHit[i].c.x) / 3;
					newTriangleCenter.y = (trianglesHit[i].a.y + trianglesHit[i].b.y + trianglesHit[i].c.y) / 3;
					newTriangleCenter.z = (trianglesHit[i].a.z + trianglesHit[i].b.z + trianglesHit[i].c.z) / 3;

					float newDistance = glm::distance(newTriangleCenter, origin);
					if (newDistance < lowestDistance)
					{
						lowestDistance = newDistance;
						finalTriangle = trianglesHit[i];
					}
				}
			}

		}

		if (hitOnce)
		{
			uvCoords = model->PositionToUvs(finalTriangle.a).uv;

			intersectionPoint.x = (finalTriangle.a.x + finalTriangle.b.x + finalTriangle.c.x) / 3;
			intersectionPoint.y = (finalTriangle.a.y + finalTriangle.b.y + finalTriangle.c.y) / 3;
			intersectionPoint.z = (finalTriangle.a.z + finalTriangle.b.z + finalTriangle.c.z) / 3;
		}
		
		return hitOnce;
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