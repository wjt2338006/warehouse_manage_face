//
// Created by ragpanda on 16-5-11.
//
#include "Output.h"
void Output::startSession(string entrance, int label)
{
    string configStr = Config::readConfig("./config/output.json");
    json configObj = std::move(json::parse(configStr));


    string model = configObj["model"];
    int port = configObj["port"].get<int>();
    string route = configObj["route"];
    string access = configObj["module_access"];
    string ip;
    string hostname;

    if(model == "ip_address")
    {
        ip = configObj["ip"];
        hostname = ip;
    }
    if(model == "hostname")
    {
        ip = configObj["ip"];
        hostname = configObj["hostname"];
    }
    /*
            "address":"172.17.0.3:8001",
            "route":"/api_cpp_startSession",
            "module_access":"123"
    */

    json request;
    request["module_access"]=access;
    request["entrance"]=entrance;
    request["label"]=label;
    string content = request.dump();


    stringstream s;
    s<<"POST "<<route<<" HTTP/1.1 \r\n";
    s<<"Host: "<<hostname<<":"<<port<<"\r\n";
    s<<"Content-Type: text/json; charset=utf-8\r\n";
    s<<"Connection: close\r\n";
    s<<"Content-Length: "<< content.length()<< "\r\n";
    s<<"Accept: */*\r\n";
    s<<"User-Agent: Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/45.0.2454.85 Safari/537.36\r\n";
    s<<"Accept: */*\r\n";
    s<<"Accept-Language: zh-CN,zh;q=0.8\r\n";
    s<<"Referer: http://172.17.0.3:8002/client_operate_index\r\n";
    s<<"Accept-Encoding: gzip, deflate\r\n";
    s<<"\r\n";
    s<<content;



    string requestString = s.str();
    //cout <<requestString <<endl;
    io_service service;
    ip::tcp::endpoint ep( ip::address::from_string(ip),port);
    ip::tcp::socket sock(service);
    sock.connect(ep);
    sock.write_some(buffer(requestString));
    char b[10240];
    memset(b,0,10240);
    sock.read_some(buffer(b,10240));
    cout << b <<endl;
    sock.close();
    //cout <<b;



}

void Output::sendLog(string intro, string detail)
{
    string configStr = Config::readConfig("./config/output.json");
    json configObj = std::move(json::parse(configStr));


    string model = configObj["model"];
    int port = configObj["port"].get<int>();
    string route = configObj["log_route"];
    string access = configObj["module_access"];
    string ip;
    string hostname;

    if(model == "ip_address")
    {
        ip = configObj["ip"];
        hostname = ip;
    }
    if(model == "hostname")
    {
        ip = configObj["ip"];
        hostname = configObj["hostname"];
    }
    /*
            "address":"172.17.0.3:8001",
            "route":"/api_cpp_startSession",
            "module_access":"123"
    */

    json request;
    request["module_access"]=access;
    request["intro"]=intro;
    request["detail"]=detail;
    string content = request.dump();


    stringstream s;
    s<<"POST "<<route<<" HTTP/1.1 \r\n";
    s<<"Host: "<<hostname<<":"<<port<<"\r\n";
    s<<"Content-Type: text/json; charset=utf-8\r\n";
    s<<"Connection: close\r\n";
    s<<"Content-Length: "<< content.length()<< "\r\n";
    s<<"Accept: */*\r\n";
    s<<"User-Agent: Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/45.0.2454.85 Safari/537.36\r\n";
    s<<"Accept: */*\r\n";
    s<<"Accept-Language: zh-CN,zh;q=0.8\r\n";
    s<<"Referer: http://172.17.0.3:8002/client_operate_index\r\n";
    s<<"Accept-Encoding: gzip, deflate\r\n";
    s<<"\r\n";
    s<<content;



    string requestString = s.str();
    //cout <<requestString <<endl;
    io_service service;
    ip::tcp::endpoint ep( ip::address::from_string(ip),port);
    ip::tcp::socket sock(service);
    sock.connect(ep);
    sock.write_some(buffer(requestString));
    char b[10240];
    memset(b,0,10240);
    sock.read_some(buffer(b,10240));
    cout << b <<endl;
    sock.close();
}
/*
string Output::getIpByHostname(string ipAddress,int port)
{
    io_service service;
    ip::tcp::resolver resolver(service);
    ip::tcp::resolver::query query(ipAddress, std::to_string(port)) ;
    ip::tcp::resolver::iterator iter = resolver.resolve( query);
    ip::tcp::endpoint ep = *iter;
    return ep.address().to_string();
}*/
