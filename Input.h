#pragma once
#include <queue>
class KeyboardEvent {
public:
	//Create an enum for storing the states of the button
	enum class EventType {
		Press, 
		Release, 
		Invalid
	};

	KeyboardEvent();
	//Set the event and the key
	KeyboardEvent(const EventType type, const unsigned char key);

	//Check if button is pressed
	bool IsPress() const				{ return type == EventType::Press; };

	//Check if button is released
	bool IsRelease() const				{ return type == EventType::Release; };

	//Check if button is valid
	bool IsValid() const				{ return type == EventType::Invalid; };

	//Get the key code for the character
	unsigned char GetKeyCode() const	{ return key; };


private:
	EventType type;
	unsigned char key;
};

class Keyboard {
public:
	Keyboard();
	//Check if a certain key is pressed
	bool KeyIsPressed(const unsigned char keycode);
	//Check if the key buffer is empty
	bool KeyBufferIsEmpty();
	//Check if the character buffer is empty
	bool CharBufferIsEmpty();

	//The enum state of the key
	KeyboardEvent ReadKey();
	//Return the character pressed
	unsigned char ReadChar();

	//
	void OnKeyPressed(const unsigned char key);
	void OnKeyReleased(const unsigned char key);
	void OnChar(const unsigned char key);

	//Set auto repeat keys and chars (If button is help repeatedly take it as a key is pressed event)
	void AutoRepeatKeys(bool state);
	void AutoRepeatChars(bool state);

	//Check if auto repeat is on or not
	bool IsKeysAutoRepeat();
	bool IsCharsAutoRepeat();

private:
	//Set variables for keyboard events
	bool autoRepeatKeys = false;
	bool autoRepeatChars = false;
	bool keyStates[256];
	std::queue<KeyboardEvent> keyBuffer;
	std::queue<unsigned char> charBuffer;
};
 
//Create a struct for where the mouse is
struct MousePoint {
	int x;
	int y;
};

//Create mouse event class
class MouseEvent {
public: 
	//Create an enum for all mouse event types
	enum class EventType {
		LPress,
		LRelease,
		RPress,
		RRelease,
		MPress,
		MRelease,
		WheelUp,
		WheelDown,
		Move,
		RAW_MOVE,
		Invalid
	};
private:
	//Set the variables for the mouse event
	EventType type;
	int x;
	int y;
public: 
	//Set mouse event types
	MouseEvent() : type(EventType::Invalid), x(0), y(0) {}
	MouseEvent(const EventType type, const int x, const int y) : type(type), x(x), y(y) {}
	//Return if event was valid
	bool IsValid() const {return type != EventType::Invalid;};
	//Get the type of event
	EventType GetType() const { return type; };
	//Get the position of the mouse
	MousePoint GetPos() const { return { x, y };};
	//Get the position of the mouse
	int GetPosX() const { return x; };
	int GetPosY() const { return y; };
};


class Mouse {
public:
	//Setters for each mouse event type
	void OnLeftPressed(int x, int y);
	void OnLeftReleased(int x, int y);
	void OnRightPressed(int x, int y);
	void OnRightReleased(int x, int y);
	void OnMiddlePressed(int x, int y);
	void OnMiddleReleased(int x, int y);
	void OnWheelUp(int x, int y);
	void OnWheelDown(int x, int y);
	void OnMouseMove(int x, int y);
	void OnMouseMoveRaw(int x, int y);

	//Getters for mouse buttons
	bool IsLeftDown() { return leftIsDown; };
	bool IsMiddleDown() { return middleIsDown; };
	bool IsRightDown() { return rightIsDown; };

	//Get the positon of the mouse
	int GetPosX() { return x; };
	int GetPosY() { return y; };
	MousePoint GetPos() { return { x, y }; };

	//Check if mouse event butter is empty
	bool EventBufferIsEmpty() { return eventBuffer.empty(); };
	//Read the mouse event
	MouseEvent ReadEvent();


private:
	//Create variables for mouse events
	std::queue<MouseEvent> eventBuffer;
	bool leftIsDown = false;
	bool rightIsDown = false;
	bool middleIsDown = false;
	int x = 0;
	int y = 0;
};