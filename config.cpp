#include "config.h"
#include <iostream>
#include <stdexcept>

using namespace std;

Config::Config()
{
	init();
}

void Config::init()
{

    ifstream configFile("ommpc.conf", ios::in);

    if(configFile.fail()) {
		std::string msg = "CONFIG: Unable to open config file: ommpc.conf";
        throw runtime_error(msg.c_str());
	}
	readConfigFile(configFile);

	std::string skinConfig;
	if(getItem("showAlbumArt") == "false")
		skinConfig = "skin_noart.conf";
	else
		skinConfig = "skin.conf";	
	ifstream skinFile(("skins/"+getItem("skin")+"/"+skinConfig).c_str(), ios::in);	
    
	if(skinFile.fail()) {
		std::string msg = "CONFIG: Unable to open skin config file: " + 
							getItem("skin") + "/" + skinConfig;
        throw runtime_error(msg.c_str());
	}
	readConfigFile(skinFile);

}

void Config::readConfigFile(ifstream& configFile)
{
    std::string curItem;
    while(!configFile.eof()) {
        getline(configFile, curItem);
        if(!curItem.empty() && curItem[0] != '#') {
            int pos = curItem.find('=');
            std::string itemName = curItem.substr(0, pos);
            std::string itemValue = curItem.substr(pos+1);
            trimStr(itemName);
            trimStr(itemValue);
            m_configItems.insert(make_pair(itemName, itemValue));
        }
	}
}

std::string Config::getItem(std::string itemName)
{
    std::map<std::string, std::string>::iterator mIter = m_configItems.find(itemName);

    if(mIter != m_configItems.end()) {
        return (*mIter).second;
    } else {
        return "";
    }
}

int Config::getItemAsNum(std::string itemName)
{
	std::string value;
    std::map<std::string, std::string>::iterator mIter = m_configItems.find(itemName);

    if(mIter != m_configItems.end()) {
        value = (*mIter).second;
    } else {
        value = "";
    }

	return atoi(value.c_str());
}

void Config::trimStr(std::string & inStr)
{
    while(inStr[0] == ' ' || inStr[0] == '\t' || inStr[0] == '\n')
        inStr = inStr.substr(1);

    while(inStr[inStr.length()-1] == ' '
            || inStr[inStr.length()-1] == '\t'
            || inStr[inStr.length()-1] == '\n')
        inStr = inStr.substr(0, inStr.length()-1);

}

