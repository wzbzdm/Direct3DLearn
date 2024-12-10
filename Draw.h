#pragma once

#include "MyWindow.h"
#include "MyTimer.h"

class Draw {
public:
	Draw();
	Draw(const Draw&) = delete;
	Draw& operator = (const Draw&) = delete;
private:
	Window window;
	Timer timer;
};