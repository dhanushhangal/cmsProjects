

#ifndef multiPad_H
#define multiPad_H

#ifndef ROOT_TCanvas
#include "TCanvas.h"
#endif


class baseCanvas: public TCanvas{

	public :
		baseCanvas(TString hname, TString title, int nrow1, int ncol1) :
			nrow(nrow1),
			ncol(ncol1),
//			TCanvas(hname, title, ncol1*550+100, nrow1*500+50)
			TCanvas(hname, title, ncol1*550+100, nrow1*400+50)
	{
		//pad=(TPad*)gPad;	
		this->SetMargin(0.15, 0.05, 0.2, 0.10);
		this->Divide(ncol,nrow,0,0);
		gStyle->SetOptStat(0);
		cname = hname;
		tx = new TLatex();
	}
		void histoConfig(TH1* h);

	public :
		const int nrow, ncol;
		TPad *pad;
		TString cname;
		TLatex *tx;
};

void baseCanvas::histoConfig(TH1* h){
	h->SetMarkerStyle(20);
	h->SetMarkerSize(1);
	h->GetXaxis()->CenterTitle();
	h->GetYaxis()->CenterTitle();
}


class multiPad: public baseCanvas{
	public :
		multiPad(TString name, TString title, int nrow1, int ncol1):
			baseCanvas(name, title, nrow1, ncol1)
	{
		tl = new TLine();
		doFrame=false;
	}
		void addHist(TH1* h, int row, int col);
		void draw();
		void useFrame();

	public :
		std::multimap<int ,TH1*> buff;
		TLine *tl;
		TH1* hframe;
		bool doFrame;
};

void multiPad::addHist(TH1* h, int row, int col){
	if(col!=1) h->GetYaxis()->SetLabelSize(0);
	if(row!=nrow) h->GetXaxis()->SetLabelSize(0.);
	buff.insert(std::pair<int, TH1*> ((row-1)*ncol+col, h)); }

void multiPad::draw(){
//	this->Draw();
	std::pair<multimap<int,TH1*>::iterator, multimap<int, TH1*>::iterator> ii;
	multimap<int, TH1*>::iterator it;
	for(int n=1; n<nrow*ncol+1; ++n){
		this->cd(n);
		ii = buff.equal_range(n);
		for(it = ii.first; it != ii.second; ++it){
			cout<<it->second->GetName()<<endl;
			if(it == ii.first) (it->second)->Draw();
			else (it->second)->Draw("same");
		}
	}
	this->cd(1);
	this->Update();
}

void multiPad::useFrame(){
}

#endif
