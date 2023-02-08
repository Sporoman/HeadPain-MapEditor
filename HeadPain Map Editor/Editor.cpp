#include "Editor.h"
#include "KeyDown.h"

Editor::Editor() : _isGameActive(false), _mode(Mode::NONE)
{
	// Initialize support systems
	_manager   = new FileManager();
	_settings  = _manager->GetSettings();

	const int renSizeX = _settings->lvlSizeX + _settings->hudMaxSizeX;
	const int renSizeY = _settings->lvlSizeY + _settings->hudMaxSizeY;
	_renSys = new RenderSystem(renSizeY, renSizeX);

	// Initialize user and hud objects
	_user = new Object(Entity::hero);
	_hudObject = new Object(Entity::empty);

	// Initialize clone objects
	_cloneObjects = new Object* [I_SIZE];
	_cloneObjects[I_EMPTY] = new Object(Entity::empty);
	_cloneObjects[I_WALL]  = new Object(Entity::wall);
	_cloneObjects[I_FOG]   = new Object(Entity::fog);

	// Initialize objects map
	_objectsMap = new Object** [_settings->lvlSizeY];
	for (int y = 0; y < _settings->lvlSizeY; ++y)
	{
		_objectsMap[y] = new Object* [_settings->lvlSizeX];
		for (int x = 0; x < _settings->lvlSizeX; ++x)
			_objectsMap[y][x] = nullptr;
	}
}

Editor::~Editor()
{
	// Delete objects map
	ClearObjectMap();
	for (int y = 0; y < _settings->lvlSizeY; ++y)
		delete _objectsMap[y];
	delete[] _objectsMap;

	// Delete user
	delete _user;

	// Delete clone objects
	for (int i = 0; i < I_SIZE; ++i)
		delete _cloneObjects[i];

	// Delete support systems
	delete _renSys;
	delete _manager;
}

void Editor::Start()
{
	ChooseMode();
	_isGameActive = true;

	while (true)
	{
		Initialize();
		PreRenderHud();

		while (_isGameActive)
		{
			Render();
			Move();
		}

		_isGameActive = true;
	}

	Shutdown();
}

void Editor::ChooseMode()
{
	_renSys->SendText(1, 4, "Choose mode:");
	_renSys->SendText(2, 4, "1) Edit level");
	_renSys->SendText(3, 4, "2) Edit background");
	_renSys->SendText(4, 4, "3) Create level");
	_renSys->SendText(5, 4, "4) Create background");
	_renSys->Render();

	bool check = true;
	while(check)
	{
		check = false;

		switch (KeyDown::getWaitKey())
		{
			case Key::NUM_1:   _mode = Mode::EDIT_LEVEL;	 break;
			case Key::NUM_2:   _mode = Mode::EDIT_BKG;		 break;
			case Key::NUM_3:   _mode = Mode::CREATE_LEVEL;   break;
			case Key::NUM_4:   _mode = Mode::CREATE_BKG;	 break;

			default:
			check = true;
			_renSys->SendText(7, 4, "Just choose mode -_-", static_cast<Color>(rand() % 15 + 1));	// 15 Colors (without black)
			_renSys->Render();
		}
	}
}

void Editor::Shutdown()
{
	_renSys->Clear();
	_renSys->SendText(1, 4, "Thank you for playing :) Bye - bye!");
	_renSys->Render();

	Sleep(3000);
	_renSys->Clear();
	_renSys->Render();
}

void Editor::Initialize()
{
	ClearObjectMap();

	switch (_mode)
	{
		case CREATE_LEVEL: case CREATE_BKG:	CreateEmptyLevel(); break;
		case EDIT_LEVEL:   case EDIT_BKG:	LoadLevel();		break;

		default: exit(-2);
	}

	// Set player coord and clear render system
	_userCoord.x = _settings->lvlSizeX / 2;
	_userCoord.y = _settings->lvlSizeY / 2;

	_renSys->Clear();
}

void Editor::Render()
{
	RenderMap();
	RenderHud();
	RenderUser();	

	_renSys->Render();
}

void Editor::RenderMap()
{
	for (int y = 0; y < _settings->lvlSizeY; ++y)
		for (int x = 0; x < _settings->lvlSizeX; ++x)
			_renSys->DrawChar(y, x, _objectsMap[y][x]->GetRenderObject());
}

void Editor::RenderUser()
{
	_renSys->DrawChar(_userCoord.y, _userCoord.x, _user->GetRenderObject());
	_renSys->DrawBkgCharColor(_userCoord.y, _userCoord.x, Color::darkBlue);
}

void Editor::RenderHud()
{
	static const int x = _settings->lvlSizeX + 4; // X indent
	static const int y = _settings->lvlSizeY + 2; // Y indent
	
	SendHudText(y,   4, "Current entity: %s       ", Object::GetEntityName(_user->GetEntity()));
	SendHudText(y+1, 4, "Objects count: %i  ", Object::GetObjectsCount());

	SendHudText(y,   x-20, "Your X coord: %i  ", _userCoord.x);
	SendHudText(y+1, x-20, "Your Y coord: %i  ", _userCoord.y);
}

void Editor::PreRenderHud()
{
	static const int x = _settings->lvlSizeX + 4; // X indent
	static const int y = _settings->lvlSizeY + 2; // Y indent

	// Entities
	SendHudText(1, x, "-Entities-", Color::green);

	SendHudEntity(2, x, Entity::empty, "1");
	SendHudEntity(3, x, Entity::hero, "2");
	SendHudEntity(4, x, Entity::wall, "3");
	SendHudEntity(5, x, Entity::door, "4");
	SendHudEntity(6, x, Entity::levelDoor, "5");
	SendHudEntity(7, x, Entity::box, "6");
	SendHudEntity(8, x, Entity::rock, "7");
	SendHudEntity(9, x, Entity::mine, "8");

	SendHudEntity(2, x + 7, Entity::key, "9");
	SendHudEntity(3, x + 7, Entity::levelKey, "0");
	SendHudEntity(4, x + 7, Entity::crystal, "x");
	SendHudEntity(5, x + 7, Entity::heart, "x");
	SendHudEntity(6, x + 7, Entity::skeleton, "x");
	SendHudEntity(7, x + 7, Entity::fakeWall, "x");
	SendHudEntity(8, x + 7, Entity::fog, "x");

	// Keys
	SendHudText(12, x, "-Keys-", Color::blue);

	SendHudText(13, x, "WASD  - move");
	SendHudText(14, x, "Q/E   - change Entity");
	SendHudText(15, x, "F     - set Empty");
	SendHudText(16, x, "SPACE - set Entity");
	SendHudText(17, x, "R     - restart");
}

void Editor::SendHudEntity(int y, int x, Entity entity, const char* number)
{
	_hudObject->SetEntity(entity);
	_renSys->DrawChar(y, x, _hudObject->GetRenderObject());
	_renSys->SendText(y, x+1, " - ");
	_renSys->SendText(y, x+4, number);
}

void Editor::SendHudText(int y, int x, const char* text, Color symbolColor, Color bkgColor)
{
	static char textBuffer[50];

	sprintf_s(textBuffer, text);
	_renSys->SendText(y, x, textBuffer, symbolColor, bkgColor);
}

void Editor::SendHudText(int y, int x, const char* text, int count, Color symbolColor, Color bkgColor)
{
	static char textBuffer[50];

	sprintf_s(textBuffer, text, count);
	_renSys->SendText(y, x, textBuffer, symbolColor, bkgColor);
}

void Editor::SendHudText(int y, int x, const char* text, const char* count, Color symbolColor, Color bkgColor)
{
	static char textBuffer[50];

	sprintf_s(textBuffer, text, count);
	_renSys->SendText(y, x, textBuffer, symbolColor, bkgColor);
}

void Editor::RestartLevel()
{
	// Restart level
	Initialize();
}

void Editor::Move()
{
	int y = _userCoord.y;
	int x = _userCoord.x;

	switch (KeyDown::getWaitKey())
	{
		case Key::W:   MoveHeroTo(y-1, x);   break;
		case Key::S:   MoveHeroTo(y+1, x);   break;
		case Key::A:   MoveHeroTo(y, x-1);   break;
		case Key::D:   MoveHeroTo(y, x+1);   break;

		case Key::E: case Key::KEY_RIGHT:   PlusPlayerEntity();   break;
		case Key::Q: case Key::KEY_LEFT:    MinusPlayerEntity();  break;

		case Key::F: case Key::KEY_DOWN:         ChangeObjectEntity(_userCoord, Entity::empty);        break;
		case Key::KEY_SPACE: case Key::KEY_UP:   ChangeObjectEntity(_userCoord, _user->GetEntity());   break;

		case Key::R:           RestartLevel();   break;
		case Key::KEY_ENTER:   SaveLevel();      break;
	}
}

void Editor::MoveHeroTo(int y, int x)
{
	int yMax = _settings->lvlSizeY;
	int xMax = _settings->lvlSizeX;

	if (y >= yMax)
		y = 0;
	else if (y < 0)
		y = yMax-1;
	
	_userCoord.y = y;

	if (x >= xMax)
		x = 0;
	else if (x < 0)
		x = xMax-1;
	
	_userCoord.x = x;
}

void Editor::ChangeObjectEntity(Coord coord, Entity newEntity)
{
	int y = coord.y;
	int x = coord.x;

	if (isCloneObject(_objectsMap[y][x]->GetEntity()))
		_objectsMap[y][x] = GetGameObject(newEntity);
	else
	{
		if (isCloneObject(newEntity))
		{
			DeleteObject(Coord{ x, y });
			_objectsMap[y][x] = GetGameObject(newEntity);
		}
		else
			_objectsMap[y][x]->SetEntity(newEntity);
	}
}

void Editor::CreateEmptyLevel()
{
	int yMax = _settings->lvlSizeY;
	int xMax = _settings->lvlSizeX;

	for (int y = 0; y < yMax; ++y)
		for (int x = 0; x < xMax; ++x)
			if (y == 0 || x == 0 || y == yMax-1 || x == xMax-1)
				_objectsMap[y][x] = GetGameObject(Entity::wall);
			else
				_objectsMap[y][x] = GetGameObject(Entity::empty);
}

void Editor::LoadLevel()
{
	if (!_manager->ReadLevel())
	{
		_isGameActive = false;
		return;
	}

	const std::string* level = _manager->GetLastLevel();
	for (int y = 0; y < _settings->lvlSizeY; ++y)
		for (int x = 0; x < _settings->lvlSizeX; ++x)
		{
			unsigned char symbol = level->at(y * _settings->lvlSizeX + x);
			_objectsMap[y][x] = GetGameObject(Object::GetEntity(symbol));
		}
}

void Editor::SaveLevel()
{
	int sizeY = _settings->lvlSizeY;
	int sizeX = _settings->lvlSizeX;

	std::string levelMap;
	for (int y = 0; y < sizeY; ++y)
	{
		for (int x = 0; x < sizeX; ++x)
			levelMap += _objectsMap[y][x]->GetMapSymbol();
		levelMap += '\n';
	}

	_manager->WriteLevel(&levelMap);
}

bool Editor::isCloneObject(Entity entity)
{
	if (entity == Entity::empty || entity == Entity::wall || entity == Entity::fog)
		return true;

	return false;
}

void Editor::ClearObjectMap()
{
	for (int y = 0; y < _settings->lvlSizeY; ++y)
		for (int x = 0; x < _settings->lvlSizeX; ++x)
			DeleteObject(Coord{ x, y });
}

Object* Editor::GetGameObject(Entity entity)
{
	switch (entity)
	{
		case Entity::empty:   return _cloneObjects[I_EMPTY];
		case Entity::wall:    return _cloneObjects[I_WALL];
		case Entity::fog:     return _cloneObjects[I_FOG];

		default:   return new Object(entity);
	}
}

void Editor::DeleteObject(Coord coord)
{
	Object* obj = _objectsMap[coord.y][coord.x];

	if (obj == nullptr)
		return;

	if (!isCloneObject(obj->GetEntity()))
		delete obj;

	_objectsMap[coord.y][coord.x] = nullptr;
}

void Editor::PlusPlayerEntity()
{
	int entity = (int)_user->GetEntity();
	++entity;

	Entity unit = (Entity)entity;
	if (unit == Entity::_size)
		unit = Entity(1);

	_user->SetEntity(unit);
}

void Editor::MinusPlayerEntity()
{
	int entity = (int)_user->GetEntity();
	--entity;

	Entity unit = (Entity)entity;
	if (unit == Entity::_error)
		unit = Entity((int)(Entity::_size) - 1);
		
	_user->SetEntity(unit);
}
