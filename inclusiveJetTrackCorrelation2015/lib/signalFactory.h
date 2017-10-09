


#ifndef signalFactory_H
#define signalFactory_H

#include "signalFactoryBase.h"

#ifndef xthd4_H
#include "xthd4.h"
#endif

#ifndef multiPad_H
#include "multiPad.h"
#endif

class signalFactory : public signalFactoryBase {
	public : 
		signalFactory(TString sname): signalFactoryBase(), fname(sname) {
			signal = new xthd4();
			mixing = new xthd4();
		}
		void readSignal(TString name, TString title, TFile *f , int nz1, float *zbin1, int nw1, float *wbin1);
		void readMixing(TString name, TString title, TFile *f , int nz1, float *zbin1, int nw1, float *wbin1);
		void getSignal(TString name, TString hname = "");
		void drawDebug();
		void histoConfig(TH1*, float x1, float x2);
	public :
		xthd4 * signal;
		xthd4 * mixing;
		TH1D** signal_deta;
		TH1D** signal_dphi;
		TH1D** signal_dr  ;
		TH1D** signal_drGeo;
		TH1D** signal_sideBand;
		TH2D** sig;
		TString fname;
		TString dumpPath = "";
		TString dumpType = "eps";
};

void signalFactory::readSignal(TString name, TString title, TFile *f , int nz1, float *zbin1, int nw1, float *wbin1){
	signal->Read(name, title, f ,  nz1, zbin1, nw1, wbin1);
}

void signalFactory::readMixing(TString name, TString title, TFile *f , int nz1, float *zbin1, int nw1, float *wbin1){
	mixing->Read(name, title, f , nz1, zbin1,  nw1, wbin1);
}

void signalFactory::getSignal(TString name, TString hname){
	if(hname ) hname = fname;
	//int nbin = 21;
	//const float xdrbins[22] = {0,0.05,0.1,0.15,0.2,0.25,0.3,0.35,0.4, 0.45, 0.5, 0.55, 0.6, 0.65, 0.675, 0.7, 0.725, 0.75, 0.8,  0.85,  0.9, 1};
	int nbin = 26;
	const float xdrbins[27] = {0,0.05,0.1,0.15,0.2,0.25,0.3,0.35,0.4, 0.45, 0.5,0.525, 0.55, 0.575,0.6,0.625, 0.65, 0.675, 0.7, 0.725, 0.75,0.775, 0.8, 0.825,  0.85,  0.9, 1};
	//int nbin = 19;
	//const float xdrbins[20] = {0,0.05,0.1,0.15,0.2,0.25,0.3,0.35,0.4, 0.45,0.5, 0.55, 0.6, 0.65,0.7, 0.75, 0.8,0.85,0.9, 1};
	//const float xdrbins[20] = {0,0.05,0.1,0.15,0.2,0.25,0.3,0.35,0.4, 0.45, 0.5, 0.55, 0.6, 0.65, 0.7,0.75, 0.8,0.85,0.9, 1};
	float phiSmin = 1.5, phiSmax=2.5;// sideBand region in dphi 
	signal_deta = new TH1D*[4*(signal->nz-1)];
	signal_dphi = new TH1D*[4*(signal->nz-1)];
	signal_dr   = new TH1D*[4*(signal->nz-1)];
	signal_drGeo= new TH1D*[4*(signal->nz-1)];
	signal_sideBand= new TH1D*[4*(signal->nz-1)];
	float xmin = signal->hist(0,0)->GetXaxis()->FindBin(-1);
	float xmax = signal->hist(0,0)->GetXaxis()->FindBin( 1);
	float ymin = signal->hist(0,0)->GetYaxis()->FindBin(-1);
	float ymax = signal->hist(0,0)->GetYaxis()->FindBin( 1);
	float xbinwidth = signal->hist(0,0)->GetXaxis()->GetBinWidth(1);
	float ybinwidth = signal->hist(0,0)->GetYaxis()->GetBinWidth(1);
	TH1D* drHist = new TH1D("drHist", "", nbin, xdrbins);
	TH1D* geoCorr = drGeoTest((TH2D*)signal->hist(0,0), drHist);
	TString stmp;
	geoCorr->Draw();
	sig         = new TH2D*[4*(signal->nz-1)];
	for(int i=0; i<signal->nz-1; ++i){
		for(int j=0; j<4; ++j){
			//if( i == 0)
			//sig = signalMaker((TH2D*)signal->hist(i,j), (TH2D*) mixing->hist(i,j), 1.5, 2.5, false);
			//else 
			sig[i*4+j] = signalMaker((TH2D*)signal->hist(i,j), (TH2D*) mixing->hist(i,j), 1.5, 2.5);
			stmp = hname+Form("_dr_%d_%d", i, j);
			TString stmp2= signal->ztitle.at(i)+signal->wtitle.at(j);
			signal_dr[i*4+j] =drDistMaker(sig[i*4+j], stmp,stmp2, nbin, xdrbins);
			stmp = hname+Form("_deta_%d_%d", i,j);
			signal_deta[i*4+j]=(TH1D*)sig[i*4+j]->ProjectionX(stmp,ymin,ymax,"e");
			signal_deta[i*4+j]->Scale(ybinwidth);
			stmp = hname+Form("_dphi_%d_%d", i,j);
			signal_dphi[i*4+j]=(TH1D*)sig[i*4+j]->ProjectionY(stmp,xmin,xmax,"e");
			signal_dphi[i*4+j]->Scale(xbinwidth);
			stmp = hname+Form("_drGeo_%d_%d", i, j);
			signal_drGeo[i*4+j]=(TH1D*) signal_dr[i*4+j]->Clone(stmp);
			signal_drGeo[i*4+j]->Multiply(geoCorr);
			stmp = hname+Form("_sideBand_%d_%d", i,j);
			signal_sideBand[i*4+j]=(TH1D*)sig[i*4+j]->ProjectionX(stmp,phiSmin,phiSmax,"e");
			signal_sideBand[i*4+j]->Scale(ybinwidth);
		}
	}
	TFile *wf = TFile::Open(name, "recreate");
	for(int i=0; i<signal->nz-1; ++i){
		for(int j=0; j<4; ++j){
			signal_dr      [i*4+j]->Write();
			signal_drGeo   [i*4+j]->Write();
			signal_deta    [i*4+j]->Write();
			signal_dphi    [i*4+j]->Write();
			signal_sideBand[i*4+j]->Write();
			sig            [i*4+j]->Write();
		}
	}
	//	wf->Close();
	TFile *wff= TFile::Open("phaseCorr.root", "recreate");
	geoCorr->Write();
}

void signalFactory::drawDebug(){
	float xmin_deta=-1., xmax_deta=1.;
	float xmin_dphi=-1.5, xmax_dphi=1.5;
	float drmin=0, drmax=0.99;

	auto mp_deta = new multiPad("mp_deta", "", signal->nz-1, 4);
	auto mp_dphi = new multiPad("mp_dphi", "", signal->nz-1, 4);
	auto mp_dr = new multiPad("mp_dr", "", signal->nz-1, 4);
	auto mp_sb = new multiPad("mp_sb", "", signal->nz-1, 4);
	for(int i=0; i<signal->nz-1; ++i){
		for(int j=0; j<4; j++){
			signal_sideBand[i*4+j]->Rebin(4);
			signal_sideBand[i*4+j]->Scale(.25);
			histoConfig(signal_deta[i*4+j],xmin_deta, xmax_deta);
			histoConfig(signal_dphi[i*4+j],xmin_dphi, xmax_dphi);
			//histoConfig(signal_dr  [i*4+j],drmin, drmax);
			histoConfig(signal_drGeo[i*4+j],drmin, drmax);
			histoConfig(signal_sideBand[i*4+j],xmin_deta, xmax_deta);
			mp_deta->addHist(signal_deta[i*4+j], i+1, 4-j);
			mp_dphi->addHist(signal_dphi[i*4+j], i+1, 4-j);
			//mp_dr  ->addHist(signal_dr  [i*4+j], i+1, 4-j);
			mp_dr  ->addHist(signal_drGeo[i*4+j], i+1, 4-j);
			mp_sb  ->addHist(signal_sideBand[i*4+j], i+1, 4-j);
		}
	}

	mp_deta->draw();
	mp_dphi->draw();
        mp_dr  ->draw();
	mp_sb  ->draw();

	mp_deta->SaveAs(dumpPath+fname+"_sigDebug_deta."+dumpType);
	mp_dphi->SaveAs(dumpPath+fname+"_sigDebug_dphi."+dumpType);
	mp_dr  ->SaveAs(dumpPath+fname+"_sigDebug_dr."  +dumpType);
	mp_sb  ->SaveAs(dumpPath+fname+"_sigDebug_sb."  +dumpType);
}

void signalFactory::histoConfig(TH1* h, float x1, float x2){
	h->SetMarkerStyle(20);
	h->SetMarkerSize(1.2);
	h->GetXaxis()->CenterTitle();
	h->GetYaxis()->CenterTitle();
	h->SetAxisRange(x1,x2, "X");
	h->GetXaxis()->SetLabelSize(0.05);
}

#endif
