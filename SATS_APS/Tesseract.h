//#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <vector>
#include <string>
#include <tesseract\baseapi.h>
#include <leptonica\allheaders.h>

class Tesseract
{
private:
	std::string passport_name, surname;
	std::vector<std::string> name_vector;
	const char *passport_image;
	
public:
	Tesseract();
	bool OCR();
	bool compare_name_strict(std::string);
	bool compare_name_relax(std::string);
};