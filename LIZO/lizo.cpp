#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main()
{
 CascadeClassifier face_cascade;
 face_cascade.load("/home/pi/Documents/opencv-master/data/haarcascades/haarcascade_frontalface_default.xml");

 Mat img;
 VideoCapture cap(0);
 cap.set(CAP_PROP_FRAME_WIDTH, 160);
 cap.set(CAP_PROP_FRAME_HEIGHT, 120);  
 cap.set(CAP_PROP_EXPOSURE, 0);
 int frameCounter = 0;
 int tick = 0;
 int fps;
 time_t timeBegin = time(0);
 time_t timeNow;
 
	while (true)
	{
	 
	 cap >> img;
	 cap.read(img);
	 frameCounter++;
     timeNow = time(0) - timeBegin;
     if (timeNow - tick >= 1)
        {
            tick++;
            fps = frameCounter;
            frameCounter = 0;
        }
     putText(img, format("FPS=%d", fps ), Point(5, 15), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0,255,0));
    
     
	 vector<Rect> faces;
	 face_cascade.detectMultiScale(img, faces, 1.1, 2, 0 | CV_HAL_CMP_GE, Size(30, 30));

	 // Draw circles on the detected faces
	 for (int i = 0; i < faces.size(); i++)
	 {
	 Point  pt1(faces[i].x,faces[i].y);
	 Point  pt2(faces[i].x + faces[i].width,faces[i].y + faces[i].height);
	 rectangle(img, pt1, pt2, Scalar(0,250,0));
	 putText(img, format("x=%d", faces[i].width ), Point(5, 30), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0,255,0));
	 putText(img, format("y=%d", faces[i].height ), Point(5, 45), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0,255,0));
	 putText(img, format("r=%d", 16100 / faces[i].height), Point(5, 60), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0,255,0));
	 }
	  
	 imshow("Detected Face", img);
	 waitKey(1);
	}
 return 0;
}                            
