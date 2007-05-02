#ifndef __CONFIG_H__
#define __CONFIG_H__

#include <string>
#include <fstream>
#include <map>
#include <SDL.h>

class Config
{
public:
    Config();

	Config(std::string file);
	void init();
    std::string getItem(std::string item);
    int getItemAsNum(std::string item);
	void setItem(std::string itemName, std::string value);
	void getItemAsColor(std::string name, Uint8& red, Uint8& green, Uint8& blue);
	void saveConfigFile();


protected:
	void readConfigFile(std::ifstream& file);
    void trimStr(std::string & inStr);
    std::map<std::string, std::string> m_configItems;
};

#endif

