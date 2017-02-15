//
// Created by ragpanda on 16-5-5.
//

#ifndef WAREHOUSE_FACE_TEST_H_H
#define WAREHOUSE_FACE_TEST_H_H
#include <iostream>
#include "RedisInput.h"
#include "FaceRecognize.h"
void detectObjectFromMat(
        const Mat &img,CascadeClassifier &cascade,Mat &result

);
//训练脸部测试
void trainFaceTest()
{
    FaceRecognize faceModule;
    Mat image1 = imread("./DataSet/01-1m.jpg");
    Mat image2 = imread("./DataSet/01-2m.jpg");
    faceModule.trainFace(1,image1);
    faceModule.trainFace(1,image2);
    Mat image3 = imread("./DataSet/02-1m.jpg");
    Mat image4 = imread("./DataSet/02-2m.jpg");
    faceModule.trainFace(2,image3);
    faceModule.trainFace(2,image4);


    Mat image5 = imread("./DataSet/01-5m.jpg");
    double r;
    int face = faceModule.findFace(image5,r);
    cout << face <<endl<<r<<endl;
}
//发现脸部测试
void findFaceTest()
{
    Mat image5 = imread("./DataSet/02-1m.jpg");
    CascadeClassifier detectFaceClassifier;
    detectFaceClassifier.load("./Classifier/haarcascade_frontalface_alt2.xml");
    detectObjectFromMat(image5,detectFaceClassifier,image5);
    double r;
    int face;
    Ptr <face::FaceRecognizer> model= face::createLBPHFaceRecognizer();
    model->load("./yml/faceData.yml");
    model->predict(image5,face,r);
    cout <<face<<endl<<r<<endl;
}
//基本测试
void faceTest()
{
    CascadeClassifier detectFaceClassifier;
    detectFaceClassifier.load("./Classifier/haarcascade_frontalface_alt2.xml");
    FileStorage fileStorage("./yml/1234.yml",FileStorage::READ);
    Ptr <face::FaceRecognizer> smodel = face::createLBPHFaceRecognizer();
    smodel->load(fileStorage);


    /*Mat a = imread("./DataSet/01-5m.jpg");
    detectObjectFromMat(a,detectFaceClassifier,a);

    vector<int> flags;
    vector<Mat> mats ;

    mats.push_back(a);
    flags.push_back(1);
    smodel->update(mats,flags);*/

    Mat b = imread("./DataSet/01-5m.jpg");


    detectObjectFromMat(b,detectFaceClassifier,b);
    int l;
    double d;
    smodel->predict(b,l,d);
    cout << l <<endl <<d<<endl;

    //smodel->save("./yml/1234.yml");



}

void detectObjectFromMat(
        const Mat &img,CascadeClassifier &cascade,Mat &result

)
{
    Mat gray;

    if (img.channels() == 3)  //彩色图像使用
    {
        cvtColor(img, gray, CV_BGR2GRAY);
    }
    else if (img.channels() == 4) //移动端图像使用
    {
        cvtColor(img, gray, CV_BGRA2GRAY);
    }
    else        //本身就是灰度图
    {
        gray = img;
    }
    Mat equalizedImg;
    equalizeHist(gray,equalizedImg);//归一化

    vector <Rect> objects;
    cascade.detectMultiScale(equalizedImg, objects,1.1,3,CASCADE_FIND_BIGGEST_OBJECT);
    if(objects.size()>0)
    {
        Rect largestObject = (Rect)objects.at(0);
        result = gray(largestObject);


    }
    else
    {
        result = gray;
    }

}
/*
void testLock()
{
    RedisInput a;
    if(a.lockRedisList("hah"))
    {
        a.unlockRedisList("hah");
    };
}*/
void testLabel()
{
    RedisInput redisInput;
    cout << redisInput.getLabel() <<endl;
}
#endif //WAREHOUSE_FACE_TEST_H_H
