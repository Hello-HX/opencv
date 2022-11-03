#include <quickopencv.h>
#include <opencv2/dnn.hpp>
#include<vector>

using namespace cv;
using namespace std;

//ɫ�ʿռ�ת��
void QuickDemo::colorSpace_Demo(Mat& image) 
{
	Mat gray, hsv;
	cvtColor(image, hsv, COLOR_BGR2HSV);//ɫ����Ҷȣ�ͨ���ı䣬�����������÷���
	// H 0 ~ 180, S, V 
	//H��S��ɫ��V���ȣ���ͬ�ռ�ɫ�ʵ���ͬ�Ķ���
	cvtColor(image, gray, COLOR_BGR2GRAY);
	imshow("HSV", hsv);
	imshow("�Ҷ�", gray);
	imwrite("D:/hsv.png", hsv);//����ͼ��
	imwrite("D:/gray.png", gray);
}

//ͼ��Ĵ����븳ֵ
void QuickDemo::mat_creation_Demo( /*Mat& image*/)
{
	/*
	//��������ͼ�����
	Mat m1, m2;
	//�����ĸ�ֵ������ָ��ͬһ��ͼ��
	//ֻ�и��ƻ��߿�¡���Ǳ�ɱ��ͼ��
	m1 = image.clone();
	image.copyTo(m2);
	*/

	//�����հ�ͼ��
	//��ʼ��zeros����ones
	//Size�����
	//CV_8UC����ͨ����
	//CV_8UC1,�޷��ŵ�8λ��ͨ��
	Mat m3 = Mat::zeros(Size(128, 128), CV_8UC3);
	
	/*
	//�����򵥵ĸ�ֵֻ�Ե�һ��ͨ������
	m3 = 127;
	*/

	//ֻҪ֪������ͨ�����Ϳ�����Scalar���иı�
	m3 = Scalar(0, 0, 255);
	std::cout << "width:" << m3.cols << "\nheight:" << m3.rows << "\nchannels" << m3.channels() << std::endl;

	Mat m4;
	m3.copyTo(m4);
	m4 = Scalar(0, 255, 255);

	/*
	imshow("ͼƬ1", m1);
	
	imshow("ͼƬ2",m2);
	*/
	
	imshow("����ͼ��", m4);

	imshow("����ͼƬ3", m3);

	/*
	//���ͼ��Ŀ�ȸ߶�ͨ����
	std::cout << "width:" << m3.cols << "\nheight:" << m3.rows << "\nchannels" << m3.channels() << std::endl;
	std::cout << m3 << std::endl;//�Ǳ�Ҫ�������
	*/

}

//���ص�ķ���
void QuickDemo::pixel_visit_demo(Mat& image)
{
	int w = image.cols;//��
	int h = image.rows;//��
	int dims = image.channels();//ͨ����
	/*
	//��������ʵ�ַ������ص�
	for (int row = 0; row < h; row++)
	{
		for (int col = 0;col < w; col++)
		{
			if (dims == 1)//��ɫͼ��
			{
				//uchar�ֽ����ͣ����ص�����
				int pv = image.at<uchar>(row, col);
				//��ɫ
				//0-255
				image.at<uchar>(row, col) = 255 - pv;
			}
			if (dims == 3)//��ɫͼ��
			{
				//ר�Ż�ȡ
				//��ȡһ�������е�����ֵ
				Vec3b bgr = image.at<Vec3b>(row, col);
				image.at<Vec3b>(row,col)[0] = 255 - bgr[0];
				image.at<Vec3b>(row,col)[1] = 255 - bgr[1];
				image.at<Vec3b>(row,col)[2] = 255 - bgr[2];
			}
		}
	}
	*/


	//��ָ�����������ص�
	for (int row = 0; row < h; row++)
	{
		//����һ��ָ��ָ��ָ��̶�����
		uchar *current_row = image.ptr<uchar>(row);
		for (int col = 0; col < w; col++)
		{
			if (dims == 1)
			{
				int pv = *current_row;
				*current_row++ = 255 - pv;
			}

			if (dims == 3)
			{
				//��ֵ�����������һ��
				//++���ȸ�ֵ��ִ��
				*current_row++ = 255 - *current_row;
				*current_row++ = 255 - *current_row;
				*current_row++ = 255 - *current_row;
			}
		}
	}

	imshow("���ض�д��ʾ", image);
}

//�������
void QuickDemo::operators_demo(Mat& image)
{

	/*
	//ÿ��ͨ����50����������֮�䰵
	//ͬ��Ҳ���ԼӼ��˳����������
	//���˷����ػ�һ�����
	Mat dst;
	//dst = image + Scalar(50, 50, 50);
	Mat m = Mat::zeros(image.size(), image.type());
	m = Scalar(2, 2, 2);
	multiply(image, m, dst);
	//����ֱ�ӳ˷�Ŷ��
	//dst = image * m;
	*/

	//����һ��ͼ�񣬳�ʼ�������С��������ԭ������һ��
	Mat dst = Mat::zeros(image.size(), image.type());
	Mat m = Mat::zeros(image.size(), image.type());
	m = Scalar(50, 50, 50);
   
	/*
	//�ֶ�ģ��ӷ�
	int w = image.cols;
	int h = image.rows;
	int dims = image.channels();

	for (int row = 0; row < h; row++)
	{
		for (int col = 0; col < w; col++)
		{
			Vec3b p1 = image.at<Vec3b>(row, col);
			Vec3b p2 = m.at<Vec3b>(row, col);
			//saturate_cast<uchar>����ֵ�����жϣ���ֹ������������⣬ʹ��0-255��
			dst.at<Vec3b>(row, col)[0] = saturate_cast<uchar>(p1[0] + p2[0]);
			dst.at<Vec3b>(row, col)[1] = saturate_cast<uchar>(p1[1] + p2[1]);
			dst.at<Vec3b>(row, col)[2] = saturate_cast<uchar>(p1[2] + p2[2]);
		}
	}
	*/

	//���Ƕ�����opencv�Դ����㷨
	//add(a,b,ans);
	//subtract(a,b,ans);
	//multiple(a,b,ans);
	//divide(a,b,ans);

	imshow("�ӷ�����",dst);
}


//����������
// �Ȱ󶨺����callback
//static������ȫ
static void on_track(int b, void* userdate)
{
	//������void*ָ�룬��ΪMat*���ٷ�����ֵ
	Mat image = *((Mat*)userdate);
	//��ʼ��
	Mat dst = Mat::zeros(image.size(), image.type());
	Mat m = Mat::zeros(image.size(), image.type());
	m = Scalar(b, b, b);

	//������������
	add(image, m, dst);
	imshow("���ȵ���", dst);
}

void on_lightness(int b, void* userdata)
{
	Mat image = *((Mat*)userdata);
	Mat dst = Mat::zeros(image.size(), image.type());
	Mat m = Mat::zeros(image.size(), image.type());

	//addweighted���������Ҫdouble�͵ģ�Ҳ����Ϊʲô��1.0��0.0
	//addweighted��ͨ��Ȩ���ںϵ�
	//������Ҫͨ������b
	addWeighted(image, 1.0, m, 0, b, dst);

	imshow("������Աȶȵ���", dst);
}


void on_contrast(int b, void* userdata)
{
	Mat image = *((Mat*)userdata);
	Mat dst = Mat::zeros(image.size(), image.type());
	Mat m = Mat::zeros(image.size(), image.type());

	//�Աȶȵķ�ΧΪ0.0-3.0������Ҫ����
	//�Աȶ���Ҫͨ������contrast
	double contrast = b / 100.0;
	addWeighted(image, contrast, m, 0.0, 0, dst);

	imshow("������Աȶȵ���", dst);
}



void QuickDemo::tracking_bar_demo(Mat& image)
{
	/*
	//���ȵ���
	//����һ������
	namedWindow("���ȵ���",WINDOW_AUTOSIZE);
	int lightness = 50;
	int max_value = 100;
	int contrast_value = 100;

	//createTrackbar(�϶������֣���������&��ʼֵ����ֵ�����ֵ�����������������void*ͼƬԴ)
	createTrackbar("Value Bar:", "���ȵ���", &lightness, max_value, on_track, (void*)(&image));

	//��ʼ����չʾ
	on_track(50, &image);
	*/


	//���Աȶ������Ⱥ���һ��
	namedWindow("������Աȶȵ���", WINDOW_AUTOSIZE);
	int lightness = 50;
	int max_value = 100;
	int contrast_value = 100;

	createTrackbar("Value Bar:", "������Աȶȵ���", &lightness, max_value, on_lightness, (void*)(&image));
	createTrackbar("Contrast Bar:", "������Աȶȵ���", &contrast_value, 200, on_contrast, (void*)(&image));

	//���п���
	on_lightness(50, &image);
}

void QuickDemo::key_demo(Mat& image)
{
	Mat dst = Mat::zeros(image.size(), image.type());
	while (true)
	{
		//���ʱ���ȡ����ֵ
		//�ǵ����Ҫ����ͼ�񴰿�
		int c = waitKey(100);

		if (c == 27) { // �˳�
			break;
		}
		if (c == 49) { // Key #1
			std::cout << "you enter key # 1 " << std::endl;
			cvtColor(image, dst, COLOR_BGR2GRAY);
		}
		if (c == 50) { // Key #2
			std::cout << "you enter key # 2 " << std::endl;
			cvtColor(image, dst, COLOR_BGR2HSV);
		}
		if (c == 51) { // Key #3
			std::cout << "you enter key # 3 " << std::endl;
			dst = Scalar(50, 50, 50);
			add(image, dst, dst);
		}
		imshow("������Ӧ", dst);
	}
}


//�Դ���ɫ�����
void QuickDemo::color_style_demo(Mat& image)
{
	//�ȶ������еĹؼ���
	int colormap[] =
	{
		COLORMAP_AUTUMN,
		COLORMAP_BONE,
		COLORMAP_JET,
		COLORMAP_WINTER,
		COLORMAP_RAINBOW,
		COLORMAP_OCEAN,
		COLORMAP_SUMMER,
		COLORMAP_SPRING,
		COLORMAP_COOL,
		COLORMAP_PINK,
		COLORMAP_HOT,
		COLORMAP_PARULA,
		COLORMAP_MAGMA,
		COLORMAP_INFERNO,
		COLORMAP_PLASMA,
		COLORMAP_VIRIDIS,
		COLORMAP_CIVIDIS,
		COLORMAP_TWILIGHT,
		COLORMAP_TWILIGHT_SHIFTED
	};

	Mat dst;
	int index = 0;
	while (true) {
		//ͣ��or����ȡֵ������waitKey����
		int c = waitKey(500);
		if (c == 27) { // �˳�
			break;
		}
		//��̼��ɣ�%19����������19��
		//applycolorMap��ԭͼ������ͼ����ɫ��
		applyColorMap(image, dst, colormap[index % 19]);
		index++;
		imshow("��ɫ���", dst);
	}
}


//λ������������
void QuickDemo::bitwise_demo(Mat& image)
{
	Mat m1 = Mat::zeros(Size(256, 256), CV_8UC3);
	Mat m2 = Mat::zeros(Size(256, 256), CV_8UC3);

	//���������
	//����0�����ƣ�����0����䣨��ֵ��
	//��������亯��
	//LINE_AA����� ��
	//LINE_8 �����ͺ���(��Щ�������ͣ�Ĭ��ȡ8)
	//Rect���������꣬�������꣩
	//Scalar������ɫ
	//���һ��0��ʾС����λ��
	//rectangle(m1, Rect(100, 100, 80, 80), Scalar(255, 255, 0), 1, LINE_8,0);
	//rectangle(m2, Rect(100, 100, 80, 80), Scalar(0, 255, 255), -1, LINE_8,0);

	rectangle(m1, Rect(100, 100, 80, 80), Scalar(255, 255, 0), -1, LINE_8, 0);
	rectangle(m2, Rect(150, 150, 80, 80), Scalar(0, 255, 255), -1, LINE_8, 0);
	imshow("m1", m1);
	imshow("m2", m2);
	Mat dst;
	
	
	//λ����
	// 8��λ��Ϊ�����ƽ���λ����
	//bitwise_or(m1, m2, dst);
	//bitwise_xor(m1, m2, dst);
	//bitwise_and(m1, m2, dst);
	//bitwise_not(image,dst);ȡ��һ��ԭͼ��һ����ͼ��255-����=ȡ��

	imshow("����λ����",dst);
}

//ͨ��������ϲ�
void QuickDemo::channels_demo(Mat& image)
{
	
	std::vector<Mat> mv;
	//split(image,mv)Ŀ��������洢����
	split(image, mv);

	//�������Ϊ��ͨ��ͼ�����ֱ�Ϊ������ɫͨ���ĻҶ�ͼ
	imshow("��ɫ", mv[0]);
	imshow("��ɫ", mv[1]);
	imshow("��ɫ", mv[2]);

	//���������ɫ�ģ���ùص�����ͨ���ٺϲ�
	Mat dst;
	mv[0] = 0;
	mv[1] = 0;
	//merge(mv,dst)����������뱣������
	merge(mv, dst);
	//ע�ⴰ������ͬ�����Ļ��ڸ�ǰ�洰�ڵĶ���
	imshow("��ɫ", dst);

	int from_to[] = { 0, 2, 1,1, 2, 0 };
	//mixChannels(&������󣬾�������&������󣬾������������б�ԣ������б���)
	mixChannels(&image, 1, &dst, 1, from_to,3);
	imshow("ͨ�����", dst);
}

//ͼ��ɫ�ʿռ�ת��
void QuickDemo::inrange_demo(Mat& image)
{
    //���ʵ�ֿ�ͼ
	Mat hsv;
	//����ɫ�ռ�ת������һ���ռ䣬hsv�ø���
	cvtColor(image, hsv, COLOR_BGR2HSV);
	Mat mask; 
	//��ֵ���������ڷ�Χ��Ϊ��ɫ������Ϊ��ɫ
	//inRange(����,min,max�����)��
	//ȡֵʱ��ɫ����Χ
	//ȥ������ɫ
	inRange(hsv, Scalar(35, 43, 46), Scalar(77, 255, 255), mask);

	Mat redback = Mat::zeros(image.size(), image.type());
	redback = Scalar(40, 40, 200);//��ɫ����
	//�ѱ��������ı����Ӱױ�Ϊ�ڣ����˱�Ϊ��
	bitwise_not(mask, mask);
	imshow("mask", mask);
	//copyTo���������ã�һ���Ǹ��ƣ���һ�����ع������Ը���һ����Ĥ
	//��Ĥ����˼������ڸ�סͼƬ
	//��������õ�����image��mask�ڸ�ס��ͼ�񣬰�ɫ���ڸǣ���ɫ0��
	image.copyTo(redback, mask);//���˸���ȥ
	imshow("roi������ȡ", redback);
}

//ͼ������ֵ��ͳ��
void QuickDemo::pixel_statistic_demo(Mat& image)
{
	//����ֻ��ʶ0��1����������Ϊ�����õ���֪��
	double minv, maxv;//ͳ�������Сֵ
	Point minLoc, maxLoc;//�����Сֵ�ĵ�ַ,����
	std::vector<Mat> mv;
	split(image, mv);
	//mv.sizeΪ�����Ĵ�С����ʱΪ3
	for (int i = 0; i < mv.size(); i++)
	{
		//minMaxLocֻ֧�ֵ�ͨ��
		//minMaxLoc(ͨ����&��Сֵ��&���ֵ��&��С���꣬&������꣬��ѡ����Mat����)
		minMaxLoc(mv[i], &minv, &maxv,&minLoc, &maxLoc, Mat());
		std::cout << "No. channels:" << i << " min value:" << minv << " max value:" << maxv << std::endl;
	}
	Mat mean, stddev;

	//meanStdDev(�����������ֵ����׼���ѡmask)
	meanStdDev(image, mean, stddev);
	std::cout << "means:\n" << mean << std::endl;
	std::cout << "stddev:\n" << stddev << std::endl;
}

//ͼ�񼸺���״�ĵĻ���
void QuickDemo::drawing_demo(Mat& image)
{
	/*
	//����һ�����Σ�Rect��
	Rect rect;//����һ��ͼ��Ĺ��
	rect.x = 100;
	rect.y = 100;//������꣬���Ͻǣ����������½�
	rect.width = 250;
	rect.height = 3300;//�����߶�
	//Mat bg = Mat::zeros(image.size(), image.type());
	//�������Ի���Ҳ�������
	//������rectangle(����, Rect��, Scalar(255, 255, 0), 1, LINE_8,0);
	rectangle(image, rect, Scalar(0, 0, 255), 2, 8, 0);
	//��Բcircle(����, PointԲ��, �뾶, Scalar(255, 0, 0), -1, 8, 0);
	circle(image, Point(350, 400), 15, Scalar(255, 0, 0), -1, 8, 0);
	//����line(����, Point���, Point�յ�, Scalar(0, 255, 0), 4, LINE_AA, 0);
	line(image, Point(100, 100), Point(350, 400), Scalar(0, 255, 0), 4, LINE_AA, 0);
	imshow("������ʾ", image);
	*/

	Rect rect;
	rect.x = 100;
	rect.y = 100;
	rect.width = 250;
	rect.height = 300;
	Mat bg = Mat::zeros(image.size(), image.type());
	rectangle(bg, rect, Scalar(0, 0, 255), -1, 8, 0);
	circle(bg, Point(350, 400), 15, Scalar(255, 0, 0), -1, 8, 0);
	line(bg, Point(100, 100), Point(350, 400), Scalar(0, 255, 0), 4, LINE_AA, 0);

	//����һ����ת����
	RotatedRect rrt;
	rrt.center = Point(200, 200);//�ʵ�
	rrt.size = Size(100, 200);//��С
	rrt.angle = 90.0;//�Ƕ�
	//����Բellipse(����, ѡ�����RotatedRect, Scalar(0, 255, 255), 2, 8);
	ellipse(bg, rrt, Scalar(0, 255, 255), 2, 8);

	Mat dst;
	//�������ں�
	addWeighted(image, 0.7, bg, 0.3, 0, dst);
	imshow("������ʾ", dst);
}

//������������ɫ
void QuickDemo::random_drawing()
{
	Mat canvas = Mat::zeros(Size(512, 512), CV_8UC3);
	int w = canvas.cols;
	int h = canvas.rows;
	RNG rng(12345);
	while (true)
	{
		int c = waitKey(10);
		if (c == 27) { // �˳�
			break;
		}
		//rng.uniform(��㣬�����)֮��������
		int x1 = rng.uniform(0, w);
		int y1 = rng.uniform(0, h);
		int x2 = rng.uniform(0, w);
		int y2 = rng.uniform(0, h);
		int b = rng.uniform(0, 255);
		int g = rng.uniform(0, 255);
		int r = rng.uniform(0, 255);
		canvas = Scalar(0, 0, 0);
		line(canvas, Point(x1, y1), Point(x2, y2), Scalar(b, g, r), 3, LINE_AA, 0);
		imshow("���������ʾ", canvas);

	}
}

//���ƶ����
void QuickDemo::polyline_drawing_demo()
{
	//�����ջ���
	Mat canvas = Mat::zeros(Size(512, 512), CV_8UC3);
	int w = canvas.cols;
	int h = canvas.rows;
	//�輸����
	Point p1(100, 100);
	Point p2(300, 150);
	Point p3(300, 350);
	Point p4(250, 450);
	Point p5(50, 450);
	//����������������ȥ
	std::vector<Point> pts;
	pts.push_back(p1);
	pts.push_back(p2);
	pts.push_back(p3);
	pts.push_back(p3);
	pts.push_back(p4);
	pts.push_back(p5);
	//����εĻ���������Ƿֱ�����
	//polylines polylines(����, ����, true�ϲ�, Scalar(0, 255, 0), ��Ȼ��Ʋ�С��0, ����, λ��);
	polylines(canvas, pts, true, Scalar(0, 255, 0), 1, 8, 0);
	//fillPoly(����, ����, Scalar(0, 255, 0), ����, λ��);
	fillPoly(canvas, pts, Scalar(0, 255, 0), 8, 0);

	//��һ����ά����
	std::vector<std::vector<Point>> contours;
	contours.push_back(pts);
	//һ���Ի����е�ͼ
	//drawContours(����, ��ά�㼯, ��ͼ������ȫ0Ϊ1, Scalar(0, 0, 255), ��������, ����);
	drawContours(canvas, contours,0, Scalar(0, 0, 255), 8, 8);
	imshow("���ƶ����", canvas);
}


//������
//ȫ�ֱ�����¼һЩ״̬
//�ؼ��������ȡ�����ˢ��
Point sp(-1, -1);
Point ep(-1, -1);
Mat temp;
//���Ʒ�������static void on_draw(����¼�, ��ʼ����, ��ʼ����, ��ק�¼�, void* �ص�����)
static void on_draw(int event, int x, int y, int flags, void* userdata)
{
	Mat image = *((Mat*)userdata);//ת��

	if (event == EVENT_LBUTTONDOWN)//��¼��ʼ����
	{
		sp.x = x;
		sp.y = y;
		std::cout << "start point:" << sp << std::endl;
	}
	else if (event == EVENT_LBUTTONUP)//��¼�յ�����
	{
		ep.x = x;
		ep.y = y;
		int dx = ep.x - sp.x;
		int dy = ep.x - sp.x;
		if (dx > 0 && dy > 0)
		{
			Rect box(sp.x, sp.y, dx, dy);
			temp.copyTo(image);//����ԭ�ȹ켣
			imshow("ROI����", image(box));//��ȡroi���򷽷�image�����꣩
			rectangle(image, box, Scalar(0, 0, 255), 2, 8, 0);
			imshow("������", image);//���ϸ���ͼƬ
			// ready for next drawing���Ͻ�
			sp.x = -1;
			sp.y = -1;
		}
	}
	else if (event == EVENT_MOUSEMOVE)
	{
		if (sp.x > 0 && sp.y > 0)//��ֹ����ƶ�Ҳ��ͼ
		{
			ep.x = x;
			ep.y = y;
			int dx = ep.x - sp.x;
			int dy = ep.x - sp.x;
			if (dx > 0 && dy > 0)
			{
				Rect box(sp.x, sp.y, dx, dy);
				temp.copyTo(image);
				rectangle(image, box, Scalar(0, 0, 255), 2, 8, 0);
				imshow("������", image);//���ϸ���ͼƬ
			}
		}
	}
}


void QuickDemo::mouse_drawing_demo(Mat& image)
{
	namedWindow("������", WINDOW_AUTOSIZE);
	//setMouseCallback("���ƴ���", ���Ʒ�������, ��void*)(�û����ݸ��ص�����));
	setMouseCallback("������", on_draw, (void*)(&image));
	imshow("������", image);
	temp = image.clone();//��¡һ�ŷŵ�ȫ������ˢ��
}

//ͼ����������ת�����һ��
void QuickDemo::norm_demo(Mat& image)
{
	Mat dst;
	std::cout << image.type() << std::endl;
	//��ʽת������.convertTo������󣬸�ʽ��
	//CV_32F��32λ�ĸ�����
	//���û�и�ʽ�����㣬��һ��ȫ��0��һƬ��
	//��ȷ���������ͺ���Ҫ���ر��Ǻ����ѵ������
	//�������imshow��������������0��1֮��
	image.convertTo(image, CV_32F);
	std::cout << image.type() << std::endl;
	//��һ��normalize(���󣬴�������ޣ����ޣ���һ����ʽ);
	normalize(image, dst, 1.0, 0, NORM_MINMAX);
	std::cout << dst.type() << std::endl;
	imshow("ͼ�����ݹ�һ��", dst);
	// CV_8UC3, CV_32FC3,CV_32S
}

//ͼ��������ֵ
void QuickDemo::resize_demo(Mat& image)
{
	Mat zoomin, zoomout;
	int h = image.rows;
	int w = image.cols;
	imshow("ԭͼ", image);
	//resize(����, �����, Size(���), 0, 0��Size, ��ֵ��ʽ);
	resize(image, zoomin, Size(w/2,h/2), 0, 0, INTER_LINEAR);
	imshow("zoomin", zoomin);
	resize(image, zoomout, Size(w * 1.5, h * 1.5), 0, 0, INTER_LINEAR);
	imshow("zoomout", zoomout);
}

//ͼ��ת
void QuickDemo::flip_demo(Mat& image)
{
	Mat dst;
	//flip(image, dst, 0); // ���·�ת
	//flip(image, dst, 1); // ���ҷ�ת
	flip(image, dst, -1); // 180����ת
	imshow("ͼ��ת", dst);
}

//ͼ����ת
void QuickDemo::rotate_demo(Mat& image)
{
	Mat dst, M;
	int w = image.cols;
	int h = image.rows;
	//�����ת����M = getRitationMatrix2D(���ĵ�, �Ƕ�, �Ƿ�Ŵ�);
	M = getRotationMatrix2D(Point(w / 2, h / 2), 45, 1.0);
	//ȡ����ֵ��ȡ�任�����cos��sin��M.at<����>�����꣩��ʾ���ص�
	//ƫ�ƴ�С���¼���
	double cos = abs(M.at<double>(0, 0));
	double sin = abs(M.at<double>(0, 1));
	int nw = cos * w + sin * h;//���¼�������ߵȣ�˭���е�˭cos
	int nh = sin * w + cos * h;
	M.at<double>(0, 2) += (nw / 2 - w / 2);//���¼������ĵ㣬����ƫ����
	M.at<double>(1, 2) += (nh / 2 - h / 2);
	//��ת����warpAffine(ԭͼ����ͼ�����󣬴�С����ֵ��ʽ���߽�����ģʽ�������ɫ));
	warpAffine(image, dst, M, Size(nw, nh), INTER_LINEAR, 0, Scalar(255, 255, 0));
	imshow("��ת��ʾ", dst);
}


//��������ͷ
/*
void QuickDemo::video_demo(Mat& image)
{
	//VideoCapture capture(0)��0�ǵ�������ͷ��1���������ͷ����ַ����ƵԴ
	VideoCapture capture(0);
	Mat frame;//��Ƶ����ͼƬ
	while (true)
	{
		//��ȡ��ǰ֡��frame
		capture.read(frame);
		flip(frame, frame, 1);//����ת
		if (frame.empty())//��ܹؼ�����ֹ����
		{
			break;
		}
		imshow("frame", frame);
		colorSpace_Demo(frame);
		// TODO: do something....
		int c = waitKey(1);//���û�����������waitKeyΪ1
		if (c == 27)
		{
			break;
		}
	}
	// release
	capture.release();//�ͷ����
}
*/

//��Ƶ�����뱣��
void QuickDemo::video_demo(Mat& image)
{
	//VideoCapture��VideoWriter��������
	//opencv��������Ƶ���Ҵ�С��2g��
	//VideoCapture capture(0)��0�ǵ�������ͷ��1���������ͷ����ַ����ƵԴ
	VideoCapture capture("C:\\Users\\�»���\\Videos\\VCG42N1163767446.mp4");
	Mat frame;//��Ƶ����ͼƬ
	int frame_width = capture.get(CAP_PROP_FRAME_WIDTH);
	int frame_height = capture.get(CAP_PROP_FRAME_HEIGHT);
	int count = capture.get(CAP_PROP_FRAME_COUNT);//֡��
	double fps = capture.get(CAP_PROP_FPS);//����������С��
	std::cout << "frame width:" << frame_width << std::endl;
	std::cout << "frame height:" << frame_height << std::endl;
	std::cout << "FPS:" << fps << std::endl;
	std::cout << "Number of Frames:" << count << std::endl;
	//VideoWriter writer("����·��", capture.get(CAP_PROP_FOURCC)��ñ��뷽ʽ, fps֡��, ��СSize(frame_width, frame_height), ��ʼٻ�֡);
	VideoWriter writer("D:\\test.mp4", capture.get(CAP_PROP_FOURCC), fps, Size(frame_width, frame_height), 0);
	while(true)
	{
		//��ȡ��ǰ֡��frame
		capture.read(frame);
		flip(frame, frame, 1);//����ת
		if (frame.empty())//��ܹؼ�����ֹ����
		{
			break;
		}
		imshow("frame", frame);
		colorSpace_Demo(frame);
		writer.write(frame);//����
		// TODO: do something....
		int c = waitKey(1);//���û�����������waitKeyΪ1
		if (c == 27)
		{
			break;
		}
	}
	// release
	capture.release();//�ͷ����
	writer.release();//�ͷŶ�ȡ
}

//ֱ��ͼ�ҵ�ʹ����csdn
//һάֱ��ͼ
//��ӳ�Ҷ�ֵ
//�����ԣ���һ������С���ɲ���
void QuickDemo::histogram_demo(Mat& image)
{
	//H 0-180 S 0-255
	//��ͨ������
	std::vector<Mat> bgr_plane;
	split(image, bgr_plane);
	//�����������
	const int channels[1] = { 0 };//ͨ��
	const int bins[1] = { 256 };//�Ҷȼ���
	float hranges[2] = { 0,255 };//ȡֵ��Χ
	const float* ranges[1] = { hranges };//ȡֵ��Χ��float
	Mat b_hist;
	Mat g_hist;
	Mat r_hist;
	// ����Blue, Green, Redͨ����ֱ��ͼ
	//calcHist(&ͨ������ͼ��, ͼ������, ͨ����0Ϊ1, Mat(��0�ż��㣬��ѡ���룬�ղ�����), b_hist���ֱ��ͼ, 1ά��, bins�Ҷȼ���, &rangesȡֵ��Χ);
	calcHist(&bgr_plane[0], 1, 0, Mat(), b_hist, 1, bins, ranges);
	calcHist(&bgr_plane[1], 1, 0, Mat(), g_hist, 1, bins, ranges);
	calcHist(&bgr_plane[2], 1, 0, Mat(), r_hist, 1, bins, ranges);

	// ��ʾֱ��ͼ
	int hist_w = 512;
	int hist_h = 400;

	//cvRoundΪ���ظ�������ӽ�������ֵ����������
	//Ϊ�˺��ڹ켣����������̵�
	int bin_w = cvRound((double)hist_w / bins[0]);
	Mat histImage = Mat::zeros(hist_h, hist_w, CV_8UC3);//�������ƴ���
	// ��һ��ֱ��ͼ����
	normalize(b_hist, b_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat());
	normalize(g_hist, g_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat());
	normalize(r_hist, r_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat());
	// ����ֱ��ͼ����
	for (int i = 1; i < bins[0]; i++) 
	{
		//����������Ϊ�˱任����ϵͼ����ʾ
		line(histImage, Point(bin_w * (i - 1), hist_h - cvRound(b_hist.at<float>(i - 1))),
			Point(bin_w * (i), hist_h - cvRound(b_hist.at<float>(i))), Scalar(255, 0, 0), 2, 8, 0);
		line(histImage, Point(bin_w * (i - 1), hist_h - cvRound(g_hist.at<float>(i - 1))),
			Point(bin_w * (i), hist_h - cvRound(g_hist.at<float>(i))), Scalar(0, 255, 0), 2, 8, 0);
		line(histImage, Point(bin_w * (i - 1), hist_h - cvRound(r_hist.at<float>(i - 1))),
			Point(bin_w * (i), hist_h - cvRound(r_hist.at<float>(i))), Scalar(0, 0, 255), 2, 8, 0);
	}
	// ��ʾֱ��ͼ
	namedWindow("Histogram Demo", WINDOW_AUTOSIZE);
	imshow("Histogram Demo", histImage);

}

//����2dֱ��ͼ
void QuickDemo::histogram_2d_demo(Mat& image)
{
	// 2D ֱ��ͼ
	Mat hsv, hs_hist;
	//תΪhsv
	//����Ԥ����ͼ��
	cvtColor(image, hsv, COLOR_BGR2HSV);
	int hbins = 30, sbins = 32;
	int hist_bins[] = { hbins, sbins };//��ǰ�����ͨ��bins
	float h_range[] = { 0, 180 };
	float s_range[] = { 0, 256 };//ע��ͨ��������flaot
	const float* hs_ranges[] = { h_range, s_range };
	int hs_channels[] = { 0, 1 };//Ҫ���ͨ��
	//calcHist(&hsv, 1, hs_channels, Mat(), hs_hist, 2, hist_bins, hs_ranges, true�ȼ��, �ۼƱ�־false);
	calcHist(&hsv, 1, hs_channels, Mat(), hs_hist, 2, hist_bins, hs_ranges, true, false);
	double maxVal = 0;
	//�ҵ����ֵ
	minMaxLoc(hs_hist, 0, &maxVal, 0, 0);
	int scale = 10;//������
	Mat hist2d_image = Mat::zeros(sbins * scale, hbins * scale, CV_8UC3);
	for (int h = 0; h < hbins; h++) {
		for (int s = 0; s < sbins; s++)
		{
			float binVal = hs_hist.at<float>(h, s);//λ�ں�h����s����Ƶ��
			int intensity = cvRound(binVal * 255 / maxVal);////��ɫ��ǿ�ȣ�Ƶ��Խ��С����Խ�ӽ���ɫ
			rectangle(hist2d_image, Point(h * scale, s * scale),
				Point((h + 1) * scale - 1, (s + 1) * scale - 1),
				Scalar::all(intensity),
				-1);
		}
	}
	//��ɫ��ͼ���ݿ������죬��������ת��ɫ�ʷ��
	applyColorMap(hist2d_image, hist2d_image, COLORMAP_JET);
	imshow("H-S Histogram", hist2d_image);
	imwrite("D:/hist_2d.png", hist2d_image);
}

void QuickDemo::histogram_eq_demo(Mat& image) 
{
	Mat gray;
	cvtColor(image, gray, COLOR_BGR2GRAY);
	imshow("�Ҷ�ͼ��", gray);
	Mat dst;
	//equalizeHist(����, ���);����ͼ����ǿ��ֻ֧�ֵ�ͨ�������Ҷ�ͼ����ת��
	equalizeHist(gray, dst);
	imshow("ֱ��ͼ���⻯��ʾ", dst);
}

//����������������Ծ����ͼ��ģ��������һ�ֲ���

void QuickDemo::blur_demo(Mat& image) 
{
	Mat dst;
	//(-1,-1)Ĭ���Ծ�������λ��Ϊ�����
	//blur(image, dst, ���ϴ�С, ��������);
	blur(image, dst, Size(15, 15), Point(-1, -1));
	imshow("ͼ��ģ��", dst);
}

void QuickDemo::gaussian_blur_demo(Mat& image) {
	Mat dst;
	//������˹�ľ��ϣ�Ч������
	//�ƻ�ͼ��ԭ����Ϣ
	GaussianBlur(image, dst, Size(0, 0), 15);
	imshow("��˹ģ��", dst);
}

void QuickDemo::bifilter_demo(Mat& image) 
{
	Mat dst;
	//����ԭ�е���Ϣ
	//bilateralFilter(image, dst, ����ֱ��,��ɫ���˱�׼����, ����ռ���˱�׼��);
	bilateralFilter(image, dst, 0, 100, 10);
	imshow("˫��ģ��", dst);
}



void QuickDemo::face_detection_demo()
{
	//�ļ���·��
	std::string root_dir = "D:\\���\\opencv\\opencv\\sources\\samples\\dnn\\face_detector\\";
	//��ȡ��TensorFlowΪ��ܸ�ʽ�洢������ģ��
   dnn::Net net = dnn::readNetFromTensorflow(root_dir + "opencv_face_detector_uint8.pb",root_dir + "opencv_face_detector.pbtxt");

	//������ͷ�����������
	VideoCapture cap(0);
	Mat frame;

	while (true)
	{
		cap.read(frame);
		
		if (frame.empty())
		{
			break;
		}

		flip(frame, frame, 1);

		//׼�����ѧϰģ����Ҫ������
		Mat blob=dnn::blobFromImage(frame,1.0,Size(300,300), Scalar(104, 177, 123), false, false);
		net.setInput(blob);

		//�������
		Mat probs = net.forward();
		Mat detectionMat(probs.size[2], probs.size[3], CV_32F, probs.ptr<float>());

		//�������
		for (int i = 0; i < detectionMat.rows; i++)
		{

			float confidence = detectionMat.at<float>(i, 2);//������ֵ �÷�

			if (confidence > 0.5) {

				//��ΪԤ������ֵΪ[0,1]��Χ���������ǻ���Ҫ*ԭͼ��Ŀ�Ⱥͳ���
				int x1 = static_cast<int>(detectionMat.at<float>(i, 3) * frame.cols);
				int y1 = static_cast<int>(detectionMat.at<float>(i, 4) * frame.rows);
				int x2 = static_cast<int>(detectionMat.at<float>(i, 5) * frame.cols);
				int y2 = static_cast<int>(detectionMat.at<float>(i, 6) * frame.rows);

				Rect rect(x1, y1, x2 - x1, y2 - y1);
				rectangle(frame, rect, Scalar(0, 0, 255), 2, 8, 0);

			}
		}
		imshow("�������", frame);

		int c = waitKey(1);
		if (c == 27) {
			break;
		}
	}
	cap.release();
}