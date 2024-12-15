#pragma once

#include "MyWindow.h"
#include "MyTimer.h"

class SimpleDraw {
public:
	SimpleDraw();
	int Draw();
	~SimpleDraw();
private:
	void HandlerEvent();
private:
	Window window;
	Timer timer;
};