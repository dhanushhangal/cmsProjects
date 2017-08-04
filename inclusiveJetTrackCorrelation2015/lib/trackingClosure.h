
#ifndef config_H
#include "config.h"
#endif

#ifndef inputTree_H
#include "inputTree.h"
#endif

#ifndef xthf4_H
#include "xthf4.h"
#endif

#ifndef trackingClosure_H
#define trackingClosure_H
using namespace jetTrack;

class trackingClosure  {
	public: 
		trackingClosure(TFile * ff):
			f(ff),
			ntrkpt(trackingClosureConfig::ntrkpt),
			ncent(trackingClosureConfig::ncent),
			nx(trackingClosureConfig::neta),
			ny(trackingClosureConfig::nphi),
			xmin(trackingClosureConfig::etamin),
			xmax(trackingClosureConfig::etamax),
			ymin(trackingClosureConfig::phimin),
			ymax(trackingClosureConfig::phimax),
			trkpt(trackingClosureConfig::trkpt),
			cent(trackingClosureConfig::cent)
	{
		t=NULL;
		isread = 1;
		gen = new xthf4();
		rec = new xthf4();
		cre = new xthf4();
	};
		trackingClosure(inputTree* tt):
			t(tt),
			ntrkpt(trackingClosureConfig::ntrkpt),
			ncent(trackingClosureConfig::ncent),
			nx(trackingClosureConfig::neta),
			ny(trackingClosureConfig::nphi),
			xmin(trackingClosureConfig::etamin),
			xmax(trackingClosureConfig::etamax),
			ymin(trackingClosureConfig::phimin),
			ymax(trackingClosureConfig::phimax),
			trkpt(trackingClosureConfig::trkpt),
			cent(trackingClosureConfig::cent)
	{
		f=NULL;
		isread = 0;
		gen = new xthf4("gen","gen", nx,xmin, xmax , ny, ymin, ymax,ntrkpt, trkpt,ncent, cent);
		rec = new xthf4("rec","rec", nx,xmin, xmax , ny, ymin, ymax,ntrkpt, trkpt,ncent, cent);
		cre = new xthf4("cre","cre", nx,xmin, xmax , ny, ymin, ymax,ntrkpt, trkpt,ncent, cent);
	};

		void runScan();
		void Read();
		void DrawClosure(TString name="", TString type= "");
		~trackingClosure();

	public: 
		TFile *f;
		inputTree * t;
		int ntrkpt;
		int ncent;
		int nx, ny;
		float *trkpt, *cent;
		float xmin, xmax, ymin, ymax;
		xthf4 * gen;
		xthf4 * rec;
		xthf4 * cre;
		bool isread;
};

void trackingClosure::runScan(){
	loadConfig();
	Long64_t nentries = t->fChain->GetEntriesFast();
	for(Long64_t jentry = 0; jentry <nentries ; ++jentry){
		if(jentry %1000 == 0) std::cout<<"processing "<<jentry<<"th event.."<<std::endl;
		t->GetEntry(jentry);
		float w_vz=1;
		if(eventCuts(t)) continue;
		w_vz = weight::vz_weight(t->vz);
		for(int j=0;j<t->trkPt->size(); ++j){
			if(trackQualityCuts(t,j))continue;
			float trkcorr = correction::trk_corr(t, j);
			rec->Fill(t->trkEta->at(j), t->trkPhi->at(j), t->trkPt->at(j), t->hiBin, w_vz);
			cre->Fill(t->trkEta->at(j), t->trkPhi->at(j), t->trkPt->at(j), t->hiBin, w_vz*trkcorr);
		}
		for(int j=0;j<t->pt->size(); ++j){
			if(genParticleCuts(t,j))continue;
			gen->Fill(t->eta->at(j), t->phi->at(j), t->pt->at(j), t->hiBin, w_vz);
		}
	}
	gen->Write();
	rec->Write();
	cre->Write();
}

void trackingClosure::Read(){
	if(f==NULL) {
		std::cout<<"trackingClosure contained histograms, can't read anymore"<<std::endl;
		return ;
	}
	int ana_ntrkpt=anaConfig::ntrkpt;
	int ana_ncent=anaConfig::ncent;
	float* ana_trkpt = anaConfig::trkpt;
	float* ana_cent = anaConfig::cent;
	gen->Read("gen", "gen", f, ntrkpt, trkpt, ncent, cent);
	rec->Read("rec", "rec", f, ntrkpt, trkpt, ncent, cent);
	cre->Read("cre", "cre", f, ntrkpt, trkpt, ncent, cent);
	gen->RebinZ(ana_ntrkpt, ana_trkpt);
	rec->RebinZ(ana_ntrkpt, ana_trkpt);
	cre->RebinZ(ana_ntrkpt, ana_trkpt);
	gen->RebinW(ana_ncent, ana_cent);
	rec->RebinW(ana_ncent, ana_cent);
	cre->RebinW(ana_ncent, ana_cent);
}

void trackingClosure::DrawClosure(TString name="", TString type= ""){
}

trackingClosure::~trackingClosure(){
	delete gen;
	delete rec; 
	delete cre; 
}
#endif
