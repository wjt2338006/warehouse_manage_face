//
// Created by ragpanda on 16-5-7.
//
#ifndef WAREHOUSE_FACE_CONFIG_H
#define WAREHOUSE_FACE_CONFIG_H
#include <string>
#include <fstream>
#include <string.h>
#include <stdexcept>

using namespace std;
class Config
{
public:
    static string readConfig(string configName)
    {
        fstream configFile;
        string configStr;

        configFile.open(configName,ios::in);
        if(!configFile.is_open())
        {
            throw runtime_error("can't open config");
        }

        while(!configFile.eof())
        {
            char tmp[2048];
            memset(tmp,0,sizeof(tmp));

            configFile.read(tmp,2047);
            configStr.append(tmp);

        }
        return configStr;
    }
};

#endif //WAREHOUSE_FACE_CONFIG_H
