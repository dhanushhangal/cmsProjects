
#ifndef HISTJETTRACKCORRELATION_H
#define HISTJETTRACKCORRELATION_H

#include "histCollection.h"

class histJetTrackCorrelation : public histCollection{
	public:
		std::vector<TString > * centLabel;
		std::vector<TString > * trkPtLabel;
		std::vector<TString > * jetPtLabel;
		histJetTrackCorrelation(): histCollection(){
			centLabel  = new std::vector<TString >();
			trkPtLabel = new std::vector<TString >();
			jetPtLabel = new std::vector<TString >();
			centLabel ->push_back("cent0");
			centLabel ->push_back("cent10");
			centLabel ->push_back("cent30");
			centLabel ->push_back("cent50");
			centLabel ->push_back("cent100");
			trkPtLabel->push_back("trkPt0p5");
			trkPtLabel->push_back("trkPt0p7");
			trkPtLabel->push_back("trkPt1");
			trkPtLabel->push_back("trkPt2");
			trkPtLabel->push_back("trkPt3");
			trkPtLabel->push_back("trkPt4");
			trkPtLabel->push_back("trkPt8");
			trkPtLabel->push_back("trkPt12");
			trkPtLabel->push_back("trkPt16");
			trkPtLabel->push_back("trkPt20");
			trkPtLabel->push_back("trkPt999");
			jetPtLabel->push_back("jetPt120");
			jetPtLabel->push_back("jetPt300");
			ncent = centLabel->size()-1;
			npt = trkPtLabel->size()-1;
			njtpt = jetPtLabel->size()-1;
		}
		void addCorrelationSet(TString caption);
		void addInclusiveSet(bool doMix=true);
		void addJetSpectra(TString caption);
		bool labelCheck(int ncent, int npt, int njtpt);

	private: 
		int ncent, npt, njtpt;
};

#endif


void histJetTrackCorrelation::addCorrelationSet(TString caption){
	TString stmp;
	for(int i=0; i<ncent;++i){
		for(int j=0;j<npt; ++j){
			for(int k=0;k<njtpt;++k){
				stmp = caption+"_"+centLabel->at(i)+"_"+centLabel->at(i+1)\
					+"_"+trkPtLabel->at(j)+"_"+trkPtLabel->at(j+1)\
					+"_"+jetPtLabel->at(k)+"_"+jetPtLabel->at(k+1);
				addHist(stmp, t_eta_phi_correlation, "");
			}
		}
	}
}

void histJetTrackCorrelation::addInclusiveSet(bool doMix){
	addCorrelationSet("Inclusive");
	addCorrelationSet("Inclusive_pTweighted");
	if( doMix) addCorrelationSet("Mixing");
	if( doMix) addCorrelationSet("Mixing_pTweighted");
	addJetSpectra("InclusiveJet");
	return;
}

void histJetTrackCorrelation::addJetSpectra(TString caption){
	if( centLabel->size()<ncent+1 ) {
		std::cout<<"centLabel dosen't ready"<<endl;
		return;
	}
	TString stmp;
	for(int i=0;i<ncent;++i){
		stmp = caption+"_"+centLabel->at(i)+"_"+centLabel->at(i+1)+"_"+"pt";
		addHist(stmp, t_jtpt);
		stmp = caption+"_"+centLabel->at(i)+"_"+centLabel->at(i+1)+"_"+"eta";
		addHist(stmp, t_eta);
		stmp = caption+"_"+centLabel->at(i)+"_"+centLabel->at(i+1)+"_"+"phi";
		addHist(stmp, t_phi);
	}
}
