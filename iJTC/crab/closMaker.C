
#include <fstream>
#include <vector>
#include "lib/config.h"
#include "lib/trackingClosure.h"
//#include "lib/trackingCorr.h"
using namespace jetTrack;

void ReadFileList(std::vector<std::string> &my_file_names, TString file_of_names, bool debug);
void closMaker(int jobID=0, bool localTest =1){
	loadConfig();
	std::vector<std::string> file_names;   file_names.clear();
	TString filename;
	if(!localTest){
		ReadFileList( file_names, Form("job_input_file_list_%d.txt",jobID), true);
		filename  = file_names.at(0);
	}
	else 	filename = "/uscms_data/d3/xiaowang/sampleSet/MC_cymbal_tune_1.root";
	/* testing the closure scan
	*/
	TFile * f = TFile::Open(filename);
	TTree* tree = (TTree*) f->Get("mixing_tree");
	TFile *wf = TFile::Open("trkClos.root","recreate");
	auto t = new inputTree(tree);
	auto  a  =new trackingClosure(t);
	a->runScan();
	/* testing the closure scan*/
	/*
	TFile *f = TFile::Open(filename);
	auto  a  =new trackingClosure(f);
	a->Read();
	a->DrawClosure();
	TFile * f = TFile::Open("MC_cymbal_tune_1.root");
	TFile *wf = TFile::Open("corr.root","recreate");
	TTree* tree = (TTree*) f->Get("mixing_tree");
	auto t = new inputTree(tree);
	auto  a  =new trackingCorr(t);
	a->runScan();
	//get closure 
	TFile *f = TFile::Open("corr.root");
	auto  a  =new trackingCorr(f);
	a->Read();
	a->getCorr("getcorr.root");
	a->showCorr();
	*/
}

void ReadFileList(std::vector<std::string> &my_file_names, TString file_of_names, bool debug)
{
	ifstream file_stream(file_of_names);
	std::string line;
	my_file_names.clear();
	if( debug ) std::cout << "Open file " << file_of_names << " to extract files to run over" << std::endl;
	if( file_stream.is_open() ) {
		if( debug ) std::cout << "Opened " << file_of_names << " for reading" << std::endl;
		int line_num = 0;
		while( file_stream >> line ) {
			if( debug ) std::cout << line_num << ": " << line << std::endl;
			std::string tstring_line(line);
			tstring_line.erase(std::remove(tstring_line.begin(), tstring_line.end(), '"'), tstring_line.end());
			tstring_line.erase(std::remove(tstring_line.begin(), tstring_line.end(), ','), tstring_line.end());
			tstring_line.erase(std::remove(tstring_line.begin(), tstring_line.end(), '['), tstring_line.end());
			tstring_line.erase(std::remove(tstring_line.begin(), tstring_line.end(), ']'), tstring_line.end());
			if( tstring_line != "" ) my_file_names.push_back(tstring_line);
			line_num++;
		}
	} else {
		std::cout << "Error, could not open " << file_of_names << " for reading" << std::endl;
		assert(0);
	}
}
