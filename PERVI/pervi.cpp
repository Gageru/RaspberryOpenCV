#include <opencv2/opencv.hpp>
#include <iostream>
#include <chrono>
#include <thread>
#include <wiringPi.h>

using namespace std;
using namespace cv;
 
int main(){

  VideoCapture cap(0); 
  cap.set(CAP_PROP_FRAME_WIDTH, 1024);
  cap.set(CAP_PROP_FRAME_HEIGHT, 768);  
  
  int nFPS;
  cap.set(CAP_PROP_EXPOSURE, 1);
  int frameCounter = 0;
  int tick = 0;
  int fps;
  time_t timeBegin = time(0);

  if(!cap.isOpened()){
    cout << "Error opening video stream or file" << endl;
    return -1;
  }
  
    Mat img; 
    cap >> img;  
    cap.read(img);
    imshow( "Frame", img);
  while(1){  
     cap >> img;
     if (img.empty())
     break;
     cap.read(img);
	   frameCounter++;
     time_t timeNow = time(0) - timeBegin;
     if (timeNow - tick >= 1)
        {
            tick++;
            fps = frameCounter;
            frameCounter = 0;
        }
     nFPS = cap.get(CAP_PROP_FPS);
     putText(img, cv::format("FPS=%d", fps ), Point(30, 30), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0,255,0));
     putText(img, cv::format("FPS=%d", nFPS ), Point(30, 45), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0,255,0));
     imshow( "Frame", img);

     char c=(char)waitKey(1);
     if(c==27)
      break;
  }
  
  cap.release();
  destroyAllWindows();
     
  return 0;
}
