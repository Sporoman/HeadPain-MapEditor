#ifndef OBJECT_H
#define OBJECT_H

#include "renderSystem//colors.h"
#include "renderSystem//RenderObject.h"

enum class Entity;

class Object
{
private:
	static int __countObjects;	// Total number of objects

	unsigned char _mapSymbol;	// Symbol on the physical map (before render)
	RenderObject  _renderObj;	// struct: symbol + symbol color + background color
	Entity _entity;

public:
	Object(unsigned char symbol);
	Object(Entity entity);
	~Object();

	void SetEntity(Entity entity);

	Entity GetEntity();
	const RenderObject& GetRenderObject();

	static int GetObjectsCount();
	static Entity GetEntity(unsigned char symbol);
	static char* GetEntityName(Entity entity);
	static unsigned char GetMapSymbol(Entity entity);
	static unsigned char GetRenderSymbol(Entity entity);
	static Color GetColorSymbol(Entity entity);
	static Color GetColorBkg(Entity entity);
	static Color GetColorFromBkgMap(unsigned char symbol);
};

enum class Entity
{
	_error,
	empty,
	hero,
	wall,
	door,
	levelDoor,
	box,
	rock,
	mine,
	key,
	levelKey,
	crystal,
	heart,
	skeleton,
	exitDoor,
	fakeWall,
	fog,
	_size
};

#endif // OBJECT_H