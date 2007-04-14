#ifndef __CONFIG_H__
#define __CONFIG_H__

#include <string>
#include <fstream>
#include <map>

class Config
{
public:
    Config();

	void init();
    std::string getItem(std::string item);
    int getItemAsNum(std::string item);


protected:
	void readConfigFile(std::ifstream& file);
    void trimStr(std::string & inStr);
    std::map<std::string, std::string> m_configItems;
};

#endif

