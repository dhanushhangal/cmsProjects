

#ifndef signalFactory_pp_H
#define signalFactory_pp_H
#ifndef jtShape_H
#include "jtShape.h"
#endif

#ifndef xthd4_H
#include "xthd4.h"
#endif

#ifndef multiPad_H
#include "multiPad.h"
#endif

class signalFactory_pp {
	public : 
		signalFactory_pp(){
			signal = new xthd4();
			mixing = new xthd4();
		}
		void readSignal(TString name, TString title, TFile *f , int nz1, float *zbin1, int nw1, float *wbin1);
		void readMixing(TString name, TString title, TFile *f , int nz1, float *zbin1, int nw1, float *wbin1);
		void getSignal(TString name, TString hname = "signal");
		void histConfig(TH1D* h);
		TH1D* integralGeoCorrection(TH1D* h);
		void debugPlot();
	public : 
		//convention: z-axis for tracks and w-axis for cent;
		xthd4 * signal; 
		xthd4 * mixing;
		jtShape jts;
		TString sname;
		TH1D**  signal_deta;
		TH1D**  signal_dphi;
		TH1D**  signal_dr;
		TH1D**  signal_drGeo;
		int ymin, ymax, xmin, xmax;
		float etamin = -1, etamax=1, phimin = -1, phimax=1;
	//	const double xdrbins[16] = {0,0.05,0.1,0.15,0.2,0.25,0.3,0.35,0.4,0.45,0.5,0.6,0.7,0.8,1., 1.2};
		float xdrbins[21] = {0.,0.05,0.1,0.15,0.2,0.25,0.3,0.35,0.4,0.45,0.5,0.6,0.7,0.8,1.,1.2,1.4,1.6,1.8,2.0, 2.1};
};

void signalFactory_pp::readSignal(TString name, TString title, TFile *f , int nz1, float *zbin1, int nw1, float *wbin1){
	signal->Read(name, title, f ,  nz1, zbin1, nw1, wbin1);
}

void signalFactory_pp::readMixing(TString name, TString title, TFile *f , int nz1, float *zbin1, int nw1, float *wbin1){
	mixing->Read(name, title, f , nz1, zbin1,  nw1, wbin1);
}

void signalFactory_pp::getSignal(TString name, TString hname){
	sname = name;
	TH2D** sig;
	sig = new TH2D*[signal->nz-1];
	//	TFile *wf = TFile::Open(name+"_signal.root","recreate");
	signal_deta = new TH1D*[signal->nz-1];
	signal_dphi = new TH1D*[signal->nz-1];
	signal_dr   = new TH1D*[signal->nz-1];
	signal_drGeo= new TH1D*[signal->nz-1];
	xmin = signal->hist(0,0)->GetXaxis()->FindBin(-1);
	xmax = signal->hist(0,0)->GetXaxis()->FindBin( 1);
	ymin = signal->hist(0,0)->GetYaxis()->FindBin(-1);
	ymax = signal->hist(0,0)->GetYaxis()->FindBin( 1);
	float xbinwidth = signal->hist(0,0)->GetXaxis()->GetBinWidth(1);
	float ybinwidth = signal->hist(0,0)->GetYaxis()->GetBinWidth(1);
	TString stmp; 
	TH1D* drHist = new TH1D("drHist","", 15, xdrbins);
	TH1D* geoCorr = jts.drGeoTest(signal->hist(0,0), drHist);
	geoCorr->Write();
	for(int i=0; i<signal->nz-1; ++i){
		stmp = hname+Form("_pp_dr_%d_0", i);
		TString stmp2= signal->ztitle.at(i);
		signal_dr[i] = new TH1D(stmp,stmp2, 15, xdrbins);
		// divide by the number of jet should be done before hist have been sent into the factory;
		sig[i] = jts.getSignal((TH2D*)signal->hist(i,0), (TH2D*) mixing->hist(i,0),1, 1.5, 2.5);
		stmp = hname+Form("_pp_%d", i);
		sig[i]->SetName(stmp);
		jts.drJetShape(sig[i],signal_dr[i]);
		stmp = hname+Form("_pp_deta_%d_0", i);
		signal_deta[i]=(TH1D*)sig[i]->ProjectionX(stmp,ymin,ymax,"e");
		signal_deta[i]->Scale(ybinwidth);
		stmp = hname+Form("_pp_dphi_%d_0", i);
		signal_dphi[i]=(TH1D*)sig[i]->ProjectionY(stmp,xmin,xmax,"e");
		signal_dphi[i]->Scale(xbinwidth);
		signal_deta[i]->Rebin(4);
		signal_deta[i]->Scale(0.25);
		signal_dphi[i]->Rebin(4);
		signal_dphi[i]->Scale(0.25);

		stmp = hname+Form("_pp_drGeo_%d_0", i);
		signal_drGeo[i]=(TH1D*)signal_dr[i]->Clone(stmp);
		signal_drGeo[i]->Multiply(geoCorr);
		signal_drGeo[i]->Write();
		signal_deta[i]->Write();
		signal_dphi[i]->Write();
		signal_dr  [i]->Write();
		signal_dr  [i]->Write();
	}
	debugPlot();
}

void signalFactory_pp::debugPlot(){
	//TCanvas * c_pp = new TCanvas(sname+"_pp_signal", "", 1000, 2500);
	//c_pp->Divide(3, 2/*signal->nz-1*/, 0, 0);
	auto mp = new multiPad(sname+"_pp_signal","",2/*signal->nz*/, 3);
	auto tl = new TLine();
	tl->SetLineStyle(2);
	for(int i=0; i<3/*signal->nz-1*/; ++i){
		histConfig((TH1D*) signal_deta[i]);
		histConfig((TH1D*) signal_dphi[i]);
		histConfig((TH1D*) signal_dr[i]);
		signal_dr[i]->SetAxisRange(0,0.8,"X");
		signal_deta[i]->GetXaxis()->SetTitle("#Delta#eta");
		signal_dphi[i]->GetXaxis()->SetTitle("#Delta#phi");
		signal_dr  [i]->GetXaxis()->SetTitle("#Deltar");
		signal_dr[i]->GetYaxis()->SetTitle("Y_{Pythia}");
		signal_dr[i]->GetYaxis()->SetTitleSize(0.07);
		mp->addHist(signal_deta[i],i+1, 3);
		mp->addHist(signal_dphi[i],i+1, 2);
		mp->addHist(signal_dr[i],i+1, 1);
		mp->draw();
		auto tx = new TLatex();
		mp->cd(1);
		tx->SetTextSize(0.1);
		tx->DrawLatexNDC(0.6,0.8, "0.7 <pT_{track}< 1");
		mp->cd(4);
		tx->SetTextSize(0.08);
		tx->DrawLatexNDC(0.6,0.8, "1 <pT_{track}< 2");
		/*
		c_pp->cd((i+1)*3);
		histConfig((TH1D*) signal_deta[i]);
		signal_deta[i]->Draw();
		c_pp->cd((i+1)*3-1);
		histConfig((TH1D*) signal_dphi[i]);
		signal_dphi[i]->Draw();
		c_pp->cd((i+1)*3-2);
		histConfig((TH1D*) signal_dr[i]);
		signal_dr[i]->SetAxisRange(0,.9, "X");
		signal_dr[i]->Draw();
		histConfig((TH1D*) signal_drGeo[i]);
		signal_drGeo[i]->SetAxisRange(0,.9, "X");
		//		signal_drGeo[i]->SetMarkerColor(kRed);
		//signal_drGeo[i]->Draw();
		*/

	}
	mp->cd(2);
	tl->DrawLine(-1.5, 0, 1.5, 0);
	mp->cd(3);
	tl->DrawLine(-1.5, 0, 1.5, 0);
	mp->cd(5);
	tl->DrawLine(-1.5, 0, 1.5, 0);
	mp->cd(6);
	tl->DrawLine(-1.5, 0, 1.5, 0);
	mp->SaveAs(sname+"_pp_signal.png");
	//delete [] sig;
	//delete [] signal_deta;
	//delete [] signal_dphi;
	//delete [] signal_dr;
}

void signalFactory_pp::histConfig(TH1D* h){
	h->SetStats(0);
	h->SetTitle("");
	h->SetAxisRange(-1.5,1.5, "X");
	h->SetMarkerStyle(20);
	h->SetMarkerSize(0.8);
	h->GetXaxis()->SetLabelSize(0.08);
	h->GetYaxis()->SetLabelSize(0.08);
	h->GetYaxis()->CenterTitle();
	h->GetXaxis()->CenterTitle();
	h->GetXaxis()->SetTitleSize(0.08);
	//	h->SetMarkerColor(kBlue);
}

TH1D* signalFactory_pp::integralGeoCorrection(TH1D* h){
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
