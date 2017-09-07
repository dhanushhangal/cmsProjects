

#ifndef signalFactory_H
#define signalFactory_H
#ifndef jtShape_H
#include "jtShape.h"
#endif

#ifndef xthd4_H
#include "xthd4.h"
#endif

class signalFactory {
	public : 
		signalFactory(){
			signal = new xthd4();
			mixing = new xthd4();
		}
		void readSignal(TString name, TString title, TFile *f , int nz1, float *zbin1, int nw1, float *wbin1);
		void readMixing(TString name, TString title, TFile *f , int nz1, float *zbin1, int nw1, float *wbin1);
		void getSignal(TString name);
		void histConfig(TH1D* h);
	public : 
		//convention: z-axis for tracks and w-axis for cent;
		xthd4 * signal; 
		xthd4 * mixing;
		jtShape jts;

};

void signalFactory::readSignal(TString name, TString title, TFile *f , int nz1, float *zbin1, int nw1, float *wbin1){
	signal->Read(name, title, f ,  nz1, zbin1, nw1, wbin1);
}

void signalFactory::readMixing(TString name, TString title, TFile *f , int nz1, float *zbin1, int nw1, float *wbin1){
	mixing->Read(name, title, f , nz1, zbin1,  nw1, wbin1);
}

void signalFactory::getSignal(TString name){
	int ymin, ymax, xmin, xmax;
	float etamin = -1, etamax=1, phimin = -1, phimax=1;
	TH2D** sig;
	sig = new TH2D*[4*(signal->nz-1)];
	TFile *wf = TFile::Open(name+"_signal.root","recreate");
	TH1D** signal_deta = new TH1D*[4*(signal->nz-1)];
	TH1D** signal_dphi = new TH1D*[4*(signal->nz-1)];
	xmin = signal->hist(0,0)->GetXaxis()->FindBin(-1);
	xmax = signal->hist(0,0)->GetXaxis()->FindBin( 1);
	ymin = signal->hist(0,0)->GetYaxis()->FindBin(-1);
	ymax = signal->hist(0,0)->GetYaxis()->FindBin( 1);
	float xbinwidth = signal->hist(0,0)->GetXaxis()->GetBinWidth(1);
	float ybinwidth = signal->hist(0,0)->GetYaxis()->GetBinWidth(1);
	for(int i=0; i<signal->nz-1; ++i){
		for(int j=0; j<4; ++j){
			//			cout<<i<<", "<<j<<endl;
			// divide by the number of jet should be done before hist have been sent into the factory;
			sig[i*4+j] = jts.getSignal((TH2D*)signal->hist(i,j), (TH2D*) mixing->hist(i,j),1, 1.5, 2.5);
			sig[i*4+j]->SetName(Form("SIG_%d_%d",i,j));
			signal_deta[i*4+j]=(TH1D*)sig[i*4+j]->ProjectionX(Form("signal_deta_%d_%d", i,j),ymin,ymax,"e");
			signal_deta[i*4+j]->Scale(ybinwidth);
			signal_dphi[i*4+j]=(TH1D*)sig[i*4+j]->ProjectionY(Form("signal_dphi_%d_%d", i,j),xmin,xmax,"e");
			signal_dphi[i*4+j]->Scale(xbinwidth);
			signal_deta[i*4+j]->Rebin(4);
			signal_deta[i*4+j]->Scale(0.25);
			signal_dphi[i*4+j]->Rebin(2);
			signal_dphi[i*4+j]->Scale(0.5);
			signal_deta[i*4+j]->Write();
			signal_dphi[i*4+j]->Write();
		}
	}
	TCanvas *c_deta = new TCanvas("debug_deta_getSignal", "", 1600, 2500);
	c_deta->Divide(4, signal->nz-1, 0, 0);
	for(int i=0; i<signal->nz-1; ++i){
		for(int j=0; j<4; ++j){
			c_deta->cd((i+1)*4-j);
			histConfig((TH1D*) signal_deta[i*4+j]);
			signal_deta[i*4+j]->Draw();
		}
	}
	c_deta->SaveAs(name+"_deta.eps");
	TCanvas *c_dphi = new TCanvas("debug_dphi_getSignal", "", 1600, 2500);
	c_dphi->Divide(4, signal->nz-1, 0, 0);
	for(int i=0; i<signal->nz-1; ++i){
		for(int j=0; j<4; ++j){
			c_dphi->cd((i+1)*4-j);
			histConfig((TH1D*) signal_dphi[i*4+j]);
			signal_dphi[i*4+j]->Draw();
		}
	}
	c_dphi->SaveAs(name+"_dphi.eps");
	delete [] sig;
	delete [] signal_deta;
	delete [] signal_dphi;
}

void signalFactory::histConfig(TH1D* h){
	h->SetStats(0);
	h->SetAxisRange(-2.5,2.5, "X");
	h->SetMarkerStyle(20);
	h->SetMarkerSize(0.8);
	h->GetXaxis()->SetLabelSize(0.08);
	h->GetYaxis()->SetLabelSize(0.08);
	//	h->SetMarkerColor(kBlue);
}

#endif
