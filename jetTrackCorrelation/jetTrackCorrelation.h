
#ifndef JETTRACKCORRELATION_H
#define JETTRACKCORRELATION_H

#include "inputTree.h"
#include "histJetTrackCorrelation.h"

class jetTrackCorrelation {
	public:
		long nentries; 
		inputTree* inputTree;
		std::vector<int > *config;
		std::vector<TString > *contral_seq;
		std::map<TString, int > seq;

		histJetTrackCorrelation* hj;
		std::vector<float> *jtpt;
		std::vector<float> *jteta;
		std::vector<float> *jtphi;

		jetTrackCorrelation(){
			//config = new std::vector<int> ();
			//contral_seq = new std::vector<TString> ();
			//contral_seq->push_back("doDijet");
			hj= new histJetTrackCorrelation();
		}

		void initialization();
		void run();
}

void ijetTrackCorrelation::nitialization(){
	jtpt = ;
}

void jetTrackCorrelation::run(){
	const double searchetacut = 2.0;
	const double pTmaxcut = 1000.;
	const double pTmincut = 120.;
	const double leadingjetcut = 120. ;
	const double subleadingjetcut = 50. ;
	const double dphicut = 5.*(TMath::Pi())/6. ; 
	const double trketamaxcut = 2.4;

	long nentries = inputTree.GetEntriesFase();
	for(int evi = 0; evi < nentries; evi++) {
		inputTree->GetEntry(evi);
		double Aj = -99.;
		double lead_pt=-1. ;
		double sublead_pt=-1. ;
		int second_highest_idx=-1 ;
		int highest_idx=-1 ;
		//search for leading jet
		if( doDijet){
			for(int j4i = 0; j4i < (int) jtpt->size() ; j4i++) {
				double jet_pt= jtpt->at(j4i);
				if(jet_pt<=leadingjetcut) continue ;
				if(jet_pt >lead_pt){
					sublead_pt=lead_pt;
					second_highest_idx = highest_idx;
					lead_pt=jet_pt;
					highest_idx=j4i;
				}
			} 
			if(TMath::Abs(jteta->at(highest_idx))>=searchetacut) continue ;
			if(highest_idx<0) continue;
		}
	}
}
