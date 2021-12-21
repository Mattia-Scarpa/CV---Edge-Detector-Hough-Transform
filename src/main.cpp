#include <roadDetector.h>

using namespace std;
using namespace cv;


void findIntersection(vector<Point> pts, Point& intPt) {
  if (pts.size() == 4) { //verifying only 2 lines have been found!

    double x1 = pts[0].x, y1 = pts[0].y, x2 = pts[1].x, y2 = pts[1].y;
    double x3 = pts[2].x, y3 = pts[2].y, x4 = pts[3].x, y4 = pts[3].y;

    int den = (x1-x2)*(y3-y4)-(y1-y2)*(x3-x4);
    if (den != 0) {
      intPt.x = cvRound(((x1*y2-y1*x2)*(x3-x4)-(x1-x2)*(x3*y4-y3*x4))/den);
      intPt.y = cvRound(((x1*y2-y1*x2)*(y3-y4)-(y1-y2)*(x3*y4-y3*x4))/den);
    }
    else {
      cout << "Lines found are parallel!" << endl;
    }
  }
}


int main(int argc, char const *argv[]) {

  // Defining Image path
  string path = "../data/input.png";

  // creating dedicated class from image path
  roadDetector* road = new roadDetector(path);


  if (road->isEmpty()) {
    cout << "Failed to load image, please verify your image source path!" << endl;
  }
  else {
    cout << "Image found!" << endl;
  }

  cout << "Extracting original image..." << endl;
  Mat originalImg;
  road->getImage(originalImg);

  // displaying original image
  string wNameOriginal = "Source";
  namedWindow(wNameOriginal);
  imshow(wNameOriginal, originalImg);
  waitKey(0);

  // Initialize Edge detector parameters container
  int tresh1 = 749;
  int tresh2 = 160;
  // Initialize Lines and Cirlces detector parameters container
  int treshold = 136;
  int rho = 96;
  int theta = 180;
  int dp = 173;
  int minDist = 8;
  int param1 = 841;
  int param2 = 41;

  Mat sourceImg;
  Mat edgesImg;
  vector<Vec2f> lines;

  road->getImage(sourceImg);
  //road->findEdges(edgesImg, (double) tresh1, (double) tresh2);
  //road->findLines(lines, (double) rho, CV_PI/(180), treshold);

  string wNameEdges = "Image edges";
  namedWindow(wNameEdges);
  string wNameLines = "Image Lines";
  namedWindow(wNameLines);

  cout << "Tuning for edges, lines and circles detection! \nPress 'enter' to continue" << endl;

  vector<Point> pts;
  char k;

  while(k!=13) {

    createTrackbar("Treshold-1", wNameEdges, &tresh1, 1000);
    createTrackbar("Treshold-2", wNameEdges, &tresh2, 1000);

    createTrackbar("Treshold", wNameLines, &treshold, 499);
    createTrackbar("Rho Resolution", wNameLines, &rho, 250);
    createTrackbar("Theta Resolution", wNameLines, &theta, 359);
    createTrackbar("Circle Resolution", wNameLines, &dp, 200);
    createTrackbar("Circle\nDistance Resolution", wNameLines, &minDist, 250);
    createTrackbar("Canny detector\n Circle threshold", wNameLines, &param1, 1000);
    createTrackbar("Accumulator threshold", wNameLines, &param2, 200);

    road->findEdges(edgesImg, (double) tresh1, (double) tresh2);
    road->findLines(lines, (double) rho/100, CV_PI/(theta+1), (treshold+1));
    Mat temp = sourceImg.clone();
    pts.clear();
    road->getLines(pts, temp);

    vector<Vec3f> circles;
    road->findCircles(circles, HOUGH_GRADIENT, (double) dp/100, temp.rows/(minDist+1), (double) param1, (double) param2);
    road->drawCircles(temp, Scalar(0,255,0), 2);

    imshow(wNameEdges, edgesImg);
    imshow(wNameLines, temp);
    k = waitKey(1);
  }

  Point intersection;
  findIntersection(pts, intersection);

  vector<Point> roadPts;
  // To draw the road look for the bottom y point in the image (i.e., the bigger y coordinate)
  for (size_t i(0); i < pts.size(); i+=2) {
    if (pts[i].y >= pts[i+1].y)
      roadPts.push_back(pts[i]);
    else
      roadPts.push_back(pts[i+1]);
  }
  roadPts.push_back(intersection);

  fillConvexPoly(originalImg, roadPts, Scalar(0,0,255));
  road->drawCircles(originalImg, Scalar(0,255,0));
  imshow("Final Image", originalImg);
  waitKey(0);
  return 0;
}
