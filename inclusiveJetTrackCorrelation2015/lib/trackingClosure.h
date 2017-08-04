
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
			etamin(trackingClosureConfig::etamin),
			etamax(trackingClosureConfig::etamax),
			phimin(trackingClosureConfig::phimin),
			phimax(trackingClosureConfig::phimax),
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
			etamin(trackingClosureConfig::etamin),
			etamax(trackingClosureConfig::etamax),
			phimin(trackingClosureConfig::phimin),
			phimax(trackingClosureConfig::phimax),
			trkpt(trackingClosureConfig::trkpt),
			cent(trackingClosureConfig::cent)
	{
		f=NULL;
		isread = 0;
		gen = new xthf4("gen","gen", nx,etamin, etamax , ny, phimin, phimax,ntrkpt, trkpt,ncent, cent);
		rec = new xthf4("rec","rec", nx,etamin, etamax , ny, phimin, phimax,ntrkpt, trkpt,ncent, cent);
		cre = new xthf4("cre","cre", nx,etamin, etamax , ny, phimin, phimax,ntrkpt, trkpt,ncent, cent);
	};

		void runScan();
		void Read();
		void DrawClosure(TString name="", TString type= "");
		TPad* ratioPlot(TH1F* hden, TH1F* hnum1, TH1F* hnum2, 
				float xmin, float xmax,float ymin,float ymax, bool doLegend);
		~trackingClosure();

	public: 
		TFile *f;
		inputTree * t;
		int ntrkpt;
		int ncent;
		int nx, ny;
		float *trkpt, *cent;
		float etamin, etamax, phimin, phimax;
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

trackingClosure::~trackingClosure(){
	delete gen;
	delete rec; 
	delete cre; 
}

void trackingClosure::DrawClosure(TString name="", TString type= ""){
	float xmin =-3 , xmax=3 , ymin = 0.85, ymax= 1.15;
	int ana_ntrkpt=anaConfig::ntrkpt;
	int ana_ncent=anaConfig::ncent;
	float* ana_trkpt = anaConfig::trkpt;
	float* ana_cent = anaConfig::cent;
	TCanvas * c[ana_ntrkpt][ana_ncent];
	TH1F *hgen, *hrec, *hcre;
	TLine *ll = new TLine();
	ll->SetLineStyle(8);
	for(int i=0;i<ana_ntrkpt ; ++i){
		c[i][0]= new TCanvas(Form("c_%d_0",i),"", 1200,600);
		c[i][0]->Divide(4,1,0,0);
		for(int j=0;j<ana_ncent; ++j){
			c[i][0]->cd(ana_ncent-j);
			hgen= (TH1F*)gen->hist(i,j)->ProjectionX();	
			hrec= (TH1F*)rec->hist(i,j)->ProjectionX();	
			hcre= (TH1F*)cre->hist(i,j)->ProjectionX();	
			hgen->Rebin();
			hrec->Rebin();
			hcre->Rebin();
			hgen->SetAxisRange(xmin, xmax , "X");
			ratioPlot(hgen, hcre, hrec,xmin, xmax, ymin, ymax, !j);
			ll->DrawLine(xmin, 1, xmax,1);
			ll->DrawLine(xmin, 1.05, xmax,1.05);
			ll->DrawLine(xmin, .95, xmax,.95);
		}		
	}
}

TPad* trackingClosure::ratioPlot(TH1F* hden, TH1F* hnum1, TH1F* hnum2, 
		float xmin, float xmax,float ymin,float ymax, bool doLegend)
{
	TH1* hratio1, *hratio2;
	hden->SetAxisRange(0, 1.8*hden->GetMaximum(), "y");
	hden->SetMarkerSize(1.4);
	hden->SetMarkerStyle(21);
	hden->SetMarkerColor(kBlue-5);
	hden->SetLabelSize(0.05, "Y");
	hden->SetLabelSize(0.05, "X");
	hnum1->SetMarkerSize(0.9);
	hnum1->SetMarkerStyle(20);
	hnum1->SetMarkerColor(kRed+1);
	hnum1->SetTitle("");
	hnum1->SetLabelSize(0.05, "Y");
	hnum1->SetLabelSize(0.05, "X");
	hnum1->GetXaxis()->SetTitleOffset(0.8);
	hnum1->GetXaxis()->CenterTitle();
	hnum1->GetXaxis()->SetTitleSize(0.06);
	hratio1=(TH1*)hnum1->Clone();
	hden->SetAxisRange(xmin, xmax, "X");
	hratio1->SetAxisRange(xmin, xmax, "X");
	hratio1->Divide(hnum1, hden, 1, 1, "B");
	hnum2->SetMarkerSize(0.9);
	hnum2->SetMarkerStyle(20);
	hnum2->SetTitle("");
	hnum2->SetMarkerColor(kBlue-3);
	hratio2=(TH1*)hnum2->Clone();
	hratio2->Divide(hnum2, hden, 1 ,1, "B");
	TPad* pad = (TPad*) gPad;
	pad->Divide(1, 2, 0,0);
	pad->cd(1);
	hden->SetStats(0);
	hden->SetTitle("");
	hden->Draw();
	hnum1->Draw("same");
	hnum2->Draw("same");
	TLegend *ltemp;
	if(doLegend){
		ltemp= new TLegend(0.15,0.67,0.6,0.97);
		ltemp->SetLineColor(kWhite);
		ltemp->SetFillColorAlpha(kWhite, 0);
	ltemp->AddEntry(hden, "gen. particles");
	ltemp->AddEntry(hnum1, "corr. tracks");
	ltemp->AddEntry(hnum2, "reco. tracks");
	ltemp->Draw();
	}
	pad->cd(2);
	hratio1->SetStats(0);
	hratio1->SetAxisRange(ymin, ymax , "Y");
	hratio1->Draw();
	hratio2->Draw("same");
	if(doLegend){
		TLegend *ltemp2 = new TLegend(0.15,0.72,0.8,0.97);
		ltemp2->SetLineColor(kWhite);
		ltemp2->SetFillColorAlpha(kWhite, 0);
		ltemp2->AddEntry(hratio1, "reco.Tracks/gen.Particles");
		ltemp2->AddEntry(hratio2, "corr.Tracks/gen.Particles");
		ltemp2->Draw();
	}
	return (TPad*)pad;
//	ratioVec.push_back(hratio1);
}

#endif
