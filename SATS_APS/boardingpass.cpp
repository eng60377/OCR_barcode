#include "boardingpass.h"

std::vector<zxing::Ref<zxing::Result>> decode(zxing::Ref<zxing::BinaryBitmap> image, zxing::DecodeHints hints) 
{
	zxing::Ref<zxing::Reader> reader(new zxing::MultiFormatReader);
	return std::vector<zxing::Ref<zxing::Result> >(1, reader->decode(image, hints));
}


int read_image(zxing::Ref<zxing::LuminanceSource> source, bool hybrid, std::string expected) 
{
	std::vector<zxing::Ref<zxing::Result>> results;
	std::string cell_result;
	int res = -1;
	
	try 
	{
		zxing::Ref<zxing::Binarizer> binarizer;
		if (hybrid) 
		{
		  binarizer = new zxing::HybridBinarizer(source);
		  //std::cout << "Using HybridBinarizer\n";
		} 
		else 
		{
		  binarizer = new zxing::GlobalHistogramBinarizer(source);
		  //std::cout << "Using GlobalHistogramBinarizer\n";
		}
		//zxing::DecodeHints hints(zxing::DecodeHints::DEFAULT_HINT);
		zxing::DecodeHints hints(zxing::DecodeHints::PDF_417_HINT);
		//hints.setTryHarder(false);
		hints.setTryHarder(true);
		zxing::Ref<zxing::BinaryBitmap> binary(new zxing::BinaryBitmap(binarizer));

		results = decode(binary, hints);

		res = 0;
	} 
	catch (const zxing::ReaderException& e) 
	{
		cell_result = "zxing::ReaderException: " + std::string(e.what());
		res = -2;
	} 
	catch (const zxing::IllegalArgumentException& e) 
	{
		cell_result = "zxing::IllegalArgumentException: " + std::string(e.what());
		res = -3;
	} 
	catch (const zxing::Exception& e) 
	{
		cell_result = "zxing::Exception: " + std::string(e.what());
		res = -4;
	} 
	catch (const std::exception& e) 
	{
		cell_result = "std::exception: " + std::string(e.what());
		res = -5;
	}

	/*if (test_mode && results.size() == 1) 
	{
		std::string result = results[0]->getText()->getText();
		if (expected.empty()) 
		{
		  cout << "  Expected text or binary data for image missing." << endl << "  Detected: " << result << endl;
		  res = -6;
		} 
		else 
		{
		  if (expected.compare(result) != 0) 
		  {
			cout << "  Expected: " << expected << endl << "  Detected: " << result << endl;
			cell_result = "data did not match";
			res = -6;
		  }
		}
	}*/

	//if (res != 0 && (verbose || (use_global ^ use_hybrid))) 
	//{
	//	cout << (hybrid ? "Hybrid" : "Global") << " binarizer failed: " << cell_result << endl;
	//} 
	//else if (!test_mode) 
	//{
		//if (verbose) 
		//{
		//  cout << (hybrid ? "Hybrid" : "Global") << " binarizer succeeded: " << endl;
		//}
		for (size_t i = 0; i < results.size(); i++) 
		{
		 // if (more) 
		 // {
			//cout << "  Format: " << BarcodeFormat::barcodeFormatNames[results[i]->getBarcodeFormat()] << endl;
			//for (int j = 0; j < results[i]->getResultPoints()->size(); j++) 
			//{
			//  cout << "  Point[" << j <<  "]: " << results[i]->getResultPoints()[j]->getX() << " "
			//	   << results[i]->getResultPoints()[j]->getY() << endl;
			//}
		 // }
		 // if (verbose) 
		 // {
			//cout << "    ";
		 // }
			boardingpass = results[i]->getText()->getText();
			//std::cout << results[i]->getText()->getText() << "\n";
			//std::cout << "barcode format: " << results[i]->getBarcodeFormat() << "\n";
		}
//	}
	std::cout << cell_result;
	return res;
}

std::string read_expected(std::string imagefilename) 
{
	std::string textfilename = imagefilename;
	std::string::size_type dotpos = textfilename.rfind(".");

  textfilename.replace(dotpos + 1, textfilename.length() - dotpos - 1, "txt");
  std::ifstream textfile(textfilename.c_str(), std::ios::binary);
  textfilename.replace(dotpos + 1, textfilename.length() - dotpos - 1, "bin");
  std::ifstream binfile(textfilename.c_str(), std::ios::binary);
  std::ifstream *file = 0;
  if (textfile.is_open()) {
    file = &textfile;
  } else if (binfile.is_open()) {
    file = &binfile;
  } else {
    return std::string();
  }
  file->seekg(0, std::ios_base::end);
  size_t size = size_t(file->tellg());
  file->seekg(0, std::ios_base::beg);

  if (size == 0) {
    return std::string();
  }

  char* data = new char[size + 1];
  file->read(data, size);
  data[size] = '\0';
  std::string expected(data);
  delete[] data;

  return expected;
}

std::string decode()
{
	std::string filename = "boardingpass.jpg";
	
	if (!use_global && !use_hybrid) 
	{
	  use_global = use_hybrid = true;
	}

	zxing::Ref<zxing::LuminanceSource> source;
	try 
	{
	  source = ImageReaderSource::create(filename);			 
	} 
	catch (const zxing::IllegalArgumentException &e) 
	{
		std::cerr << e.what() << " (ignoring)" << std::endl;
	  system("pause");
	  return "error";
	}

	std::string expected = read_expected(filename);

	int gresult = 1;
	int hresult = 1;
	if (use_hybrid) 
	{
	  hresult = read_image(source, true, expected);
	}
	if (use_global && (verbose || hresult != 0)) 
	{
	  gresult = read_image(source, false, expected);

	  if (!verbose && gresult != 0) 
	  {
		  std::cout << "decoding failed" << std::endl;
		  system("pause");
		  exit(1);
	  }
	}
	return boardingpass;
}