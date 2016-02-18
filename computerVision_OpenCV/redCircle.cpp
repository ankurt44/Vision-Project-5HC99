
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

using namespace cv;
using namespace std;

Mat src; Mat src_gray; Mat src_hsv;
int radiuscircle ;

/**
 * @function main
 */
int main( int, char** argv )
{
  /// Load source image and convert it to gray
  src = imread( argv[1], 1 );

  /// Convert image to gray and blur it
  cvtColor( src, src_gray, COLOR_BGR2GRAY );
  blur( src_gray, src_gray, Size(3,3) );
  cvtColor(src, src_hsv, COLOR_BGR2HSV);


  /// Create Window to show the source image
  const char* source_window = "Source";
  namedWindow( source_window, WINDOW_AUTOSIZE );
  imshow( source_window, src );
  waitKey(1000);
  imshow(source_window, src_hsv);
  waitKey(1000);
  Mat lower_red_hue_range;
  Mat upper_red_hue_range;
 inRange(src_hsv, Scalar(0, 100, 100), Scalar(10, 255, 255), lower_red_hue_range);
 inRange(src_hsv, Scalar(160, 100, 100), Scalar(179, 255, 255), upper_red_hue_range);
  
  imshow( source_window, lower_red_hue_range );
  waitKey(1000);
  imshow(source_window, upper_red_hue_range);	
  
  
  Mat red_hue_image;
  addWeighted(lower_red_hue_range, 1.0, upper_red_hue_range, 1.0, 0.0, red_hue_image);
  GaussianBlur(red_hue_image, red_hue_image, Size(9, 9), 2, 2);
  vector<Vec3f> circles;
  HoughCircles(red_hue_image, circles, CV_HOUGH_GRADIENT, 1, red_hue_image.rows/2, 100, 20, 0, 0);
  imshow(source_window,red_hue_image);
  waitKey(1000);

  if(circles.size() == 0) exit(-1);
for(size_t current_circle = 0; current_circle < circles.size(); ++current_circle)
{
	Point center(cvRound(circles[current_circle][0]), cvRound(circles[current_circle][1]));
	int radius = cvRound(circles[current_circle][2]);
	circle(src, center, radius, Scalar(0, 255, 0), 5);
	radiuscircle = cvRound(circles[current_circle][2]);
	 imshow(source_window,src);
	 waitKey(1000);
}
	int size_of_marker=130; int focallength=30; int distance_to_camera=0;
	distance_to_camera = ((size_of_marker*focallength)/radiuscircle);
	cout<< "Markers Distance"<< distance_to_camera << endl;
	
	waitKey(0);
	return(0);
}
