#pragma once
#include "Vector2.h"

class Screen {
private:
	Vector2 m_vec2Screen;

public:
	Screen() : m_vec2Screen(Vector2(500, 500)) {}

	Vector2 GetSize() const { return m_vec2Screen; }
};