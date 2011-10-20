#pragma once

#include "glm/glm.hpp"
#include <C3/Events.h>
#include <GL/glu.h>

#define _USE_MATH_DEFINES
#include <math.h>

class Camera
{
private:
	//Private Attributes
	glm::vec2 m_AngleNew;
	glm::bvec4 m_KeysPress;
	glm::vec3 m_Forward;
	glm::vec3 m_Left;
	glm::ivec2 m_OldPositionMouse;
	bool m_MouseClick;
public:
	// Public Attributes
	glm::vec3 Position;
	glm::vec3 Target;
	glm::vec3 Up;
	float MoveFactor;

	Camera(const glm::vec3& position, const glm::vec3& target) :
		m_AngleNew(0.f),
		m_KeysPress(false),
		m_MouseClick(false),
		Position(position),
		Target(target),
		Up(0,1,0),
		MoveFactor(10.0f)
	{
	}

	void ComputeAngles()
	{
		double r_temp = cos(-m_AngleNew.x * M_PI / 180);
		m_Forward.y = sin(-m_AngleNew.x * M_PI / 180);
		m_Forward.x = r_temp * cos(m_AngleNew.y * M_PI / 180);
		m_Forward.z = r_temp * sin(m_AngleNew.y * M_PI / 180);

		m_Left = glm::core::function::geometric::cross(Up, m_Forward);
		m_Left = glm::normalize(m_Left);

		Target = Position + m_Forward;
	}

	void UpdateEvent(C3::Event& event)
	{
		// GetEvents
		if(event.Type == C3::Event::KeyPressed)
		{
			if(event.Key.Code == C3::Key::Up)
				m_KeysPress.x = true;
			else if(event.Key.Code == C3::Key::Down)
				m_KeysPress.y = true;
			else if(event.Key.Code == C3::Key::Right)
				m_KeysPress.z = true;
			else if(event.Key.Code == C3::Key::Left)
				m_KeysPress.w = true;
		}
		else if(event.Type == C3::Event::KeyReleased)
		{
			if(event.Key.Code == C3::Key::Up)
				m_KeysPress.x = false;
			else if(event.Key.Code == C3::Key::Down)
				m_KeysPress.y = false;
			else if(event.Key.Code == C3::Key::Right)
				m_KeysPress.z = false;
			else if(event.Key.Code == C3::Key::Left)
				m_KeysPress.w = false;
		}
		else if(event.Type == C3::Event::MouseButtonPressed)
		{
			m_MouseClick = true;
			m_OldPositionMouse.x = event.MouseButton.X;
			m_OldPositionMouse.y = event.MouseButton.Y;
		}
		else if(event.Type == C3::Event::MouseButtonReleased)
		{
			m_MouseClick = false;
		}
		else if(event.Type == C3::Event::MouseMoved)
		{
			if(m_MouseClick)
			{
				std::cout << m_AngleNew.x << " | " << m_AngleNew.y << std::endl;
				m_AngleNew.y += event.MouseMove.X - m_OldPositionMouse.x;
				m_AngleNew.x += event.MouseMove.Y - m_OldPositionMouse.y;
				m_AngleNew.x = std::min(89.f, std::max(-89.f, m_AngleNew.x));
			}
			m_OldPositionMouse.x = event.MouseMove.X;
			m_OldPositionMouse.y = event.MouseMove.Y;
		}
	}

	void Update(float deltaTime)
	{
		const float factor = (deltaTime * MoveFactor);
		if(m_KeysPress.x)
			Position += (deltaTime*m_Forward);
		if(m_KeysPress.y)
			Position -= m_Forward*deltaTime;
		if(m_KeysPress.z)
			Position -= m_Left*deltaTime;
		if(m_KeysPress.w)
			Position += m_Left*deltaTime;
		ComputeAngles();
	}

	void ApplyView()
	{
		gluLookAt(Position.x, Position.y, Position.z,
				  Target.x, Target.y, Target.z,
				  Up.x, Up.y, Up.z);
	}
};

std::ostream& operator<<(std::ostream& out, const glm::vec3& v)
{
	out << "<" << v.x << "; " << v.y << "; " << v.z << ">";
	return out;
}

std::ostream& operator<<(std::ostream& out, const Camera& c)
{
	out << "(Position : " << c.Position << " | Target : " << c.Target << ")";
	return out;
}
