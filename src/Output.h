//
// Created by ragpanda on 16-5-7.
//

#ifndef WAREHOUSE_FACE_OUTPUT_H
#define WAREHOUSE_FACE_OUTPUT_H
#include <string>
#include <fstream>
#include "./lib/json.hpp"
#include "Config.h"
#include <boost/asio.hpp>
#include  <pthread.h>
using namespace std;
using namespace boost::asio;
using json = nlohmann::json;
class Output
{

public:
    static void startSession(string entrance, int label);
    static void sendLog( string intro, string detail);

    //static string getIpByHostname(string ipAddress,int port);
};


#endif //WAREHOUSE_FACE_OUTPUT_H
