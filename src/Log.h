//
// Created by ragpanda on 16-5-4.
//

#ifndef WAREHOUSE_FACE_LOG_H
#define WAREHOUSE_FACE_LOG_H
#include <string>
#include <iostream>
#include <fstream>
#include <memory>
#include <mutex>
#include "Output.h"
using namespace std;
class Log
{

public:
    static void info(const char* intro,string detail);
    static void error(const char* intro,string detail);

public:
    static shared_ptr<fstream> fs;
    static bool init;
    static mutex m;


};

#endif //WAREHOUSE_FACE_LOG_H
