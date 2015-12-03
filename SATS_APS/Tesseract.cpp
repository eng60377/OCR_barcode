#include "Tesseract.h"

Tesseract::Tesseract()
{
	passport_image = "passport.jpg";
}

bool Tesseract::OCR()
{	
    tesseract::TessBaseAPI *api = new tesseract::TessBaseAPI();
    
	// Initialize tesseract-ocr with English, without specifying tessdata path
	//if (api->Init(NULL, "eng"))	
	
	if (api->Init("..\\tesseract\\tessdata", "eng"))
	{
        fprintf(stderr, "Could not initialize tesseract.\n");
        return 0;
    }
	api->SetVariable("tessedit_char_whitelist", "ABCDEFGHIJKLMNOPQRSTUVWXYZ<");
    // Open input image with leptonica library
	Pix *image = pixRead(passport_image);
    api->SetImage(image);
    // Get OCR result
    passport_name = api->GetUTF8Text();

    // Destroy used object and release memory
    api->End();
    pixDestroy(&image);

////////////////////////////////////////////////////////////////
	//Start of test code. Delete after test.
	//passport_name = "P<MYSMOHD<SIRATZULFITRY<SALIMAN<<<<<<";
	//End of test code.
//////////////////////////////////////////////////////////////

	//Extract country code from ocr result
	std::string country = passport_name.substr(2,3);

	//Extract name from passport ocr result
	int index;
	//if(country == "MYS") index = passport_name.find("<"); 
	//else 
	index = passport_name.find('<',5); //Look for the filler char '<' from 5th position onwards		
	//Cut out surname from passportname and save it in surname by reading from 6th character until you reach filler char "<"
	surname = passport_name.substr(5, index - 5);

	std::cout << "Name on passport: " << surname << ' '; 

	// Save each of the remaining names in string array
	int index_last = passport_name.find("<<<", index + 1); //Get the position of the last character in the passport name
	if(country != "MYS") //Malaysian passports have only one instead of two filler character '<' after the surname
		index++; //Increase index by 1 so that the same code can be used in the while loop later to get all the first names

	std::string first_names;
	int i = 0, index_current;
	//std::cout << "name on passport: ";
	while (index < index_last)
	{
		index_current = passport_name.find("<", index + 1);
		first_names = passport_name.substr(index + 1, index_current - index - 1);
		name_vector.push_back(first_names);
		std::cout << name_vector[i] << ' ';
		index = index_current;
		i++;
	}	
    return 1;
}

bool Tesseract::compare_name_strict(std::string boardingpass)
{
	bool match = true;
	std::string boarding_pass = boardingpass;
	//https://shaun.net/posts/whats-contained-in-a-boarding-pass-barcode

	//Convert boarding pass string to all caps.
	char *p1 = new char[boarding_pass.size() + 1];
    for(int i = 0; i < boarding_pass.size(); i++)
        p1[i] = toupper(boarding_pass[i]);
    p1[boarding_pass.size()] = '\0';
	boarding_pass = p1;

	// Look for surname in boarding pass name
	int board_sur = boarding_pass.find(surname);
	if (board_sur == -1)	
	{
		match = false;
		std::cout << "\nSurname not found.\n";
		//std::cout << "Name on passport: " << passport_name << std::endl; 
		std::cout << "String boarding_pass = " << boarding_pass << "\n";
	}
	else
	{
		//std::cout << "\nSurname found\n";
		int board_name;
		for(int i=0; i<name_vector.size(); i++)
		{
			//Exclude the words "bin", "binte", "binti", "bte" etc in search
			if(name_vector[i]!="BIN" && name_vector[i]!="BINTI" && name_vector[i]!="BINTE" && name_vector[i]!="BTE") 
			{
				board_name = boarding_pass.find(name_vector[i]);
				if (board_name == -1)	
				{
					std::cout << "The name " << name_vector[i] << " cannot be found on the boarding pass." << '\n';
					std::cout << "String boarding_pass = " << boarding_pass << "\n";
					match = false;
				}
			}
		}
	}
	return match;
}

bool Tesseract::compare_name_relax(std::string boardingpass)
{
	bool match = true;
	std::string boarding_pass = boardingpass;
	//https://shaun.net/posts/whats-contained-in-a-boarding-pass-barcode

	//Convert boarding pass string to all caps.
	char *p1 = new char[boarding_pass.size() + 1];
    for(int i = 0; i < boarding_pass.size(); i++)
        p1[i] = toupper(boarding_pass[i]);
    p1[boarding_pass.size()] = '\0';
	boarding_pass = p1;

	int board_name;
	for(int i=0; i<name_vector.size(); i++)
	{
		if(name_vector[i]!="BIN" && name_vector[i]!="BINTI" && name_vector[i]!="BINTE" && name_vector[i]!="BTE")
		{
			board_name = boarding_pass.find(name_vector[i]);
			if (board_name == -1)	
			{
				std::cout << "The name " << name_vector[i] << " cannot be found on the boarding pass." << '\n';
				match = false;
			}
		}
	}		

	return match;
}