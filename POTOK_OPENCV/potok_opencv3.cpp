#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <opencv2/opencv.hpp>
#include <iostream>
#include <cmath>
#include <thread>
#include <mutex>
#include <wiringPi.h>
#include <cstdio>
using namespace std;
using namespace cv;
//drv8825
#define LED6 0
#define  LED10 2
//a4898
#define  LED4 4
#define LED5 5



Mat frm;
mutex mtx;
vector<Rect> faces;
int c=0;
int fps = 0;
int xb = 80;
int yb = 60;
int x,y;
double step1, step2;
double a1,a2,vx;
bool isMainRunning;
bool is_frame_prepare;
bool cordinatiions_ready;
bool dir1, dir2;



string window_name = "Video";

void Go_Step(){
		while(isMainRunning){
			if(cordinatiions_ready == true){
				vx = 0 ;
				if (x < xb && y < yb){ //относительно прошлой переменной
				dir1 = 1;
				dir2 = 1;
				a1 = 4.2 * x / 80;
				a1 = 4.2 - a1 ;
				step1 = a1 * 2048 / 360;
				a2 = 3.15 * y / 60;
				a2 = 3.15 - a2;
				step2 = a2 * 2048 / 360 ; 
				vx = 1;
				}
				if (x > xb && y <= yb){
				dir1 = 0;
				dir2 = 1;
				a1 = 4.2 * x /80;
				a1 = a1 - 4.2;
				step1 = a1 * 2048 / 360 ;
				a2 = 3.15 * y / 60;
				a2 = 3.15 - a2;
				step2 = a2 * 2048 / 360 ; 
				vx = 1;
				}
				if (x <= xb && y > yb){
				dir1 = 1;
				dir2 = 0; 
				a1 = 4.2 * x / 80;
				a1 = 4.2 - a1 ;
				step1 = a1 * 2048 / 360;
				a2 = 3.15 * y / 60;
				a2 = a2 - 3.15;
				step2 = a2 * 2048 / 360 ;
				vx = 1;
				}
				if (x > xb && y > yb){
				dir1 = 0;
				dir2 = 0; 
				a1 = 4.2 * x / 80;
				a1 = a1 - 4.2 ;
				step1 = a1 * 2048 / 360;
				a2 = 3.15 * y / 60;
				a2 = a2 - 3.15;
				step2 = a2 * 2048 / 360 ;
				vx = 1;
				}
				cout << step1 << " //" << step2 << endl;
				if (vx == 1){
				digitalWrite (LED5, dir1);
				digitalWrite (LED10, dir2);
					if (abs(step1 / 2)  <= abs(step2)) {
						int ot = int (round(step2 / (step1 / 2)));
						for (int i = 0 ; i <step2 ; i++){
							if (i % ot  == 0){
								digitalWrite (LED4, 1);
							}
							digitalWrite (LED6, 1);
							delayMicroseconds(1000);
							if (i % ot == 0){
								digitalWrite (LED4, 0);
							} 
							digitalWrite (LED6, 0);
							delayMicroseconds(1000);
							cout << i <<endl;
						}
					}
					if (abs(step1 / 2)  > abs(step2)){
						int ot = int (round((step1 / 2) / step2)) ;
						for (int i = 0 ; i <step1 ; i++){
							if (i % ot == 0){
								digitalWrite (LED6, 1);
							}
							digitalWrite (LED4, 1);
							delayMicroseconds(1000);
							if (i % ot == 0){
								digitalWrite (LED6, 0);
							} 
							digitalWrite (LED4, 0);
							delayMicroseconds(1000);
							cout << i << endl;
						}
					}
				a1 = 0;
				a2 = 0;
				step1 = 0;
				step2 = 0;	
				vx = 0 ;
				xb = x;
				yb = y;
				cordinatiions_ready = false;
				}
			}
		}		
}

void return_x(){
	
}
void run_capture()
{
    VideoCapture capture(0);
    capture.set(CAP_PROP_FRAME_WIDTH, w);
    capture.set(CAP_PROP_FRAME_HEIGHT, h);  
   
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
			
			face_cascade.detectMultiScale(frame, faces, 1.1, 2, 0 | CV_HAL_CMP_GE, Size(30, 30));
			cordinatiions_ready = true;
			for (int i = 0; i < faces.size(); i++)
			{			
				Point  center(faces[i].x + faces[i].width*0.5,faces[i].y + faces[i].height*0.5);   
				circle(frame, center , 0.5, Scalar(0, 255, 0), 1, 8, 0);        
				x = faces[i].x + faces[i].width*0.5;
				y = faces[i].y + faces[i].height*0.5;	
				cordinatiions_ready = true;
			}			
			resize(frame,buf,Size(),3.6,3.3);
			putText(buf, cv::format("FPS=%d", fps ), Point(15, 30), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0,255,0));
			
            imshow(window_name, buf);
            
				
			
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
	xb = 80;
	yb = 60;
	wiringPiSetup () ; //инициализируем библиотеку 
	pinMode (LED6, OUTPUT) ; //step
	pinMode (LED10, OUTPUT) ; //dir
	pinMode (LED4, OUTPUT) ; 
	pinMode (LED5, OUTPUT) ; 
    is_frame_prepare = false;
    isMainRunning = true;
    cordinatiions_ready = false;
    thread cap(run_capture);
    thread dis(detect_and_display);
    thread shag(Go_Step);
    cap.join();
    dis.join();
    shag.join();
    return 0;
}

