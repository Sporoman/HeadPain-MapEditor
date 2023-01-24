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

	static const int I_EMPTY = 0;
	static const int I_WALL  = 1;
	static const int I_FOG   = 2;
	static const int I_SIZE  = 3;

	Object*   _user;
	Object**  _cloneObjects;
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
	Object* GetGameObject(Entity entity);
	void DeleteObject(Coord coord);		// Empty, wall and fog do not can be removed
	void PlusPlayerEntity();
	void MinusPlayerEntity();

	void CreateEmptyLevel();
	void LoadLevel();
	bool isCloneObject(Object* obj);
	bool isCloneObject(Entity entity);
};

#endif // GAME_H