#include <fstream>
#include <string>
#include "ImageReaderSource.h"
#include "Tesseract.h"
#include "Camera.h"
#include <zxing/MultiFormatReader.h>
#include <zxing/ReaderException.h>
#include <zxing/common/HybridBinarizer.h>

namespace {
bool use_hybrid = false;
bool use_global = false;
bool verbose = false;
std::string boardingpass;
}

std::vector<zxing::Ref<zxing::Result>> decode(zxing::Ref<zxing::BinaryBitmap>, zxing::DecodeHints);
int read_image(zxing::Ref<zxing::LuminanceSource>, bool, std::string);
std::string read_expected(std::string);
std::string decode();