//////////////////////////////////////////////
///////  รวมมมมมม     ////////////////////////////////
/////////////////////////////////////////////




#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/video/background_segm.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <stdio.h>

#include <iostream>
#include <vector>

using namespace std;
using namespace cv;

//const string trackbarWindowName = "Trackbars";

//const string windowName = "Original Image ภาพธรรมดา";

//const string windowName2 = "Thresholded Image ภาพ ขาวดำ";

void on_trackbar( int, void* ){}
string intToString(int number){

std::stringstream ss;
ss << number;
return ss.str();
}

int main(int argc, char* argv[]){

	VideoCapture cap(1);

	    if ( !cap.isOpened() )
	    {
	         cout << "ใส่กล้องผิด" << endl;
	         return -1;
	    }
	    namedWindow("Control", CV_WINDOW_AUTOSIZE); //create a window called "Control"

	     int iLowH = 56;
	    int iHighH = 193;
	   int iLowS = 10;
	    int iHighS = 245;
	   int iLowV = 229;
	    int iHighV = 249;


	    createTrackbar("LowH", "Control", &iLowH, 255); //Hue (0 - 179)
	     createTrackbar("HighH", "Control", &iHighH, 255);

	      createTrackbar("LowS", "Control", &iLowS, 255); //Saturation (0 - 255)
	     createTrackbar("HighS", "Control", &iHighS, 255);

	      createTrackbar("LowV", "Control", &iLowV, 255);//Value (0 - 255)
	     createTrackbar("HighV", "Control", &iHighV, 255);
VideoCapture capture(1);

//bool update_bg_model = true;


cv::BackgroundSubtractorMOG2 bg;//(100, 3, 0.3, 5);
bg.set ("nmixtures", 200);
std::vector < std::vector < cv::Point > >contours;

//cv::namedWindow ("Frame");
//cv::namedWindow ("Background");

Mat frame, fgmask, fgimg, backgroundImage,bw;

Mat threshold2;
Mat HSV,bw2;
Mat threshold;

capture.open(1);

int DLastX = -1;
int DLastY = -1;
int CLastX = -1;
 int CLastY = -1;
Mat imgTmp;
capture.read(imgTmp);
Mat DimgLines = Mat::zeros( imgTmp.size(), CV_8UC3 );;
Mat CimgTmp;
 cap.read(CimgTmp);
 Mat CimgLines = Mat::zeros( CimgTmp.size(), CV_8UC3 );;

/////**********************************

 ////////***************************
while(1){
capture >> frame;
///////////////////////////////////////////////////////////////////////////////////
Mat imgOriginal;
bool bSuccess = cap.read(imgOriginal); // read a new frame from video
if (!bSuccess){ //if not success, break loop

   cout << "Cannot read a frame from video stream" << endl;
             break;
        }

    Mat imgHSV;

   cvtColor(imgOriginal, imgHSV, COLOR_BGR2HSV); //Convert the captured frame from BGR to HSV

  Mat imgThresholded;

   inRange(imgHSV, Scalar(iLowH, iLowS, iLowV), Scalar(iHighH, iHighS, iHighV), imgThresholded); //Threshold the image
  erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
  dilate( imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
  dilate( imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
  erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );

   //Calculate the moments of the thresholded image
  Moments CoMoments = moments(imgThresholded);
  /////////////////////////////////////////////////////////////////////////////

//////////////////////////////////จับ การเคลื่อนไหว//////////////////////////////
bg.operator()(frame, fgimg);
bg.getBackgroundImage (backgroundImage);
erode (fgimg, fgimg, Mat ());

findContours (fgimg, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
drawContours (frame, contours, -2, Scalar (0, 0, 255),1);

//////////////////////////////////จับสี//////////////////////////////
cvtColor(frame,HSV,COLOR_BGR2HSV);
inRange(HSV,Scalar(61,0,124),Scalar(124,75,255),threshold);
/////////////////////นำจุดสีแดงมาแสดง///////////////////
inRange(frame, Scalar(0, 0, 255), Scalar(0, 0, 255), bw);

////////////////

Moments DoMoments = moments(bw);

double DdM01 = DoMoments.m01;
double DdM10 = DoMoments.m10;
double DdArea = DoMoments.m00;
double CdM01 = CoMoments.m01;
  double CdM10 = CoMoments.m10;
  double CdArea = CoMoments.m00;

  /*
///////////////--------------------------------------------------------------/////////////////////


  if (CdArea > 50000 &&CdArea<100000)  //---------------------------------ของสี
    {
     //calculate the position of the ball
     int CposX = CdM10 / CdArea;
     int CposY = CdM01 / CdArea;


     if (CLastX >= 0 && CLastY >= 0 && CposX >= 0 && CposY >= 0)
     {
      //Draw a red line from the previous point to the current point
      line(CimgLines, Point(CposX, CposY), Point(CLastX, CLastY), Scalar(0,0,255), 2);
      //cout << " X " << iLastX;
      //cout << " Y " << iLastY<< endl;
      cout<<" DDDDDD";cout<<" C-X"<<CposX;
      cout<<" C-Y "<<CposY<< endl;


     }

      CLastX = CposX;
     CLastY = CposY;

    }

  ////////////////////
  if (DdArea > 50000&&DdArea < 100000)  ////////---------------------------ของภาพเคลื่อนไหว
{

int DposX = DdM10 / DdArea;
int DposY = DdM01 / DdArea;
//int CposX = CdM10 / CdArea;
//int CposY = CdM01 / CdArea;
//cout<<" D-X"<<DposX;
//cout<<" D-Y "<<DposY<< endl;
//cout<<" C-X"<<CposX;
//cout<<" C-Y "<<CposY<< endl;
//if (DLastX ==CiLastX && DLastY ==CiLastY && DposX ==CposX && DposY ==CposY)
if (DLastX >= 0 && DLastY >= 0 && DposX >= 0 && DposY >= 0)
{
line(DimgLines, Point(DposX, DposY), Point(DLastX, DLastY), Scalar(0,0,255), 2);
cout<<" CCCCCC";cout<<" D-X"<<DposX;
cout<<" D-Y "<<DposY<< endl;
//cout<<" C-X"<<CposX;
//cout<<" C-Y "<<CposY<< endl;

}

DLastX = DposX;
DLastY = DposY;
//CLastX = CposX;
//CLastY = CposY;
}
  */
/////////////////////////////---------------------------------/////////////////////////////////
  /////////////////////-----------รวม------------------//////////////////////////////
 /* if (DdArea >1047310.7){
  	  cout<<" 10 เซนติเมตร"<<endl;
    }

    if (DdArea >958087.6&& DdArea< 1047309){
    	  cout<<" 20 เซนติเมตร"<<endl;
      }
    if (DdArea >766635&& DdArea< 958086){
    	  cout<<" 40 เซนติเมตร"<<endl;
      }
    if (DdArea >718316&& DdArea< 766634){
    	  cout<<" 60 เซนติเมตร"<<endl;
      }
    if (DdArea >519932.1&& DdArea< 718315){
    	  cout<<" 80 เซนติเมตร"<<endl;
      }
    if (DdArea >353923.7&& DdArea< 519931){
    	  cout<<" 100 เซนติเมตร"<<endl;
      }
    if (DdArea >171125.3&& DdArea< 353922){
    	  cout<<" 120 เซนติเมตร "<<endl;
      }
    if (DdArea >138792.3&& DdArea< 171124){
    	  cout<<" 140 เซนติเมตร"<<endl;
      }

    if (DdArea >72753.2&& DdArea< 138791){
    	  cout<<" 160 เซนติเมตร"<<endl;
      }
    if (DdArea >43287.7&& DdArea< 72753){
      	  cout<<" 180 เซนติเมตร"<<endl;
        }
    if (DdArea >0&& DdArea< 43287){
        	  cout<<" 200 เซนติเมตร"<<endl;
          }


*/
 // cout<<" จำนวณจุด สี "<<CdArea<<"\n"<< endl;
  //cout<<" จำนวณจุด เคลื่อนไหว "<<DdArea<<"\n"<< endl;
  if (CdArea >5000&&DdArea > 10000&&DdArea < 600000)
  {

	  int CposX = CdM10 / CdArea;
	  int CposY = CdM01 / CdArea;
	  int DposX = DdM10 / DdArea;
	  int DposY = DdM01 / DdArea;

	  //////////-----แบ่ง ตาราง 10ช่อง -------///////



	  //////////-----แบ่ง ตาราง 10ช่อง -------///////
	  	    if(CposX>=0&&CposX<=130&&CposY>=0&&CposY<=250&&DposX>=0&&DposX<=130&&DposY>=0&&DposY<=250){
	  	    	//cout<<" X"<<CposX;
	  	    	//cout<<" Y "<<CposY<<endl;
	  	    	circle( imgOriginal, Point(CposX, CposY), 50, Scalar(0,0,255), -1, 8, 10 );
	  	    	cout<<" ช่องที่ 1 "<< endl;
	  	    }
	  	    if(CposX>130&&CposX<=260&&CposY>=0&&CposY<=250&&DposX>130&&DposX<=260&&DposY>=0&&DposY<=250){
	  	    	//cout<<" X"<<CposX;
	  	    	//cout<<" Y "<<CposY<<endl;
	  	    	circle( imgOriginal, Point(CposX, CposY), 50, Scalar(0,0,255), -1, 8, 10 );
	  	    	cout<<" ช่องที่ 2 "<< endl;
	  	    }
	  	    if(CposX>260&&CposX<=390&&CposY>=0&&CposY<=250&&DposX>260&&DposX<=390&&DposY>=0&&DposY<=250){
	  	    	//cout<<" X"<<CposX;
	  	    	//cout<<" Y "<<CposY<<endl;
	  	    	circle( imgOriginal, Point(CposX, CposY), 50, Scalar(0,0,255), -1, 8, 10 );
	  	    	cout<<" ช่องที่ 3 "<< endl;
	  	    }
	  	    if(CposX>390&&CposX<=520&&CposY>=0&&CposY<=250&&DposX>390&&DposX<=520&&DposY>=0&&DposY<=250){
	  	    	//cout<<" X"<<CposX;
	  	    	//cout<<" Y "<<CposY<<endl;
	  	    	circle( imgOriginal, Point(CposX, CposY), 50, Scalar(0,0,255), -1, 8, 10 );
	  	    	cout<<" ช่องที่ 4 "<< endl;
	  	    }
	  	    if(CposX>520&&CposX<=650&&CposY>=0&&CposY<=250&&DposX>520&&DposX<=650&&DposY>=0&&DposY<=250){
	  	    	//cout<<" X"<<CposX;
	  	    	//cout<<" Y "<<CposY<<endl;
	  	    	circle( imgOriginal, Point(CposX, CposY), 50, Scalar(0,0,255), -1, 8, 10 );
	  	    	cout<<" ช่องที่ 5 "<< endl;
	  	    }
	  	    ////-/-/-/-/-/-
	  	    if(CposX>=0&&CposX<=130&&CposY>250&&CposY<=500&&DposX>=0&&DposX<=130&&DposY>250&&DposY<=500){
	  	    	//cout<<" X"<<CposX;
	  	    	//cout<<" Y "<<CposY<<endl;
	  	    	circle( imgOriginal, Point(CposX, CposY), 50, Scalar(0,0,255), -1, 8, 10 );
	  	    	cout<<" ช่องที่ 6 "<< endl;
	  	    }
	  	    if(CposX>130&&CposX<=260&&CposY>250&&CposY<=500&&DposX>130&&DposX<=260&&DposY>250&&DposY<=500){
	  	    	//cout<<" X"<<CposX;
	  	    	//cout<<" Y "<<CposY<<endl;
	  	    	circle( imgOriginal, Point(CposX, CposY), 50, Scalar(0,0,255), -1, 8, 10 );
	  	    	cout<<" ช่องที่ 7 "<< endl;
	  	    }
	  	    if(CposX>260&&CposX<=390&&CposY>250&&CposY<=500&&DposX>260&&DposX<=390&&DposY>250&&DposY<=500){
	  	    	//cout<<" X"<<CposX;
	  	    	//cout<<" Y "<<CposY<<endl;
	  	    	circle( imgOriginal, Point(CposX, CposY), 50, Scalar(0,0,255), -1, 8, 10 );
	  	    	cout<<" ช่องที่ 8 "<< endl;
	  	    }
	  	    if(CposX>390&&CposX<=520&&CposY>250&&CposY<=500&&DposX>390&&DposX<=520&&DposY>250&&DposY<=500){
	  	    	//cout<<" X"<<CposX;
	  	    	//cout<<" Y "<<CposY<<endl;
	  	    	circle( imgOriginal, Point(CposX, CposY), 50, Scalar(0,0,255), -1, 8, 10 );
	  	    	cout<<" ช่องที่ 9 "<< endl;
	  	    }
	  	    if(CposX>520&&CposX<=650&&CposY>250&&CposY<=500&&DposX>520&&DposX<=650&&DposY>250&&DposY<=500){
	  	    	//cout<<" X"<<CposX;
	  	    	//cout<<" Y "<<CposY<<endl;
	  	    	circle( imgOriginal, Point(CposX, CposY), 50, Scalar(0,0,255), -1, 8, 10 );
	  	    	cout<<" ช่องที่ 10 "<< endl;

	  	    }




	    //////////////---------------------------------------/


  }



  ////////////////////-----------จบรวม------------------//////////////////////////////



///////////////////////ส่วนเเสดงสี
//imshow(windowName2,threshold);
imshow("จุดเคลื่อนไหว",bw);
//imshow ("Frame", imgOriginal);
imshow("จุดสี", imgThresholded); //show the thresholded image

   imgOriginal = imgOriginal + CimgLines;
  imshow("Original", imgOriginal);

        if (waitKey(30) == 27){cout << "esc key is pressed by user" << endl;
            break;
        }
}

return 0;
}
