//
// Created by ragpanda on 16-5-4.
//

#ifndef WAREHOUSE_FACE_FACERECOGNIZE_H
#define WAREHOUSE_FACE_FACERECOGNIZE_H

#include <opencv2/opencv.hpp>
#include <opencv2/face.hpp>
#include <vector>
using namespace cv;
class FaceRecognize
{
public:
    FaceRecognize();
    ~FaceRecognize();
    void save();
    void trainFace(int flag,Mat &face);
    void updateFace(int flag,Mat &face);

    int findFace(Mat face,double &reliable);
    void detectObjectFromMat(const Mat img,Mat &result);
    void balanceFace(Mat & face);

private:
    CascadeClassifier detectFaceClassifier;
    Ptr <face::FaceRecognizer> model;
    FileStorage fileObj;
    const char* faceYMLName;


};


#endif //WAREHOUSE_FACE_FACERECOGNIZE_H
