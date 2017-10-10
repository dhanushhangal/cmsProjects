

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
		drawLine=false;
	}
		void addHist(TH1* h, int row, int col);
		void draw();
		void useFrame();
		void baseLine(float, float, float, float);

	public :
		std::multimap<int ,TH1*> buff;
		TLine *tl;
		TH1* hframe;
		bool doFrame;
		bool drawLine;
		float lx1, lx2, ly1, ly2;
};

void multiPad::addHist(TH1* h, int row, int col){
	if(col!=1) h->GetYaxis()->SetLabelSize(0);
	if(row!=nrow) h->GetXaxis()->SetLabelSize(0.);
	buff.insert(std::pair<int, TH1*> ((row-1)*ncol+col, h)); }

void multiPad::draw(){
//	this->Draw();
	if(drawLine)tl->SetLineStyle(2);
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
		if(drawLine) tl->DrawLine(lx1, ly1, lx2, ly2);
	}
	this->cd(1);
	this->Update();
}

void multiPad::useFrame(){
}
void multiPad::baseLine(float x1, float y1, float x2, float y2){
	lx1=x1;	lx2=x2;	ly1=y1; ly2=y2;
	drawLine=true;
}

#endif
