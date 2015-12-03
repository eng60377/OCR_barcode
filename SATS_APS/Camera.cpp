#include "Camera.h"
#include <iostream>

using std::cout;

bool Camera::capture(int type)
{
	bool bSuccess = false;
	cv::VideoCapture cap(0); //0: Default camera, 1 - 99: other cameras attached
	if (!cap.isOpened()) 
	{
		cout << "Unable to connect to camera.\n";
		return bSuccess;
	}

	//double dWidth = cap.get(CV_CAP_PROP_FRAME_WIDTH); //get the width of frames of the video
	//double dHeight = cap.get(CV_CAP_PROP_FRAME_HEIGHT); //get the height of frames of the video	
	//cout << "Frame size : " << dWidth << " x " << dHeight << ".\n";
	cout << "Press the ESC key once the image is aligned and ready to be captured.\n";
	//cv::namedWindow("MyVideo",CV_WINDOW_AUTOSIZE); //create a window called "MyVideo"
	
	while(1)
	{
		bSuccess = cap.read(frame); // read a new frame from video

		if (!bSuccess) //if not success, break loop
		{
			cout << "Cannot read a frame from video stream" << ".\n";
			return bSuccess;
		}
		
		imshow("MyVideo", frame); //show the frame in "MyVideo" window

		if (cv::waitKey(30) == 27) //wait for 'esc' key press for 30ms. If 'esc' key is pressed, break loop
		{	
			bSuccess = cap.read(frame); 
			break;
		}
	}
	cv::destroyWindow("MyVideo");

	//bool bSuccess = cap.read(frame); // read a new frame from video
	if (!bSuccess) 
	{
		cout << "Unable to capture frame from camera.\n";	
		return bSuccess;
	}
    //compression_params.push_back(CV_IMWRITE_JPEG_QUALITY); //specify the compression technique
    //compression_params.push_back(98); //specify the compression quality
	//compression_params.push_back(CV_IMWRITE_PNG_COMPRESSION); //specify the compression technique
	//compression_params.push_back(9); //specify the compression quality
	switch(type) //Capture boarding pass or photo?
	{
	case 1:
//		cap >> destination; 
//		if(destination.empty())
//		{
//			std::cerr << "Something is wrong with the webcam, could not get frame." << std::endl;
//		}
//		else bSuccess = imwrite("D:/boardingpass.png", destination, compression_params);
		//frame.convertTo(destination, CV_16U);
		//bSuccess = imwrite("./boardingpass.bmp", frame, compression_params); //http://docs.opencv.org/doc/tutorials/introduction/load_save_image/load_save_image.html
		bSuccess = cvSaveImage("boardingpass.jpg", &(IplImage(frame)));
		break;
	case 2:
		//bSuccess = imwrite("./passport1.bmp", frame, compression_params);
		bSuccess = cvSaveImage("passport.jpg", &(IplImage(frame)));
		break;
	default:
		cout << "Invalid command.\n";
		return false;
		break;
	};
	if (!bSuccess) cout << "Error saving image to file.\n";

	return bSuccess;
}