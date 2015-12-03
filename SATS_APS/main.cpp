//Need to add "/FORCE:MULTIPLE" to linker command to remove error LNK2005:'already defined in main.obj'
//Need to rename the cpp files that have the same names. Otherwise some of them will be skipped during compilation.
//BRAND_NEW
#include <conio.h>

#include "boardingpass.h"

int main(int argc, char** argv) 
{
	Camera c;
	char terminate;
	while(1)
	{
		std::cout << "Scan boarding pass barcode.\n";
		if(!c.capture(1)) //Argument 1 is for boarding pass, 2 is for passport
		{
			std::cout << "Capture fail";
			system("pause");
			return -1;
		}
	
		std::string boardingpass = decode();

		std::cout << "Scan passport machine readable zone.\n";
		if(!c.capture(2))
		{
			system("pause");
			return -1;
		}

/////////////////////////////////////////////////////////////////////////////////////////////
//Insert code here to extract name from string boardingpass and save it back to string boardingpass
////////////////////////////////////////////////////////////////////////////////////////////
  		bool match = false;
		Tesseract s;
		if(!s.OCR())
		{
			std::cout<<"Error.";
			system("pause");
			return -1;
		}
		else match = s.compare_name_strict(boardingpass);

		if(match == false) 
		{
			std::cout << "\nNo match.\n";
			std::cout << "\nPress 'r' to recompare with relaxed criteria, 'x' to quit and any other key to scan the next passenger.\n";
		}
		else 
		{
			std::cout << "\nMatch.\n";
			std::cout << "\nPress 'x' to exit and any other key to scan the next passenger.\n";
		}
		terminate = getch();
		switch(terminate)
		{
		case 'x':
			std::cout << "Terminating...\n";
			break;
		case 'r':
			match = s.compare_name_relax(boardingpass);
			if(match == false) 
				std::cout << "\nStill no match.\n";				
			else
				std::cout << "Name partial match.\n";
			std::cout << "\nPress 'x' to exit and any other key to continue scanning.\n";
			terminate = getch();
			break;
		default:
			break;
		};
		if(terminate == 'x') break;
	} // end while(1)

	return 0;
}