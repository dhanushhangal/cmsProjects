
#include "../../lib/config.h"
#include "../../lib/trackingCorr.h"
using namespace jetTrack;
void corrMaker(){
	loadConfig();
	TFile *f = TFile::Open("../../dataSet/corrScan_Cymbal_noDCAcuts.root");
	auto a= new trackingCorr(f);
	a->Read();
	//do symmetrization;
//	a->loadDataTracks("../../dataSet/tracking/data_clsoure.root");
	a->getCorr("corrTable_cymbal_noDCAcuts.root");
	//a->getCorr("cymbalCorr_aux.root");
	a->showCorr(0,1);
}
