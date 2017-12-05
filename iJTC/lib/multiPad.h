

#ifndef multiPad_H
#define multiPad_H

#ifndef ROOT_TCanvas
#include "TCanvas.h"
#endif

//#ifndef xTHD12_H
//#include "xTHD12.h"
//#endif

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
		void addHist(TH1* h, int row, int col, TString opt= "");
		void draw();
		void useFrame();
		void addLine(float x1, float y1, float x2, float y2);

	public :
		std::multimap<int ,TH1*> buff;
		std::map<TH1*, TString> buf_opt;
		TLine *tl;
		TH1* hframe;
		bool doFrame;
		float lx1, lx2, ly1, ly2;
		bool doline=0;
};

void multiPad::addHist(TH1* h, int row, int col, TString opt){
	if(col!=1) h->GetYaxis()->SetLabelSize(0);
	else h->GetYaxis()->SetLabelSize(0.1);
	if(row!=nrow) h->GetXaxis()->SetLabelSize(0.);
	else  { 
			h->GetXaxis()->SetLabelSize(0.08); 
			h->GetXaxis()->SetTitleSize(0.1);
			h->GetXaxis()->SetTitleOffset(0.8);
	}
	buff.insert(std::pair<int, TH1*> ((row-1)*ncol+col, h)); 
	buf_opt.insert(std::pair<TH1*, TString > (h, opt));
}

void multiPad::addLine(float x1, float y1, float x2, float y2){
		doline = 1;
		lx1=x1; lx2=x2; ly1=y1; ly2=y2;
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
//			gPad->SetLogy();
			TString opt = buf_opt.at(it->second);
			if(it != ii.first) opt = "same "+opt;
			(it->second)->Draw(opt);
		}
		if(doline) tl->DrawLine(lx1, ly1, lx2, ly2);
	}
	this->cd(1);
	this->Update();
}

void multiPad::useFrame(){
}


#endif
