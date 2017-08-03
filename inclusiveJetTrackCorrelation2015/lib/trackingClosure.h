
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
		trackingClosure(inputTree* tt):
			t(tt),
			ntrkpt(trackingClosureConfig::ntrkpt),
			ncent(trackingClosureConfig::ncent),
			nx(trackingClosureConfig::neta),
			ny(trackingClosureConfig::nphi),
			xmin(trackingClosureConfig::etamin),
			xmax(trackingClosureConfig::etamax),
			ymin(trackingClosureConfig::phimin),
			ymax(trackingClosureConfig::phimax)
	{
		trkpt=trackingClosureConfig::trkpt;
		cent=trackingClosureConfig::cent;
		gen = new xthf4("gen","gen", nx,xmin, xmax , ny, ymin, ymax,ntrkpt, trkpt,ncent, cent);
		rec = new xthf4("rec","rec", nx,xmin, xmax , ny, ymin, ymax,ntrkpt, trkpt,ncent, cent);
		cre = new xthf4("cre","cre", nx,xmin, xmax , ny, ymin, ymax,ntrkpt, trkpt,ncent, cent);
	};

		void runScan();

	public: 
		inputTree * t;
		int ntrkpt;
		int ncent;
		int nx, ny;
		float *trkpt, *cent;
		float xmin, xmax, ymin, ymax;
		xthf4 * gen;
		xthf4 * rec;
		xthf4 * cre;
};

void trackingClosure::runScan(){
	loadConfig();
	Long64_t nentries = t->fChain->GetEntriesFast();
	for(Long64_t jentry = 0; jentry <nentries ; ++jentry){
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
		for(int j=0;j<t->gen->size(); ++j){
			if(genParticleCuts(t,j))continue;
			gen->Fill(t->trkEta->at(j), t->trkPhi->at(j), t->trkPt->at(j), t->hiBin);
		}
	}
	gen->Write();
	rec->Write();
	cre->Write();
}
#endif
