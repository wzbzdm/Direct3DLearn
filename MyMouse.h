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
		long long timestamp;
	public:
		Event(Type type, const Mouse& parent) noexcept
			:
			type(type),
			leftIsPressed(parent.leftIsPressed),
			rightIsPressed(parent.rightIsPressed),
			x(parent.x),
			y(parent.y),
			timestamp(CurrentTimeMS())
		{}
		Event(Type type, const Mouse& parent, long long timestamp) noexcept
			:
			type(type),
			leftIsPressed(parent.leftIsPressed),
			rightIsPressed(parent.rightIsPressed),
			x(parent.x),
			y(parent.y),
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
	bool IsEmpty() const noexcept
	{
		return buffer.empty();
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
};