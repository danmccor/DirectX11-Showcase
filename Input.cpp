#include "Input.h"

KeyboardEvent::KeyboardEvent() : type(EventType::Invalid), key(0u)
{
}

KeyboardEvent::KeyboardEvent(const EventType type, const unsigned char key) : type(type), key(key)
{
}

Keyboard::Keyboard()
{
	//Set all keystates to false, as none will be pressed on initation
	for (int i = 0; i < 256; i++) {
		keyStates[i] = false;
	}
}

bool Keyboard::KeyIsPressed(const unsigned char keycode)
{
	//Return the keystate of the keycode
	return keyStates[keycode];
}

bool Keyboard::KeyBufferIsEmpty()
{
	//return buffer empty status
	return keyBuffer.empty();
}

bool Keyboard::CharBufferIsEmpty()
{
	//return buffer empty status
	return charBuffer.empty();
}

KeyboardEvent Keyboard::ReadKey()
{
	//if the key buffer is empty 
	if (this->keyBuffer.empty()) {
		//return empty key
		return KeyboardEvent();
	}
	//else
	else {
		//keyboard event e = the front of the buffer
		KeyboardEvent e = keyBuffer.front();
		//pop the keybuffer to remove the key
		keyBuffer.pop();
		//return the key
		return e;
	}
}

unsigned char Keyboard::ReadChar()
{
	//if char key buffer is empty 
	if (this->charBuffer.empty()) {
		//return empty char
		return 0u;
	}
	else {
		//store character from buffer
		unsigned char e = charBuffer.front();
		//remove character from buffer
		charBuffer.pop();
		//return character
		return e;
	}
}

void Keyboard::OnKeyPressed(const unsigned char key)
{
	//when key is pressed, set the keystate of key to true
	keyStates[key] = true;
	//push the key into the buffer
	keyBuffer.push(KeyboardEvent(KeyboardEvent::EventType::Press, key));
}

void Keyboard::OnKeyReleased(const unsigned char key)
{
	//if key is released, set the keystate of the key to false
	keyStates[key] = false;
	//push release event to the buffer
	keyBuffer.push(KeyboardEvent(KeyboardEvent::EventType::Release, key));
}

void Keyboard::OnChar(const unsigned char key)
{
	//push the charcter to the char buffer
	charBuffer.push(key);
}

void Keyboard::AutoRepeatKeys(bool state)
{
	//set autorepeat keys
	autoRepeatKeys = state;
}

void Keyboard::AutoRepeatChars(bool state)
{
	//set auto repeat chars
	autoRepeatChars = state;
}

bool Keyboard::IsKeysAutoRepeat()
{
	//get is key autorepeat on
	return autoRepeatKeys;
}

bool Keyboard::IsCharsAutoRepeat()
{
	//get is char autorepeat on
	return autoRepeatChars;
}



void Mouse::OnLeftPressed(int x, int y)
{
	//set left is down to true
	leftIsDown = true;
	//push the event into the buffer with position
	eventBuffer.push(MouseEvent(MouseEvent::EventType::LPress, x, y));
}

void Mouse::OnLeftReleased(int x, int y)
{
	//set left is down to false
	leftIsDown = false;
	//push the event into the buffer with position
	eventBuffer.push(MouseEvent(MouseEvent::EventType::LRelease, x, y));
}

void Mouse::OnRightPressed(int x, int y)
{
	//set right is down to true
	rightIsDown = true;
	//push the event into the buffer with position
	eventBuffer.push(MouseEvent(MouseEvent::EventType::RPress, x, y));
}

void Mouse::OnRightReleased(int x, int y)
{
	//set right is down to false
	rightIsDown = false;
	//push the event into the buffer with position
	eventBuffer.push(MouseEvent(MouseEvent::EventType::RRelease, x, y));
}

void Mouse::OnMiddlePressed(int x, int y)
{
	//set middle is down to true
	middleIsDown = true;
	//push the event into the buffer with position
	eventBuffer.push(MouseEvent(MouseEvent::EventType::MPress, x, y));
}

void Mouse::OnMiddleReleased(int x, int y)
{
	//set middle is down to false
	middleIsDown = false;
	//push the event into the buffer with position
	eventBuffer.push(MouseEvent(MouseEvent::EventType::MRelease, x, y));
}

void Mouse::OnWheelUp(int x, int y)
{
	//push the event into the buffer with position
	eventBuffer.push(MouseEvent(MouseEvent::EventType::WheelUp, x, y));
}

void Mouse::OnWheelDown(int x, int y)
{
	//push the event into the buffer with position
	eventBuffer.push(MouseEvent(MouseEvent::EventType::WheelDown, x, y));
}

void Mouse::OnMouseMove(int x, int y)
{
	this->x = x; 
	this->y = y; 
	//push the event into the buffer with position
	eventBuffer.push(MouseEvent(MouseEvent::EventType::Move, x, y));
}

void Mouse::OnMouseMoveRaw(int x, int y)
{
	//push the event into the buffer with position
	eventBuffer.push(MouseEvent(MouseEvent::EventType::RAW_MOVE, x, y));
}


MouseEvent Mouse::ReadEvent()
{
	if (eventBuffer.empty()) {
		//return an empty mouse event
		return MouseEvent();
	}
	else {
		//get the front of the event buffer
		MouseEvent me = eventBuffer.front();
		//remove front of event buffer
		eventBuffer.pop();
		//return mouse event
		return me;
	}
}
