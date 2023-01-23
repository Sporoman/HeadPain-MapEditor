#ifndef GAME_H
#define GAME_H

#include "renderSystem/renderSystem.h"
#include "FileManager.h"
#include "Object.h"

enum Mode
{
	NONE,
	CREATE_LEVEL,
	EDIT_LEVEL,
	CREATE_BKG,
	EDIT_BKG
};

struct Coord {
	int x = 0;
	int y = 0;
};

class Editor
{
private:
	bool _isGameActive;
	Mode _mode;

	RenderSystem* _renSys;
	FileManager* _manager;
	const Settings* _settings;

	Coord _userCoord;

	Object* _user;
	Object* _empty;
	Object* _wall;
	Object* _fog;

	Object*** _objectsMap;

public:
	Editor();
	~Editor();

	void Start();

private:
	void ChooseMode();
	void Shutdown();

	void Initialize();
	void Render();
	void Move();
	
	void MoveHeroTo(Coord coord);
	void MoveHeroTo(int y, int x);
	void ChangeEntity(Coord coord, Entity entity);
	void RenderMap();
	void RenderHud();
	void RenderUser();
	void SendHudText(int y, int x, const char* text, Color symbolColor = Color::gray, Color bkgColor = Color::black);
	void SendHudText(int y, int x, const char* text, int count, Color symbolColor = Color::gray, Color bkgColor = Color::black);
	void RestartLevel();
	void ClearObjectMap();
	Object* CreateObject(unsigned char symbol);
	Object* CreateObject(Entity entity);
	void DeleteObject(Coord coord);		// Empty, wall and fog do not can be removed
	void PlusPlayerEntity();
	void MinusPlayerEntity();

	void CreateEmptyLevel();
	void LoadLevel();
};

#endif // GAME_H