
#ifndef inTree_H
#include "inTree.h"
#endif

#include "TMath.h"
#include "iostream"

int hiBin = -999;
float vz = -999;
float pthat = -999;
std::vector<float> trkPt, trkEta, trkPhi, trkPtError, trkDz, trkDzError, trkDxy, trkDxyError;
std::vector<float> genPt, genEta, genPhi, genCharge;

bool eventCuts(inTree *t){
	if ( t->HBHENoiseFilterResultRun2Loose ==0) return 1;
	if ( t->pcollisionEventSelection ==0) return 1;
	if ( t->pprimaryVertexFilter ==0) return 1;
	if ( t->phfCoincFilter3 ==0) return 1;
	if ( t->vz>15 || t->vz<-15) return 1;
	return 0;
}

bool trackCuts(inTree *t, int j){
	if (!t->highPurity->at(j)) return 1;
	float Et = (t->pfHcal->at(j)+t->pfEcal->at(j))/TMath::CosH(t->trkEta->at(j));
	if (!(t->trkPt->at(j)<20 || (Et>0.5*t->trkPt->at(j)))) return 1;
	if ( TMath::Abs(t->trkDz->at(j)/t->trkDzError->at(j)) >=3) return 1;
	if ( TMath::Abs(t->trkDxy->at(j)/t->trkDxyError->at(j)) >=3) return 1;
	if ( t->trkPtError->at(j)/t->trkPt->at(j)>=0.1) return 1;
	if ( t->trkPtError->at(j)/t->trkPt->at(j)>=0.3) return 1;
	if ( t->trkNHit->at(j)< 11) return 1;
	if ((float)t->trkChi2->at(j)/(float)t->trkNdof->at(j)/(float)t->trkNlayer->at(j)>0.15) return 1;
	return 0;
}

bool genTrackCuts(inTree *t, int j){
	return 0;
}

void miniTreeScan(TTree *intree , TString save_name, bool isMC=0){

	inTree* t = new inTree(intree, isMC);
	TFile *wf = TFile::Open(save_name, "recreate");
	TTree * outTree = new TTree("trackingMiniTree", "");

	outTree->Branch("hiBin", &hiBin);
	outTree->Branch("vz", &vz);
	outTree->Branch("trkPt", &trkPt);
	outTree->Branch("trkEta", &trkEta);
	outTree->Branch("trkPhi", &trkPhi);
	outTree->Branch("trkDz", &trkDz);
	outTree->Branch("trkDxy", &trkDxy);
	if( isMC){
		outTree->Branch("genPt", &genPt );
		outTree->Branch("genEta", &genEta );
		outTree->Branch("genPhi", &genPhi );
		outTree->Branch("genCharge", &genCharge );
	}

	Long64_t nentries = t->fChain->GetEntriesFast();
	for(Long64_t jentry = 0; jentry<nentries; ++jentry){
		if(jentry %1000==0) std::cout<<"processing event "<<jentry<<std::endl;
		t->GetEntry(jentry);
		if( eventCuts(t ) ) continue;
		hiBin = t->hiBin;
		vz = t->vz;
		if(isMC) pthat = t->pthat;
		for( int i=0; i<int(t->trkPt->size()); ++i){
			if( trackCuts(t, i)) continue;
			trkPt      .push_back(t->trkPt->at(i));
			trkEta     .push_back(t->trkEta->at(i));
			trkPhi     .push_back(t->trkPhi->at(i));
			trkDz      .push_back(t->trkDz->at(i));
			trkDxy     .push_back(t->trkDxy->at(i));
		}
		if( isMC){
			for(int i=0; i<int(t->pt->size()); ++i){
				if(genTrackCuts(t, i)) continue;
				genPt     .push_back(t->pt->at(i));
				genEta    .push_back(t->eta->at(i));
				genPhi    .push_back(t->phi->at(i));
				genCharge .push_back(t->chg->at(i));
			}
		}
		outTree->Fill();
		trkPt      .clear();
		trkEta     .clear();
		trkPhi     .clear();
		trkDz      .clear();
		trkDxy     .clear();
		if( isMC){
			genPt     .clear();
			genEta    .clear();
			genPhi    .clear();
			genCharge .clear();
		}
	}
	outTree->Write();

}

