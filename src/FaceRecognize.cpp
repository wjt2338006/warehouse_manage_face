//
// Created by ragpanda on 16-5-4.
//

#include "FaceRecognize.h"
#include "Log.h"
#include <vector>
using namespace std;

FaceRecognize::FaceRecognize()
:
model(face::createLBPHFaceRecognizer())
{
    faceYMLName ="./yml/faceData.yml";
    detectFaceClassifier.load("./Classifier/haarcascade_frontalface_alt2.xml");
    //使用快速的Haar检测器
    //FileStorage fileStorage(faceYMLName,FileStorage::READ);
    //fileObj = fileStorage;
    model->load(faceYMLName);


}
FaceRecognize::~FaceRecognize()
{
    //fileObj.release();
    model->save(faceYMLName);
}
void FaceRecognize::save()
{
    model->save(faceYMLName);
}
void FaceRecognize::trainFace(int flag,Mat &face)
{

    detectObjectFromMat(face,face);
    //imshow("tu",face);
    //waitKey(0);
    vector<Mat> faces;
    vector<int> ids;
    ids.push_back(flag);
    faces.push_back(face);

    model->train(faces,ids);
    save();

    //model->save(faceYMLName);
}
void FaceRecognize::updateFace(int flag,Mat &face)
{
    detectObjectFromMat(face,face);
    //imshow("tu",face);
    //waitKey(0);
    vector<Mat> faces;
    vector<int> ids;
    ids.push_back(flag);
    faces.push_back(face);
    model->update(faces,ids);
    save();

}


int FaceRecognize::findFace(Mat face,double &reliable)
{
    Mat handle;
    detectObjectFromMat(face,handle);
    int label;
    model->predict(handle,label,reliable);


    //save image
    string fileNameSource("./faceTmp/source_");
    string fileNameHandle("./faceTmp/handle_");
    char tmp[64];
    time_t t = time(0);
    strftime( tmp, sizeof(tmp), "%Y%m%d%H%M%S.jpg",localtime(&t) );
    fileNameSource+=tmp;
    fileNameHandle+=tmp;
    cout <<"保存处理后图像" << fileNameSource<<fileNameHandle <<endl;
    imwrite(fileNameSource,face);
    imwrite(fileNameHandle,handle);


    if(reliable > 100)
    {
        Log::info("置信度过低(>100)：",string("训练集中找不到合适的")+string(" 可能标签：")+std::to_string(label)+string(" 置信度：")+std::to_string(reliable));
        return -1;
    }


    return label;
}
void FaceRecognize::detectObjectFromMat(const Mat img,Mat &result)
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
    detectFaceClassifier.detectMultiScale(equalizedImg, objects,1.1,3,CASCADE_FIND_BIGGEST_OBJECT);
    if(objects.size()>0)
    {
        Rect largestObject = (Rect)objects.at(0);
        result = gray(largestObject);
        //平衡左右脸的光线
        //balanceFace(result);
        //Mat filtered = Mat(result.size(),CV_8U);
        //不要对LBPH进行滤波
        // 双边滤波，是一种非线性的滤波方法，是结合图像空间邻近度和像素值相似度的一种折中处理，同时考虑空域信息和灰度相似性，
        //达到保边去噪目的
        //bilateralFilter(result,filtered,0,10.0,2.0);

        //imshow("均值处理前图像",result);
        //result = filtered;
        //imshow("探测面部并处理后图像",result);
        //waitKey(0);


    }
    else
    {
        Log::info("探测脸部失败:", " 没有在图像中找到脸");
        result = gray;
    }

}
void FaceRecognize::balanceFace(Mat & face)
{
    int w = face.cols;
    int h = face.rows;
    Mat wholeFace;
    equalizeHist(face,wholeFace);
    int midX = w/2;
    Mat leftSide = face(Rect(0,0,midX,h));
    Mat rightSide = face(Rect(midX,0,w-midX,h));
    equalizeHist(leftSide,leftSide);
    equalizeHist(rightSide,rightSide);

}
