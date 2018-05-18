#include "Input.h"
#include "gl_core_4_4.h"
#include <GLFW/glfw3.h>

#include <iostream>

Input* Input::m_instance = nullptr;

Input::Input() {
	// track current/previous key and mouse button states
	m_lastKeys = new int[GLFW_KEY_LAST + 1];
	m_currentKeys = new int[GLFW_KEY_LAST + 1];

	auto window = glfwGetCurrentContext();

	for (int i = GLFW_KEY_SPACE; i <= GLFW_KEY_LAST; ++i)
		m_lastKeys[i] = m_currentKeys[i] = glfwGetKey(window, i);

	auto MouseMoveCallback = [](GLFWwindow* window, double x, double y) {
		int w = 0, h = 0;
		glfwGetWindowSize(window, &w, &h);

		Input::getInstance()->onMouseMove((int)x, h - (int)y);

		for (auto& f : Input::getInstance()->m_mouseMoveCallbacks)
			f(window, x, h - y);
	};

	glfwSetCursorPosCallback(window, MouseMoveCallback);
}

Input::~Input() {
	delete[] m_lastKeys;
	delete[] m_currentKeys;
}

void Input::onMouseMove(int newXPos, int newYPos) {
	m_mouseX = newXPos;
	m_mouseY = newYPos;
}

bool Input::isKeyDown(int inputKeyID) {
	return m_currentKeys[inputKeyID] == GLFW_PRESS;
}

bool Input::isMouseButtonDown(int inputMouseID) {
	return m_currentButtons[inputMouseID] == GLFW_PRESS;
}

void Input::getMouseXY(int * x, int * y) {
	if (x != nullptr) *x = m_mouseX;
	if (y != nullptr) *y = m_mouseY;
}

void Input::clearStatus() {
	m_pressedKeys.clear();

	auto window = glfwGetCurrentContext();

	// update keys
	for (int i = GLFW_KEY_SPACE; i <= GLFW_KEY_LAST; ++i) {
		m_lastKeys[i] = m_currentKeys[i];
		m_currentKeys[i] = glfwGetKey(window, i);
	}

	// update mouse
	for (int i = 0; i < 8; ++i) {
		m_lastButtons[i] = m_currentButtons[i];
		m_currentButtons[i] = glfwGetMouseButton(window, i);
	}
}
