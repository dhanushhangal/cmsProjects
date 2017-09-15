

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
			TCanvas(hname, title, ncol1*650, nrow1*500)
	{
		//pad=(TPad*)gPad;	
		this->SetMargin(0.15, 0.05, 0.25, 0.2);
		this->Divide(ncol,nrow,0,0);
		gStyle->SetOptStat(0);
		cname = hname;
	}
		void histoConfig(TH1* h);

	public :
		const int nrow, ncol;
		TPad *pad;
		TString cname;
};

void baseCanvas::histoConfig(TH1* h){
	h->SetMarkerStyle(20);
	h->SetMarkerSize(0.8);
}


class multiPad: public baseCanvas{
	public :
		multiPad(TString name, TString title, int nrow1, int ncol1):
			baseCanvas(name, title, nrow1, ncol1)
	{
	}
		void addHist(TH1* h, int row, int col);
		void draw();

	public :
		std::multimap<int ,TH1*> buff;
		TLegend *tl;
};

void multiPad::addHist(TH1* h, int row, int col){
	buff.insert(std::pair<int, TH1*> ((row-1)*ncol+col, h));
}

void multiPad::draw(){
//	this->Draw();
	std::pair<multimap<int,TH1*>::iterator, multimap<int, TH1*>::iterator> ii;
	multimap<int, TH1*>::iterator it;
	for(int n=1; n<nrow*ncol+1; ++n){
		this->cd(n);
		ii = buff.equal_range(n);
		for(it = ii.first; it != ii.second; ++it){
			cout<<it->second->GetName()<<endl;
			(it->second)->Draw("same");
		}
	}
	this->cd(1);
	this->Update();
}

#endif
