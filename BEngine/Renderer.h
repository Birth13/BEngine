#pragma once
#include "common.h"

class Renderer {
private:
	std::wstring m_window_name;

public:
	Renderer(std::wstring window_name) : m_window_name(window_name) {}
	virtual ~Renderer() {}

	//

	virtual void Prepare_Render() {}
	virtual void Render(HDC hdc) {}
};

