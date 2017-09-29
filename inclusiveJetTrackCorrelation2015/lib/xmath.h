

#ifndef xmath_H
#define xmath_H
#include "xmath.h"


#ifndef ROOT_TMath
#define ROOT_TMath
#include "TMath.h"
#endif

class xmath{
	public : 
		xmath(){};

	public : 
		void solarIntegral_dphi(TH2* , TH1*, float, float);
		void solarIntegral_dr(TH2* , TH1*);
		Double_t getAzimuth(Double_t, Double_t);

};

void xmath::solarIntegral_dr(TH2* signal, TH1* drDist){

	//for(int jx=1; jx<signal->GetNbinsX()+1; jx++){
	//	for(int jy=1; jy<signal->GetNbinsY()+1; jy++){
	//for(int jx=signal->GetXaxis()->FindBin(-1.2); jx<signal->GetXaxis()->FindBin(0.0); jx++){
	//	for(int jy=signal->GetYaxis()->FindBin(-1.2); jy<signal->GetYaxis()->FindBin(0.0); jy++){
	for(int jx=signal->GetXaxis()->FindBin(0.0); jx<signal->GetXaxis()->FindBin(1.2); jx++){
		for(int jy=signal->GetYaxis()->FindBin(-1.2); jy<signal->GetYaxis()->FindBin(0.0); jy++){
			float dr = sqrt( pow(signal->GetXaxis()->GetBinCenter(jx),2) +\
					pow(signal->GetYaxis()->GetBinCenter(jy),2));
			float xwidth = signal->GetXaxis()->GetBinWidth(jx);
			float ywidth = signal->GetYaxis()->GetBinWidth(jy);
			float content = signal->GetBinContent(jx,jy)*xwidth*ywidth;
			float binError = signal->GetBinError(jx,jy)*xwidth*ywidth;
			float error = sqrt(pow(drDist->GetBinError(drDist->FindBin(dr)),2)+\
					pow(binError,2));
			if( content ) {
				drDist->Fill(dr, content);
				drDist->SetBinError(drDist->FindBin(dr), error);
			}
		}
	}
	for(int i=1; i<drDist->GetNbinsX()+1;i++){
		float cont = drDist->GetBinContent(i);
		float error= drDist->GetBinError(i);
		float width= drDist->GetBinWidth(i);
		drDist->SetBinContent(i, cont/width);
		drDist->SetBinError(i, error/width);
	}
	return ;
}

void xmath::solarIntegral_dphi(TH2* h2, TH1* h, float phi1 , float phi2){
	TString temp = h->GetName();
	TH1D* drCounts = (TH1D*) h->Clone(temp+"_counts");
	float r, phi;
	float content, binError, xwidth, ywidth, error;
	for(int jx=1; jx<h2->GetNbinsX()+1; jx++){
		for(int jy=1; jy<h2->GetNbinsY()+1; jy++){
			phi = getAzimuth(h2->GetXaxis()->GetBinCenter(jx),h2->GetYaxis()->GetBinCenter(jy)); 
			r = sqrt( pow(h2->GetXaxis()->GetBinCenter(jx),2) +\
					pow(h2->GetYaxis()->GetBinCenter(jy),2));
			if( phi < phi1 || phi > phi2) continue;
			drCounts->Fill(r);
			xwidth = h2->GetXaxis()->GetBinWidth(jx);
			ywidth = h2->GetYaxis()->GetBinWidth(jy);
			content = h2->GetBinContent(jx,jy)*xwidth*ywidth;
			binError = h2->GetBinError(jx,jy)*xwidth*ywidth;
			error = sqrt(pow(h->GetBinError(h->FindBin(r)),2)+\
					pow(binError,2));
			h->Fill(r, content);
			h->SetBinError(h->FindBin(r), error);
		}
	}
	for(int i=1; i<h->GetNbinsX()+1;i++){
		float cont = h->GetBinContent(i);
		float error= h->GetBinError(i);
		float width= h->GetBinWidth(i);
		h->SetBinContent(i, cont/width);
		h->SetBinError(i, error/width);
	}
}

Double_t xmath::getAzimuth(Double_t x, Double_t y){
	Double_t dr = sqrt(pow(x,2)+pow(y,2));
	//Double_t ang = TMath::Abs(TMath::ASin(x/dr));
	Double_t ang = TMath::ASin(TMath::Abs(y/dr));
	if(x>=0 && y>=0)    return ang;
	else if(x>=0&& y<0) return 2*TMath::Pi()-ang;
	else if(x<0&& y>=0) return TMath::Pi()-ang;
	else return TMath::Pi()+ang;
/*
	if(y<0) return 2*TMath::Pi()-ang;
	else return ang;
	*/
}

#endif
