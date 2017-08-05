
#ifndef config_H
#include "config.h"
#endif

#ifndef inputTree_H
#include "inputTree.h"
#endif

#ifndef xthf4_H
#include "xthf4.h"
#endif

#ifndef trackingCorr_H
#define trackingCorr_H
using namespace jetTrack;

class trackingCorr  {
	public: 
		trackingCorr(inputTree* tt):
			t(tt),
			ntrkpt(trackingCorrConfig::ntrkpt_in),
			ncent (trackingCorrConfig::ncent_in),
			trkpt (trackingCorrConfig::trkpt_in),
			cent  (trackingCorrConfig::cent_in),
			nx    (trackingCorrConfig::neta),
			ny    (trackingCorrConfig::nphi),
			xmin  (trackingCorrConfig::etamin),
			xmax  (trackingCorrConfig::etamax),
			ymin  (trackingCorrConfig::phimin),
			ymax  (trackingCorrConfig::phimax)
	{
		readonly =0;
		gen = new xthf4("gen","gen", nx,xmin, xmax , ny, ymin, ymax,ntrkpt, trkpt,ncent, cent);
		rec = new xthf4("rec","rec", nx,xmin, xmax , ny, ymin, ymax,ntrkpt, trkpt,ncent, cent);
	};
		trackingCorr(TFile *ff ):
			ntrkpt(trackingCorrConfig::ntrkpt_in),
			ncent (trackingCorrConfig::ncent_in),
			trkpt (trackingCorrConfig::trkpt_in),
			cent  (trackingCorrConfig::cent_in),
			nx    (trackingCorrConfig::neta),
			ny    (trackingCorrConfig::nphi),
			xmin  (trackingCorrConfig::etamin),
			xmax  (trackingCorrConfig::etamax),
			ymin  (trackingCorrConfig::phimin),
			ymax  (trackingCorrConfig::phimax),
			f(ff)
	{
		readonly = 1;
	}

		void runScan();
		void Read();
		void getCorr(TString file);

	public: 
		inputTree * t;
		int ntrkpt;
		int ncent;
		int nx, ny;
		float *trkpt, *cent;
		float xmin, xmax, ymin, ymax;
		xthf4 * gen;
		xthf4 * rec;
		bool readonly;
		TFile* f;
};

void trackingCorr::runScan(){
	Long64_t nentries = t->fChain->GetEntriesFast();
	for(Long64_t jentry = 0; jentry <nentries ; ++jentry){
		t->GetEntry(jentry);
		float w_vz=1;
		if(eventCuts(t)) continue;
		w_vz = weight::vz_weight(t->vz);
		for(int j=0;j<t->trkPt->size(); ++j){
			if(trackQualityCuts(t,j))continue;
			rec->Fill(t->trkEta->at(j), t->trkPhi->at(j), t->trkPt->at(j), t->hiBin, w_vz);
		}
		for(int j=0;j<t->gen->size(); ++j){
			if(genParticleCuts(t,j))continue;
			gen->Fill(t->trkEta->at(j), t->trkPhi->at(j), t->trkPt->at(j), t->hiBin);
		}
	}
	gen->Write();
	rec->Write();
}

void trackingCorr::Read(){
	if(!readonly) {
		std::cout<<"already filled by obj, can't load anymore!"<<std::endl;
		return 0;
	}
	gen->Read("gen", "gen", f, ntrkpt, trkpt, ncent, cent);
	rec->Read("rec", "rec", f, ntrkpt, trkpt, ncent, cent);
	return ;
}

void trackingCorr::getCorr(TString file){
	TFile* wf = TFile::Open(file, "recreate");
	int ntrkpt_out = trackingCorrConfig::ntrkpt_out;
	int ncent_out = trackingCorrConfig::ncent_out;
	float *trkpt_out = trackingCorrConfig::trkpt_out;
	float *cent_out = trackingCorrConfig::cent_out;
	gen->RebinZ(ntrkpt_out, trkpt_out);
	rec->RebinZ(ntrkpt_out, trkpt_out);
	gen->RebinW(ncent_out, cent_out);
	rec->RebinW(ncent_out, cent_out);

	int nhist = ntrkpt_out*ncent_out;
	auto corr= new TH2F*[nhist];
	for(int i=0; i<ntrkpt_out; ++i){
		for(int j=0; i<ncent_out; ++i){
			corr[i+j*ntrkpt]= (TH2F*) gen->hist(i,j)->Clone(Form("corr_%d_%d",i,j));
			corr[i+j*ntrkpt]->Divide( (TH2F*) rec->hist(i,j));
			corr[i+j*ntrkpt]->Write();
		}
	}
	wf->Cloes();
}

#endif
