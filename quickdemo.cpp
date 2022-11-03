#include <quickopencv.h>
#include <opencv2/dnn.hpp>
#include<vector>

using namespace cv;
using namespace std;

//色彩空间转换
void QuickDemo::colorSpace_Demo(Mat& image) 
{
	Mat gray, hsv;
	cvtColor(image, hsv, COLOR_BGR2HSV);//色彩与灰度，通道改变，第三个参数用法？
	// H 0 ~ 180, S, V 
	//H，S颜色，V亮度，不同空间色彩调不同的东西
	cvtColor(image, gray, COLOR_BGR2GRAY);
	imshow("HSV", hsv);
	imshow("灰度", gray);
	imwrite("D:/hsv.png", hsv);//保存图像
	imwrite("D:/gray.png", gray);
}

//图像的创建与赋值
void QuickDemo::mat_creation_Demo( /*Mat& image*/)
{
	/*
	//声明两个图像变量
	Mat m1, m2;
	//单纯的赋值依旧是指向同一个图像
	//只有复制或者克隆才是变成别的图像
	m1 = image.clone();
	image.copyTo(m2);
	*/

	//创建空白图像
	//初始化zeros或者ones
	//Size（规格）
	//CV_8UC数字通道数
	//CV_8UC1,无符号的8位单通道
	Mat m3 = Mat::zeros(Size(128, 128), CV_8UC3);
	
	/*
	//这样简单的赋值只对第一个通道有用
	m3 = 127;
	*/

	//只要知道几个通道，就可以用Scalar进行改变
	m3 = Scalar(0, 0, 255);
	std::cout << "width:" << m3.cols << "\nheight:" << m3.rows << "\nchannels" << m3.channels() << std::endl;

	Mat m4;
	m3.copyTo(m4);
	m4 = Scalar(0, 255, 255);

	/*
	imshow("图片1", m1);
	
	imshow("图片2",m2);
	*/
	
	imshow("创建图像", m4);

	imshow("创建图片3", m3);

	/*
	//输出图像的宽度高度通道数
	std::cout << "width:" << m3.cols << "\nheight:" << m3.rows << "\nchannels" << m3.channels() << std::endl;
	std::cout << m3 << std::endl;//非必要不输出流
	*/

}

//像素点的访问
void QuickDemo::pixel_visit_demo(Mat& image)
{
	int w = image.cols;//列
	int h = image.rows;//行
	int dims = image.channels();//通道数
	/*
	//用数组来实现访问像素点
	for (int row = 0; row < h; row++)
	{
		for (int col = 0;col < w; col++)
		{
			if (dims == 1)//灰色图像
			{
				//uchar字节类型，像素点类型
				int pv = image.at<uchar>(row, col);
				//反色
				//0-255
				image.at<uchar>(row, col) = 255 - pv;
			}
			if (dims == 3)//彩色图像
			{
				//专门获取
				//获取一个像素中的三个值
				Vec3b bgr = image.at<Vec3b>(row, col);
				image.at<Vec3b>(row,col)[0] = 255 - bgr[0];
				image.at<Vec3b>(row,col)[1] = 255 - bgr[1];
				image.at<Vec3b>(row,col)[2] = 255 - bgr[2];
			}
		}
	}
	*/


	//用指针来访问像素点
	for (int row = 0; row < h; row++)
	{
		//定义一个指针指针指向固定的行
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
				//赋值完后往后面走一个
				//++是先赋值再执行
				*current_row++ = 255 - *current_row;
				*current_row++ = 255 - *current_row;
				*current_row++ = 255 - *current_row;
			}
		}
	}

	imshow("像素读写演示", image);
}

//运算操作
void QuickDemo::operators_demo(Mat& image)
{

	/*
	//每个通道加50，变亮，反之变暗
	//同理也可以加减乘除，矩阵操作
	//但乘法得迂回一点操作
	Mat dst;
	//dst = image + Scalar(50, 50, 50);
	Mat m = Mat::zeros(image.size(), image.type());
	m = Scalar(2, 2, 2);
	multiply(image, m, dst);
	//不能直接乘法哦！
	//dst = image * m;
	*/

	//创立一个图像，初始化，其大小与类型与原来保持一致
	Mat dst = Mat::zeros(image.size(), image.type());
	Mat m = Mat::zeros(image.size(), image.type());
	m = Scalar(50, 50, 50);
   
	/*
	//手动模拟加法
	int w = image.cols;
	int h = image.rows;
	int dims = image.channels();

	for (int row = 0; row < h; row++)
	{
		for (int col = 0; col < w; col++)
		{
			Vec3b p1 = image.at<Vec3b>(row, col);
			Vec3b p2 = m.at<Vec3b>(row, col);
			//saturate_cast<uchar>对数值进行判断，防止溢出带来的问题，使其0-255内
			dst.at<Vec3b>(row, col)[0] = saturate_cast<uchar>(p1[0] + p2[0]);
			dst.at<Vec3b>(row, col)[1] = saturate_cast<uchar>(p1[1] + p2[1]);
			dst.at<Vec3b>(row, col)[2] = saturate_cast<uchar>(p1[2] + p2[2]);
		}
	}
	*/

	//我们都是用opencv自带的算法
	//add(a,b,ans);
	//subtract(a,b,ans);
	//multiple(a,b,ans);
	//divide(a,b,ans);

	imshow("加法操作",dst);
}


//滚动条函数
// 先绑定后调用callback
//static保护安全
static void on_track(int b, void* userdate)
{
	//传入是void*指针，改为Mat*，再访问数值
	Mat image = *((Mat*)userdate);
	//初始化
	Mat dst = Mat::zeros(image.size(), image.type());
	Mat m = Mat::zeros(image.size(), image.type());
	m = Scalar(b, b, b);

	//增亮操作操作
	add(image, m, dst);
	imshow("亮度调整", dst);
}

void on_lightness(int b, void* userdata)
{
	Mat image = *((Mat*)userdata);
	Mat dst = Mat::zeros(image.size(), image.type());
	Mat m = Mat::zeros(image.size(), image.type());

	//addweighted传入的数据要double型的，也就是为什么有1.0，0.0
	//addweighted是通过权加融合的
	//亮度主要通过操作b
	addWeighted(image, 1.0, m, 0, b, dst);

	imshow("亮度与对比度调整", dst);
}


void on_contrast(int b, void* userdata)
{
	Mat image = *((Mat*)userdata);
	Mat dst = Mat::zeros(image.size(), image.type());
	Mat m = Mat::zeros(image.size(), image.type());

	//对比度的范围为0.0-3.0，所以要除法
	//对比度主要通过操作contrast
	double contrast = b / 100.0;
	addWeighted(image, contrast, m, 0.0, 0, dst);

	imshow("亮度与对比度调整", dst);
}



void QuickDemo::tracking_bar_demo(Mat& image)
{
	/*
	//亮度调整
	//创建一个窗口
	namedWindow("亮度调整",WINDOW_AUTOSIZE);
	int lightness = 50;
	int max_value = 100;
	int contrast_value = 100;

	//createTrackbar(拖动条名字，窗口名，&初始值及变值，最大值，返回与操作函数，void*图片源)
	createTrackbar("Value Bar:", "亮度调整", &lightness, max_value, on_track, (void*)(&image));

	//开始操作展示
	on_track(50, &image);
	*/


	//将对比度与亮度合在一起
	namedWindow("亮度与对比度调整", WINDOW_AUTOSIZE);
	int lightness = 50;
	int max_value = 100;
	int contrast_value = 100;

	createTrackbar("Value Bar:", "亮度与对比度调整", &lightness, max_value, on_lightness, (void*)(&image));
	createTrackbar("Contrast Bar:", "亮度与对比度调整", &contrast_value, 200, on_contrast, (void*)(&image));

	//可有可无
	on_lightness(50, &image);
}

void QuickDemo::key_demo(Mat& image)
{
	Mat dst = Mat::zeros(image.size(), image.type());
	while (true)
	{
		//间隔时间获取键盘值
		//记得鼠标要放在图像窗口
		int c = waitKey(100);

		if (c == 27) { // 退出
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
		imshow("键盘响应", dst);
	}
}


//自带颜色表操作
void QuickDemo::color_style_demo(Mat& image)
{
	//先定义所有的关键词
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
		//停顿or键盘取值都可以waitKey（）
		int c = waitKey(500);
		if (c == 27) { // 退出
			break;
		}
		//编程技巧，%19，所有数在19内
		//applycolorMap（原图，保存图，颜色表）
		applyColorMap(image, dst, colormap[index % 19]);
		index++;
		imshow("颜色风格", dst);
	}
}


//位操作与绘制填充
void QuickDemo::bitwise_demo(Mat& image)
{
	Mat m1 = Mat::zeros(Size(256, 256), CV_8UC3);
	Mat m2 = Mat::zeros(Size(256, 256), CV_8UC3);

	//绘制与填充
	//大于0，绘制，下于0，填充（厚值）
	//绘制与填充函数
	//LINE_AA反锯齿 慢
	//LINE_8 这样就好了(这些代表线型，默认取8)
	//Rect（左上坐标，右下坐标）
	//Scalar（）颜色
	//最后一个0表示小数点位数
	//rectangle(m1, Rect(100, 100, 80, 80), Scalar(255, 255, 0), 1, LINE_8,0);
	//rectangle(m2, Rect(100, 100, 80, 80), Scalar(0, 255, 255), -1, LINE_8,0);

	rectangle(m1, Rect(100, 100, 80, 80), Scalar(255, 255, 0), -1, LINE_8, 0);
	rectangle(m2, Rect(150, 150, 80, 80), Scalar(0, 255, 255), -1, LINE_8, 0);
	imshow("m1", m1);
	imshow("m2", m2);
	Mat dst;
	
	
	//位操作
	// 8个位变为二进制进行位运算
	//bitwise_or(m1, m2, dst);
	//bitwise_xor(m1, m2, dst);
	//bitwise_and(m1, m2, dst);
	//bitwise_not(image,dst);取反一个原图，一个存图，255-现在=取反

	imshow("像素位操作",dst);
}

//通道分离与合并
void QuickDemo::channels_demo(Mat& image)
{
	
	std::vector<Mat> mv;
	//split(image,mv)目标数组与存储数组
	split(image, mv);

	//分离出来为单通道图，即分别为三个颜色通道的灰度图
	imshow("蓝色", mv[0]);
	imshow("绿色", mv[1]);
	imshow("红色", mv[2]);

	//若想输出红色的，则得关掉其他通道再合并
	Mat dst;
	mv[0] = 0;
	mv[1] = 0;
	//merge(mv,dst)被混合数组与保存数组
	merge(mv, dst);
	//注意窗口名相同则后面的会掩盖前面窗口的东西
	imshow("红色", dst);

	int from_to[] = { 0, 2, 1,1, 2, 0 };
	//mixChannels(&输入矩阵，矩阵数，&输出矩阵，矩阵数，复制列表对，复制列表数)
	mixChannels(&image, 1, &dst, 1, from_to,3);
	imshow("通道混合", dst);
}

//图像色彩空间转换
void QuickDemo::inrange_demo(Mat& image)
{
    //编程实现抠图
	Mat hsv;
	//从颜色空间转换到另一个空间，hsv好割离
	cvtColor(image, hsv, COLOR_BGR2HSV);
	Mat mask; 
	//二值化函数，在范围内为白色，否则为黑色
	//inRange(输入,min,max，输出)；
	//取值时看色道范围
	//去除背景色
	inRange(hsv, Scalar(35, 43, 46), Scalar(77, 255, 255), mask);

	Mat redback = Mat::zeros(image.size(), image.type());
	redback = Scalar(40, 40, 200);//红色背景
	//把保留下来的背景从白变为黑，而人变为白
	bitwise_not(mask, mask);
	imshow("mask", mask);
	//copyTo有两个作用，一个是复制，另一个是重构，可以附加一个掩膜
	//掩膜顾名思义就是掩盖住图片
	//所以下面得到的是image被mask掩盖住的图像，白色不掩盖，黑色0盖
	image.copyTo(redback, mask);//把人盖上去
	imshow("roi区域提取", redback);
}

//图像像素值的统计
void QuickDemo::pixel_statistic_demo(Mat& image)
{
	//电脑只认识0和1，故像素作为数据让电脑知道
	double minv, maxv;//统计最大最小值
	Point minLoc, maxLoc;//最大最小值的地址,坐标
	std::vector<Mat> mv;
	split(image, mv);
	//mv.size为向量的大小，此时为3
	for (int i = 0; i < mv.size(); i++)
	{
		//minMaxLoc只支持单通道
		//minMaxLoc(通道，&最小值，&最大值，&最小坐标，&最大坐标，可选子列Mat（）)
		minMaxLoc(mv[i], &minv, &maxv,&minLoc, &maxLoc, Mat());
		std::cout << "No. channels:" << i << " min value:" << minv << " max value:" << maxv << std::endl;
	}
	Mat mean, stddev;

	//meanStdDev(输入参数，均值，标准差，可选mask)
	meanStdDev(image, mean, stddev);
	std::cout << "means:\n" << mean << std::endl;
	std::cout << "stddev:\n" << stddev << std::endl;
}

//图像几何形状的的绘制
void QuickDemo::drawing_demo(Mat& image)
{
	/*
	//定义一个矩形，Rect类
	Rect rect;//定义一个图像的规格
	rect.x = 100;
	rect.y = 100;//起点坐标，左上角，最大的在右下角
	rect.width = 250;
	rect.height = 3300;//宽度与高度
	//Mat bg = Mat::zeros(image.size(), image.type());
	//不仅可以绘制也可以填充
	//画矩形rectangle(对象, Rect类, Scalar(255, 255, 0), 1, LINE_8,0);
	rectangle(image, rect, Scalar(0, 0, 255), 2, 8, 0);
	//画圆circle(对象, Point圆心, 半径, Scalar(255, 0, 0), -1, 8, 0);
	circle(image, Point(350, 400), 15, Scalar(255, 0, 0), -1, 8, 0);
	//画线line(对象, Point起点, Point终点, Scalar(0, 255, 0), 4, LINE_AA, 0);
	line(image, Point(100, 100), Point(350, 400), Scalar(0, 255, 0), 4, LINE_AA, 0);
	imshow("绘制演示", image);
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

	//定义一个旋转矩阵
	RotatedRect rrt;
	rrt.center = Point(200, 200);//质点
	rrt.size = Size(100, 200);//大小
	rrt.angle = 90.0;//角度
	//画椭圆ellipse(对象, 选择矩阵RotatedRect, Scalar(0, 255, 255), 2, 8);
	ellipse(bg, rrt, Scalar(0, 255, 255), 2, 8);

	Mat dst;
	//按比例融合
	addWeighted(image, 0.7, bg, 0.3, 0, dst);
	imshow("绘制演示", dst);
}

//随机数与随机颜色
void QuickDemo::random_drawing()
{
	Mat canvas = Mat::zeros(Size(512, 512), CV_8UC3);
	int w = canvas.cols;
	int h = canvas.rows;
	RNG rng(12345);
	while (true)
	{
		int c = waitKey(10);
		if (c == 27) { // 退出
			break;
		}
		//rng.uniform(起点，到最大)之间的随机数
		int x1 = rng.uniform(0, w);
		int y1 = rng.uniform(0, h);
		int x2 = rng.uniform(0, w);
		int y2 = rng.uniform(0, h);
		int b = rng.uniform(0, 255);
		int g = rng.uniform(0, 255);
		int r = rng.uniform(0, 255);
		canvas = Scalar(0, 0, 0);
		line(canvas, Point(x1, y1), Point(x2, y2), Scalar(b, g, r), 3, LINE_AA, 0);
		imshow("随机绘制演示", canvas);

	}
}

//绘制多边形
void QuickDemo::polyline_drawing_demo()
{
	//创建空画布
	Mat canvas = Mat::zeros(Size(512, 512), CV_8UC3);
	int w = canvas.cols;
	int h = canvas.rows;
	//设几个点
	Point p1(100, 100);
	Point p2(300, 150);
	Point p3(300, 350);
	Point p4(250, 450);
	Point p5(50, 450);
	//创建容器塞东西进去
	std::vector<Point> pts;
	pts.push_back(p1);
	pts.push_back(p2);
	pts.push_back(p3);
	pts.push_back(p3);
	pts.push_back(p4);
	pts.push_back(p5);
	//多边形的绘制与填充是分别开来的
	//polylines polylines(画布, 数组, true合并, Scalar(0, 255, 0), 厚度绘制不小于0, 线型, 位移);
	polylines(canvas, pts, true, Scalar(0, 255, 0), 1, 8, 0);
	//fillPoly(画布, 数组, Scalar(0, 255, 0), 线形, 位移);
	fillPoly(canvas, pts, Scalar(0, 255, 0), 8, 0);

	//建一个二维向量
	std::vector<std::vector<Point>> contours;
	contours.push_back(pts);
	//一次性画所有的图
	//drawContours(画布, 二维点集, 绘图数量负全0为1, Scalar(0, 0, 255), 填充与绘制, 线型);
	drawContours(canvas, contours,0, Scalar(0, 0, 255), 8, 8);
	imshow("绘制多边形", canvas);
}


//鼠标操作
//全局变量记录一些状态
//关键：如何提取，如何刷新
Point sp(-1, -1);
Point ep(-1, -1);
Mat temp;
//绘制方法函数static void on_draw(鼠标事件, 初始坐标, 初始坐标, 拖拽事件, void* 回调参数)
static void on_draw(int event, int x, int y, int flags, void* userdata)
{
	Mat image = *((Mat*)userdata);//转换

	if (event == EVENT_LBUTTONDOWN)//记录起始坐标
	{
		sp.x = x;
		sp.y = y;
		std::cout << "start point:" << sp << std::endl;
	}
	else if (event == EVENT_LBUTTONUP)//记录终点坐标
	{
		ep.x = x;
		ep.y = y;
		int dx = ep.x - sp.x;
		int dy = ep.x - sp.x;
		if (dx > 0 && dy > 0)
		{
			Rect box(sp.x, sp.y, dx, dy);
			temp.copyTo(image);//覆盖原先轨迹
			imshow("ROI区域", image(box));//提取roi区域方法image（坐标）
			rectangle(image, box, Scalar(0, 0, 255), 2, 8, 0);
			imshow("鼠标绘制", image);//不断更新图片
			// ready for next drawing，严谨
			sp.x = -1;
			sp.y = -1;
		}
	}
	else if (event == EVENT_MOUSEMOVE)
	{
		if (sp.x > 0 && sp.y > 0)//防止起点移动也画图
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
				imshow("鼠标绘制", image);//不断更新图片
			}
		}
	}
}


void QuickDemo::mouse_drawing_demo(Mat& image)
{
	namedWindow("鼠标绘制", WINDOW_AUTOSIZE);
	//setMouseCallback("绘制窗口", 绘制方法函数, （void*)(用户数据给回调函数));
	setMouseCallback("鼠标绘制", on_draw, (void*)(&image));
	imshow("鼠标绘制", image);
	temp = image.clone();//克隆一张放到全局用于刷新
}

//图像像素类型转换与归一化
void QuickDemo::norm_demo(Mat& image)
{
	Mat dst;
	std::cout << image.type() << std::endl;
	//格式转换对象.convertTo（存对象，格式）
	//CV_32F，32位的浮点型
	//如果没有格式化浮点，归一后全是0，一片黑
	//正确的数据类型很重要，特别是后面的训练数据
	//如果你想imshow浮点数，必须在0到1之间
	image.convertTo(image, CV_32F);
	std::cout << image.type() << std::endl;
	//归一化normalize(对象，存对象，上限，下限，归一化方式);
	normalize(image, dst, 1.0, 0, NORM_MINMAX);
	std::cout << dst.type() << std::endl;
	imshow("图像数据归一化", dst);
	// CV_8UC3, CV_32FC3,CV_32S
}

//图像放缩与插值
void QuickDemo::resize_demo(Mat& image)
{
	Mat zoomin, zoomout;
	int h = image.rows;
	int w = image.cols;
	imshow("原图", image);
	//resize(对象, 存对象, Size(规格), 0, 0按Size, 插值方式);
	resize(image, zoomin, Size(w/2,h/2), 0, 0, INTER_LINEAR);
	imshow("zoomin", zoomin);
	resize(image, zoomout, Size(w * 1.5, h * 1.5), 0, 0, INTER_LINEAR);
	imshow("zoomout", zoomout);
}

//图像翻转
void QuickDemo::flip_demo(Mat& image)
{
	Mat dst;
	//flip(image, dst, 0); // 上下翻转
	//flip(image, dst, 1); // 左右翻转
	flip(image, dst, -1); // 180°旋转
	imshow("图像翻转", dst);
}

//图像旋转
void QuickDemo::rotate_demo(Mat& image)
{
	Mat dst, M;
	int w = image.cols;
	int h = image.rows;
	//获得旋转矩阵M = getRitationMatrix2D(中心点, 角度, 是否放大);
	M = getRotationMatrix2D(Point(w / 2, h / 2), 45, 1.0);
	//取绝对值，取变换矩阵的cos和sin，M.at<类型>（坐标）表示像素点
	//偏移大小重新计算
	double cos = abs(M.at<double>(0, 0));
	double sin = abs(M.at<double>(0, 1));
	int nw = cos * w + sin * h;//重新计算宽度与高等，谁被叫到谁cos
	int nh = sin * w + cos * h;
	M.at<double>(0, 2) += (nw / 2 - w / 2);//重新计算中心点，加上偏移量
	M.at<double>(1, 2) += (nh / 2 - h / 2);
	//旋转操作warpAffine(原图，存图，矩阵，大小，插值方式，边界像素模式，填充颜色));
	warpAffine(image, dst, M, Size(nw, nh), INTER_LINEAR, 0, Scalar(255, 255, 0));
	imshow("旋转演示", dst);
}


//调用摄像头
/*
void QuickDemo::video_demo(Mat& image)
{
	//VideoCapture capture(0)，0是电脑摄像头，1是外接摄像头，地址是视频源
	VideoCapture capture(0);
	Mat frame;//视频就是图片
	while (true)
	{
		//读取当前帧给frame
		capture.read(frame);
		flip(frame, frame, 1);//镜像翻转
		if (frame.empty())//这很关键，防止流错
		{
			break;
		}
		imshow("frame", frame);
		colorSpace_Demo(frame);
		// TODO: do something....
		int c = waitKey(1);//如果没有特殊情况，waitKey为1
		if (c == 27)
		{
			break;
		}
	}
	// release
	capture.release();//释放相机
}
*/

//视频处理与保存
void QuickDemo::video_demo(Mat& image)
{
	//VideoCapture和VideoWriter这种是类
	//opencv不处理音频，且大小在2g内
	//VideoCapture capture(0)，0是电脑摄像头，1是外接摄像头，地址是视频源
	VideoCapture capture("C:\\Users\\陈焕新\\Videos\\VCG42N1163767446.mp4");
	Mat frame;//视频就是图片
	int frame_width = capture.get(CAP_PROP_FRAME_WIDTH);
	int frame_height = capture.get(CAP_PROP_FRAME_HEIGHT);
	int count = capture.get(CAP_PROP_FRAME_COUNT);//帧数
	double fps = capture.get(CAP_PROP_FPS);//处理能力，小数
	std::cout << "frame width:" << frame_width << std::endl;
	std::cout << "frame height:" << frame_height << std::endl;
	std::cout << "FPS:" << fps << std::endl;
	std::cout << "Number of Frames:" << count << std::endl;
	//VideoWriter writer("保存路径", capture.get(CAP_PROP_FOURCC)获得编码方式, fps帧数, 大小Size(frame_width, frame_height), 真彩假灰帧);
	VideoWriter writer("D:\\test.mp4", capture.get(CAP_PROP_FOURCC), fps, Size(frame_width, frame_height), 0);
	while(true)
	{
		//读取当前帧给frame
		capture.read(frame);
		flip(frame, frame, 1);//镜像翻转
		if (frame.empty())//这很关键，防止流错
		{
			break;
		}
		imshow("frame", frame);
		colorSpace_Demo(frame);
		writer.write(frame);//保存
		// TODO: do something....
		int c = waitKey(1);//如果没有特殊情况，waitKey为1
		if (c == 27)
		{
			break;
		}
	}
	// release
	capture.release();//释放相机
	writer.release();//释放读取
}

//直方图我的痛，看csdn
//一维直方图
//反映灰度值
//不变性：归一化，缩小依旧不变
void QuickDemo::histogram_demo(Mat& image)
{
	//H 0-180 S 0-255
	//三通道分离
	std::vector<Mat> bgr_plane;
	split(image, bgr_plane);
	//定义参数变量
	const int channels[1] = { 0 };//通道
	const int bins[1] = { 256 };//灰度级别
	float hranges[2] = { 0,255 };//取值范围
	const float* ranges[1] = { hranges };//取值范围用float
	Mat b_hist;
	Mat g_hist;
	Mat r_hist;
	// 计算Blue, Green, Red通道的直方图
	//calcHist(&通道输入图像, 图形数量, 通道数0为1, Mat(非0才计算，可选掩码，空不操作), b_hist输出直方图, 1维度, bins灰度级别, &ranges取值范围);
	calcHist(&bgr_plane[0], 1, 0, Mat(), b_hist, 1, bins, ranges);
	calcHist(&bgr_plane[1], 1, 0, Mat(), g_hist, 1, bins, ranges);
	calcHist(&bgr_plane[2], 1, 0, Mat(), r_hist, 1, bins, ranges);

	// 显示直方图
	int hist_w = 512;
	int hist_h = 400;

	//cvRound为返回跟参数最接近的整数值，四舍五入
	//为了后期轨迹点坐标计算铺垫
	int bin_w = cvRound((double)hist_w / bins[0]);
	Mat histImage = Mat::zeros(hist_h, hist_w, CV_8UC3);//创建绘制窗口
	// 归一化直方图数据
	normalize(b_hist, b_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat());
	normalize(g_hist, g_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat());
	normalize(r_hist, r_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat());
	// 绘制直方图曲线
	for (int i = 1; i < bins[0]; i++) 
	{
		//减法操作是为了变换坐标系图像显示
		line(histImage, Point(bin_w * (i - 1), hist_h - cvRound(b_hist.at<float>(i - 1))),
			Point(bin_w * (i), hist_h - cvRound(b_hist.at<float>(i))), Scalar(255, 0, 0), 2, 8, 0);
		line(histImage, Point(bin_w * (i - 1), hist_h - cvRound(g_hist.at<float>(i - 1))),
			Point(bin_w * (i), hist_h - cvRound(g_hist.at<float>(i))), Scalar(0, 255, 0), 2, 8, 0);
		line(histImage, Point(bin_w * (i - 1), hist_h - cvRound(r_hist.at<float>(i - 1))),
			Point(bin_w * (i), hist_h - cvRound(r_hist.at<float>(i))), Scalar(0, 0, 255), 2, 8, 0);
	}
	// 显示直方图
	namedWindow("Histogram Demo", WINDOW_AUTOSIZE);
	imshow("Histogram Demo", histImage);

}

//绘制2d直方图
void QuickDemo::histogram_2d_demo(Mat& image)
{
	// 2D 直方图
	Mat hsv, hs_hist;
	//转为hsv
	//进行预处理图像
	cvtColor(image, hsv, COLOR_BGR2HSV);
	int hbins = 30, sbins = 32;
	int hist_bins[] = { hbins, sbins };//提前定义各通道bins
	float h_range[] = { 0, 180 };
	float s_range[] = { 0, 256 };//注意通道，且是flaot
	const float* hs_ranges[] = { h_range, s_range };
	int hs_channels[] = { 0, 1 };//要求的通道
	//calcHist(&hsv, 1, hs_channels, Mat(), hs_hist, 2, hist_bins, hs_ranges, true等间隔, 累计标志false);
	calcHist(&hsv, 1, hs_channels, Mat(), hs_hist, 2, hist_bins, hs_ranges, true, false);
	double maxVal = 0;
	//找到最大值
	minMaxLoc(hs_hist, 0, &maxVal, 0, 0);
	int scale = 10;//扩大倍数
	Mat hist2d_image = Mat::zeros(sbins * scale, hbins * scale, CV_8UC3);
	for (int h = 0; h < hbins; h++) {
		for (int s = 0; s < sbins; s++)
		{
			float binVal = hs_hist.at<float>(h, s);//位于横h，列s处的频次
			int intensity = cvRound(binVal * 255 / maxVal);////白色的强度，频次越大，小矩形越接近白色
			rectangle(hist2d_image, Point(h * scale, s * scale),
				Point((h + 1) * scale - 1, (s + 1) * scale - 1),
				Scalar::all(intensity),
				-1);
		}
	}
	//灰色的图像不容看出差异，这里我们转换色彩风格
	applyColorMap(hist2d_image, hist2d_image, COLORMAP_JET);
	imshow("H-S Histogram", hist2d_image);
	imwrite("D:/hist_2d.png", hist2d_image);
}

void QuickDemo::histogram_eq_demo(Mat& image) 
{
	Mat gray;
	cvtColor(image, gray, COLOR_BGR2GRAY);
	imshow("灰度图像", gray);
	Mat dst;
	//equalizeHist(输入, 输出);用于图像增强，只支持单通道，即灰度图像，先转换
	equalizeHist(gray, dst);
	imshow("直方图均衡化演示", dst);
}

//卷积操作，常用线性卷积，图像模糊是其中一种操作

void QuickDemo::blur_demo(Mat& image) 
{
	Mat dst;
	//(-1,-1)默认以卷集合中心位置为输出点
	//blur(image, dst, 卷集合大小, 卷集合中心);
	blur(image, dst, Size(15, 15), Point(-1, -1));
	imshow("图像模糊", dst);
}

void QuickDemo::gaussian_blur_demo(Mat& image) {
	Mat dst;
	//产生高斯的卷集合，效果更好
	//破坏图像原有信息
	GaussianBlur(image, dst, Size(0, 0), 15);
	imshow("高斯模糊", dst);
}

void QuickDemo::bifilter_demo(Mat& image) 
{
	Mat dst;
	//保护原有的信息
	//bilateralFilter(image, dst, 领域直径,颜色过滤标准差大好, 坐标空间过滤标准差);
	bilateralFilter(image, dst, 0, 100, 10);
	imshow("双边模糊", dst);
}



void QuickDemo::face_detection_demo()
{
	//文件夹路径
	std::string root_dir = "D:\\软件\\opencv\\opencv\\sources\\samples\\dnn\\face_detector\\";
	//读取以TensorFlow为框架格式存储的网络模型
   dnn::Net net = dnn::readNetFromTensorflow(root_dir + "opencv_face_detector_uint8.pb",root_dir + "opencv_face_detector.pbtxt");

	//对摄像头进行人脸检测
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

		//准备深度学习模型需要的数据
		Mat blob=dnn::blobFromImage(frame,1.0,Size(300,300), Scalar(104, 177, 123), false, false);
		net.setInput(blob);

		//完成推理
		Mat probs = net.forward();
		Mat detectionMat(probs.size[2], probs.size[3], CV_32F, probs.ptr<float>());

		//解析结果
		for (int i = 0; i < detectionMat.rows; i++)
		{

			float confidence = detectionMat.at<float>(i, 2);//第三个值 得分

			if (confidence > 0.5) {

				//因为预测来的值为[0,1]范围的数，我们还需要*原图像的宽度和长度
				int x1 = static_cast<int>(detectionMat.at<float>(i, 3) * frame.cols);
				int y1 = static_cast<int>(detectionMat.at<float>(i, 4) * frame.rows);
				int x2 = static_cast<int>(detectionMat.at<float>(i, 5) * frame.cols);
				int y2 = static_cast<int>(detectionMat.at<float>(i, 6) * frame.rows);

				Rect rect(x1, y1, x2 - x1, y2 - y1);
				rectangle(frame, rect, Scalar(0, 0, 255), 2, 8, 0);

			}
		}
		imshow("人脸检测", frame);

		int c = waitKey(1);
		if (c == 27) {
			break;
		}
	}
	cap.release();
}