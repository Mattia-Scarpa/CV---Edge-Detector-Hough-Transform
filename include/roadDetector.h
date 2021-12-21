#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include <iostream>

class roadDetector{

public:

  // constructors
  roadDetector(cv::Mat& src);

  roadDetector(std::string path, bool display = false);

  // function

  bool isEmpty();

  void getImage(cv::Mat& dst);

  void findEdges(cv::Mat& dst, double tresh1, double tresh2, int apertureSobel = 3, bool l2gradient = true, bool display = false);

  void findLines(std::vector<cv::Vec2f>& lines, double rho, double theta, int tresh, double srn = 0, double stn = 0, double min_theta = 0, double max_theta = CV_PI);

  void getLines(std::vector<cv::Point>& linesPts, cv::Mat& temp);

  void findCircles(std::vector<cv::Vec3f>& circles, int method, double dp, double minDist, double param1 = 100, double param2 = 100);

  void drawCircles(cv::Mat& temp, cv::Scalar color, int thick = cv::FILLED, int lineType = 8, int shift = 0);

private:

  cv::Mat originalImg;
  cv::Mat grayImg;
  cv::Mat edgesImg;
  std::vector<cv::Vec2f> linesPolar;
  std::vector<cv::Point> linesPoints;
  std::vector<cv::Vec3f> circlesInfo;

  void showImg(cv::Mat src, std::string name);
};
