#ifndef KEYDOWN_H
#define KEYDOWN_H

enum class Key;

class KeyDown
{
private:
	KeyDown();

public:
	static Key getWaitKey();
};

enum class Key
{
	_ERROR,
	KEY_UP,
	KEY_DOWN,
	KEY_LEFT,
	KEY_RIGHT,
	NUM_1,
	NUM_2,
	NUM_3,
	NUM_4,
	NUM_5,
	NUM_6,
	NUM_7,
	NUM_8,
	NUM_9,
	NUM_0,
	W,
	A,
	S,
	D,
	Q,
	E,
	R,
	F,
	KEY_ESC,
	KEY_ENTER,
	KEY_SPACE,
	_SIZE
};

#endif // KEYDOWN_H