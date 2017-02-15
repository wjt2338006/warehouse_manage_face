//
// Created by ragpanda on 16-5-4.
//

#ifndef WAREHOUSE_FACE_REDISINPUT_H
#define WAREHOUSE_FACE_REDISINPUT_H
#include <iostream>
#include <fstream>
#include <string>
#include "opencv2/opencv.hpp"
#include <stdio.h>
#include <hiredis/hiredis.h>

#include "lib/json.hpp"

#include "Log.h"


using namespace std;
using json = nlohmann::json;
class RedisInput
{
public:

    RedisInput();
    RedisInput(const RedisInput&) = delete;
    RedisInput& operator=(const RedisInput&) =delete;
    ~RedisInput();

    //底层队列图片获取方法
    cv::Mat getImage(string sizeKey,string dataKey,string timeout);
    //测试用推入图片
    void putImage(cv::Mat img);

    //封装后的获取待识别脸
    cv::Mat getRecognizeFace(string &entranceId);
    //获取队列中的一条入口id
    string getEntranceID();

    //封装后的获取待训练脸
    cv::Mat getTrainFace(int &label,string &action);
    //获取队列中的标签,如果不存在，则返回-1
    int getLabel();
    string getAction();

    //bool lockRedisList(string key);
    //bool unlockRedisList(string key);



private:
    json config;
    shared_ptr<redisContext> conn;


    /*
      "redis_recognize_list": "warehouse:faceRecognizeList",
      "redis_facesize_list": "warehouse:faceRecognizeSizeList",
      "redis_recognize_entrance_list":"warehouse:faceRecognizeEntranceList",

      "redis_block_read": true,
      "redis_block_timeout": " 60",

      "redis_train_list":"warehouse:faceTrainList",
      "redis_trainsize_list":"warehouse:faceTrainSizeList",
      "redis_train_label_list":"warehouse:faceTrainLabelList"
     */
};


#endif //WAREHOUSE_FACE_REDISINPUT_H
