


#ifndef signalFactoryBase_H
#define signalFactoryBase_H

#ifndef ROOT_TH1D
#include "TH1D.h"
#endif

#ifndef ROOT_TH2D
#include "TH2D.h"
#endif

#ifndef doublePad_H
#include "doublePad.h"
#endif


class signalFactoryBase {
	public : 
		signalFactoryBase(){}
		TH2D* mixingTableMaker(TH2D* mix, bool doSmooth = true);
		TH2D* getV2Bkg(TH2D* signal, float sideMin=1.5 ,float sideMax = 2.5);
		TH2D* signalMaker(TH2D* signalH2, TH2D* meH2, float sideMin, float sideMax, bool doSmoothME = true);
		void drIntegral(TH2D* signal, TH1D* drDist);
		TH1D* drDistMaker(TH2D* signal, TString name, TString title, int nbin, const float * bins);
		TH1D* drGeoTest(TH2D* signal, TH1D* drDist);
	public : 
		float midLeft = -0.15;
		float midRight=  0.15;
		TString dump_path = "";
};

TH2D* signalFactoryBase::mixingTableMaker(TH2D* mix, bool doSmooth){
	mix->Scale(1.0/mix->Integral()/mix->GetXaxis()->GetBinWidth(1)/mix->GetYaxis()->GetBinWidth(1));
	TH1D* temp = (TH1D*)mix->ProjectionX("_eta");
	TString name = mix->GetName();
	if( doSmooth) name = name+"_smoothed";
	else name = name+"_noEmptyBin";
	TH2D* ME = (TH2D*) mix->Clone(name);
	float mean=0;
	int binLeft = temp->FindBin(midLeft);
	int binRight= temp->FindBin(midRight)+1;
	for(int i=binLeft;i<binRight; i++){
		mean += temp->GetBinContent(i);
	}
	mean = mean /(temp->FindBin(midRight)-temp->FindBin(midLeft)+1)/mix->GetNbinsY();
	temp->Scale(1.0/mean);
	if(doSmooth){
		for(int ix=1; ix<mix->GetNbinsX()+1; ix++){
			for(int iy=1; iy<mix->GetNbinsY()+1; iy++){
				if( ix< binRight && ix>= binLeft){
					ME->SetBinContent(ix, iy, 1);
					ME->SetBinError(ix, iy, 0);
				}
				else{
					ME->SetBinContent(ix, iy, temp->GetBinContent(ix)/mix->GetNbinsY());
					ME->SetBinError(ix, iy, temp->GetBinError(ix)/sqrt(mix->GetNbinsY()));
				}
			}
		}
	}
	else { //only fill the empty bin of the mixing by the content we have
		ME->Scale(1.0/mean);
		for(int ix=1; ix<mix->GetNbinsX()+1; ix++){
			for(int iy=1; iy<mix->GetNbinsY()+1; iy++){
				if( ix< binRight && ix>= binLeft){
					ME->SetBinContent(ix, iy, 1);
					ME->SetBinError(ix, iy, 0);
				}
				else if(ME->GetBinContent(ix,iy)==0){
					ME->SetBinContent(ix, iy, temp->GetBinContent(ix)/mix->GetNbinsY());
					ME->SetBinError(ix, iy, temp->GetBinError(ix)/sqrt(mix->GetNbinsY()));
				}
			}
		}
	}
	return ME;
}
TH2D* signalFactoryBase::getV2Bkg(TH2D* signal, float sideMin,float sideMax){
	TString stemp = signal->GetName();
	TH2D* bkg = (TH2D*)signal->Clone(stemp+"_bkg");
	int outterRight= signal->GetXaxis()->FindBin(sideMax); 	
	int innerRight = signal->GetXaxis()->FindBin(sideMin); 	
	int outterLeft = signal->GetXaxis()->FindBin(-signal->GetXaxis()->GetBinCenter(outterRight)); 	
	int innerLeft  = signal->GetXaxis()->FindBin(-signal->GetXaxis()->GetBinCenter(innerRight)); 
	TH1D* temp = (TH1D*)bkg->ProjectionY("bkg_sideBandProj", innerRight, outterRight, "e");
	temp->Add(bkg->ProjectionY("", outterLeft, innerLeft, "e"));

	TH2D* aux_bkg = (TH2D*)signal->Clone("aux_bkg");
	for(int iy=1; iy<bkg->GetNbinsY()+1; iy++){
		for(int ix=1; ix<bkg->GetNbinsX()+1; ix++){
			aux_bkg->SetBinContent(ix, iy, 1);
		}
	}
	TH1D* aux_proj = (TH1D*)aux_bkg->ProjectionY("aux_proj", innerRight, outterRight);
	aux_proj->Add(aux_bkg->ProjectionY("", outterLeft, innerLeft));
	for(int iy=1; iy<bkg->GetNbinsY()+1; iy++){
		int nbin = int(aux_proj->GetBinContent(iy));
		for(int ix=1; ix<bkg->GetNbinsX()+1; ix++){
			bkg->SetBinContent(ix, iy, temp->GetBinContent(iy)/nbin);
			bkg->SetBinError(ix, iy, temp->GetBinError(iy)/sqrt(nbin));
		}
	}
	temp->Delete();
	aux_bkg->Delete();
	aux_proj->Delete();
	return bkg;
}

TH2D* signalFactoryBase::signalMaker(TH2D* signalH2, TH2D* meH2, float sideMin, float sideMax, bool doSmoothME){
	TString hname = signalH2->GetName();
	TH2D* signal = (TH2D*)signalH2->Clone(hname+"_signal");
	signal->GetXaxis()->SetTitle("d#eta");
	signal->GetYaxis()->SetTitle("d#phi");
	signal->Scale(1.0/signal->GetXaxis()->GetBinWidth(1)/signal->GetYaxis()->GetBinWidth(1));
	TH2D* mix= mixingTableMaker(meH2, doSmoothME);
	signal->Divide(mix);
	TH2D* bkg = (TH2D*) getV2Bkg(signal,sideMin , sideMax );
	signal->Add(signal, bkg, 1, -1);
	return signal;
}

void signalFactoryBase::drIntegral(TH2D* signal, TH1D* drDist){
	float content;
	float error;
	float width;
	float dr;
	float xwidth, ywidth;
	for(int jx=0; jx<signal->GetNbinsX(); jx++){
		for(int jy=0; jy<signal->GetNbinsY(); jy++){
			dr = sqrt( pow(signal->GetXaxis()->GetBinCenter(jx),2) +\
					pow(signal->GetYaxis()->GetBinCenter(jy),2));
			xwidth = signal->GetXaxis()->GetBinWidth(jx);
			ywidth = signal->GetYaxis()->GetBinWidth(jy);
			// integrand f(x,y)dxdy
			content = signal->GetBinContent(jx,jy)*xwidth*ywidth;
			if( content ) {
			error = sqrt(pow(drDist->GetBinError(drDist->FindBin(dr)),2)+\
					pow(signal->GetBinError(jx,jy)*xwidth*ywidth,2));
				drDist->Fill(dr, content);
				drDist->SetBinError(drDist->FindBin(dr), error);
			}
		}
	}
	for(int i=1; i<drDist->GetNbinsX()+1;i++){
		content = drDist->GetBinContent(i);
		error= drDist->GetBinError(i);
		width= drDist->GetBinWidth(i);
		drDist->SetBinContent(i, content/width);
		drDist->SetBinError(i, error/width);
	}
	return;
}

TH1D* signalFactoryBase::drDistMaker(TH2D* signal, TString name, TString title, int nbin, const float * bins){
	TH1D* drDist = new TH1D(name, title, nbin, bins);
	drIntegral(signal, drDist);
	return drDist;
}

TH1D* signalFactoryBase::drGeoTest(TH2D* signal, TH1D* drDist){
	TString temp = drDist->GetName();
	TH1D* drCounts = (TH1D*) drDist->Clone(temp+"_counts");
	TH1D* area_ideal = (TH1D*) drDist->Clone("ideal_phase");
	float dr;
	for(int jx=0; jx<signal->GetNbinsX(); jx++){
		for(int jy=0; jy<signal->GetNbinsY(); jy++){
			dr = sqrt( pow(signal->GetXaxis()->GetBinCenter(jx),2) +\
					pow(signal->GetYaxis()->GetBinCenter(jy),2));
			drCounts->Fill(dr); // get how many bins in the dr region
		}
	}
	drCounts->Scale((signal->GetXaxis()->GetBinWidth(1))*(signal->GetYaxis()->GetBinWidth(1)));
	for(int i=1; i<drCounts->GetNbinsX()+1; ++i){
		float rlow = area_ideal->GetBinLowEdge(i);
		float width = area_ideal->GetBinWidth(i);
		float rup = rlow + width;
		drCounts->SetBinContent(i, drCounts->GetBinContent(i)/width);
		area_ideal->SetBinContent(i, TMath::Pi()*(pow(rup,2)-pow(rlow,2))/width); 
		area_ideal->SetBinError(i, 0); 
	}
	TH1D* geoCorr = (TH1D*) area_ideal ->Clone("geoCorr");
	geoCorr->Divide(drCounts);

	//plot the report map;
	TCanvas* c = new TCanvas("c_geoCorrection", "", 600,800);
	auto dp = new doublePad((TPad*)gPad);
	dp->addRatioPair(area_ideal, drCounts, kBlue, "2#pir", "grids/#Deltar");
	drCounts->SetMarkerStyle(20);
	drCounts->SetMarkerSize(0.8);
	area_ideal->SetLineColor(kRed);

	auto tl = new TLine();
	tl->SetLineStyle(2);
	dp->y2min=0.71; dp->y2max=1.29;
	dp->xmin=0; dp->xmax=0.99;
	dp->setXtitle("#Deltar");
	dp->setYtitle("#DeltaS/#Deltar");
	dp->Draw();
	dp->drawLegend();
	dp->putText("phase-space adjustment",0.4, 0.9, 2);
	dp->hpad->cd(2);
	tl->DrawLine(0, 1, 1, 1);
	dp->h1frame->GetYaxis()->SetLabelSize(0.06);
	dp->h1frame->GetYaxis()->CenterTitle();
	dp->h1frame->GetYaxis()->SetTitleSize(0.08);
	dp->h1frame->GetYaxis()->SetTitleOffset(0.5);
	dp->h2frame->GetYaxis()->SetLabelSize(0.05);
	dp->h2frame->GetXaxis()->SetLabelSize(0.05);
	dp->h2frame->GetXaxis()->CenterTitle();
	dp->h2frame->GetXaxis()->SetTitleSize(0.05);
	c->SaveAs(dump_path+"phaseSpaceCorrection.eps");
	return geoCorr;
}	


#endif
