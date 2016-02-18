#include "opencv2/opencv.hpp"
#include<iostream>
//#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
using namespace cv;
using namespace std;

Mat src, srcHSV, srcGRAY;

vector<Vec3f> preCircle;


int main(int, char**)
{
    int radiuscircle;
    VideoCapture cap(0); // open the default camera
    cout << "started" << endl; 
    if(!cap.isOpened()) { // check if we succeeded
        return -1;
    }
    //cvWaitKey(0);
    const char* sourceWin = "videocap";
    Mat edges;
    namedWindow(sourceWin,1);
    for(;;)
    {
        cap >> src; // get a new frame from camera
        
        //imshow(sourceWin, src);
        //waitKey(1000);
        
        cvtColor(src, srcHSV, COLOR_BGR2HSV);

        Mat lowerRedHueRange;
        Mat upperRedHueRange;
        inRange(srcHSV, Scalar(0,100,100), Scalar(10,255,255),lowerRedHueRange);
        inRange(srcHSV, Scalar(160,100,100), Scalar(179,255,255),upperRedHueRange);
        //add lower and upper red frames
        Mat redHueImage;
        addWeighted(lowerRedHueRange, 1.0, upperRedHueRange, 1.0, 0.0, redHueImage);
        //delete lowerRedHueRange;
        //delete upperRedHueRange;
        GaussianBlur(redHueImage, redHueImage, Size(9,9), 2, 2);
        //imshow(sourceWin, redHueImage);
        //waitKey(2000);
        vector<Vec3f> circles;
        HoughCircles(redHueImage, circles, CV_HOUGH_GRADIENT, 1, redHueImage.rows/2, 100, 20, 5, 200);

	    if(circles.size() == 0) {cout<<"No circles"<<endl;continue;}//exit(-1);
        for(size_t current_circle = 0; current_circle < circles.size(); ++current_circle)
        {
	        Point center(cvRound(circles[current_circle][0]), cvRound(circles[current_circle][1]));
	        int radius = cvRound(circles[current_circle][2]);
	        circle(src, center, radius, Scalar(0, 255, 0), 5);
            radiuscircle = cvRound(circles[current_circle][2]);
       //     cout << radius << endl;
        }

        imshow(sourceWin, src);
        int size_of_marker=130; int focallength=50; int distance_to_camera=0;
        distance_to_camera = ((size_of_marker*focallength)/radiuscircle);
        cout<< "Markers Distance"<< distance_to_camera << endl;

        if(waitKey(2000) >= 0) break;
        /////convert color - RGB to Gray
        //cvtColor(frame, edges, CV_BGR2GRAY);
        //GaussianBlur(edges, edges, Size(7,7), 1.5, 1.5);
        //Canny(edges, edges, 0, 30, 3);
        //imshow("edges", edges);
        if(waitKey(30) >= 0) break;
    }

    // the camera will be deinitialized automatically in VideoCapture destructor
    return 0;
}
