#include "Editor.h"
#include "KeyDown.h"

Editor::Editor() : _isGameActive(false), _mode(Mode::NONE)
{
	_manager   = new FileManager();
	_settings  = _manager->GetSettings();

	const int renSizeX = _settings->lvlSizeX + _settings->hudMaxSizeX;
	const int renSizeY = _settings->lvlSizeY + _settings->hudMaxSizeY;
	_renSys = new RenderSystem(renSizeY, renSizeX);

	_user	= new Object(Entity::hero);
	_empty	= new Object(Entity::empty);
	_wall   = new Object(Entity::wall);
	_fog    = new Object(Entity::fogOfWar);

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
	ClearObjectMap();
	for (int y = 0; y < _settings->lvlSizeY; ++y)
		delete _objectsMap[y];
	delete[] _objectsMap;

	delete _user;
	delete _empty;
	delete _wall;
	delete _fog;

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
	// Clear object map
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
	// Render map
	for (int y = 0; y < _settings->lvlSizeY; ++y)
		for (int x = 0; x < _settings->lvlSizeX; ++x)
			_renSys->DrawChar(y, x, _objectsMap[y][x]->GetRenderObject());
	RenderHud();

	// Render player
	_renSys->DrawChar(_userCoord.y, _userCoord.x, _user->GetRenderObject());
	_renSys->DrawBkgCharColor(_userCoord.y, _userCoord.x, Color::darkBlue);

	_renSys->Render();
}

void Editor::RenderHud()
{
	static const int x = _settings->lvlSizeX + 5; // X indent
	static const int y = _settings->lvlSizeY;     // Y indent

	SendHudText(4, x, "Level Key", Color::blue);
	SendHudText(5, x, "Keys",      Color::yellow);
	
	SendHudText(13, x, "Your X coord: %i  ", _userCoord.x);
	SendHudText(14, x, "Your Y coord: %i  ", _userCoord.y);

	SendHudText(y + 1, 4, "WASD - move");
	SendHudText(y + 2, 4, " R   - restart");
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
	int u_x = _userCoord.x;
	int u_y = _userCoord.y;
	Entity p_entity = _user->GetEntity();

	switch (KeyDown::getWaitKey())
	{
		case Key::W:   MoveHeroTo(u_y-1, u_x);   break;
		case Key::S:   MoveHeroTo(u_y+1, u_x);   break;
		case Key::A:   MoveHeroTo(u_y, u_x-1);   break;
		case Key::D:   MoveHeroTo(u_y, u_x+1);   break;

		case Key::E: case Key::KEY_RIGHT:   PlusPlayerEntity();   break;
		case Key::Q: case Key::KEY_LEFT:    MinusPlayerEntity();  break;

		case Key::F: case Key::KEY_DOWN:         ChangeEntity(_userCoord, Entity::empty);   break;
		case Key::KEY_SPACE: case Key::KEY_UP:   ChangeEntity(_userCoord, p_entity);	    break;

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

void Editor::ChangeEntity(Coord coord, Entity entity)
{
	int y = coord.y;
	int x = coord.x;
	Object* obj = _objectsMap[y][x];

	if (obj == _empty || obj == _wall || obj == _fog)
	{
		// Create the new object
		switch (entity)
		{
			case Entity::empty:		_objectsMap[y][x] = _empty;		break;
			case Entity::wall:		_objectsMap[y][x] = _wall;		break;
			case Entity::fogOfWar:	_objectsMap[y][x] = _fog;		break;
		
			default:	_objectsMap[y][x] = CreateObject(entity);
		}
	}
	else
	{
		switch (entity)
		{
			case Entity::empty:		DeleteObject(coord);		_objectsMap[y][x] = _empty;		break;
			case Entity::wall:		DeleteObject(coord);		_objectsMap[y][x] = _wall;		break;
			case Entity::fogOfWar:	DeleteObject(coord);		_objectsMap[y][x] = _fog;		break;

			default:	_objectsMap[y][x]->SetEntity(entity);
		}
	}
}

void Editor::CreateEmptyLevel()
{
	for (int y = 0; y < _settings->lvlSizeY; ++y)
		for (int x = 0; x < _settings->lvlSizeX; ++x)
			if (y == 0 || x == 0)
				_objectsMap[y][x] = _wall;
			else
				_objectsMap[y][x] = _empty;
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
			// Take symbol from level map
			unsigned char symbol = level->at(y * _settings->lvlSizeX + x);

			// Create an object
			if (symbol == _empty->GetMapSymbol())
				_objectsMap[y][x] = _empty;
			else if (symbol == _wall->GetMapSymbol())
				_objectsMap[y][x] = _wall;
			else if (symbol == _fog->GetMapSymbol())
				_objectsMap[y][x] = _fog;
			else
			{
				// Create and set the object on objects map
				Object* object = CreateObject(symbol);
				_objectsMap[y][x] = object;
			}
		}
}

void Editor::ClearObjectMap()
{
	for (int y = 0; y < _settings->lvlSizeY; ++y)
		for (int x = 0; x < _settings->lvlSizeX; ++x)
			if ((_objectsMap[y][x] != _user) && (_objectsMap[y][x] != _empty)
				&& (_objectsMap[y][x] != _wall) && (_objectsMap[y][x] != _fog))
			{
				delete _objectsMap[y][x];
				_objectsMap[y][x] = nullptr;
			}
}

Object* Editor::CreateObject(unsigned char symbol)
{
	return new Object(symbol);
}

Object* Editor::CreateObject(Entity entity)
{
	return new Object(entity);
}

void Editor::DeleteObject(Coord coord)
{
	Object* obj = _objectsMap[coord.y][coord.x];

	if (obj != _empty && obj != _wall && obj != _fog)
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
