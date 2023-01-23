#include "KeyDown.h"
#include <conio.h>
#include <iostream>

Key KeyDown::getWaitKey()
{
	unsigned char inputChar = _getch();
	inputChar = tolower(inputChar);

	if (inputChar == 0 || inputChar == 224)	// for special keys
		switch (_getch())
		{
			// Arrays (Up Down Left Right)
			case 72:   return Key::KEY_UP;
			case 80:   return Key::KEY_DOWN;
			case 75:   return Key::KEY_LEFT;
			case 77:   return Key::KEY_RIGHT;

			default:   return Key::_ERROR;
		}
	else
		switch (inputChar)
		{
			case 'w': case 230: case 150:   return Key::W;
			case 'a': case 228: case 148:   return Key::A;
			case 's': case 235: case 155:   return Key::S;
			case 'd': case 162: case 130:   return Key::D;
			case 'q': case 169: case 137:   return Key::Q;
			case 'e': case 227: case 147:   return Key::E;
			case 'r': case 170: case 138:   return Key::R;
			case 'f': case 160: case 128:   return Key::F;

			case '1':   return Key::NUM_1;
			case '2':   return Key::NUM_2;
			case '3':   return Key::NUM_3;
			case '4':   return Key::NUM_4;
			case '5':   return Key::NUM_5;
			case '6':   return Key::NUM_6;
			case '7':   return Key::NUM_7;
			case '8':   return Key::NUM_8;
			case '9':   return Key::NUM_9;
			case '0':   return Key::NUM_0;

			case 32:    return Key::KEY_SPACE;
			case 27:    return Key::KEY_ESC;
			case 13:    return Key::KEY_ENTER;

			default:   return Key::_ERROR;
		}
}
