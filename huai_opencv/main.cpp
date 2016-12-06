#include "Classifi.h"




int main()
{
	Mat image = imread("6_1_30.JPG");


	Classifi *example = new Classifi(image);
	int testTem[5];
	testTem[0] = 13;
	testTem[1] = 16;
	testTem[2] = 9;
	testTem[3] = 11;
	testTem[4] = 6;
	
	imwrite("6_1_30_Rad.JPG", example->createRadImg(testTem));
	return 0;
}