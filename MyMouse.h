#pragma once

#include <queue>
#include <deque>
#include <optional>
#include <chrono>

constexpr long long MINCLICKTIME = 150;
constexpr long long MINDCLICKTIME = 300;

class Mouse {
	friend class Window;
public:
	class Event {
	public:
		enum class Type {
			LPress,
			LRelease,
			RPress,
			RRelease,
			MPress,
			MRelease,
			WheelUp,
			WheelDown,
			Move,
			Enter,
			Leave,
			// 扩展事件
			LClick,	// 点击
			RClick ,	// 右击
			LDClick,	// 双击
			RDClick,	// 右键双击
			LPMove,	// 左键按下移动
			RPMove,	// 右键按下移动
			MPMove,	// 中键按下移动
		};
	private:
		Type type;
		bool leftIsPressed;
		bool rightIsPressed;
		int x;
		int y;
		int offsetX;
		int offsetY;
		long long timestamp;
	public:
		Event(Type type, const Mouse& parent) noexcept
			:
			type(type),
			leftIsPressed(parent.leftIsPressed),
			rightIsPressed(parent.rightIsPressed),
			x(parent.x),
			y(parent.y),
			offsetX(0),
			offsetY(0),
			timestamp(CurrentTimeMS())
		{}
		Event(Type type, int offsetX, int offsetY, const Mouse& parent) noexcept
			:
			type(type),
			leftIsPressed(parent.leftIsPressed),
			rightIsPressed(parent.rightIsPressed),
			x(parent.x),
			y(parent.y),
			offsetX(offsetX),
			offsetY(offsetY),
			timestamp(CurrentTimeMS())
		{}
		Event(Type type, const Mouse& parent, long long timestamp) noexcept
			:
			type(type),
			leftIsPressed(parent.leftIsPressed),
			rightIsPressed(parent.rightIsPressed),
			x(parent.x),
			y(parent.y),
			offsetX(0),
			offsetY(0),
			timestamp(timestamp)
		{}
		Type GetType() const noexcept {
			return type;
		}
		long long GetTime() const noexcept {
			return timestamp;
		}
		std::pair<int, int> GetPos() const noexcept {
			return { x,y };
		}
		int GetPosX() const noexcept {
			return x;
		}
		int GetPosY() const noexcept {
			return y;
		}
		int GetOffX() const noexcept {
			return offsetX;
		}
		int GetOffY() const noexcept {
			return offsetY;
		}
		int GetPrevX() const noexcept {
			return x - offsetX;
		}
		int GetPrevY() const noexcept {
			return y - offsetY;
		}
		bool LeftIsPressed() const noexcept {
			return leftIsPressed;
		}
		bool RightIsPressed() const noexcept {
			return rightIsPressed;
		}
	};
public:
	Mouse() : x(-1), y(-1) {};
	Mouse(const Mouse&) = delete;
	Mouse& operator = (const Mouse&) = delete;
	std::pair<int, int> GetPos() const noexcept;
	int GetPosX() const noexcept;
	int GetPosY() const noexcept;
	bool IsInWindow() const noexcept;
	bool LeftIsPressed() const noexcept;
	bool RightIsPressed() const noexcept;
	std::optional<Mouse::Event> Read() noexcept;
	std::optional<Mouse::Event> ReadEvent() noexcept;
	void PushBoth(Mouse::Event&& mevent) noexcept;
	void PushBothMove(Mouse::Event&& mevent) noexcept;
	bool IsEmpty() const noexcept {
		return buffer.empty();
	}
	bool IsEventEmpty() const noexcept {
		return events.empty();
	}
	void Flush() noexcept;
private:
	static long long CurrentTimeMS() noexcept;
	void OnMouseMove(int x, int y) noexcept;
	void OnMouseLeave() noexcept;
	void OnMouseEnter() noexcept;
	void OnLeftPressed(int x, int y) noexcept;
	void OnLeftReleased(int x, int y) noexcept;
	void OnRightPressed(int x, int y) noexcept;
	void OnRightReleased(int x, int y) noexcept;
	void OnMidPressed(int x, int y) noexcept;
	void OnMidReleased(int x, int y) noexcept;
	void OnWheelUp(int x, int y) noexcept;
	void OnWheelDown(int x, int y) noexcept;
	void TrimBuffer() noexcept;
	void OnWheelDelta(int x, int y, int delta) noexcept;
private:
	static constexpr unsigned int bufferSize = 16u;
	int x;
	int y;
	bool leftIsPressed = false;
	bool rightIsPressed = false;
	bool midIsPressed = false;
	bool isInWindow = false;
	int wheelDeltaCarry = 0;
	std::deque<Event> buffer;
	std::deque<Event> events;
};