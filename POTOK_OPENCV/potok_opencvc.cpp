#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <opencv2/opencv.hpp>
#include <iostream>
#include <cstdio>
#include <thread>
#include <mutex>

using namespace std;
using namespace cv;

Mat frm;
mutex mtx;
int c=0;
int fps = 0;
bool isMainRunning;
bool is_frame_prepare;
string window_name = "Video";

void GoStep(int &x, int &y){
	
}
void run_capture()
{
    VideoCapture capture(0);
    capture.set(CAP_PROP_FRAME_WIDTH, 160);
    capture.set(CAP_PROP_FRAME_HEIGHT, 120);  
   
    int count = 0;
    int tick = 0;
	time_t timeBegin = time(0);
	time_t timeNow;
    if( capture.isOpened() )
    {
        while (isMainRunning)
        {
            mtx.lock();
            if (is_frame_prepare) {
            
            } else {
                cout <<"frame: "<< count++ <<" \n";
                capture.read(frm);
                time_t timeNow = time(0) - timeBegin;
                if (timeNow - tick >= 1)
				{
					tick++;
					fps = count;
					count = 0;
				}
                if (frm.empty()) {
                    isMainRunning = false;
                }
                is_frame_prepare = true;
            }
            mtx.unlock();
        }
    }

}

void detect_and_display()
{
	Mat frame,buf;	
    CascadeClassifier face_cascade;
	face_cascade.load("/home/pi/Documents/opencv-master/data/haarcascades/haarcascade_frontalface_default.xml");
	
    bool isExist;
    int stell;
    int x,y;
    time_t timeBegin = time(0);
    
    while (isMainRunning)
    {
        isExist = false;

        mtx.lock();
        if (is_frame_prepare) {
            cout <<"get frame\n";
            isExist = true;
            //cvtColor(frm,frame,COLOR_BGR2GRAY);
            frm.copyTo(frame);
            is_frame_prepare = false;
        }
        mtx.unlock();

        if (isExist)
        {       
			
            vector<Rect> faces;
			face_cascade.detectMultiScale(frame, faces, 1.1, 2, 0 | CV_HAL_CMP_GE, Size(30, 30));
	
			for (int i = 0; i < faces.size(); i++)
			{			
				Point  center(faces[i].x + faces[i].width*0.5,faces[i].y + faces[i].height*0.5);   
				circle(frame, center , 0.5, Scalar(0, 255, 0), 1, 8, 0);        
				x = faces[i].x + faces[i].width*0.5;
				y = faces[i].y + faces[i].height*0.5;		
			}			
			resize(frame,buf,Size(),3.6,3.3);
			putText(buf, cv::format("FPS=%d", fps ), Point(15, 30), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0,255,0));
			
            imshow(window_name, buf);
            GoStep(x,y);
            c = waitKey(1);
            if( (char)c == 'c' ) {
                break;
            }

        } else {
            cout <<"sleep\n";
            std::this_thread::sleep_for (std::chrono::milliseconds(1));
        }
    }
}


int main()
{
    is_frame_prepare = false;
    isMainRunning = true;
    thread cap(run_capture);
    thread dis(detect_and_display);
    cap.join();
    dis.join();
    return 0;
}
