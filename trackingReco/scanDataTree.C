
#include "inTree.h"
#include "miniTreeScan.h"

void ReadFileList(std::string & line, TString file_of_names, bool debug);

void scanDataTree(int jobID=0, bool isMC=0, bool localTest = 1){


	std::string filename;
	if(localTest) filename = "Data2015_25.root";
	else ReadFileList( filename, Form("job_input_file_list_%d.txt",jobID), true);
	TFile *f = TFile::Open(filename.c_str());
	TTree * t = (TTree*)f->Get("mixing_tree");
	miniTreeScan(t , "minTree.root", isMC);

}
void ReadFileList(std::string & tstring_line, TString file_of_names, bool debug)
{
	ifstream file_stream(file_of_names);
	if( debug ) std::cout << "Open file " << file_of_names << " to extract files to run over" << std::endl;
	if( file_stream.is_open() ) {
		file_stream >> tstring_line;
		if( debug ) std::cout << "Opened " << file_of_names << " for reading" << std::endl;
		tstring_line.erase(std::remove(tstring_line.begin(), tstring_line.end(), '"'), tstring_line.end());
		tstring_line.erase(std::remove(tstring_line.begin(), tstring_line.end(), ','), tstring_line.end());
		tstring_line.erase(std::remove(tstring_line.begin(), tstring_line.end(), '['), tstring_line.end());
		tstring_line.erase(std::remove(tstring_line.begin(), tstring_line.end(), ']'), tstring_line.end());
	} else {
		std::cout << "Error, could not open " << file_of_names << " for reading" << std::endl;
		assert(0);
	}
	std::cout<<"going to read the root file "<<tstring_line<<std::endl;
}
