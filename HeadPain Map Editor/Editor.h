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

class Editor
{
private:
	bool _isGameActive;
	Mode _mode;

	RenderSystem* _renSys;
	FileManager* _manager;
	const Settings* _settings;

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
	void ClearObjectMap();
	Object* CreateObject(Entity entity);
	void Shutdown();

	void Initialize();
	void Render();
	void RenderHud();
	void RestartLevel();
	void Move();
	void MoveHeroTo(int y, int x);
};

#endif // GAME_H