#ifndef RENDEROBJECT_H
#define RENDEROBJECT_H

#include "Colors.h"

struct RenderObject
{
	unsigned char symbol;	// Symbol on the visual map (after render)
	Color symbolColor;		// Symbol color
	Color bkgColor;			// Background color
};

#endif // RENDEROBJECT