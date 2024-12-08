#pragma once

#include "MyGraphics.h"	

class Draw {
public:
	Draw();
	~Draw();
	Draw(const Draw&) = delete;
	Draw& operator = (const Draw&) = delete;
private:
	Graphics grp;
};