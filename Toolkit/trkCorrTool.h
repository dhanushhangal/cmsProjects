
#ifndef TRKCORRTOOL_H
#define TRKCORRTOOL_H

#ifndef INPUTCONFIG_H
#include "inputConfig.h"
#endif

#ifndef INPUTTREE_H
#include "inputTree.h"
#endif


class trkCorrTool{
	public:
		inputTree *_t;
		trkCorrTool(inputTree * _it);
		void getCorrTable(TString svname, const int opt = 1);
};

trkCorrTool::trkCorrTool(intputTree* _it){
	_t = _it;
}

void trkCorrTool::getCorrTable(TString svname, const int opt){
	if(opt >3) {
		std::cout<<"wrong option number!"<<std::endl;
		return;
	}

	inputConfig* trkconfig = new inputConfig(_inputTree);
	int* centBin = trkconfig.centbin;
	const int ncentBin = trkconfig.ncentbin;
	int* ptBin = trkconfig.ptbin;
	TString* ptBinTitle = trkconfig.ptbintitle;
	const int nptBin = trkconfig.nptbin;

	TH2F* temp = new TH2F("temp", "", 100, -5, 5,\
			72,-TMath::Pi(),TMath::Pi());
	TH2F* gen [ncentBin][nptBin][3];
	TH2F* reco[ncentBin][nptBin][3];
	for ( int i = 0; i< ncentBin; i++){
		for ( int j=0; j<nptBin; j++){
			for ( int k=0; k<opt; k++){
				hgen [i][j][k]=(TH2F*)temp->Clone(Form("gen_%d_%d_%d",i,j,k));
				hreco[i][j][k]=(TH2F*)temp->Clone(Form("reco_%d_%d_%d",i,j,k));
				hgen [i][j][k]->SetTitle(Form("gen: cent in [%d,%d), ",centBin[i],centBin[i+1])+\
						ptBinTitle[j]+"<=pt<"+ptBinTitle[j+1]);
				hreco[i][j][k]->SetTitle(Form("reco: cent in [%d,%d), ",centBin[i],centBin[i+1])+\
						ptBinTitle[j]+"<=pt<"+ptBinTitle[j+1]);
				hgen [i][j][k]->Sumw2();
				hreco[i][j][k]->Sumw2();
			}
		}
	}
	int hi= -1, hj = -1;
	//weight will be store in an array
	// weight[0] = 1 //no weight at all;
	// weight[1] = vzWeight;
	// weight[2] = vzWeight* centWeight;
	float weight[2];
	weight[0]=1;
	Long64_t nentries = _t->fChain->GetEntriesFast();
	std::cout<<"started to filling!"<<std::endl;
	for( Long64_t jentry=0; jentry < nentries; jentry++){
		hi   =-1;
		hj   =-1;
		if( inputConfig.evtSelection(jentry)) continue;
		weight[1]=inputConfig.weight1(jentry);
		weight[2]=weight[1]*inputConfig.weight2(jentry);
		hi = inputConfig.centNumber(jentry);
		if( hi < 0) continue;
		// filling the gen particles--------------------------
		for ( int j =0; j< _t->pt->size(); j++){
			if(_t->chg->at(j) ==0) continue;
			hj = -1;
			for ( int k = 0; k< nptbin; k++) if(_t->pt->at(j) >= ptBin[k] && _t->pt->at(j) < ptBin[k+1]) {
				hj = k;
				break;
			}
			if ( hj <0) continue;
			for(int k=0;k<opt;k++){
				hgen[hi][hj][k]->Fill(_t->eta->at(j),_t->phi->at(j),weight[k]);
			}
		}
		//filling the reco ---------------------------------------
		for ( int j =0; j< t->trkPt->size(); j++){
			hj = -1;
			if(trkQualityCuts(j)) continue;
			for(int k=0;k<opt;k++){
				hreco[hi][hj][k]->Fill(_t->trkEta->at(j),_t->trkPhi->at(j),weight[k]);
			}
		}
	}
	TFile *wf = TFile::Open(svname,"recreate");
	for( int i =0; i<ncentBin; i++){
		for( int j =0; j<nptBin; j++){
			for ( int k = 0; k< opt; k++){
				hgen[i][j][k]->Write();
				hreco[i][j][k]->Write();
			}
		}
	}
	return;
}

#endif
