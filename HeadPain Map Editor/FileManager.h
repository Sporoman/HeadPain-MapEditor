#ifndef MAPMANAGER
#define MAPMANAGER

#include <string>
#include <map>

struct Settings;

class FileManager
{
private:
	std::map <std::string, int> _mapSettings;
	Settings* _settings;
	std::string* _lastLevel;

public:
	FileManager();
	~FileManager();

	bool ReadLevel(std::string& const fileName);
	const std::string* GetLastLevel();
	const Settings* const GetSettings();

private:
	bool ReadSettings();
	void SetupSettings();
};

struct Settings 
{
	int lvlSizeY = 21;
	int lvlSizeX = 40;
	int hudMaxSizeX = 25;
	int hudMaxSizeY = 10;
};

#endif // !MAPMANAGER