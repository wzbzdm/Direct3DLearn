#pragma once

class Mouse {
public:
	Mouse();
	~Mouse();
	Mouse(const Mouse&) = delete;
	Mouse& operator = (const Mouse&) = delete;	
private:
	int x;
	int y;
};