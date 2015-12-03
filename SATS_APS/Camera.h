//http://opencv-srf.blogspot.com/2011/09/capturing-images-videos.html
#include <opencv2/highgui/highgui.hpp>

class Camera
{
private:
	cv::Mat frame, destination;	
	std::vector<int> compression_params; //vector that stores the compression parameters of the image

public:
	bool capture(int);
};