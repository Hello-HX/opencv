#include<opencv2/opencv.hpp>
#include<quickopencv.h>
#include<iostream>
#include<vector>

using namespace std;
using namespace cv;

int main(int argc, char** argv)
{
	//��μ���һ��ͼ��
	Mat src = imread("C:\\Users\\�»���\\Pictures\\try.jpg");
	//IMREAD_COLOR��ɫ
	//IMREAD_GRAYSCALE��ɫ
	//����ͼ�񣺵�ַ��Mat����ͼ�����Ǿ���,��ά����
	//��ַҪô��������б�ˣ�����һ�������

	if (src.empty())//���ͼƬ����Ϊ�գ��ж�ͼƬ
	{
		printf("could not load image...");
		return -1;
	}

	//namedWindow("���봰��", WINDOW_FREERATIO);
	//����һ�����ɴ����ұ�������
	//imshow("���봰��", src);
	//��ʾͼ��

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

	waitKey(0);//ͣ�ټ��룬��������ִ�У�300ms���ܱ�����
	destroyAllWindows();//�������д����Ĵ���
	return 0;
}