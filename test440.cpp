#include<opencv2/opencv.hpp>
#include<quickopencv.h>
#include<iostream>
#include<vector>

using namespace std;
using namespace cv;

int main(int argc, char** argv)
{
	//如何加载一个图像
	Mat src = imread("C:\\Users\\陈焕新\\Pictures\\try.jpg");
	//IMREAD_COLOR彩色
	//IMREAD_GRAYSCALE灰色
	//读入图像：地址，Mat矩阵，图像本身是矩阵,二维数组
	//地址要么向右两个斜杆，或者一个向左的

	if (src.empty())//如果图片加载为空，判断图片
	{
		printf("could not load image...");
		return -1;
	}

	//namedWindow("输入窗口", WINDOW_FREERATIO);
	//创建一个自由窗口且比例自由
	//imshow("输入窗口", src);
	//显示图像

	QuickDemo qd;
	//qd.colorSpace_Demo(src);

    //qd.mat_creation_Demo(/*src*/);

	//qd.pixel_visit_demo(src);

	//qd.operators_demo(src);

	//qd.tracking_bar_demo(src);

	//qd.key_demo(src);

	//qd.color_style_demo(src);

	//qd.bitwise_demo(src)

	//qd.channels_demo(src);

	//qd.inrange_demo(src);

	//qd.pixel_statistic_demo(src);

	//qd.drawing_demo(src);

	//qd.random_drawing();

	//qd.polyline_drawing_demo();

	//qd.mouse_drawing_demo(src);

	//qd.norm_demo(src);

	//qd.resize_demo(src);

	//qd.flip_demo(src);

	//qd.rotate_demo(src);

	//qd.video_demo(src);

	//qd.histogram_demo(src);

	//qd.histogram_2d_demo(src);

	//qd.histogram_eq_demo(src);

	//qd.blur_demo(src);

	//qd.gaussian_blur_demo(src);

    //qd.bifilter_demo(src);

	qd.face_detection_demo();

	waitKey(0);//停顿几秒，继续往下执行，300ms才能辨别出来
	destroyAllWindows();//销毁所有创建的窗口
	return 0;
}