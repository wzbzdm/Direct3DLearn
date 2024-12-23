#include "MyWindow.h"
#include "MyMouse.h"

std::pair<int, int> Mouse::GetPos() const noexcept
{
	return { x,y };
}

int Mouse::GetPosX() const noexcept
{
	return x;
}

int Mouse::GetPosY() const noexcept
{
	return y;
}

bool Mouse::IsInWindow() const noexcept
{
	return isInWindow;
}

bool Mouse::LeftIsPressed() const noexcept
{
	return leftIsPressed;
}

bool Mouse::RightIsPressed() const noexcept
{
	return rightIsPressed;
}

std::optional<Mouse::Event> Mouse::Read() noexcept
{
	if (buffer.size() > 0u)
	{
		Mouse::Event e = buffer.front();
		buffer.pop_front();
		return e;
	}
	return {};
}

std::optional<Mouse::Event> Mouse::ReadEvent() noexcept
{
	if (events.size() > 0u)
	{
		Mouse::Event e = events.front();
		events.pop_front();
		return e;
	}
	return {};
}

void Mouse::PushBoth(Mouse::Event&& mevent) noexcept {
	// 使用 std::move 转换为右值，避免拷贝
	buffer.push_back(std::move(mevent));
	events.push_back(buffer.back());
}

// 保存PMove时保存相对位移
void Mouse::PushBothMove(Mouse::Event&& mevent) noexcept {
	Mouse::Event last = buffer.back();
	int offsetX = mevent.GetPosX() - last.GetPosX();
	int offsetY = mevent.GetPosY() - last.GetPosY();
	buffer.push_back(std::move(mevent));
	const Mouse::Event now = buffer.back();
	events.push_back(Mouse::Event(now.GetType(), offsetX, offsetY, *this));
}

void Mouse::Flush() noexcept
{
	buffer = std::deque<Event>();
}

long long Mouse::CurrentTimeMS() noexcept {
	auto now = std::chrono::system_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch());
	return duration.count();
}

void Mouse::OnMouseMove(int newx, int newy) noexcept
{
	x = newx;
	y = newy;

	if (!IsEmpty()) {
		Mouse::Event last = buffer.back();
		if (last.GetPosX() != newx || last.GetPosY() != newy) {
			if (last.GetType() == Mouse::Event::Type::LPress || last.GetType() == Mouse::Event::Type::LPMove) {
				PushBothMove(Mouse::Event(Mouse::Event::Type::LPMove, *this));
			}else if (last.GetType() == Mouse::Event::Type::RPress || last.GetType() == Mouse::Event::Type::RPMove) {
				PushBothMove(Mouse::Event(Mouse::Event::Type::RPMove, *this));
			} else if (last.GetType() == Mouse::Event::Type::MPress || last.GetType() == Mouse::Event::Type::MPMove) {
				PushBothMove(Mouse::Event(Mouse::Event::Type::MPMove, *this));
			} else {
				buffer.push_back(Mouse::Event(Mouse::Event::Type::Move, *this));
			}
		}
	} else {
		buffer.push_back(Mouse::Event(Mouse::Event::Type::Move, *this));
	}
	
	TrimBuffer();
}

void Mouse::OnMouseLeave() noexcept
{
	isInWindow = false;
	buffer.push_back(Mouse::Event(Mouse::Event::Type::Leave, *this));
	TrimBuffer();
}

void Mouse::OnMouseEnter() noexcept
{
	isInWindow = true;
	buffer.push_back(Mouse::Event(Mouse::Event::Type::Enter, *this));
	TrimBuffer();
}

void Mouse::OnLeftPressed(int x, int y) noexcept
{
	this->x = x;
	this->y = y;
	leftIsPressed = true;

	buffer.push_back(Mouse::Event(Mouse::Event::Type::LPress, *this));
	TrimBuffer();
}

void Mouse::OnLeftReleased(int x, int y) noexcept
{
	this->x = x;
	this->y = y;
	leftIsPressed = false;
	if (!IsEmpty()) {
		Mouse::Event last = buffer.back();
		long long currentTime = Mouse::CurrentTimeMS();
		if (last.GetType() == Mouse::Event::Type::LPress) {
			buffer.pop_back();
			if (!IsEmpty()) {
				Mouse::Event lastlast = buffer.back();
				if (lastlast.GetType() == Mouse::Event::Type::LClick && (currentTime - lastlast.GetTime()) < MINDCLICKTIME) {
					buffer.pop_back();
					PushBoth(Mouse::Event(Mouse::Event::Type::LDClick, *this));
				}
				else {
					PushBoth(Mouse::Event(Mouse::Event::Type::LClick, *this));
				}
			} else {
				PushBoth(Mouse::Event(Mouse::Event::Type::LClick, *this));
			}
		} else {
			buffer.push_back(Mouse::Event(Mouse::Event::Type::LRelease, *this));
		}
	} else {
		buffer.push_back(Mouse::Event(Mouse::Event::Type::LRelease, *this));
	}
	
	TrimBuffer();
}

void Mouse::OnRightPressed(int x, int y) noexcept
{
	this->x = x;
	this->y = y;
	rightIsPressed = true;

	buffer.push_back(Mouse::Event(Mouse::Event::Type::RPress, *this));
	TrimBuffer();
}

void Mouse::OnRightReleased(int x, int y) noexcept
{
	this->x = x;
	this->y = y;
	rightIsPressed = false;
	if (!IsEmpty()) {
		Mouse::Event last = buffer.back();
		long long currentTime = Mouse::CurrentTimeMS();
		if (last.GetType() == Mouse::Event::Type::RPress && (currentTime - last.GetTime()) < MINCLICKTIME) {
			buffer.pop_back();
			if (!IsEmpty()) {
				Mouse::Event lastlast = buffer.back();
				if (lastlast.GetType() == Mouse::Event::Type::RClick && (currentTime - lastlast.GetTime()) < MINDCLICKTIME) {
					buffer.pop_back();
					PushBoth(Mouse::Event(Mouse::Event::Type::RDClick, *this));
				}
				else {
					PushBoth(Mouse::Event(Mouse::Event::Type::RClick, *this));
				}
			}
			else {
				PushBoth(Mouse::Event(Mouse::Event::Type::RClick, *this));
			}

		}
		else {
			buffer.push_back(Mouse::Event(Mouse::Event::Type::RRelease, *this));
		}
	}
	else {
		buffer.push_back(Mouse::Event(Mouse::Event::Type::RRelease, *this));
	}
	TrimBuffer();
}

void Mouse::OnMidPressed(int x, int y) noexcept {
	this->x = x;
	this->y = y;
	midIsPressed = true;

	buffer.push_back(Mouse::Event(Mouse::Event::Type::MPress, *this));
	TrimBuffer();
}

void Mouse::OnMidReleased(int x, int y) noexcept {
	this->x = x;
	this->y = y;
	midIsPressed = false;

	buffer.push_back(Mouse::Event(Mouse::Event::Type::MRelease, *this));
	TrimBuffer();
}

void Mouse::OnWheelUp(int x, int y) noexcept
{
	this->x = x;
	this->y = y;
	PushBoth(Mouse::Event(Mouse::Event::Type::WheelUp, *this));
	TrimBuffer();
}

void Mouse::OnWheelDown(int x, int y) noexcept
{
	this->x = x;
	this->y = y;
	PushBoth(Mouse::Event(Mouse::Event::Type::WheelDown, *this));
	TrimBuffer();
}

void Mouse::TrimBuffer() noexcept
{
	while (buffer.size() > bufferSize)
	{
		buffer.pop_front();
	}
}

void Mouse::OnWheelDelta(int x, int y, int delta) noexcept
{
	wheelDeltaCarry += delta;
	// generate events for every 120 
	while (wheelDeltaCarry >= WHEEL_DELTA)
	{
		wheelDeltaCarry -= WHEEL_DELTA;
		OnWheelUp(x, y);
	}
	while (wheelDeltaCarry <= -WHEEL_DELTA)
	{
		wheelDeltaCarry += WHEEL_DELTA;
		OnWheelDown(x, y);
	}
}
