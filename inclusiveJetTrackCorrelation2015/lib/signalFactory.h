

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
			tl.SetLineStyle(2);
		}
		void readSignal(TString name, TString title, TFile *f , int nz1, float *zbin1, int nw1, float *wbin1);
		void readMixing(TString name, TString title, TFile *f , int nz1, float *zbin1, int nw1, float *wbin1);
		void getSignal(TString name, TString hname = "signal");
		void histConfig(TH1D* h);
		void Write();
		TH1D* integralGeoCorrection(TH1D* h);
	public : 
		//convention: z-axis for tracks and w-axis for cent;
		xthd4 * signal; 
		xthd4 * mixing;
		jtShape jts;
		TH1D** signal_deta ;
		TH1D** signal_dphi ;
		TH1D** signal_dr   ;
		TH1D** signal_drGeo;
		TH1D** signal_sideBand ;
		TH1D** signal_ratio ;
		TH1D** signal_area ;
		int nhist;
		TLine tl;
		TString sname;
};

void signalFactory::readSignal(TString name, TString title, TFile *f , int nz1, float *zbin1, int nw1, float *wbin1){
	signal->Read(name, title, f ,  nz1, zbin1, nw1, wbin1);
}

void signalFactory::readMixing(TString name, TString title, TFile *f , int nz1, float *zbin1, int nw1, float *wbin1){
	mixing->Read(name, title, f , nz1, zbin1,  nw1, wbin1);
}

void signalFactory::getSignal(TString name, TString hname){
	sname = name;
	int ymin, ymax, xmin, xmax;
	float etamin = -1, etamax=1, phimin = -1, phimax=1;
	float phiSmin = 1.2, phiSmax=1.8;// sideBand region in dphi 
	const double xdrbins[16] = {0,0.05,0.1,0.15,0.2,0.25,0.3,0.35,0.4,0.45,0.5,0.6,0.7,0.8,1., 1.2};
	TH2D** sig;
	sig = new TH2D*[4*(signal->nz-1)];
	//	TFile *wf = TFile::Open(name+"_signal.root","recreate");
	signal_deta = new TH1D*[4*(signal->nz-1)];
	signal_dphi = new TH1D*[4*(signal->nz-1)];
	signal_dr   = new TH1D*[4*(signal->nz-1)];
	signal_drGeo= new TH1D*[4*(signal->nz-1)];
	signal_sideBand= new TH1D*[4*(signal->nz-1)];
	nhist = 4*(signal->nz-1);
	xmin = signal->hist(0,0)->GetXaxis()->FindBin(-1);
	xmax = signal->hist(0,0)->GetXaxis()->FindBin( 1);
	ymin = signal->hist(0,0)->GetYaxis()->FindBin(-1);
	ymax = signal->hist(0,0)->GetYaxis()->FindBin( 1);
	float xbinwidth = signal->hist(0,0)->GetXaxis()->GetBinWidth(1);
	float ybinwidth = signal->hist(0,0)->GetYaxis()->GetBinWidth(1);
	TString stmp; 
TH1D* geo;

	for(int i=0; i<signal->nz-1; ++i){
		for(int j=0; j<4; ++j){
			stmp = hname+Form("_dr_%d_%d", i, j);
			TString stmp2= signal->ztitle.at(i)+signal->wtitle.at(j);
			signal_dr[i*4+j] = new TH1D(stmp,stmp2, 15, xdrbins);
			//			cout<<i<<", "<<j<<endl;
			// divide by the number of jet should be done before hist have been sent into the factory;
			sig[i*4+j] = jts.getSignal((TH2D*)signal->hist(i,j), (TH2D*) mixing->hist(i,j),1, 1.5, 2.5);
			stmp = hname+Form("_%d_%d", i,j);
			sig[i*4+j]->SetName(stmp);
			geo=(TH1D*) jts.drJetShape(sig[i*4+j],signal_dr[i*4+j]);
			stmp = hname+Form("_deta_%d_%d", i,j);
			signal_deta[i*4+j]=(TH1D*)sig[i*4+j]->ProjectionX(stmp,ymin,ymax,"e");
			signal_deta[i*4+j]->Scale(ybinwidth);
			stmp = hname+Form("_dphi_%d_%d", i,j);
			signal_dphi[i*4+j]=(TH1D*)sig[i*4+j]->ProjectionY(stmp,xmin,xmax,"e");
			signal_dphi[i*4+j]->Scale(xbinwidth);
			signal_deta[i*4+j]->Rebin(4);
			signal_deta[i*4+j]->Scale(0.25);
			signal_dphi[i*4+j]->Rebin(2);
			signal_dphi[i*4+j]->Scale(0.5);
			signal_deta[i*4+j]->Write();
			signal_dphi[i*4+j]->Write();
			signal_dr  [i*4+j]->Write();
			stmp = hname+Form("_drGeo_%d_%d", i, j);
			signal_drGeo[i*4+j]=(TH1D*) signal_dr[i*4+j]->Clone(stmp);
			stmp = hname+Form("_sideBand_%d_%d", i,j);
			signal_sideBand[i*4+j]=(TH1D*)sig[i*4+j]->ProjectionX(stmp,phiSmin,phiSmax,"e");
			signal_sideBand[i*4+j]->Scale(ybinwidth);
			signal_sideBand[i*4+j]->Rebin(4);
			signal_sideBand[i*4+j]->Scale(0.25);
	//		stmp = hname+Form("_ratio_%d_%d", i,j);
	//		signal_ratio[i*4+j] = (TH1D*) signal_dr[i*4+j]->Clone(stmp);
	//		signal_ratio[i*4+j]->Divide(signal_area[i*4+j]);
		}
	}
	TH1D* geoCorr = integralGeoCorrection(geo);
	TCanvas *c_deta = new TCanvas(name+"_deta_getSignal", "", 1600, 2500);
	c_deta->Divide(4, signal->nz-1, 0, 0);
	for(int i=0; i<signal->nz-1; ++i){
		for(int j=0; j<4; ++j){
			c_deta->cd((i+1)*4-j);
			histConfig((TH1D*) signal_deta[i*4+j]);
			signal_deta[i*4+j]->Draw();
			tl.DrawLine(etamin, 0, etamax, 0);
		}
	}
	c_deta->SaveAs(name+"_deta.png");
	TCanvas *c_dphi = new TCanvas(name+"_dphi_getSignal", "", 1600, 2500);
	c_dphi->Divide(4, signal->nz-1, 0, 0);
	for(int i=0; i<signal->nz-1; ++i){
		for(int j=0; j<4; ++j){
			c_dphi->cd((i+1)*4-j);
			histConfig((TH1D*) signal_dphi[i*4+j]);
			signal_dphi[i*4+j]->Draw();
			tl.DrawLine(phimin, 0, phimax, 0);
		}
	}
	c_dphi->SaveAs(name+"_dphi.png");
	TCanvas *c_dr = new TCanvas(name+"_dr_getSignal", "", 1600, 2500);
	c_dr->Divide(4, signal->nz-1, 0, 0);
	for(int i=0; i<signal->nz-1; ++i){
		for(int j=0; j<4; ++j){
			c_dr->cd((i+1)*4-j);
			histConfig((TH1D*) signal_dr[i*4+j]);
			signal_dr[i*4+j]->SetAxisRange(0,.9, "X");
			signal_dr[i*4+j]->Draw();
			histConfig((TH1D*) signal_drGeo[i*4+j]);
			signal_drGeo[i*4+j]->Multiply(geoCorr);
			signal_drGeo[i*4+j]->SetMarkerColor(kRed);
			signal_drGeo[i*4+j]->Draw("same");
			//tl.DrawLine(xmin, 0, xmax, 0);
		}
	}
	c_dr->SaveAs(name+"_dr.png");
	TCanvas *c_side = new TCanvas(name+"_side_getSignal", "", 1600, 2500);
	c_side->Divide(4, signal->nz-1, 0, 0);
	for(int i=0; i<signal->nz-1; ++i){
		for(int j=0; j<4; ++j){
			c_side->cd((i+1)*4-j);
			histConfig((TH1D*) signal_sideBand[i*4+j]);
			signal_sideBand[i*4+j]->Draw();
			tl.DrawLine(etamin, 0, etamax, 0);
		}
	}
	c_side->SaveAs(name+"_sideCheck.png");

	/*
	   TCanvas *c_ratio = new TCanvas(name+"_ratio_getSignal", "", 1600, 2500);
	   c_ratio->Divide(4, signal->nz-1, 0, 0);
	   for(int i=0; i<signal->nz-1; ++i){
	   for(int j=0; j<4; ++j){
	   c_ratio->cd((i+1)*4-j);
	   histConfig((TH1D*) signal_ratio[i*4+j]);
	   signal_ratio[i*4+j]->SetAxisRange(0,.9, "X");
	   signal_ratio[i*4+j]->Draw();
	//tl.DrawLine(xmin, 0, xmax, 0);
	}
	}
	c_ratio->SaveAs(name+"_ratio.png");

	TCanvas *c_area = new TCanvas(name+"_area_getSignal", "", 1600, 2500);
	c_area->Divide(4, signal->nz-1, 0, 0);
	for(int i=0; i<signal->nz-1; ++i){
	for(int j=0; j<4; ++j){
	c_area->cd((i+1)*4-j);
	histConfig((TH1D*) signal_area[i*4+j]);
	signal_area[i*4+j]->SetAxisRange(0,.6, "X");
	signal_area[i*4+j]->Draw();
	area_ideal->SetLineColor(kRed);
	area_ideal->Draw("same");
	//tl.DrawLine(xmin, 0, xmax, 0);
	}
	}
	c_area->SaveAs(name+"_area.png");
	*/
	//delete [] sig;
	//delete [] signal_deta;
	//delete [] signal_dphi;
	//delete [] signal_dr;
}
void signalFactory::Write(){
	for(int i=0; i<nhist; ++i){
		signal_deta[i]->Write();
		signal_dphi[i]->Write();
		signal_dr[i]->Write();  
	}
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

TH1D* signalFactory::integralGeoCorrection(TH1D* h){
	TH1D* area_ideal = (TH1D*)h->Clone("area_ideal");
	for(int i=1; i<h->GetNbinsX()+1; ++i){
		float r = area_ideal->GetBinCenter(i);
		area_ideal->SetBinContent(i, 2*r*TMath::Pi()); 
	}
	h->SetMarkerStyle(20);
	h->SetMarkerSize(0.8);
	h->GetXaxis()->SetLabelSize(0.08);
	h->GetYaxis()->SetLabelSize(0.08);
	area_ideal->SetLineColor(kRed);

	TCanvas* c = new TCanvas("c_geoCorrection", "", 800,600);
	h->SetStats(0);
	h->SetTitle("Geometry Correction");
	h->Draw();
	area_ideal->Draw("same");
	c->SaveAs(sname+"_geoCorrection.png");
	area_ideal->Divide(h);
	return area_ideal;
}
#endif
