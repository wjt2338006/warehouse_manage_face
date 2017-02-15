//
// Created by ragpanda on 16-5-4.
//

#include "RedisInput.h"
#include "Config.h"
#include <opencv2/face.hpp>
#include <unistd.h>


RedisInput::RedisInput()
{

    string configStr = Config::readConfig("./config/redis.json");
    //Log::info("输出配置:\n",configStr.c_str());

    this->config = std::move(json::parse(configStr));

    int port = stoi(config["redis_port"].get<string>());
    string ip = config["redis_ip"].get<string>();
    conn = shared_ptr<redisContext>(redisConnect(ip.c_str(),port));
    if (conn == NULL || conn->err) {
        if (conn) {

            throw runtime_error(conn->errstr);

        } else
        {
            throw runtime_error("Can't allocate redis context\n");
        }
    }

}

cv::Mat RedisInput::getImage(string sizeKey,string dataKey,string timeout)
{
    string command;


    if(config["redis_block_read"] == true)
    {
        command.append("BLPOP ");
    }
    else
    {
        command.append("lpop ");
        timeout = " ";
    }

    command.append(dataKey+" ");
    command.append(timeout);
    redisReply *reply = (redisReply *)redisCommand(conn.get(),command.c_str());
    if(reply==NULL)
    {
        throw runtime_error("get reply* fail");
    }
    if(reply->type == REDIS_REPLY_ARRAY)
    {
        char* replyResult = (char*)reply->element[1]->str;

        //获取图片大小
        string sizeListCommand;
        sizeListCommand.append("lpop ");
        sizeListCommand.append(sizeKey);
        redisReply *sizeListReply = (redisReply *)redisCommand(conn.get(),sizeListCommand.c_str());
        if(sizeListReply==NULL)
        {
            throw runtime_error("get reply* fail");
        }

        int size = stoi(string(sizeListReply->str));
        int  i =0;
        vector<uchar> imageBinary;
        while(i < size)
        {
            uchar tmp = (uchar) *(replyResult+i);
            imageBinary.push_back(tmp);
            i++;
        }
        cv::Mat img = cv::imdecode(imageBinary,cv::ImreadModes::IMREAD_COLOR);
        //cv::imshow("new image",img);
        //cv::waitKey(0);
        return img;


    }
    else
    {
        //Log::info("等待:","无法获取/没有等待到图片");
        freeReplyObject(reply);
        return cv::Mat();
    }

}

void RedisInput::putImage(cv::Mat inputImg)
{
    fstream imageFileStream;
    imageFileStream.open("./DataSet/01-1m.jpg",ios::in|ios::binary);
    if(!imageFileStream)
    {
        cerr << "error image" <<endl;
        return;
    }
    char buff[1024];

    vector<uchar> imageBinary;
    while(!imageFileStream.eof())
    {
        imageFileStream.read(buff,1024*sizeof(char));
        int i = 0;
        while(i<1024)
        {
            uchar tmp = (uchar) buff[i];
            imageBinary.push_back(tmp);
            i++;
        }
    }
    cv::Mat img = cv::imdecode(imageBinary,cv::ImreadModes::IMREAD_COLOR );
    if(img.data==NULL)
    {
        cerr << "err" << endl;
    }
    /*cv::imshow("ss",img);
    cv::waitKey(0);*/


    //存入数据
    redisReply *reply = (redisReply *)redisCommand(conn.get(),"rpush warehouse:faceList %b ",imageBinary.data(),(size_t)imageBinary.size());

    if(reply->type == REDIS_REPLY_ERROR)
    {
        Log::error("redis错误",reply->str);
    }

    freeReplyObject(reply);

    //存入数据长度
    redisReply *replySizeList  = (redisReply* ) redisCommand(conn.get(),"rpush warehouse:faceSizeList %d", imageBinary.size());
    if(replySizeList ->type == REDIS_REPLY_ERROR)
    {
        Log::error("redis错误",replySizeList->str);
    }
    freeReplyObject(replySizeList);



}

RedisInput::~RedisInput()
{
    //莫名其妙的双重释放
    //redisFree(conn.get());
}

cv::Mat RedisInput::getRecognizeFace(string &entranceId)
{

    string data = config["redis_recognize_list"].get<string>();
    string size = config["redis_facesize_list"].get<string>();
    string timeout = config["redis_block_timeout"].get<string>();
    cv::Mat result = getImage(size,data,timeout);
    if(result.empty()){return result;}
    entranceId = getEntranceID();

    return result;
}

string RedisInput::getEntranceID()
{
    string command("lpop ");
    string key = config["redis_recognize_entrance_list"];
    command+=key;
    redisReply *reply = (redisReply*) redisCommand(conn.get(),command.c_str());
    if(reply->type == REDIS_REPLY_STRING)
    {
        string id = reply->str;
        freeReplyObject(reply);
        return id;
    }
    else
    {
        freeReplyObject(reply);
        return NULL;
    }
}

cv::Mat RedisInput::getTrainFace(int &label,string &action)
{

    string data = config["redis_train_list"].get<string>();
    string size = config["redis_trainsize_list"].get<string>();
    string timeout = config["redis_block_timeout"].get<string>();
    cv::Mat image = getImage(size,data,timeout);
    if(image.empty())
    {
        return image;
    }
    label = getLabel();
    action = getAction();


    return image;
}

int RedisInput::getLabel()
{
    string command("lpop ");
    string key = config["redis_train_label_list"];
    command+=key;
    redisReply *reply = (redisReply*) redisCommand(conn.get(),command.c_str());
    if(reply->type == REDIS_REPLY_STRING)//type =  1
    {
        string label = reply->str;
        freeReplyObject(reply);
        return stoi(label);
    }
    else
    {
        freeReplyObject(reply);
        return -1;
    }

}
string RedisInput::getAction()
{
    string command("lpop ");
    string key = config["redis_train_action_list"];
    command+=key;
    redisReply *reply = (redisReply*) redisCommand(conn.get(),command.c_str());
    if(reply->type == REDIS_REPLY_STRING)//type =  1
    {
        string action(reply->str);
        freeReplyObject(reply);
        return std::move(action);
    }
    else
    {
        freeReplyObject(reply);
        return string("");
    }
}

/*
bool RedisInput::lockRedisList(string key)
{

    while(true)
    {
        string command("get ");
        command += key;
        redisReply *reply = (redisReply *)redisCommand(conn.get(),command.c_str());
        if(reply->type == REDIS_REPLY_STRING)
        {
            if(string("false") == reply->str)
            {
                freeReplyObject(reply);
                break;
            }
        }
        if(reply->type == REDIS_REPLY_NIL)
        {
            freeReplyObject(reply);
            break;
        }
        freeReplyObject(reply);
        sleep(1);
    }

    string command("set ");
    command += key;
    command += string(" true");
    redisReply *replySet = (redisReply *)redisCommand(conn.get(),command.c_str());
    if(replySet->type !=REDIS_REPLY_STRING)
    {
        if(string("OK") == "OK")
        {
            freeReplyObject(replySet);
            return true;
        }
    }

    freeReplyObject(replySet);
    return false;



}

bool RedisInput::unlockRedisList(string key)
{

    while(true)
    {
        string command("get ");
        command += key;
        redisReply *reply = (redisReply *)redisCommand(conn.get(),command.c_str());
        if(reply->type == REDIS_REPLY_STRING)
        {
            if(string("true") == reply->str)
            {
                freeReplyObject(reply);
                break;
            }
        }
        if(reply->type == REDIS_REPLY_NIL)
        {
            freeReplyObject(reply);
            return false;
        }
        freeReplyObject(reply);
        return false;
    }

    string command("set ");
    command += key;
    command += string(" false");
    redisReply *replySet = (redisReply *)redisCommand(conn.get(),command.c_str());
    if(replySet->type !=REDIS_REPLY_STRING)
    {
        if(string("OK") == "OK")
        {
            freeReplyObject(replySet);
            return true;
        }
    }

    freeReplyObject(replySet);
    return false;



}*/