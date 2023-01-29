#include "Object.h"
#include <stdio.h>

int Object::__countObjects = 0;

Object::Object(unsigned char symbol) : Object(GetEntity(symbol))
{}

Object::Object(Entity entity)
{
	SetEntity(entity);

	// Plus the count of objects
	__countObjects++;
}

Object::~Object()
{
	// Minus the count of objects
	__countObjects--;
}

void Object::SetEntity(Entity entity)
{
	_entity = entity;

	// Initializing the object
	_mapSymbol = GetMapSymbol(_entity);

	// Initializing the render object
	_renderObj.symbol      = GetRenderSymbol(_entity);
	_renderObj.symbolColor = GetColorSymbol(_entity);
	_renderObj.bkgColor    = GetColorBkg(_entity);
}

Entity Object::GetEntity()
{
	return _entity;
}

const RenderObject& Object::GetRenderObject()
{
	return _renderObj;
}

int Object::GetObjectsCount()
{
	return __countObjects;
}

Entity Object::GetEntity(unsigned char symbol)
{
	switch (symbol)
	{
		case ' ':   return Entity::empty;
		case 'H':	return Entity::hero;
		case '#':	return Entity::wall;
		case 'd':	return Entity::door;
		case 'D':	return Entity::levelDoor;
		case 'b':	return Entity::box;
		case 'r':	return Entity::rock;
		case '*':	return Entity::mine;
		case 'k':	return Entity::key;
		case 'K':	return Entity::levelKey;
		case 'c':	return Entity::crystal;
		case '3':	return Entity::heart;
		case 's':	return Entity::skeleton;
		case '&':	return Entity::fakeWall;
		case 'E':	return Entity::exitDoor;
		case 'F':	return Entity::fog;
		
		default:    return Entity::_error;
	}
}

char* Object::GetEntityName(Entity entity)
{
	static char buffer[15];
	
	switch (entity)
	{
		case Entity::empty:		  sprintf_s(buffer, "Empty");        break;
		case Entity::hero:		  sprintf_s(buffer, "Hero");         break;
		case Entity::wall:		  sprintf_s(buffer, "Wall");         break;
		case Entity::door:		  sprintf_s(buffer, "Door");         break;
		case Entity::levelDoor:   sprintf_s(buffer, "Level door");   break;
		case Entity::box:		  sprintf_s(buffer, "Box");          break;
		case Entity::rock:		  sprintf_s(buffer, "Rock");         break;
		case Entity::mine:		  sprintf_s(buffer, "Mine");         break;
		case Entity::key:		  sprintf_s(buffer, "Key");          break;
		case Entity::levelKey:	  sprintf_s(buffer, "Level key");    break;
		case Entity::crystal:	  sprintf_s(buffer, "Crystal");      break;
		case Entity::heart:		  sprintf_s(buffer, "Heart");        break;
		case Entity::skeleton:    sprintf_s(buffer, "Skeleton");     break;
		case Entity::exitDoor:	  sprintf_s(buffer, "Exit door");    break;
		case Entity::fakeWall:	  sprintf_s(buffer, "Fake wall");    break;
		case Entity::fog:		  sprintf_s(buffer, "Fog");          break;

		default:   sprintf_s(buffer, "?");
	}

	return buffer;
}

unsigned char Object::GetMapSymbol(Entity entity)
{
	switch (entity)
	{
		case Entity::empty:		  return ' ';
		case Entity::hero:		  return 'H';
		case Entity::wall:		  return '#';
		case Entity::door:		  return 'd';
		case Entity::levelDoor:   return 'D';
		case Entity::box:		  return 'b';
		case Entity::rock:		  return 'r';
		case Entity::mine:		  return '*';
		case Entity::key:		  return 'k';
		case Entity::levelKey:	  return 'K';
		case Entity::crystal:	  return 'c';
		case Entity::heart:		  return '3';
		case Entity::skeleton:    return 's';
		case Entity::exitDoor:	  return 'E';
		case Entity::fakeWall:	  return '&';
		case Entity::fog:		  return 'F';

		default:   return '?';
	}
}

unsigned char Object::GetRenderSymbol(Entity entity)
{
	switch (entity)
	{
		case Entity::empty:		  return ' ';
		case Entity::hero:		  return 2;
		case Entity::wall:		  return 177;
		case Entity::door:		  return 219;
		case Entity::levelDoor:   return 219;
		case Entity::box:		  return 254;
		case Entity::rock:		  return 254;
		case Entity::mine:		  return 15;
		case Entity::key:		  return 21;
		case Entity::levelKey:	  return 21;
		case Entity::crystal:	  return 4;
		case Entity::heart:		  return 3;
		case Entity::skeleton:    return 2;
		case Entity::exitDoor:	  return 178;
		case Entity::fakeWall:	  return 177;
		case Entity::fog:		  return 176;

		default:   return '?';
	}
}

Color Object::GetColorSymbol(Entity entity)
{
	switch (entity)
	{
		case Entity::empty:		  return Color::black;
		case Entity::hero:		  return Color::green;
		case Entity::wall:		  return Color::gray;
		case Entity::door:		  return Color::yellow;
		case Entity::levelDoor:   return Color::blue;
		case Entity::box:		  return Color::brown;
		case Entity::rock:		  return Color::darkGray;
		case Entity::mine:		  return Color::red;
		case Entity::key:		  return Color::yellow;
		case Entity::levelKey:	  return Color::blue;
		case Entity::crystal:	  return Color::magenta;
		case Entity::heart:		  return Color::red;
		case Entity::skeleton:    return Color::white;
		case Entity::exitDoor:	  return Color::magenta;
		case Entity::fakeWall:	  return Color::darkGray;
		case Entity::fog:		  return Color::gray;

		default:   return Color::red;
	}
}

Color Object::GetColorBkg(Entity entity)
{
	switch (entity)
	{
		case Entity::empty:		  return Color::black;
		case Entity::hero:		  return Color::black;
		case Entity::wall:		  return Color::black;
		case Entity::door:		  return Color::black;
		case Entity::levelDoor:   return Color::black;
		case Entity::box:		  return Color::black;
		case Entity::rock:		  return Color::black;
		case Entity::mine:		  return Color::black;
		case Entity::key:		  return Color::black;
		case Entity::levelKey:	  return Color::black;
		case Entity::crystal:	  return Color::black;
		case Entity::heart:		  return Color::black;
		case Entity::skeleton:    return Color::black;
		case Entity::exitDoor:	  return Color::darkGreen;
		case Entity::fakeWall:	  return Color::black;
		case Entity::fog:		  return Color::black;

		default:   return Color::darkMagenta;
	}
}

Color Object::GetColorFromBkgMap(unsigned char symbol)
{
	switch (symbol)
	{
		case '#': 
		case ' ':   return Color::black;
		case 'b':   return Color::darkBlue;
		case 'g':   return Color::darkGreen;
		case 'c':   return Color::darkCyan;
		case 'r':   return Color::darkRed;
		case 'm':   return Color::darkMagenta;
		case 'y':   return Color::brown;
		case '-':   return Color::gray;
		case '_':   return Color::darkGray;
		case 'B':   return Color::blue;
		case 'G':   return Color::green;
		case 'C':   return Color::cyan;
		case 'R':   return Color::red;
		case 'M':   return Color::magenta;
		case 'Y':   return Color::yellow;
		case 'W':   return Color::white;

		default: return Color::black;
	}
}
