
#ifndef jtShape_H
#define jtShape_H

#include "TH1F.h"
#include "TH2F.h"
#include "TH1D.h"
#include "TH2D.h"

#ifndef doublePad_H
#include "doublePad.h"
#endif

class jtShape {
	public:
		bool debug = false;

		jtShape();
		TH2D* getSignal(TH2D* signalH2, TH2D* meH2, TH1D* jetDist, float sideMin=1.5, float sideMax=2);
		TH2D* getSignal(TH2D* signalH2, TH2D* meH2, int njet, float sideMin, float sideMax, bool doSmoothME = true, bool doSub = true);
		TH2D* getV2Bkg(TH2D* signalH2, float sideMin=1.5 ,float sideMax = 2);
		void smoothMixingTable(TH2D* mix);
		void drJetShape(TH2D* signalh2, TH1D* drDisth1, float weight=1, TString opt ="e");
		void drIntegral(TH2D* signalh2, TH1D* drDisth1, TString opt ="e");
		// testing the bin scheme affect on the phase space and return the correction histogram.
		TH1D* drGeoTest(TH2D* signalh2, TH1D* drDisth1); 


	private : 
		float midLeft = -0.3;
		float midRight = 0.3;
		int xmin_signal, xmax_signal, ymin_signal, ymax_signal;
		TString stemp;
};

jtShape::jtShape(){
	std::cout<<"the configuration for the jet shape:"<<std::endl;
	std::cout<<"1.mixing Table will be smoothed;"<<std::endl;
	std::cout<<"2.eta interval for taking average is ["<<midLeft<<", "<<midRight<<"];"<<std::endl;
	//std::cout<<"3.sideband region (default) is [1.5, 2]"<<std::endl;
}


void jtShape::smoothMixingTable(TH2D* mix){
	mix->Scale(1.0/mix->Integral()/mix->GetXaxis()->GetBinWidth(1)/mix->GetYaxis()->GetBinWidth(1));
	TH1D* temp = (TH1D*)mix->ProjectionX("_eta");
	TCanvas* c_mixing_debug;
	TH2D* h_mix_debug, *h_mix_debug2;
	TH1D* h_mix_proj_debug1,* h_mix_proj_debug2;
	float mean=0;
	int binLeft = temp->FindBin(midLeft);
	int binRight= temp->FindBin(midRight)+1;
	for(int i=binLeft;i<binRight; i++){
		mean += temp->GetBinContent(i);
	}
	mean = mean /(temp->FindBin(midRight)-temp->FindBin(midLeft)+1)/mix->GetNbinsY();
	//mean = mean /(temp->FindBin(midRight)-temp->FindBin(midLeft)+1);
	if( debug){                   
		c_mixing_debug = new TCanvas("c_mixing_debug","Mixing Debug output", 1200,400);
		c_mixing_debug->Divide(3,1);
		c_mixing_debug->cd(1);
		stemp = mix->GetName();
		h_mix_debug = (TH2D*) mix->Clone(stemp+"_debug");
		h_mix_debug->SetTitle("raw mixing table input");
		h_mix_debug->Draw("surf2");
	}
	temp->Scale(1.0/mean);
	for(int ix=1; ix<mix->GetNbinsX()+1; ix++){
		for(int iy=1; iy<mix->GetNbinsY()+1; iy++){
			if( ix< binRight && ix>= binLeft){
				mix->SetBinContent(ix, iy, 1);
				mix->SetBinError(ix, iy, 0);
			}
			else{
				mix->SetBinContent(ix, iy, temp->GetBinContent(ix)/mix->GetNbinsY());
				mix->SetBinError(ix, iy, temp->GetBinError(ix)/sqrt(mix->GetNbinsY()));
			}
		}
	}
	//mix ->Scale(1.0/mean);

	if( debug){
		c_mixing_debug->cd(2);
		stemp = mix->GetName();
		h_mix_debug2 = (TH2D*) mix->Clone(stemp+"_debug2");
		h_mix_debug2->SetTitle("smoothed mixing table");
		h_mix_debug2->Draw("surf2");
		c_mixing_debug->cd(3);
		//h_mix_proj_debug1 = h_mix_debug->ProjectionX();
		h_mix_proj_debug1 = (TH1D*) temp->Clone("h_mix_proj_debug1");
		h_mix_proj_debug2 = h_mix_debug2->ProjectionX();
		h_mix_proj_debug1->SetLineWidth(5);
		h_mix_proj_debug2->SetLineWidth(5);
		h_mix_proj_debug1->SetMarkerStyle(20);
		h_mix_proj_debug2->SetMarkerStyle(20);
		h_mix_proj_debug1->SetMarkerSize(0.7);
		h_mix_proj_debug2->SetMarkerSize(0.7);
		h_mix_proj_debug1->SetLineColorAlpha(kBlue-3,1);
		h_mix_proj_debug2->SetLineColorAlpha(kRed-3,0.5);
		h_mix_proj_debug1->Rebin(4);
		h_mix_proj_debug2->Rebin(4);
		h_mix_proj_debug1->SetStats(0);
		h_mix_proj_debug1->Draw();
		//h_mix_proj_debug2->Draw("same");
		TLegend* tl_mix = new TLegend(0.35, 0.2, 0.7, 0.4);
		tl_mix->SetLineColor(0);
		tl_mix->AddEntry(h_mix_proj_debug1, "raw input mixing");
		//tl_mix->AddEntry(h_mix_proj_debug2, "reproduced mixing");
		tl_mix->Draw();
	}
	//mix->Scale(1.0/mean);
	temp->Delete();
}

TH2D* jtShape::getV2Bkg(TH2D* signal, float sideMin,float sideMax){
	stemp = signal->GetName();
	TH2D* bkg = (TH2D*)signal->Clone(stemp+"_bkg");
	TH1D* temp = (TH1D*) bkg->ProjectionX();
	int outterRight= temp->FindBin(sideMax); 	
	int innerRight = temp->FindBin(sideMin); 	
	int outterLeft = temp->FindBin(-temp->GetBinCenter(outterRight)); 	
	int innerLeft  = temp->FindBin(-temp->GetBinCenter(innerRight)); 
	temp->Delete();
	temp = (TH1D*)bkg->ProjectionY("bkg_sideBandProj", innerRight, outterRight, "e");
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
	TCanvas* c_bkg_debug;
	TH1D *hproj_debug1, *hproj_debug2;
	TH2D *h2bkg_debug, *h2signal_debug;
	if( debug){
		h2bkg_debug   = (TH2D*) bkg->Clone("h2_bkg_debug");
		h2signal_debug= (TH2D*) signal->Clone("h2_signal_debug");
		h2bkg_debug->SetLineWidth(4);
		h2signal_debug->SetLineWidth(3);
		h2bkg_debug->SetLineColorAlpha(kRed-3,0.7);
		h2signal_debug->SetLineColorAlpha(kBlue,1);
		c_bkg_debug= new TCanvas("c_bkg_debug","Debug output for bkg substraction",1200,400);
		TLegend* tl_bkg = new TLegend(0.35, 0.6, 0.7, 0.8);
		tl_bkg->SetLineColor(0);
		c_bkg_debug->Divide(3,1);
		c_bkg_debug->cd(1);
		hproj_debug1 = (TH1D*) temp->Clone("bkg_proj_debug1");
		hproj_debug2 = (TH1D*)bkg->ProjectionY("bkg_proj_debug2", innerRight, outterRight, "e");
		hproj_debug2 ->Add(bkg->ProjectionY("", outterLeft, innerLeft, "e"));
		hproj_debug1->Rebin(4);
		hproj_debug2->Rebin(4);
		hproj_debug1->Draw();
		hproj_debug2->Draw("same");
		hproj_debug2->SetLineColorAlpha(kRed-3,0.75);
		tl_bkg->AddEntry(hproj_debug1,"side band proj");
		tl_bkg->AddEntry(hproj_debug2,"produced bkg proj");
		tl_bkg->Draw();
		h2bkg_debug->Rebin2D(4,4);
		h2signal_debug->Rebin2D(4,4);
		c_bkg_debug->cd(2);
		h2signal_debug->ProjectionX("mix_eta", ymin_signal, ymax_signal)->Draw();
		h2bkg_debug->ProjectionX("mix_bkg_eta", ymin_signal, ymax_signal)->Draw("same");
		c_bkg_debug->cd(3);
		h2signal_debug->ProjectionY("mix_phi", xmin_signal, xmax_signal)->Draw();
		h2bkg_debug->ProjectionY("mix_bkg_phi", xmin_signal, xmax_signal)->Draw("same");
	}
	temp->Delete();
	aux_bkg->Delete();
	aux_proj->Delete();
	return bkg;
}

TH2D* jtShape::getSignal(TH2D* signalH2, TH2D* meH2, TH1D* jetDist, float sideMin, float sideMax){
	return (TH2D*) getSignal(signalH2, meH2, int(jetDist->Integral()), sideMin, sideMax);
}

TH2D* jtShape::getSignal(TH2D* signalH2, TH2D* meH2, int njet, float sideMin, float sideMax, bool doSmoothME, bool doSub){
	//bool doSmoothME = true;
	//bool doSub = true;
	TH2D* mixing = (TH2D*)meH2->Clone("mixing");
	TH2D* bkg;
	TString hname = signalH2->GetName();
	TH2D* signal = (TH2D*)signalH2->Clone(hname+"_signal");
	signal->GetXaxis()->SetTitle("#eta");
	signal->GetYaxis()->SetTitle("#phi");
	signal->Scale(1.0/njet/signal->GetXaxis()->GetBinWidth(1)/signal->GetYaxis()->GetBinWidth(1));
	//	signal->Scale(1.0/jetDist->Integral());

	TCanvas *c_signal_debug;
	TH2D *signal_raw_debug, *signal_bkg_debug, *signal_final_debug;
	if( debug){
		signal_raw_debug =(TH2D*) signalH2->Clone("raw_signal_input");
		signal_raw_debug->SetTitle("raw signal input");
		signal_raw_debug->Rebin2D(4,4);
		ymin_signal = signal_raw_debug->GetYaxis()->FindBin(-1);
		ymax_signal = signal_raw_debug->GetYaxis()->FindBin( 1);
		xmin_signal = signal_raw_debug->GetXaxis()->FindBin(-1);
		xmax_signal = signal_raw_debug->GetXaxis()->FindBin( 1);
		c_signal_debug = new TCanvas("c_signal_debug","Bkg substraction debug output",1200,800);
		c_signal_debug->Divide(3,2);
		c_signal_debug->cd(1);
		signal_raw_debug->ProjectionX("raw_eta",ymin_signal, ymax_signal, "e")->Draw();
		c_signal_debug->cd(4);
		signal_raw_debug->ProjectionY("raw_phi",xmin_signal, xmax_signal, "e")->Draw();
	}


	if( doSmoothME ){
		smoothMixingTable(mixing);
	}
	signal->Divide(mixing);
	mixing->Delete();

	if(debug){
		signal_bkg_debug =(TH2D*) signal->Clone("signal_bkg_input");
		signal_bkg_debug->Rebin2D(4,4);
		signal_bkg_debug->SetTitle("signal corrected by mixing table");
		c_signal_debug->cd(2);
		signal_bkg_debug->ProjectionX("signal_bkg_eta",ymin_signal, ymax_signal, "e")->Draw();
		//signal_bkg_debug->ProjectionX()->Draw();
		c_signal_debug->cd(5);
		signal_bkg_debug->ProjectionY("signal_bkg_phi",xmin_signal, xmax_signal, "e")->Draw();
	}

	if( doSub) {
		bkg = (TH2D*) getV2Bkg(signal,sideMin , sideMax );
		signal->Add(signal, bkg, 1, -1);
	}
	if( debug){
		signal_final_debug = (TH2D*) signal->Clone("signal_final_debug");
		signal_final_debug->SetTitle("final signal output");
		signal_final_debug->Rebin2D(4,4);
		c_signal_debug->cd(3);
		signal_final_debug->ProjectionX("_eta",ymin_signal, ymax_signal, "e")->Draw();
		c_signal_debug->cd(6);
		signal_final_debug->ProjectionY("_phi",xmin_signal, xmax_signal, "e")->Draw();
	}
	if( !debug)bkg->Delete();
	return signal;
}

void jtShape::drJetShape(TH2D* signal, TH1D* drDist, float weight, TString opt){
	drIntegral(signal, drDist , opt);
	float cont;
	float error;
	float width;
	for(int i=1; i<drDist->GetNbinsX()+1;i++){
		cont = drDist->GetBinContent(i);
		error= drDist->GetBinError(i);
		width= drDist->GetBinWidth(i);
		drDist->SetBinContent(i, cont/width);
		drDist->SetBinError(i, error/width);

	}
	return ;
}

void jtShape::drIntegral(TH2D* signal, TH1D* drDist, TString opt){	
	//in this integral, we suppose that the distribution in the TH2D* signal has been normalized by the 
	//bin size ( which can be represented as f(x,y)), then the integral should be f(x,y)dxdy (we'll add 
	//the dxdy automatically).
	TString temp = drDist->GetName();
	TH1D* drCounts = (TH1D*) drDist->Clone(temp+"_counts");
	float dr;
	Double_t content;
	Double_t error;
	for(int jx=0; jx<signal->GetNbinsX(); jx++){
		for(int jy=0; jy<signal->GetNbinsY(); jy++){
			dr = sqrt( pow(signal->GetXaxis()->GetBinCenter(jx),2) +\
					pow(signal->GetYaxis()->GetBinCenter(jy),2));
			drCounts->Fill(dr); // get how many bins in the dr region
			float xwidth = signal->GetXaxis()->GetBinWidth(jx);
			float ywidth = signal->GetYaxis()->GetBinWidth(jy);
			//here is f(x,y)dxdy:
			content = signal->GetBinContent(jx,jy)*xwidth*ywidth;
			float binError = signal->GetBinError(jx,jy)*xwidth*ywidth;
			if(opt == "e"){
				error = sqrt(pow(drDist->GetBinError(drDist->FindBin(dr)),2)+\
						pow(binError,2));
			}
			if( content ) {
				//	cout<<content<<endl;
				drDist->Fill(dr, content);
				if(opt=="e") drDist->SetBinError(drDist->FindBin(dr), error);
			}
		}
	}

	//	drDist->Divide(drCounts);
	// convert the counts into the ring area distribution along the ring radius:
	//drCounts->Scale((signal->GetXaxis()->GetBinWidth(1))*(signal->GetYaxis()->GetBinWidth(1)));
	return ;
}

TH1D* jtShape::drGeoTest(TH2D* signal, TH1D* drDist){
	std::cout<<"running the phase space testing ..."<<std::endl;
	TString temp = drDist->GetName();
	TH1D* drCounts = (TH1D*) drDist->Clone(temp+"_counts");
	float dr;
	for(int jx=0; jx<signal->GetNbinsX(); jx++){
		for(int jy=0; jy<signal->GetNbinsY(); jy++){
			dr = sqrt( pow(signal->GetXaxis()->GetBinCenter(jx),2) +\
					pow(signal->GetYaxis()->GetBinCenter(jy),2));
			drCounts->Fill(dr); // get how many bins in the dr region
		}
	}
	drCounts->Scale((signal->GetXaxis()->GetBinWidth(1))*(signal->GetYaxis()->GetBinWidth(1)));

	TH1D* area_ideal = (TH1D*)drCounts->Clone("area_ideal");
	for(int i=1; i<drCounts->GetNbinsX()+1; ++i){
		float rlow = area_ideal->GetBinLowEdge(i);
		float width = area_ideal->GetBinWidth(i);
		float rup = rlow + width;
		//drCounts->SetBinContent(i, drCounts->GetBinContent(i));
		//area_ideal->SetBinContent(i, TMath::Pi()*(pow(rup,2)-pow(rlow,2))); 
		drCounts->SetBinContent(i, drCounts->GetBinContent(i)/width);
		area_ideal->SetBinContent(i, TMath::Pi()*(pow(rup,2)-pow(rlow,2))/width); 
	}
	TCanvas* c = new TCanvas("c_geoCorrection", "", 600,800);
	auto dp = new doublePad((TPad*)gPad);
	dp->addRatioPair(area_ideal, drCounts, kBlue, "2#pir", "grids/#Deltar");
	drCounts->SetMarkerStyle(20);
	drCounts->SetMarkerSize(0.8);
	area_ideal->SetLineColor(kRed);
	/*
	TLegend* tl= new TLegend(0.2,0.7,0.4, 0.85);
	tl->AddEntry(drCounts, "Ring Area/#Deltar");
	tl->AddEntry(area_ideal, "2#pi r");
	tl->SetLineColor(0);
	area_ideal->SetStats(0);
	drCounts->SetTitle("Geometry Correction");
	area_ideal->Draw();
	drCounts->Draw("same");
	tl->Draw();
	*/
	
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
	c->SaveAs("phaseSpaceCorrection.png");
	area_ideal->Divide(drCounts);
	area_ideal->SetName("phaseCorr");
	return area_ideal;
}	

#endif
