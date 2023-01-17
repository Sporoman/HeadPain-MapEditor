#ifndef OBJECT_H
#define OBJECT_H

#include "renderSystem//colors.h"
#include "renderSystem//RenderObject.h"

enum class Entity;
enum class DirMove;

class Object
{
private:
	unsigned char _mapSymbol;	// Symbol on the physical map (before render)
	RenderObject  _renderObj;	// struct: symbol + color symbol + color backgorund

public:
	Object(unsigned char symbol);
	Object(Entity entity);
	~Object();

	void SetEntity(Entity entity);

	unsigned char GetMapSymbol();
	unsigned char GetRenderSymbol();
	Color GetColorSymbol();
	Color GetColorBackground();
	const RenderObject& GetRenderObject();
	
	static Entity GetInitEntity(unsigned char symbol);
	static unsigned char GetInitMapSymbol(Entity entity);
	static unsigned char GetInitRenderSymbol(Entity entity);
	static Color GetInitColorSymbol(Entity entity);
	static Color GetInitColorBkg(Entity entity);
	static Color GetInitColorFromBkgMap(unsigned char symbol);
};

enum class Entity
{
	error,
	empty,
	hero,
	wall,
	door,
	levelDoor,
	key,
	levelKey,
	box,
	exitDoor,
	crystal,
	mine,
	fogOfWar,
	heart,
	skeleton,
	rock
};

#endif // OBJECT_H