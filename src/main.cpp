#include <iostream>
#include "RedisInput.h"
#include "FaceRecognize.h"
#include "test.h"
#include "Output.h"
#include <thread>
#include <mutex>

using namespace std;
mutex redisMutex;
mutex faceMutex;
void  deamon();
void run();
int main()
{
    deamon();
    run();

    return 0;
}

void deamon()
{
    int pid;
    if(pid =fork())
    {
        exit(0);
    }
    if(pid < 0)
    {
        exit(1);
    }
    setsid();

    if(pid = fork())
    {
        exit(0);
    }
    if(pid < 0)
    {
        exit(1);
    }

}
void run()
{

    FaceRecognize faceModel;
    vector<thread> threadList;
    RedisInput redisRecognizeRead;
    RedisInput redisTrainRead;


    bool continueWaitImage = true;
    thread recognizeControlThread([&]()
                                  {
                                      while(continueWaitImage)
                                      {
                                          try
                                          {
                                              string entranceId;
                                              Mat someFace = redisTrainRead.getRecognizeFace(entranceId);
                                              if(someFace.empty())
                                              {
                                                  continue;
                                              }
                                              //imshow("sassa",someFace);
                                              //waitKey(0);

                                              thread imageHandle([someFace,entranceId,&faceModel]
                                                                 {
                                                                     double reliable;
                                                                     int face;

                                                                     unique_lock<mutex> lockG(faceMutex);
                                                                     //lockG.lock();
                                                                     face = faceModel.findFace(someFace,reliable);
                                                                     lockG.unlock();



                                                                     if(face == -1)
                                                                     {
                                                                         return;
                                                                     }
                                                                     Log::info("完成认证",string("face:")+std::to_string(face)+string(" reliable: ")+std::to_string(reliable));
                                                                     //请求php服务器
                                                                     Output::startSession(entranceId,face);
                                                                 });
                                              imageHandle.detach();
                                          }
                                          catch (cv::Exception e)
                                          {
                                              Log::error("识别出错",e.msg.c_str());
                                          }


                                      }

                                  });

    thread trainControlThread([&]
                              {

                                  while(continueWaitImage)
                                  {
                                      try
                                      {
                                          int label;
                                          string action;
                                          Mat someFace = redisRecognizeRead.getTrainFace(label,action);
                                          if(someFace.empty())
                                          {

                                              continue;
                                          }
                                          //单线程处理训练
                                          unique_lock<mutex> lk(faceMutex);
                                          lk.lock();
                                          if(action == string("train"))
                                          {
                                              faceModel.trainFace(label,someFace);
                                              Log::info("训练成功:",string("标签为 = ")+std::to_string(label));
                                              continue;
                                          }
                                          if(action == string("update"))
                                          {
                                              faceModel.updateFace(label,someFace);
                                              Log::info("训练成功",string("标签为：")+std::to_string(label));
                                              continue;
                                          }
                                          lk.unlock();
                                          Log::info("训练失败",string("未知的动作，标签为：")+std::to_string(label)+string(" 动作：")+action);
                                      }
                                      catch (cv::Exception e)
                                      {
                                          Log::error("训练出错",e.msg.c_str());
                                      }


                                  }
                              });

    trainControlThread.join();
    recognizeControlThread.join();


}
