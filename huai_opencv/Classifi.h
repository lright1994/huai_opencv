#include <cv.h>
#include <highgui.h>
//#include "afx.h"

//#define LUODI_R 175
//#define LVDI_R 134
//#define LUOLIN_R 110
//#define LVLIN_R 76
//#define SHUITI_R 30
#define LUODI_R 180
#define LVDI_R 130
#define LUOLIN_R 100
#define LVLIN_R 70
#define SHUITI_R 30
#define TOLERANCE_SCOPE 10


using namespace cv;

class Classifi
{
public:
	Classifi(Mat image)
	{
		this->originalImg = image.clone();
		//pcvMemStorage = cvCreateMemStorage();
	}
	~Classifi()
	{

	}
public:
	Mat getImg(char* name)
	{
		if (name=="luodi")
		{
			return luodiImg;
		}
		else if (name == "lvdi")
		{
			return lvdiImg;
		}
		else if (name == "luolin")
		{
			return luolinImg;
		}
		else if (name == "lvlin")
		{
			return lvlinImg;
		}
		else if (name == "shuiti")
		{
			return shuitiImg;
		}
		else if (name=="rad")
		{
			return radImg;
		}
	}

	bool compareEqual(int a, int b,int scope)
	{
		if (abs(a - b) <= scope)
		{
			return true;
		}
		else{ return false; }
	}

	Mat classifiContour(int r, int scope, vector<vector<Point>> &contours)
	{
		Mat imgTem = originalImg.clone();

		for (int i = 0; i < imgTem.rows; i++)
		{
			for (int j = 0; j < imgTem.cols; j++)
			{
				if (compareEqual(imgTem.at<Vec3b>(i, j)[2], r, scope))
				{
					imgTem.at<Vec3b>(i, j)[0] = 0;
					imgTem.at<Vec3b>(i, j)[1] = 0;
					imgTem.at<Vec3b>(i, j)[2] = 0;
				}
				else
				{
					imgTem.at<Vec3b>(i, j)[0] = 255;
					imgTem.at<Vec3b>(i, j)[1] = 255;
					imgTem.at<Vec3b>(i, j)[2] = 255;
				}
			}
		}
			Mat grayTem;
			Mat edgeTem;
			Mat binTem;
			cvtColor(imgTem, grayTem, CV_BGR2GRAY);
			blur(grayTem, edgeTem, Size(3, 3));
			threshold(grayTem, binTem, 10, 255, THRESH_BINARY_INV);
			imwrite("test.jpg", binTem);
			findContours(binTem, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
			//findContours(binTem, pcvMemStorage, &pcvSeq[0], sizeof(CvContour), CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE,Point(0,0));

		return binTem;
	}

	Mat createRadImg(int* temperature)
	{
		classifiContour(LUODI_R,3*TOLERANCE_SCOPE,luodiC);
		classifiContour(LVDI_R, 2*TOLERANCE_SCOPE,lvdiC);
		classifiContour(LUOLIN_R, TOLERANCE_SCOPE,luolinC);
		classifiContour(LVLIN_R, 2*TOLERANCE_SCOPE,lvlinC);
		classifiContour(SHUITI_R, 2*TOLERANCE_SCOPE,shuitiC);

		int radValue[5];
		for (int i = 0; i < 5; i++)
		{
			radValue[i] =(temperature[i]+20)*255/60;
		}

		Mat radPart=originalImg.clone();
		drawContours(radPart, luodiC, -1, Scalar(0,0,radValue[0]), -1);
		drawContours(radPart, lvdiC, -1, Scalar(0,0, radValue[1]), -1);
		drawContours(radPart, luolinC, -1, Scalar(0,0, radValue[2]), -1);
		drawContours(radPart, lvlinC, -1, Scalar(0, 0, radValue[3]), -1);
		drawContours(radPart, shuitiC, -1, Scalar(0, 0, radValue[4]), -1);


		addWeighted(radPart, 0.4, originalImg, 0.6, 0.0, radImg);

		return radImg;
	}

private:
	Mat originalImg;
	Mat luodiImg;
	Mat lvdiImg;
	Mat luolinImg;
	Mat lvlinImg;
	Mat shuitiImg;
	Mat radImg;
	//CvMemStorage pcvMemStorage;
	//CvSeq *pcvSeq[5];

	vector<vector<Point>> luodiC;
	vector<vector<Point>> lvdiC;
	vector<vector<Point>> luolinC;
	vector<vector<Point>> lvlinC;
	vector<vector<Point>> shuitiC;
};