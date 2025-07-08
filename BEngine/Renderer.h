#pragma once
#include "common.h"

class Renderer {
public:
	Renderer() {}
	virtual ~Renderer() {}

	//

	virtual void Prepare_Render() {}
	virtual void Render() {}
};

