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

	Coord _playerCoord;

	Object* _player;
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
	
	void MoveHeroTo(int y, int x);
	void ChangeEntity(int y, int x, Entity entity);
	void RenderHud();
	void RestartLevel();
	void ClearObjectMap();
	Object* CreateObject(unsigned char symbol);
	Object* CreateObject(Entity entity);
	void DeleteObject(int y, int x);		// Empty, wall and fog do not can be removed
	void PlusPlayerEntity();
	void MinusPlayerEntity();

	void CreateEmptyLevel();
	void LoadLevel();
};

#endif // GAME_H