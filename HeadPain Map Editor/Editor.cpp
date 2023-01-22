#include "Editor.h"
#include <string>
#include <iostream>
#include <conio.h>

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
		unsigned char inputChar = _getch();
		inputChar = tolower(inputChar);

		switch (inputChar)
		{
			case '1':	_mode = Mode::EDIT_LEVEL;		break;
			case '2':	_mode = Mode::EDIT_BKG;			break;
			case '3':	_mode = Mode::CREATE_LEVEL;		break;
			case '4':	_mode = Mode::CREATE_BKG;		break;

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
	_playerCoord.x = _settings->lvlSizeX / 2;
	_playerCoord.y = _settings->lvlSizeY / 2;

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
	_renSys->DrawChar(_playerCoord.y, _playerCoord.x, _user->GetRenderObject());

	_renSys->Render();
}

void Editor::RenderHud()
{
	static char textBuffer[25];
	const int _indentX = _settings->lvlSizeX + 5;

	// GLHF
	sprintf_s(textBuffer, "Level Key");
	_renSys->SendText(4, _indentX, textBuffer, Color::blue);
	
	sprintf_s(textBuffer, "Keys");
	_renSys->SendText(5, _indentX, textBuffer, Color::yellow);
	
	sprintf_s(textBuffer, "Your X coord: %i  ", _playerCoord.x);
	_renSys->SendText(13, _indentX, textBuffer);
	sprintf_s(textBuffer, "Your Y coord: %i  ", _playerCoord.y);
	_renSys->SendText(14, _indentX, textBuffer);

	_renSys->SendText(_settings->lvlSizeY + 1, 4, "WASD - move ");
	_renSys->SendText(_settings->lvlSizeY + 2, 4, " R   - restart");
}

void Editor::RestartLevel()
{
	// Restart level
	Initialize();
}

void Editor::Move()
{
	unsigned char inputChar = _getch();
	inputChar = tolower(inputChar);

	int p_x = _playerCoord.x;
	int p_y = _playerCoord.y;
	Entity p_entity = _user->GetEntity();

	if (inputChar == 0 || inputChar == 224)	// for special keys
		switch (_getch())
		{
			// Up Down Left Right for arrays
			case 72:   ChangeEntity(p_y, p_x, p_entity);	    break;
			case 80:   ChangeEntity(p_y, p_x, Entity::empty);	break;
			case 75:   MinusPlayerEntity();	  break;
			case 77:   PlusPlayerEntity();    break;
		}
	else
		switch (inputChar)
		{
			// Up Down Left Right
			case 'w': case 230: case 150:   MoveHeroTo(_playerCoord.y - 1, _playerCoord.x);	  break;
			case 's': case 235: case 155:	MoveHeroTo(_playerCoord.y + 1, _playerCoord.x);	  break;
			case 'a': case 228: case 148:	MoveHeroTo(_playerCoord.y, _playerCoord.x - 1);	  break;
			case 'd': case 162: case 130:	MoveHeroTo(_playerCoord.y, _playerCoord.x + 1);   break;

			case 'e': case 227: case 147:	PlusPlayerEntity();    break;
			case 'q': case 169: case 137:   MinusPlayerEntity();   break;
			case 'f': case 160: case 128:	ChangeEntity(p_y, p_x, Entity::empty);	break;
			case 32:	ChangeEntity(p_y, p_x, p_entity);	break;	// Space

			case 'r': case 170: case 138:   RestartLevel();   break;	// Restart level
		}
}

void Editor::MoveHeroTo(int y, int x)
{
	if (y >= 0 && y < _settings->lvlSizeY)
		_playerCoord.y = y;

	if (x >= 0 && x < _settings->lvlSizeX)
		_playerCoord.x = x;
}

void Editor::ChangeEntity(int y, int x, Entity entity)
{
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
			case Entity::empty:		DeleteObject(y, x);		_objectsMap[y][x] = _empty;		break;
			case Entity::wall:		DeleteObject(y, x);		_objectsMap[y][x] = _wall;		break;
			case Entity::fogOfWar:	DeleteObject(y, x);		_objectsMap[y][x] = _fog;		break;

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

void Editor::DeleteObject(int y, int x)
{
	Object* obj = _objectsMap[y][x];

	if (obj != _empty && obj != _wall && obj != _fog)
		delete obj;

	_objectsMap[y][x] = nullptr;
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
