#include "FileManager.h"
#include <fstream>


FileManager::FileManager()
{
	_settings  = new Settings();
	_lastLevel = new std::string();

	// Setup start settings
	_mapSettings["LvlSizeX"] = _settings->lvlSizeX;
	_mapSettings["LvlSizeY"] = _settings->lvlSizeY;
	_mapSettings["HudMaxSizeX"] = _settings->hudMaxSizeX;
	_mapSettings["HudMaxSizeY"] = _settings->hudMaxSizeY;

	// Reading settings from a settings file
	ReadSettings();
}

FileManager::~FileManager()
{
	delete _settings;
	delete _lastLevel; 
}

bool FileManager::ReadLevel()
{
	std::string fileName = "levels/level.txt";
	std::fstream file(fileName, std::ios_base::in);
	if (!file.is_open())
		return false;

	// Reading level from the file
	std::string line;

	_lastLevel->clear();
	while (getline(file, line))
		_lastLevel->append(line);

	file.close();
	return true;
}

bool FileManager::WriteLevel()
{
	std::string fileName = "levels/level.txt";
	std::fstream file(fileName, std::ios_base::out);
	if (!file.is_open())
		return false;

	// Writing level to the file
	std::string levelStr;
	file << levelStr;

	file.close();
	return true;
}

const std::string* FileManager::GetLastLevel()
{
	return _lastLevel;
}

const Settings* const FileManager::GetSettings()
{
	return _settings;
}

bool FileManager::ReadSettings()
{
	std::fstream file("settings.txt", std::ios_base::in);
	
	// if the settings file is not found, try creating it
	if (!file.is_open())
	{
		file.open("settings.txt", std::ios_base::out);
		if (file.is_open())
		{
			for (auto i = _mapSettings.begin(); i != _mapSettings.end(); ++i)
				file << i->first << "=" << i->second << '\n';

			file.close();
			return true;
		}
		else
			return false;
	}
		

	std::string str;
	while (getline(file, str))
	{
		auto pos = str.find('=');
		if (pos != std::string::npos)
		{
			std::string param = str.substr(0, pos);
			std::string value = str.substr(pos + 1);

			auto it = _mapSettings.find(param);
			if (it != _mapSettings.end())
				it->second = stoi(value);
		}
	}

	file.close();

	// Loading the read settings
	SetupSettings();

	return true;
}

void FileManager::SetupSettings()
{
	_settings->lvlSizeX = _mapSettings["LvlSizeX"];
	_settings->lvlSizeY = _mapSettings["LvlSizeY"];
	_settings->hudMaxSizeX = _mapSettings["HudMaxSizeX"];
	_settings->hudMaxSizeY = _mapSettings["HudMaxSizeY"];
}