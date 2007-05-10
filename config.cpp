/*****************************************************************************************

ommpc(One More Music Player Client) - A Music Player Daemon client targetted for the gp2x

Copyright (C) 2007 - Tim Temple(codertimt@gmail.com)

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

*****************************************************************************************/

#include "config.h"
#include <iostream>
#include <stdexcept>

using namespace std;

Config::Config()
{
	init();
}

Config::Config(string file)
{
    ifstream configFile(file.c_str(), ios::in);
    if(configFile.fail()) {
		std::string msg = "CONFIG: Unable to open config file: " + file;
        throw runtime_error(msg.c_str());
	}
	
	readConfigFile(configFile);
}

void Config::init()
{
	m_configItems.clear();
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

void Config::saveConfigFile()
{
	ofstream configFile("ommpc.conf", ios::out|ios::trunc);

    if(configFile.fail()) {
		std::string msg = "CONFIG: Unable to open config file: ommpc.conf";
        throw runtime_error(msg.c_str());
	}

	for(std::map<std::string, std::string>::iterator cIter =  m_configItems.begin();
	cIter != m_configItems.end();
	++cIter) {
		if((*cIter).first.substr(0,3) != "sk_") {
			configFile << (*cIter).first << "=" << (*cIter).second << endl;
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

void Config::setItem(string itemName, string value) 
{
	//this is intentially this simple to allow inserting of new config item....
	m_configItems[itemName] = value;
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

void Config::getItemAsColor(std::string name, Uint8& red, Uint8& green, Uint8& blue)
{
    std::map<std::string, std::string>::iterator mIter = m_configItems.find(name);

    if(mIter != m_configItems.end()) {
       	int commaPos1 = (*mIter).second.find(',');
		red = atoi(((*mIter).second.substr(0, commaPos1)).c_str());
		int commaPos2 = (*mIter).second.find(',', commaPos1+1);
		green = atoi(((*mIter).second.substr(commaPos1+1, commaPos2-commaPos1)).c_str());;
		int commaPos3 = (*mIter).second.find(',', commaPos2+1);
		blue = atoi(((*mIter).second.substr(commaPos2+1, commaPos3-commaPos2)).c_str());
	}
}
