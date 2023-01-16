#include "renderSystem.h"
#include <cstdio>

RenderSystem::RenderSystem(const int screenY, const int screenX)
	: _screenY(screenY), _screenX(screenX)
{
	// Setup buffers
	_backBuffer   = new RenderObject* [_screenY];
	_screenBuffer = new RenderObject* [_screenY];

	for (int y = 0; y < _screenY; ++y)
	{
		_backBuffer[y]   = new RenderObject[_screenX];
		_screenBuffer[y] = new RenderObject[_screenX];

		for (int x = 0; x < _screenX; ++x)
		{
			_backBuffer[y][x].symbol      = 0;
			_backBuffer[y][x].symbolColor = Color::black;
			_backBuffer[y][x].bkgColor    = Color::black;

			_screenBuffer[y][x] = _backBuffer[y][x];
		}
	}

	// Get console handle
	_consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);

	// Set console font
	SetConsoleFont();

	// Hide console cursor
	HideCursor();
}

RenderSystem::RenderSystem() : RenderSystem(s_default_screenY, s_default_screenX)
{}

RenderSystem::~RenderSystem()
{
	for (int y = 0; y < _screenY; ++y)
	{
		delete[] _backBuffer[y];
		delete[] _screenBuffer[y];
	}

	delete[] _backBuffer;
	delete[] _screenBuffer;
}

void RenderSystem::Clear()
{
	for (int y = 0; y < _screenY; ++y)
		for (int x = 0; x < _screenX; ++x)
		{
			_backBuffer[y][x].symbol      = 0;
			_backBuffer[y][x].symbolColor = Color::black;
			_backBuffer[y][x].bkgColor    = Color::black;
		}
}

void RenderSystem::DrawChar(int y, int x, const RenderObject& obj)
{
	if (y < 0 || x < 0 || y >= _screenY || x >= _screenX)
		return;

	_backBuffer[y][x].symbol      = obj.symbol;
	_backBuffer[y][x].symbolColor = obj.symbolColor;

	// If the object has a black background, it is not rendered(= transparent)
	if (obj.bkgColor != Color::black)
		_backBuffer[y][x].bkgColor = obj.bkgColor;
}

void RenderSystem::DrawFrontChar(int y, int x, const RenderObject& obj)
{
	if (y < 0 || x < 0 || y >= _screenY || x >= _screenX)
		return;

	_backBuffer[y][x].symbol      = obj.symbol;
	_backBuffer[y][x].symbolColor = obj.symbolColor;
}

void RenderSystem::DrawBkgCharColor(int y, int x, Color bkgColor)
{
	if (y < 0 || x < 0 || y >= _screenY || x >= _screenX)
		return;

	_backBuffer[y][x].bkgColor = bkgColor;
}

void RenderSystem::SendText(int y, int x, const char* text, Color symbolColor, Color bkgColor)
{
	int next_x = x;
	unsigned char symbol = *text;

	while (symbol != 0)
	{
		DrawChar(y, next_x, RenderObject{ symbol, symbolColor, bkgColor });

		++text;
		++next_x;
		symbol = *text;

		if (next_x >= _screenX)
			return;
	}
}

void RenderSystem::Render()
{
	bool screenBufferModified = false;

	for (int y = 0; y < _screenY; ++y)
		for (int x = 0; x < _screenX; ++x)
			if (CompareBuffers(&_backBuffer[y][x], &_screenBuffer[y][x]))
			{
				// Copy symbol data from back to screen buffer
				_screenBuffer[y][x] = _backBuffer[y][x];

				// Draw symbol in (y, x) position
				SetCursor(y, x);
				SetColor(_screenBuffer[y][x].symbolColor, _screenBuffer[y][x].bkgColor);
				printf("%c", _screenBuffer[y][x].symbol);

				screenBufferModified = true;
			}
}

bool RenderSystem::CompareBuffers(const RenderObject* buf_1, const RenderObject* buf_2) const
{
	if (  (buf_1->symbol      != buf_2->symbol)
	   || (buf_1->symbolColor != buf_2->symbolColor)
	   || (buf_1->bkgColor    != buf_2->bkgColor)
	   )
		return true;
	else
		return false;
}

void RenderSystem::SetColor(Color symbolColor, Color bkgColor)
{
	int consoleColor = static_cast<int>(symbolColor) | static_cast<int>(bkgColor) << 4;
	SetConsoleTextAttribute(_consoleHandle, consoleColor);
}

void RenderSystem::SetColor(Color symbolColor)
{
	int consoleColor = static_cast<int>(symbolColor);
	SetConsoleTextAttribute(_consoleHandle, consoleColor);
}

void RenderSystem::SetDefaultColor()
{
	SetColor(Color::gray, Color::black);
}

void RenderSystem::SetCursor(int y, int x)
{
	COORD cursorCoord;
	cursorCoord.X = x;
	cursorCoord.Y = y;
	SetConsoleCursorPosition(_consoleHandle, cursorCoord);
}

void RenderSystem::HideCursor()
{
	CONSOLE_CURSOR_INFO cursorInfo;
	cursorInfo.bVisible = false;
	cursorInfo.dwSize = 1;
	SetConsoleCursorInfo(_consoleHandle, &cursorInfo);
}

void RenderSystem::ShowCursor()
{
	CONSOLE_CURSOR_INFO cursorInfo;
	cursorInfo.bVisible = true;
	cursorInfo.dwSize = 25;
	SetConsoleCursorInfo(_consoleHandle, &cursorInfo);
}

void RenderSystem::SetConsoleFont()
{
	CONSOLE_FONT_INFOEX font;
	font.cbSize = sizeof(font);
	font.nFont  = 0;
	font.dwFontSize.X = 8;
	font.dwFontSize.Y = 16;
	font.FontFamily   = FF_DONTCARE;
	font.FontWeight   = FW_NORMAL;
	wcscpy_s(font.FaceName, L"Consolas");
	SetCurrentConsoleFontEx(_consoleHandle, FALSE, &font);
}