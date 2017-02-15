//
// Created by ragpanda on 16-5-9.
//

#include "Log.h"

void Log::info(const char* intro,string detail)
{
    /*lock_guard<mutex> lk(Log::m);
    if(!Log::init)
    {

        Log::fs->open("./log",ios::out);
        Log::init = true;
    }


    *Log::fs << std::move(string(intro))<< detail <<endl;
    Log::fs->close();*/

    Output::sendLog(intro,detail);
    //cout << std::move(string(intro))<< detail <<endl;
}
void Log::error(const char* intro,string detail)
{
    /*lock_guard<mutex> lk(Log::m);
    if(!Log::init)
    {

        Log::fs->open("./log",ios::out);
        Log::init = true;
    }


    *Log::fs << std::move(string(intro))<< detail <<endl;
    Log::fs->close();*/
    Output::sendLog(intro,detail);
    //cout << std::move(string(intro))<< detail <<endl;
}
bool Log::init = false;
shared_ptr<fstream> Log::fs = make_shared<fstream>();
mutex Log::m;
