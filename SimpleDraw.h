#pragma once

#include "MyWindow.h"
#include "MyTimer.h"

class SimpleDraw {
public:
	SimpleDraw();
	int Draw();
	~SimpleDraw();
private:
	void Update();
private:
	Window window;
	Timer timer;
	std::vector<std::unique_ptr<class Shape3DBase>> boxes;
};