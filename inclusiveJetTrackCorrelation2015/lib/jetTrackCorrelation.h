
#ifndef jetTrackCorrelation_H
#define jetTrackCorrelation_H

#ifndef xTree_H
#include "xTree.h"
#endif

#ifndef ROOT_TH1
#include "TH1.h"
#endif

#ifndef ROOT_TH2
#include "TH2.h"
#endif

#ifndef ROOT_TH2D
#include "TH2D.h"
#endif

#ifndef ROOT_TMath
#include "TMath.h"
#endif

#include "xthd5.h"
#include "TBenchmark.h"

using namespace jetTrack;

class jetTrackCorrelation{
	public : 
		jetTrackCorrelation(TString name, TString tname, bool isMC, bool ispp);
		void data_correlation();
		void bookHisto(bool isMC, bool ispp);
		void runAnalyzer();
		void PbPbAnalyzer();
		void jetInform(int j);
		void getJetAxis(int j);
		void getTrk(int j);
		void getEventWeight(Long64_t j);
		void fillCorrelation();
		void addMixing(xTree* t);
		void Write();
	public : 
		xthd5* gen;
		xthd5* rec;
		int ntrkpt;
		int ncent;
		int njetpt;
		int ndeta, ndphi;
		float dphilow, dphiup, detalow, detaup;
		float *trkpt, *jetpt, *cent;
		bool isMC, ispp;
		TFile *signal_f;
		TTree* tree;
		xTree* t;
		TChain* me_tree;

		float jet_pt, jet_eta, jet_phi;

		xthd5* recSignal, *recSignal_pTweighted, *recME_pt, *recME, *genSignal,*genSignal_pTweighted, *genME_pt, *genME;
		TH2D* recJetPt, *recJetEta,*recJetPhi, *genJetPt, *genJetEta, *genJetPhi;
		float wvz    =1; 
		float wcent  =1; 
		float wpthat =1; 
		float trk_correction=1;
		float trk_pt, trk_eta, trk_phi;
};

jetTrackCorrelation::jetTrackCorrelation(TString name,TString tname,  bool is_MC, bool is_pp):
	ntrkpt (correlationConfig::ntrkpt ),
	ncent  (correlationConfig::ncent  ),
	njetpt (correlationConfig::njetpt ),
	ndeta  (correlationConfig::ndeta  ),
	ndphi  (correlationConfig::ndphi  ),
	dphilow(correlationConfig::dphilow),
	dphiup (correlationConfig::dphiup ),
	detalow(correlationConfig::detalow),
	detaup (correlationConfig::detaup ),
	trkpt  (correlationConfig::trkpt  ),
	jetpt  (correlationConfig::jetpt  ),
	cent   (correlationConfig::cent   ),
	isMC   (is_MC  ),
	ispp   (is_pp  )
{
	signal_f=TFile::Open(name);
	tree = (TTree*) signal_f->Get(tname);
	t = new xTree(tname, signal_f, is_MC, is_pp);
	me_tree = new TChain(tname);
}

void jetTrackCorrelation::bookHisto(bool isMC, bool ispp){
	Double_t centbin[5] = {0,10, 30, 50, 100};
	if(!ispp){
		recSignal=new xthd5("recSignal","", ndeta, detalow, detaup, ndphi, dphilow, dphiup, ntrkpt, trkpt, ncent, cent, njetpt, jetpt, "trkpt", "cent", "jetpt");
		recSignal_pTweighted=new xthd5("recSignal_pTweighted","", ndeta, detalow, detaup, ndphi, dphilow, dphiup, ntrkpt, trkpt, ncent, cent, njetpt, jetpt, "trkpt", "cent", "jetpt");
		recME_pt=new xthd5("recME_pTweighted","", ndeta, detalow, detaup, ndphi, dphilow, dphiup, ntrkpt, trkpt, ncent, cent, njetpt, jetpt, "trkpt", "cent", "jetpt");
		recME=new xthd5("recME","", ndeta, detalow, detaup, ndphi, dphilow, dphiup,ntrkpt, trkpt, ncent, cent, njetpt, jetpt, "trkpt", "cent", "jetpt");
		recJetPt =new TH2D("recJetPt",  "jet pt-hiBin", 50, 0, 500, 4, centbin);
		recJetEta=new TH2D("recJetEta", "jet eta-hiBin", 100, -5, 5, 4, centbin);
		recJetPhi=new TH2D("recJetPhi", "jet phi-hiBin", 72, -TMath::Pi(), TMath::Pi(), 4, centbin);
		if(isMC){
			genSignal=new xthd5("genSignal_pTweighted","gen", ndeta, detalow, detaup, ndphi, dphilow, dphiup,
					ntrkpt, trkpt, ncent, cent, njetpt, jetpt, "trkpt", "cent", "jetpt");
			genME_pt=new xthd5("genMC_pTweighted","gen", ndeta, detalow, detaup, ndphi, dphilow, dphiup,
					ntrkpt, trkpt, ncent, cent, njetpt, jetpt, "trkpt", "cent", "jetpt");
			genME=new xthd5("genMC","gen", ndeta, detalow, detaup, ndphi, dphilow, dphiup,
					ntrkpt, trkpt, ncent, cent, njetpt, jetpt, "trkpt", "cent", "jetpt");
			genJetPt =new TH2D("genJetPt",  "jet pt-hiBin", 50, 0, 500, 4, centbin);
			genJetEta=new TH2D("genJetEta", "jet eta-hiBin", 100, -5, 5, 4, centbin);
			genJetPhi=new TH2D("genJetPhi", "jet phi-hiBin", 72, -TMath::Pi(), TMath::Pi(), 4, centbin);
		}
	}
}

void jetTrackCorrelation::runAnalyzer(){
	PbPbAnalyzer();
}

void jetTrackCorrelation::PbPbAnalyzer(){
	loadConfig();
	bookHisto(isMC,ispp);

	TBenchmark ben;
	ben.Start("job");
//	Long64_t nentries = t->tree->GetEntriesFast();
	Long64_t nentries = 101;
	int counter =0;
	for(Long64_t jentry=0; jentry<nentries; ++jentry){
		if( jentry % 100==0) {
			std::cout<<"finished "<<jentry<<" events..."<<std::endl;
			ben.Show("job");
			ben.Start("job");
		}
		t->GetEntry(jentry);

		if(eventCuts(t)){ 
			continue;
		}
		getEventWeight(jentry);


		for(int j_jet=0; j_jet<t->corrpt->size(); ++j_jet){
			if(jetCut(t,j_jet)) continue;
			getJetAxis(j_jet);
			//filling the jet information:
			jetInform(j_jet);
		counter=0;
			for(int j_trk=0; j_trk<t->trkPt->size(); ++j_trk){
				if(trackQualityCuts(t,j_trk)) continue;
		counter++;
				getTrk(j_trk);
				//trk_correction=correction::trk_corr(t,j_trk);
				fillCorrelation();
			/*
				*/
			}
		cout<<"counter="<<counter<<endl;
		}
	}
}

void jetTrackCorrelation::getEventWeight(Long64_t j){
	if(isMC) {
		if(ispp) {
		}
		else {
			wvz=jetTrack::weight::vz_weight(t->vz);
			wcent=jetTrack::weight::cent_weight(t->hiBin);
		}
	}
}

void jetTrackCorrelation::getJetAxis(int j){
	if(isMC) {
	}
	else {
		jet_pt=t->corrpt->at(j);
		jet_eta=t->jteta->at(j);
		jet_phi=t->jtphi->at(j);
	}
}

void jetTrackCorrelation::jetInform(int j){
	recJetPt ->Fill(jet_pt , t->hiBin   ,wvz*wcent*wpthat);
	recJetEta->Fill(jet_eta, t->hiBin   ,wvz*wcent*wpthat);
	recJetPhi->Fill(jet_phi, t->hiBin   ,wvz*wcent*wpthat);
}

void jetTrackCorrelation::getTrk(int j){
	trk_pt=t->trkPt->at(j);
	trk_eta=t->trkEta->at(j);
	trk_phi=t->trkPhi->at(j);
}

void jetTrackCorrelation::fillCorrelation(){
	float deta = jet_eta-trk_eta;
	float dphi = jet_phi-trk_phi;
	if(dphi>(1.5*TMath::Pi())){dphi+= -2*TMath::Pi();}
	if(dphi<(-0.5*TMath::Pi())){dphi+= 2*TMath::Pi();}
	recSignal_pTweighted->Fill(deta, dphi,trk_pt,t->hiBin,jet_pt, wvz*wcent*wpthat*trk_correction*trk_pt);
	//recSignal->Fill(deta, dphi,trk_pt,t->hiBin,jet_pt, wvz*wcent*wpthat*trk_correction);
	recSignal->Fill(deta, dphi,trk_pt,t->hiBin,jet_pt, 1);
}

void jetTracCorrelation::addMixing(xTree* t){
	me_tree->Add();
}
void jetTrackCorrelation::Write(){
	if(ispp){
	}
	else {
		recJetPt->Write();
		recJetEta->Write();
		recJetPhi->Write();
		recSignal->Write();
		recSignal_pTweighted->Write();
	}
}
#endif
