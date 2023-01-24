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

	// Initialize user object
	_user = new Object(Entity::hero);

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

	// Set player coord
	_userCoord.x = _settings->lvlSizeX / 2;
	_userCoord.y = _settings->lvlSizeY / 2;

	// Clear render system
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

void Editor::RenderHud()
{
	static const int x = _settings->lvlSizeX + 5; // X indent
	static const int y = _settings->lvlSizeY + 1; // Y indent

	SendHudText(4, x, "Level Key", Color::blue);
	SendHudText(5, x, "Keys",      Color::yellow);
	
	SendHudText(13, x, "Your X coord: %i  ", _userCoord.x);
	SendHudText(14, x, "Your Y coord: %i  ", _userCoord.y);

	SendHudText(y, 4, "WASD - move");
	SendHudText(y + 1, 4, " R   - restart");
}

void Editor::RenderUser()
{
	_renSys->DrawChar(_userCoord.y, _userCoord.x, _user->GetRenderObject());
	_renSys->DrawBkgCharColor(_userCoord.y, _userCoord.x, Color::darkBlue);
}

void Editor::SendHudText(int y, int x, const char* text, Color symbolColor, Color bkgColor)
{
	static char textBuffer[25];

	sprintf_s(textBuffer, text);
	_renSys->SendText(y, x, textBuffer, symbolColor, bkgColor);
}

void Editor::SendHudText(int y, int x, const char* text, int count, Color symbolColor, Color bkgColor)
{
	static char textBuffer[25];

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

		case Key::F: case Key::KEY_DOWN:         ChangeEntity(_userCoord, Entity::empty);        break;
		case Key::KEY_SPACE: case Key::KEY_UP:   ChangeEntity(_userCoord, _user->GetEntity());   break;

		case Key::R:   RestartLevel();   break;
	}
}

void Editor::MoveHeroTo(Coord coord)
{
	MoveHeroTo(coord.y, coord.x);
}

void Editor::MoveHeroTo(int y, int x)
{
	if (y >= 0 && y < _settings->lvlSizeY)
		_userCoord.y = y;

	if (x >= 0 && x < _settings->lvlSizeX)
		_userCoord.x = x;
}

void Editor::ChangeEntity(Coord coord, Entity newEntity)
{
	int y = coord.y;
	int x = coord.x;

	if (isCloneObject(_objectsMap[y][x]))
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
	for (int y = 0; y < _settings->lvlSizeY; ++y)
		for (int x = 0; x < _settings->lvlSizeX; ++x)
			if (y == 0 || x == 0)
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
			_objectsMap[y][x] = GetGameObject(Object::GetInitEntity(symbol));
		}
}

bool Editor::isCloneObject(Object* obj)
{
	return isCloneObject(obj->GetEntity());
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

		case Entity::_error:  return nullptr;

		default:   return new Object(entity);
	}
}

void Editor::DeleteObject(Coord coord)
{
	Object* obj = _objectsMap[coord.y][coord.x];

	if (_objectsMap[coord.y][coord.x] == nullptr)
		return;

	if (!isCloneObject(obj))
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
