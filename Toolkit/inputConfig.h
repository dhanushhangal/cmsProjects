
#ifndef INPUTCONFIG_H
#define INPUTCONFIG_H

#ifndef INPUTTREE_H
#include "inputTree.h"
#endif

class inputConfig{

	public: 
		inputConfig(inputTree* t);

		void getEntry(Long64_t jentry);
		int trkQualityCuts(int jtrk);
		int evtSelection(Long64_t jentry);
		float getVzWeight(float vz);

	private: 
		const int ncentbin = 50;
		const int nptbin  = 10;
		inputTree * tt;
		int    *centbin;
		float  *  ptbin;
		TString* ptbintitle;
};

inputConfig::inputConfig(inputTree*t){
	tt=t;
	int centBin[ncentbin];
	float ptBin[nptbin];
	centbin = centBin;
	ptbin = ptBin;
	
}

void inputConfig::getEntry(Long64_t jentry){
	tt->GetEntry(jentry);
	return;
}

int inputConfig::trkQualityCuts(int jtrk){
	if (tt->highPurity->at(jtrk)){
		float Et = (tt->pfHcal->at(jtrk)+tt->pfEcal->at(jtrk))/TMath::CosH(tt->trkEta->at(jtrk));
		if(!(tt->trkPt->at(jtrk)<20 || (Et>0.5*tt->trkPt->at(jtrk)))) return 1;
		else if ( abs(tt->trkDz->at(jtrk)/tt->trkDzError->at(jtrk)) >=3) return 1;
		else if ( abs(tt->trkDxy->at(jtrk)/tt->trkDxyError->at(jtrk)) >=3) return 1;
		else if ( tt->trkPtError->at(jtrk)/tt->trkPt->at(jtrk)>=0.1) return 1;
		else if ( tt->trkNHit->at(jtrk)< 11) return 1;
		else if ( (float)tt->trkChi2->at(jtrk)/(float)tt->trkNdof->at(jtrk)/(float)tt->trkNlayer->at(jtrk)>0.15) return 1;
		else return 0;
	}
	else return 1;
}

int inputConfig::evtSelection(Long64_t jentry){
		if ( tt->HBHENoiseFilterResultRun2Loose ==0) continue;
		if ( tt->pcollisionEventSelection ==0) continue;
		if ( tt->pprimaryVertexFilter ==0) continue;
		if ( tt->phfCoincFilter3 ==0) continue;
		if( abs(tt->vz) > 15) continue;
}
#endif
