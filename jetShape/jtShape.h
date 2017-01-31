

#ifndef JTSHAPE_H
#define JTSHAPE_H

#include "TH1F.h"
#include "TH2F.h"
#include "TH2F.h"


class jtShape {
	public:
		jtShape();
		TCanvas* c_debug;
		TH2D* bkgSub(TH2F* signalH2, TH2F* meH2, TH1F* jetDist, float sideMin = 1.5,float sideMax = 2);
		//Divide the signal by the mixing table;
		void corrMixEvent(TH2D* signal, TH2D* bkg, bool doNorm = true);
		//for getting the sideband average;
		TH1D* getBkgTH1D(TH2D* signalH2, TH2D* meH2,TString hname, float sideMin = 1.5,float sideMax = 2);
		//producing a TH2 from TH1 by propagating the TH1 along x/y axis
		void makeTH2DFromTH1D(TH1D* h1, TH2D* h2, string axis = "x");
		TH1D* drIntegral(TH2D* signalh2, TH1D* drDisth1, TString opt ="e");
		TH1D* drJetShape(TH2D* signalh2, TH1D* drDisth1, float weight, TString opt ="e");
		//for testing the asymmetry of the sideband we used to substract the v2 contribution;
		TH1D* asymmetryTest(TH2D* signal, TH2D* bkg, TString hname, float min, float max);
		// custermed function ---------------
		TH1D* drRatio(TH2D* hnum, TH2D* hden, TH1D* hdr);

};

jtShape::jtShape(){
}


/* the bkgSub including both the mixing event correction and the sideband based background substraction.
 */
TH2D* jtShape::bkgSub(TH2F* signalH2, TH2F* meH2, TH1F* jetDist, float sideMin = 1.5, float sideMax = 2){

	bool doSmoothME = true;
	bool doSub = true;

	bool debug = false;	
	TH1D* fBkgh1[4];
	TH2D* fBkgh2[4];

	//preserve the input variables
	TH2D* bkg = (TH2D*)meH2->Clone("jtShape_bkg");
	TString hname = signalH2->GetName();
	TH2D* signal = (TH2D*)signalH2->Clone(hname+"_signal");
	//make the mixing table smooth along dphi
	TH1D* temp = (TH1D*)bkg->ProjectionX("_eta");

	/* for debugging, it will generate 2by3 plots showing:
	 * 1. signal    2. smoothed mixing     3. signal/mixing
	 * 4. signal & background overlay in y direction
	 * 5. signal & background overlay in x direction
	 * 6. signal after background substraction.
	 */
	if( debug ){
		c_debug =new TCanvas("c_debug","jtShapeBkgSub_debugPlots", 1000, 800);
		c_debug->Divide(3,2);
		fBkgh2[0] = (TH2D*) signalH2->Clone("fBkgh2_1");
	}
	//smooth the mixing table
	if( doSmoothME ){
		for(int ix=1; ix<bkg->GetNbinsX()+1; ix++){
			for(int iy=1; iy<bkg->GetNbinsY()+1; iy++){
				bkg->SetBinContent(ix, iy, temp->GetBinContent(ix)/bkg->GetNbinsY());
				bkg->SetBinError(ix, iy, temp->GetBinError(ix)/sqrt(bkg->GetNbinsY()));
			}
		}
	}
	bkg->Scale(1.0/bkg->GetBinContent(bkg->FindBin(0., 0.)));
	if(debug){
		c_debug->cd(1);
		fBkgh2[0]->Draw("surf2");
		c_debug->cd(2);
		fBkgh2[1] = (TH2D*)bkg->Clone("fBkgh2_2");
		fBkgh2[1]->Draw("surf2");
	}
	
//	signal->Scale(1.0/jetDist->Integral()/signal->GetXaxis()->GetBinWidth(1)/signal->GetYaxis()->GetBinWidth(1));
	signal->Scale(1.0/jetDist->Integral());

	if(debug){
		c_debug->cd(3);
		fBkgh2[2]=(TH2D*) signal->Clone("fBkgh2_3");
		fBkgh2[2]->Draw("surf2");
	}
	//divide the signal by mixing table
	signal->Divide(bkg);
	//normalize the signal table by # of jt
	//get the v2 contribution from the sideband and remove the v2;
	if(doSub){
		int outterRight= temp->FindBin(sideMax); 	
		int innerRight = temp->FindBin(sideMin); 	
		int outterLeft = temp->FindBin(-temp->GetBinCenter(outterRight)); 	
		int innerLeft  = temp->FindBin(-temp->GetBinCenter(innerRight)); 
		bkg->Delete();
		bkg = (TH2D*)meH2->Clone("jtShape_bkg");
		temp = (TH1D*)signal->ProjectionY("jtShape_sideBandProj", innerRight, outterRight, "e");
		temp->Add(signal->ProjectionY("", outterLeft, innerLeft, "e"));
		int nbin = outterRight-innerRight+innerLeft-outterLeft+2;
		for(int ix=1; ix<bkg->GetNbinsX()+1; ix++){
			for(int iy=1; iy<bkg->GetNbinsY()+1; iy++){
				if(signal->GetBinContent(ix,iy)!=0){
					bkg->SetBinContent(ix, iy, temp->GetBinContent(iy)/nbin);
					bkg->SetBinError(ix, iy, temp->GetBinError(iy)/sqrt(nbin));
				}
				else{
					bkg->SetBinContent(ix,iy,0);
					bkg->SetBinError(ix,iy,0);
				}
			}
		}

		if( debug){
			c_debug->cd(4);
			fBkgh1[0]=(TH1D*)bkg->ProjectionY("fBkgh_sideband_py",outterLeft, outterRight);
			fBkgh1[1]=(TH1D*)signal->ProjectionY("fBkgh_signal_py",outterLeft, outterRight);
			fBkgh1[2]=(TH1D*)bkg->ProjectionX("fBkgh_sideband_px");
			fBkgh1[3]=(TH1D*)signal->ProjectionX("fBkgh_signal_px");
			fBkgh1[1]->SetMarkerStyle(20);
			fBkgh1[1]->SetMarkerSize(.5);
			fBkgh1[1]->Draw();
			fBkgh1[0]->SetLineColor(kRed);
			fBkgh1[0]->SetMarkerStyle(20);
			fBkgh1[0]->SetMarkerSize(0.5);
			fBkgh1[0]->SetMarkerColor(kRed);
			fBkgh1[0]->Draw("same");

			c_debug->cd(5);
			fBkgh1[3]->SetAxisRange(-sideMax,sideMax,"x");
			fBkgh1[3]->SetMarkerStyle(20);
			fBkgh1[3]->SetMarkerSize(.5);
			fBkgh1[3]->Draw();
			fBkgh1[2]->SetLineColor(kRed);
			fBkgh1[2]->SetMarkerStyle(20);
			fBkgh1[2]->SetMarkerSize(0.5);
			fBkgh1[2]->SetMarkerColor(kRed);
			fBkgh1[2]->Draw("same");

		}
		signal->Add(signal, bkg, 1, -1);
		if(debug){
			c_debug->cd(6);
			fBkgh2[3] = (TH2D*)signal->Clone("fBkgh2_signal");
			fBkgh2[3]->Draw("surf2");
		}
	}

	//---------------------
		temp->Delete();
		bkg->Delete();
	/*
	*/
	return signal;
}

/* the drIntegral function really just integral the TH2 histogram in along different radius rang.
 * The TH1 input drDist should be the histogram candidate for containning the dr distribution and
 * the function will fill it automatically.
 * The option opt here just refer to should we calculate the error while the filling.
 * and the returned TH1 histogram just show the dA/dr (area) distribution of the ring
 */
TH1D* jtShape::drIntegral(TH2D* signal, TH1D* drDist, TString opt){	
	TString temp = drDist->GetName();
	TH1D* drCounts = (TH1D*) drDist->Clone(temp+"_counts");
	float dr;
	Double_t content;
	Double_t error;
	for(int jx=0; jx<signal->GetNbinsX(); jx++){
		for(int jy=0; jy<signal->GetNbinsY(); jy++){
			dr = sqrt( pow(signal->GetXaxis()->GetBinCenter(jx),2) +\
					pow(signal->GetYaxis()->GetBinCenter(jy),2));
			drCounts->Fill(dr);
			if(opt == "e"){
			error = sqrt(pow(drDist->GetBinError(drDist->FindBin(dr)),2)+\
					pow(signal->GetBinError(jx,jy),2));
			}
			content = signal->GetBinContent(jx,jy);
			if( content ) {
				//	cout<<content<<endl;
				drDist->Fill(dr, content);
				if(opt=="e") drDist->SetBinError(drDist->FindBin(dr), error);
			}
		}
	}

//	drDist->Divide(drCounts);
	return drCounts;
}


TH1D* jtShape::drJetShape(TH2D* signal, TH1D* drDist, float weight, TString opt){
	drIntegral(signal, drDist , opt);

	float cont;
	float error;
	float width;
	for(int i=1; i<drDist->GetNbinsX();i++){
		cont = drDist->GetBinContent(i);
		error= drDist->GetBinError(i);
		width= drDist->GetBinWidth(i);
		drDist->SetBinContent(i, cont/width);
		drDist->SetBinError(i, error/width);
	}
	return drDist;
}

void jtShape::makeTH2DFromTH1D(TH1D* h1, TH2D* h2, string axis){
	if(axis == "x"){
		for(int ix=1; ix<h2->GetNbinsX()+1; ix++){
			for(int iy=1; iy<h2->GetNbinsY()+1; iy++){
				h2->SetBinContent(ix, iy, h1->GetBinContent(ix)/h2->GetNbinsY());
				h2->SetBinError(ix, iy, h1->GetBinError(ix)/sqrt(h2->GetNbinsY()));
			}
		}
	}
	else if( axis=="y"){
		for(int ix=1; ix<h2->GetNbinsX()+1; ix++){
			for(int iy=1; iy<h2->GetNbinsY()+1; iy++){
				h2->SetBinContent(ix, iy, h1->GetBinContent(iy)/h2->GetNbinsX());
				h2->SetBinError(ix, iy, h1->GetBinError(iy)*sqrt(h2->GetNbinsX()));
			}
		}
	}
}

TH1D* jtShape::drRatio(TH2D* hnum, TH2D* hden, TH1D* hdr){

	TString hname = hnum->GetName();
	TH1D* hreturn =(TH1D*) hdr->Clone("drRatio_"+hname); 
	drIntegral(hnum, hreturn);

	TH1D* htemp =(TH1D*) hdr->Clone("jtShape_drRatio_temp"); 
	drIntegral(hden , htemp);	

	hreturn->Divide(hreturn, htemp, 1,1,"B");
	htemp->Delete();

	return hreturn;
}

/* here is only the mixing event correction part same as the part in bkgSub, 
 * In the several following functions, they are just the separation of the function
 * bkgSub.
 */
void jtShape::corrMixEvent(TH2D* signal, TH2D* bkg, bool doNorm){
	bool doSmoothME = true;
	//make the mixing table smooth along dphi
	TH1D* temp = (TH1D*)bkg->ProjectionX("_eta");
	makeTH2DFromTH1D(temp, bkg);
	bkg->Scale(1.0/bkg->GetBinContent(bkg->FindBin(0., 0.)));
	if(doNorm) signal->Scale(1.0/signal->GetXaxis()->GetBinWidth(1)/signal->GetYaxis()->GetBinWidth(1));
	signal->Divide(signal,bkg);
	temp->Delete();
}

TH1D* jtShape::getBkgTH1D(TH2D* signalH2, TH2D* meH2,TString hname, float sideMin = 1.5,float sideMax = 2){
	TH2D* signal = (TH2D*)signalH2->Clone("signal_temp");
	TH2D* bkg = (TH2D*) meH2->Clone("bkg");
	corrMixEvent(signal, bkg, true);
	int outterRight= bkg->GetXaxis()->FindBin(sideMax); 	
	int innerRight = bkg->GetXaxis()->FindBin(sideMin); 	
	int outterLeft = bkg->GetXaxis()->FindBin(-bkg->GetXaxis()->GetBinCenter(outterRight)); 	
	int innerLeft  = bkg->GetXaxis()->FindBin(-bkg->GetXaxis()->GetBinCenter(innerRight)); 
	TH1D* temp = (TH1D*)signal->ProjectionY(hname, innerRight, outterRight, "e");
	temp->Add(signal->ProjectionY("", outterLeft, innerLeft, "e"));
	int nbin = outterRight-innerRight+innerLeft-outterLeft+2;
	temp->Scale(1.0/(nbin));
	signal->Delete();
	bkg->Delete();
	return temp;
}

TH1D* jtShape::asymmetryTest(TH2D* signalH2, TH2D* meH2, TString hname , float sideMin, float sideMax){
	TH2D* signal = (TH2D*)signalH2->Clone("signal_temp");
	TH2D* bkg = (TH2D*) meH2->Clone("bkg");
	corrMixEvent(signal, bkg, true);
	int outterRight= bkg->GetXaxis()->FindBin(sideMax); 	
	int innerRight = bkg->GetXaxis()->FindBin(sideMin); 	
	int outterLeft = bkg->GetXaxis()->FindBin(-bkg->GetXaxis()->GetBinCenter(outterRight)); 	
	int innerLeft  = bkg->GetXaxis()->FindBin(-bkg->GetXaxis()->GetBinCenter(innerRight)); 
	int nbin = outterRight-innerRight+1;

	TH1D* temp = (TH1D*)signal->ProjectionY(hname, innerRight, outterRight, "e");
	temp->Add(temp, signal->ProjectionY("", outterLeft, innerLeft, "e"), 1,-1);
	return temp;
}

#endif
