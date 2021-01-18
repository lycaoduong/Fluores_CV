#include <opencv2\highgui.hpp>
#include <opencv2\imgproc.hpp>
#include <QImage>
#include "opencv2/core/cuda.hpp"
#include "qcustomplot.h"

using namespace cv;
using namespace std;
using namespace cuda;

class ImageProcess
{
private:
public:
//	Mat static QImage2Mat(QImage const& src);
//    QImage static Mat2QImage(cv::Mat src);
    QImage static Cv2QImage(cv::Mat cvimg);
    Mat static QImage2Cv(QImage qtimg);
    Mat static BGR2RGB(cv::Mat cvimg);
	Mat static applyCustomColorMap(Mat& im_gray, int mode);
	Mat static CustomColorMap2Gray(cv::Mat& im_color, int mode);
	Mat static VerticalConcat(Mat image1, Mat image2);
	Mat static Merge_Image(Mat imageBground, Mat imageFground);
	Mat static gray_level(int w, int h);
	Mat static scale_level(Mat im_gray, int min, int max);
    Mat static ReadImage(std::string Path);
	void static displayImage(string pathImg);

    Mat static crop_Image(cv::Mat sr, QPoint topL, QPoint rightB);
    QPoint static find_Max_Min(cv::Mat sr);
    Mat static colorMap2Mat(QCPColorMap clm, QCustomPlot *Object);
    QCPColorGradient static customGradient(int mode);
};

