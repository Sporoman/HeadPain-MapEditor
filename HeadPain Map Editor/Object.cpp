#include "Object.h"

int Object::__idObjects    = 0;
int Object::__countObjects = 0;

Object::Object(unsigned char symbol) : Object(symbol, Coord{ 0,0 })
{}

Object::Object(unsigned char symbol, Coord coord) : Object(GetInitEntity(symbol), coord)
{}

Object::Object(Entity entity, Coord coord) : _entity(entity), _coord(coord)
{
	// Object id
	_id = __idObjects;
	__idObjects++;

	// Plus the count of objects
	__countObjects++;

	// Initializing the object
	_mapSymbol = GetInitMapSymbol(_entity);

	// Initializing the render object
	_renderObj.symbol      = GetInitRenderSymbol(_entity);
	_renderObj.symbolColor = GetInitColorSymbol(_entity);
	_renderObj.bkgColor    = GetInitColorBkg(_entity);
}

Object::Object(Entity entity) : Object(entity, Coord{ 0,0 })
{}

Object::~Object()
{
	// Minus the count of objects
	__countObjects--;
}

void Object::MoveOn(DirMove dir)
{
	switch (dir)
	{
		case DirMove::up:       MoveOnUp();       break;
		case DirMove::left:     MoveOnLeft();     break;
		case DirMove::down:     MoveOnDown();     break;
		case DirMove::right:    MoveOnRight();    break;

		default: break;
	}
}

void Object::SetCoord(int x, int y)
{
	Coord coord{ x, y };
	SetCoord(coord);
}

void Object::SetCoord(Coord coord)
{
	if (coord.x < 0)
		_coord.x = 0;
	else
		_coord.x = coord.x;

	if (coord.y < 0)
		_coord.y = 0;
	else
		_coord.y = coord.y;
}

unsigned char Object::GetMapSymbol()
{
	return _mapSymbol;
}

unsigned char Object::GetRenderSymbol()
{
	return _renderObj.symbol;
}

Color Object::GetColorSymbol()
{
	return _renderObj.symbolColor;
}

Color Object::GetColorBackground()
{
	return _renderObj.bkgColor;
}

const RenderObject& Object::GetRenderObject()
{
	return _renderObj;
}

Coord Object::GetCoord()
{
	return _coord;
}

Entity Object::GetEntity()
{
	return _entity;
}

int Object::GetObjectsCount()
{
	return __countObjects;
}

void Object::MoveOnUp()
{
	if (_coord.y <= 0)
		_coord.y = 0;
	else
		_coord.y--;
}

void Object::MoveOnLeft()
{
	if (_coord.x <= 0)
		_coord.x = 0;
	else
		_coord.x--;
}

void Object::MoveOnRight()
{
	_coord.x++;
}

void Object::MoveOnDown()
{
	_coord.y++;
}

Entity Object::GetInitEntity(unsigned char symbol)
{
	switch (symbol)
	{
		case ' ':	return Entity::empty;
		case 'H':	return Entity::hero;
		case '#':	return Entity::wall;
		case 'd':	return Entity::door;
		case 'D':	return Entity::levelDoor;
		case 'k':	return Entity::key;
		case 'K':	return Entity::levelKey;
		case 'b':	return Entity::box;
		case 'E':	return Entity::exitDoor;
		case 'c':	return Entity::crystal;
		case 'm':	return Entity::mine;
		case 'F':	return Entity::fogOfWar;
		case '3':	return Entity::heart;
		case 's':	return Entity::skeleton;
		case 'r':	return Entity::rock;

		default: return Entity::error;
	}
}

unsigned char Object::GetInitMapSymbol(Entity entity)
{
	switch (entity)
	{
		case Entity::empty:			return ' ';
		case Entity::hero:			return 'H';
		case Entity::wall:			return '#';
		case Entity::door:			return 'd';
		case Entity::levelDoor:		return 'D';
		case Entity::key:			return 'k';
		case Entity::levelKey:		return 'K';
		case Entity::box:			return 'b';
		case Entity::exitDoor:		return 'E';
		case Entity::crystal:		return 'c';
		case Entity::mine:			return 'm';
		case Entity::fogOfWar:		return 'F';
		case Entity::heart:			return '3';
		case Entity::skeleton:		return 's';
		case Entity::rock:			return 'r';

		case Entity::error:
		default: return '?';
	}
}

unsigned char Object::GetInitRenderSymbol(Entity entity)
{
	switch (entity)
	{
		case Entity::empty:			return ' ';
		case Entity::hero:			return 2;
		case Entity::wall:			return 177;
		case Entity::door:			return 219;
		case Entity::levelDoor:		return 219;
		case Entity::key:			return 21;
		case Entity::levelKey:		return 21;
		case Entity::box:			return 254;
		case Entity::exitDoor:		return 178;
		case Entity::crystal:		return 4;
		case Entity::mine:			return 15;
		case Entity::fogOfWar:		return 176;
		case Entity::heart:			return 3;
		case Entity::skeleton:		return 2;
		case Entity::rock:			return 254;

		case Entity::error:
		default: return '?';
	}
}

Color Object::GetInitColorSymbol(Entity entity)
{
	switch (entity)
	{
		case Entity::empty:			return Color::black;
		case Entity::hero:			return Color::green;
		case Entity::wall:			return Color::gray;
		case Entity::door:			return Color::yellow;
		case Entity::levelDoor:		return Color::blue;
		case Entity::key:			return Color::yellow;
		case Entity::levelKey:		return Color::blue;
		case Entity::box:			return Color::brown;
		case Entity::exitDoor:		return Color::magenta;
		case Entity::crystal:		return Color::magenta;
		case Entity::mine:			return Color::red;
		case Entity::fogOfWar:		return Color::gray;
		case Entity::heart:			return Color::red;
		case Entity::skeleton:		return Color::white;
		case Entity::rock:			return Color::darkGray;

		case Entity::error:
		default: return Color::red;
	}
}

Color Object::GetInitColorBkg(Entity entity)
{
	switch (entity)
	{
		case Entity::empty:			return Color::black;
		case Entity::hero:			return Color::black;
		case Entity::wall:			return Color::black;
		case Entity::door:			return Color::black;
		case Entity::levelDoor:		return Color::black;
		case Entity::key:			return Color::black;
		case Entity::levelKey:		return Color::black;
		case Entity::box:			return Color::black;
		case Entity::exitDoor:		return Color::darkGreen;
		case Entity::crystal:		return Color::black;
		case Entity::mine:			return Color::black;
		case Entity::fogOfWar:		return Color::black;
		case Entity::heart:			return Color::black;
		case Entity::skeleton:		return Color::black;
		case Entity::rock:			return Color::black;

		case Entity::error:
		default: return Color::darkMagenta;
	}
}

Color Object::GetInitColorFromBkgMap(unsigned char symbol)
{
	switch (symbol)
	{
		case '#': 
		case ' ':	 return Color::black;
		case 'b':    return Color::darkBlue;
		case 'g':    return Color::darkGreen;
		case 'c':    return Color::darkCyan;
		case 'r':    return Color::darkRed;
		case 'm':    return Color::darkMagenta;
		case 'y':    return Color::brown;
		case '-':    return Color::gray;
		case '_':    return Color::darkGray;
		case 'B':    return Color::blue;
		case 'G':    return Color::green;
		case 'C':    return Color::cyan;
		case 'R':    return Color::red;
		case 'M':    return Color::magenta;
		case 'Y':    return Color::yellow;
		case 'W':    return Color::white;

		default: return Color::black;
	}
}
