

#ifndef JTCSystError_H
#define JTCSystError_H 

#ifndef multiPad_H
#include "multiPad.h"
#endif

class JTCSystError{
		public : 
				JTCSystError(){
						h1 = NULL;
						h2= NULL;
						spill = NULL;jff= NULL; error=NULL;
				};
				JTCSystError(TH1D* h){
						h1 = h;
						h2= NULL;
						spill = NULL;jff= NULL; error=NULL;
				};
				JTCSystError(TH2D* h){
						h2 = h;
						h1= NULL;
						spill = NULL;jff= NULL; error=NULL;
				};
				static Double_t fpol0(Double_t *x, Double_t *par);
				TH1D* proj_deta(TH2D *h, TString name); // project 2D hist into deta 1D with analysis bin;
				void getSystError(bool doFit = true);
				void getAllError(TH1D* signal, TH1D* hjff, TH1D* hspill=NULL, bool isdr = 0);
				void getError();
				void symmetrization(TH1D* h);

		public :
				TH1D* h1; 
				TH2D* h2;
				double mean;
				double fit_error;
				double left_ave;
				double right_ave;
				double in_ave;
				double out_ave;
				double me_err;
				double bg_err;
				double spill_err;
				double jff_err;
				double rela_err;
				double bkg_err; //  it should equal to sqrt( me^2+bg^2) 
				TH1D* spill;
				TH1D* jff;
				TH1D* error;
				TH1D* sig;  // store relative error
				TH1D* bkg;
};

Double_t JTCSystError::fpol0(Double_t *x, Double_t *par){
		//this is the function will be used in exclusive fitting. 
		//the fitting range is [-2.5, -1.5] + [1.5, 2.5];
		if (x[0]<-2.5 || x[0]>2.5 || (x[0]<1.5 && x[0]>-1.5)) {
				TF1::RejectPoint();
				return 0;
		}
		return par[0];
}

TH1D* JTCSystError::proj_deta(TH2D *h, TString name){
		// the input h here should be the signal before/after the bkg subtraction ;
		// and haven't been normalized into the invariant form yet. 
		// the axises correspondence has to be
		// x	:	deta
		// y	:	dphi
		Double_t bin_bounds_eta[20]
				= {-2.5, -2.,-1.5,  -1,  -0.8,  -0.6,  -0.4, -0.3, -0.2, -0.1, 0.1, 0.2, 0.3, 0.4, 0.6,  0.8, 1, 1.5,2., 2.5 };	
		int dphi1 = h->GetYaxis()->FindBin(-1);
		int dphi2 = h->GetYaxis()->FindBin( 1)-1;
		TString stmp = h->GetName();
		stmp = stmp +"_dEta";
		TH1D* tmp = (TH1D*) h->ProjectionX(stmp, dphi1, dphi2);
		Int_t new_bins = sizeof(bin_bounds_eta) / sizeof(bin_bounds_eta[0])-1;
		TH1D* h1 =  (TH1D*) tmp->Rebin(new_bins, name, bin_bounds_eta);
		for(int i=1; i<h1->GetNbinsX()+1; ++i){
				h1->SetBinContent(i, h1->GetBinContent(i)/h1->GetBinWidth(i));
				h1->SetBinError  (i, h1->GetBinError  (i)/h1->GetBinWidth(i));
		}
		delete tmp; tmp = NULL;
		return h1;
}

void JTCSystError::symmetrization(TH1D* h){
		int stop; 
		if( (h->FindBin(0)-1)*2== h->GetNbinsX()) stop = h->FindBin(0)-1;
		else stop = h->FindBin(0);
		for(int k=1; k<stop+1 ; ++k){
				float c = h->GetBinContent(k);
				float x = h->GetBinCenter(k);
				c=c+ h->GetBinContent(h->FindBin(-x));
				h->SetBinContent(k, c/2);
				h->SetBinContent(h->FindBin(-x), c/2);
		}
		return ;
}

void JTCSystError::getSystError(bool doFit){
		// the input h here should come from the function proj_deta
		// this function should return the set storing all the 
		// syst error for both ME + BG
		// if doFit = false then the bkg is set to be 0 but the err still comes from fitting
		TString stmp ; 
		if( h1 ==NULL){
				stmp = h2->GetName();
				stmp = stmp+"_deta_proj";
				h1 = proj_deta(h2, stmp);
		}
		TF1* fbkg = new TF1("fbkg", JTCSystError::fpol0, -2.5, 2.5, 1);	
		h1->Fit(fbkg, "0");
		int l1p5 = h1->FindBin(-1.4999)-1;
		int l2   = h1->FindBin(-2.0001);
		int r2   = h1->FindBin( 2.0);
		int r1p5 = h1->FindBin( 1.5);
		if( doFit) {
				mean = double( fbkg->GetParameter(0));
		}
		else {
				mean = 0;
		}
		fit_error= double( fbkg->GetParError(0) );
		left_ave  = (h1->GetBinContent(l1p5)+h1->GetBinContent(l2))/2;
		right_ave = (h1->GetBinContent(r1p5)+h1->GetBinContent(r2))/2;
		in_ave = (h1->GetBinContent(l1p5)+h1->GetBinContent(r1p5))/2;
		out_ave= (h1->GetBinContent(l2)+h1->GetBinContent(r2))/2;
		me_err = 2*max(fabs(left_ave-mean), fabs(right_ave-mean));
		bg_err = 2*max(fabs(in_ave-mean), fabs(out_ave-mean));
		return ;
}

void JTCSystError::getAllError(TH1D* h, TH1D* hjff, TH1D* hspill, bool isdr){
		//should setup h1 (to be the signal ), rela_err (reconstruction error), spill_err and jff_err before call this function
		//
		TString st = hjff->GetName();  st = st+"_jff_err";
		jff = (TH1D*) hjff->Clone(st);

		st = h->GetName();  st = st+"_syst_err";
		error = (TH1D*) h->Clone(st);

		st = h->GetName();  st = st+"_spill_err";
		if( hspill == NULL) spill = (TH1D*) h->Clone(st);
		else {
				spill = (TH1D*) hspill->Clone(st);
				spill->Scale(spill_err);
		}

		st = h->GetName();  st = st+"_rela_err";
		sig = (TH1D*) h->Clone(st);
		sig->Scale(rela_err);

		jff->Scale(jff_err);
		st = h->GetName();  st = st+"_bkg_err";
		bkg = (TH1D*) h->Clone(st);
		spill->SetBinContent(spill->FindBin(2.2), spill->GetBinContent(spill->FindBin(-2.5)));
		symmetrization(h);
		symmetrization(jff);
		for(int k=1; k<h->GetNbinsX()+1; ++k){
				spill->SetBinError(k,0);
				jff->SetBinError(k,0);
				bkg->SetBinError(k,0);
				sig->SetBinError(k,0);
				jff->SetBinContent(k,fabs(jff->GetBinContent(k)));
				if( hspill ==NULL) spill->SetBinContent(k, 0);
				if(isdr){
						// pi*(R^2-r^2) for ring area, divided by 2 for dphi projection length
						float ring =TMath::Pi()*(pow(bkg->GetBinLowEdge(k)+bkg->GetBinWidth(k),2)-
										pow(bkg->GetBinLowEdge(k),2))/2/bkg->GetBinWidth(k);
						bkg->SetBinContent(k, bkg_err*ring);
				}
				else bkg->SetBinContent(k, bkg_err);
				float a = spill->GetBinContent(k);
				float b = jff->GetBinContent(k);
				float c = sig->GetBinContent(k);
				float d = bkg->GetBinContent(k);
				error->SetBinContent(k,pow(a*a+b*b+c*c+d*d,0.5));
				error->SetBinError(k,0);
		}
		if(!isdr)symmetrization(error);
}


class JTCSystCanvas : public baseCanvas {
		public : 		
				JTCSystCanvas(TString name, TString title, int nrow1, int ncol1): baseCanvas(name, title, nrow1, ncol1){
						isempty = 1;
						list.resize(nrow1*ncol1);
				};
				int index(int i, int j){
						//return the index for pad , the index for list should be minus 1
						return (i-1)*ncol+j;
				};
				void addError(JTCSystError *err, int row, int col);
				void drawFit();
				void drawSummary(float x, float y, bool symm=1);
				void adjustPlotRange(TH1D* h, double center);
				JTCSystError * getErr(int i, int j){
						return list.at(index(i,j)-1);
				};
				void coutErr();
		public : 
				std::vector< JTCSystError *> list;	
				bool isempty;
};

void JTCSystCanvas::addError(JTCSystError *err, int row, int col){
		cout<<row<<", "<<col<<", "<<index(row, col)<<endl;
		list.at(index(row, col)-1) = err;
		return;	
}

void JTCSystCanvas::drawSummary(float x, float y, bool symm){
		cout<<list.size()<<endl;
		for(int i=1; i<nrow+1; ++i){
				float maximum;
				if(!symm) maximum= getErr(i,ncol)->error->GetMaximum()*1.2;
				else maximum= getErr(i,ncol)->error->GetBinContent(getErr(i,ncol)->error->FindBin(0))*1.2;
				for(int j=1; j<ncol+1; ++j){
						cd(index(i,j));
						cout<<i<<", "<<j<<", "<<index(i,j)<<endl;
						TH1D* h= getErr(i,j)->error;
						if(j!=1) h->GetYaxis()->SetLabelSize(0);
						else h->GetYaxis()->SetLabelSize(0.1);
						if(i!=nrow) h->GetXaxis()->SetLabelSize(0.);
						else  h->GetXaxis()->SetLabelSize(0.08);
						h->SetAxisRange(x, y , "X");
						h->SetAxisRange(0, maximum , "Y");

//						getErr(i,j)->error->SetLineColor(kAzure-9);
						getErr(i,j)->error->SetLineColor(kBlack);
						getErr(i,j)->bkg->SetLineColor(kGreen+2);
						getErr(i,j)->sig->SetLineColor(kViolet);
						getErr(i,j)->jff->SetLineColor(kRed+1);
						getErr(i,j)->spill->SetLineColor(kBlue);
						getErr(i,j)->error->SetLineWidth(2);
						getErr(i,j)->bkg->SetLineWidth(1);
//						getErr(i,j)->bkg->SetLineStyle(1);
						getErr(i,j)->error->Draw();
						getErr(i,j)->jff->Draw("same");
						getErr(i,j)->spill->Draw("same");
						getErr(i,j)->bkg->Draw("same");
						getErr(i,j)->sig->Draw("same");
				}
		}
		/*
		TLegend * lege  = new TLegend(0.4, 0.3, 0.9, 0.75);
		lege->SetFillColor(0);
		lege->SetLineColor(0);
		lege->AddEntry(getErr(1,1)->error, "Total Error");
		lege->AddEntry(getErr(1,1)->bkg, "ME+BG");
		lege->AddEntry(getErr(1,1)->jff, "Jff residual");
		lege->AddEntry(getErr(1,1)->spill, "Spill-over");
		cd(index(1, 1));
		lege->Draw();
		*/
}

void JTCSystCanvas::drawFit(){
		TLine line;
		TBox box;
		box.SetFillColorAlpha(kAzure+6, 0.5);
		for(int i=1; i<nrow+1; ++i){
				for(int j=1; j<ncol+1; ++j){
						TH1D* h;
						h = getErr(i,1)->h1;
						double err = h->GetBinError(h->FindBin(-2.));
						float center  =  0;
						cd(index(i,j)); h = getErr(i,j)->h1;
						if(j!=1) h->GetYaxis()->SetLabelSize(0);
						else h->GetYaxis()->SetLabelSize(0.1);
						if(i!=nrow) h->GetXaxis()->SetLabelSize(0.);
						else  h->GetXaxis()->SetLabelSize(0.08);
						//adjustPlotRange(h, getErr(i,j)->mean );
						h->SetAxisRange(center-5*err, center+5*err, "Y");
						h->Draw();
						line.SetLineColor(kBlack);
						line.SetLineStyle(2);
						line.DrawLine(-2.5, getErr(i,j)->mean, 2.5, getErr(i,j)->mean);			
						line.SetLineColor(kRed);
						line.SetLineStyle(1);
						line.DrawLine(-2.5, getErr(i,j)->left_ave, -1.5, getErr(i,j)->left_ave);			
						line.DrawLine(1.5, getErr(i,j)->right_ave, 2.5, getErr(i,j)->right_ave);			
						box.DrawBox(-2.5,getErr(i,j)->mean-getErr(i,j)->me_err/2, -1.5, getErr(i,j)->mean+getErr(i,j)->me_err/2);
						box.DrawBox(1.5,getErr(i,j)->mean-getErr(i,j)->me_err/2, 2.5, getErr(i,j)->mean+getErr(i,j)->me_err/2);
				}
		}
}

void JTCSystCanvas::coutErr(){
		std::cout<<"ME Error"<<std::endl;
		for(int i=1; i<nrow+1; ++i){
				for(int j=1; j<ncol+1; ++j){
						std::cout<<getErr(i,j)->me_err/2<<"   ";
				}
				cout<<endl;
		}
		std::cout<<"BG Error"<<std::endl;
		for(int i=1; i<nrow+1; ++i){
				for(int j=1; j<ncol+1; ++j){
						std::cout<<getErr(i,j)->bg_err/2<<"   ";
				}
				cout<<endl;
		}
		std::cout<<"Syst Error"<<std::endl;
		for(int i=1; i<nrow+1; ++i){
				std::cout<<i<<endl;
				for(int j=1; j<ncol+1; ++j){
						std::cout<<pow(pow(getErr(i,j)->bg_err/2,2)+pow(getErr(i,j)->me_err/2,2),0.5) <<"   ";
				}
				cout<<endl;
		}
}


void JTCSystCanvas::adjustPlotRange(TH1D* h, double center){
		double err = h->GetBinError(h->FindBin(-2.));
		h->SetAxisRange(center-20*err, center+10*err, "Y");
		return;
}
#endif
