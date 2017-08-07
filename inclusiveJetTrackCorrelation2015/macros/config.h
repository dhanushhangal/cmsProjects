

#define config_H
#include "TMath.h"
#include "../lib/inputTree.h"
#include "../lib/xthf4.h"
//#include "../dataSet/corrTableCymbal/xiaoTrkCorr.h"

namespace jetTrack{
	namespace ioconfig{
		TString trackingOutput = "../hist/";
	}
	namespace weight {
		TF1* fvz   = new TF1("fvz","pol6",-15,15);
		TF1* fcent1= new TF1("fcent1","[0]+[1]*x+[2]*x^2+[3]*x^3+[4]*x^4+[7]*exp([5]+[6]*x)",0,180);
		float cent_weight(int hiBin ){
			     return (hiBin < 194) ? fcent1->Eval(hiBin) : 1;
		}
		float vz_weight(float vz){
			return fvz->Eval(vz);
		}
	}

	namespace correction{
	//	auto trkc = new xiaoTrkCorr("../dataSet/corrTableCymbal/inputCorr_cymbalTune.root");
	//	float trk_corr(inputTree *t, int j ){
	//		return trkc->getTrkCorr(t->trkPt->at(j), t->trkEta->at(j), t->trkPhi->at(j),t->hiBin);	
	//	}
	}

	namespace trackingClosureConfig{
		int ntrkpt = 9;
		float trkpt_c[10]= {0.7, 1, 2, 3, 4, 8, 12, 16,20,999};
		int ncent = 50; 
		float cent[51];// initiation in load()
		int neta= 100;
		int nphi= 72;
		float etamin = -5, etamax =5;
		float phimin = -TMath::Pi(), phimax = TMath::Pi() ;
		float* trkpt = trkpt_c;
	}

	namespace trackingCorrConfig{
		int ntrkpt_in = 11;
		float trkpt_in_c[12]= {0.7,0.8, 0.9, 1, 2, 3, 4, 8, 12, 16,20,999};
		int ntrkpt_out = 11;
		float trkpt_out_c[12]= {0.7,0.8, 0.9,1, 2, 3, 4, 8, 12, 16,20,999};
		int ncent_in = 50; 
		float cent_in_c[51]; 
		int ncent_out = 4; 
		float cent_out_c[5] = {0, 20, 60, 100, 200}; 

		int neta= 100;
		int nphi= 72;
		float etamin = -5, etamax =5;
		float phimin = -TMath::Pi(), phimax = TMath::Pi() ;
		float * cent_in = cent_in_c;
		float * cent_out = cent_out_c;
		float* trkpt_in= trkpt_in_c;
		float *trkpt_out = trkpt_out_c;
	}

	namespace anaConfig{
		int ntrkpt = 9;
		float trkpt_c[10]= {0.7,1, 2, 3, 4, 8, 12, 16,20,999};
		int ncent = 4; 
		float cent[5] = {0, 20, 60, 100, 200}; 
		float *trkpt= trkpt_c;
	}

	void loadConfig(){
		weight::fvz->SetParameters(1.18472,-0.132675,0.00857998,-0.000326085,-1.48786e-06,4.68665e-07,-7.32942e-09 );	
		weight::fcent1->SetParameters(4.40810, -7.75301e-02, 4.91953e-04, -1.34961e-06, 1.44407e-09, -160, 1, 3.68078e-15);
		/* initiate the cent bin for tracking */
		for(int i=0;i<trackingClosureConfig::ncent+1;++i) trackingClosureConfig::cent[i] = i*4;	
		for(int i=0;i<trackingCorrConfig::ncent_in+1;++i) trackingCorrConfig::cent_in_c[i] = i*4;	
	}

	bool trackQualityCuts(inputTree * t , int j){
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

	bool eventCuts(inputTree *t){
		if ( t->HBHENoiseFilterResultRun2Loose ==0) return 1;
		if ( t->pcollisionEventSelection ==0) return 1;
		if ( t->pprimaryVertexFilter ==0) return 1;
		if ( t->phfCoincFilter3 ==0) return 1;
		if ( t->vz>15 || t->vz<-15) return 1;
		return 0;
	}
	bool genParticleCuts(inputTree *t,int j){
		if (t->chg->at(j)==0) return 1;
		return 0;
	}
}

