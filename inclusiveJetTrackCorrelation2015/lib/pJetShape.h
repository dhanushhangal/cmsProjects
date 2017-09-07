
#include "stackHist.h"

class pJetShape: public TCanvas{
	public :
		pJetShape(TString name, TString title="", float w=2500, float h=1600): TCanvas(name,title, w, h){
			c=(TPad*)gPad;
			c->SetMargin(0.2, 0.05, 0.25, 0.2);
			c->Divide(5,2,0,0);
			gStyle->SetOptStat(0);
			for(int i=0; i<5; ++i) st[i]=new stackHist(Form("st_%d",i));
			tl= new TLine();
			tl->SetLineStyle(2);
			tx= new TLatex();
		};
		~pJetShape();
		void addHist(TH1* h, int jcent);
		void initHist();
		void draw();
		void getRatio();
		void addLegend(TString *);
		void xAxisSetup(TAxis *);
		void xAxisSetup(TGaxis *);
		void yAxisSetup(TAxis *);
		void yAxisSetup(TGaxis *);
		void drawLabels(TGaxis *);

	public :
		TPad * c;
		stackHist *st[5];
		TH1D* ratio[5];
		TLine * tl;
		TLatex * tx;
		TLegend* leg;
		TString* legTxt;
		TGaxis* axis_x;
		TGaxis *axis_y;
};


void pJetShape::addHist(TH1* h, int jcent){
	st[jcent]->addHist((TH1*) h);	
}

void pJetShape::initHist(){
	for(int i=0;i<5;++i){
		st[i]->setRange(0,0.99,"x");
		st[i]->setRange(0.5,3000,"y");
		st[i]->setLabelSize(0);
		st[i]->setLabelSize(0, "y");
	}
}

void pJetShape::getRatio(){
	ratio[4]=(TH1D*) st[4]->sumOver();
	for(int jc=0; jc<4; ++jc){
		ratio[jc]=(TH1D*) st[jc]->sumOver();
		ratio[jc]->Divide(ratio[4]);
		xAxisSetup(ratio[jc]->GetXaxis());
		ratio[jc]->SetAxisRange(0,0.99,"X");
		ratio[jc]->SetAxisRange(0,3.2,"Y");
		ratio[jc]->SetMarkerStyle(20);
		ratio[jc]->SetMarkerSize(0.8);
		ratio[jc]->GetXaxis()->SetTitle("#Delta r");
		ratio[jc]->GetYaxis()->SetLabelSize(0);
	}
}

void pJetShape::addLegend(TString *ss){
	legTxt=ss;
}

void pJetShape::draw(){
	initHist();
	getRatio();
	st[4]->ytitle = "#Rho(#Deltar) (GeV)";
	for(int jcent=0; jcent<5; ++jcent){
		c->cd(5-jcent);
		gPad->SetLogy();
		st[jcent]->drawStack();
	}
	yAxisSetup(st[4]->hst->GetYaxis());
	st[4]->hst->Draw();
	for(int jc=0; jc<4; ++jc){
		c->cd(10-jc);
		gPad->SetBottomMargin(0.2);
		ratio[jc]->Draw("same");
		tl->DrawLineNDC(0, 1, 1,1);
	}
	leg = st[1]->makeLegend(legTxt, 0.05, 0.05,0.9, 0.85);
	c->cd(6);
	leg->Draw();	
	axis_y= new TGaxis(1    ,0.2, 1, 1, 0, 3.2,510, "");
	axis_x= new TGaxis(0.235, 1 , 1, 1, 0, 1  ,505, "");
	xAxisSetup(axis_x);
	yAxisSetup(axis_y);
	axis_y->SetTitle("#Rho(#Deltar)_{PbPb}/#Rho(#Deltar)_{pp}");
	axis_y->Draw();
	axis_x->SetTitle("#Delta r");
	axis_x->Draw("same");
	drawLabels();
}

void pJetShape::drawLabels(){
	c->cd(1);
	tx->SetTextFont(63);
	tx->SetTextSizePixels(20);
	tx->SetLineColor(kWhite);
	tx->DrawLatexNDC(0.3, 0.8, "CMS");
	tx->SetTextFont(53);
	tx->DrawLatexNDC(0.45, 0.8, "Preliminary");

	tx->SetLineColor(kBlack);
	tx->SetTextFont(63);
	tx->DrawLatexNDC(0.28, 0.87, "pp reference");
	c->cd(2);
	tx->DrawLatexNDC(0.08, 0.87, "PbPb Cent.50-100%");
	c->cd(3);
	tx->DrawLatexNDC(0.08, 0.87, "PbPb Cent.30-50%");
	c->cd(4);
	tx->DrawLatexNDC(0.08, 0.87, "PbPb Cent.10-30%");
	c->cd(5);
	tx->DrawLatexNDC(0.08, 0.87, "PbPb Cent.0-10%");
}

void pJetShape::xAxisSetup(TAxis* a){
	a->SetLabelOffset(0.02);
	a->SetNdivisions(505);
	a->SetTitleOffset(0.9);
	a->SetTitleSize(0.08);
	a->SetLabelSize(0.08);
	a->SetTickLength(0.025);
	a->SetLabelFont(42);
	a->CenterTitle();
	a->SetTitleFont(42);
}
void pJetShape::xAxisSetup(TGaxis* a){
	a->SetLabelOffset(0.02);
	a->SetLabelSize(0.07);
	a->SetTitleOffset(0.9);
	a->SetTitleSize(0.07);
	a->SetTitleFont(42);
	a->SetTickLength(0.025);
	a->SetLabelFont(42);
	a->CenterTitle();
}
void pJetShape::yAxisSetup(TAxis* a){
	a->SetNdivisions(505);
	a->SetTitleSize(0.1);
	a->SetTitleOffset(0.9);
	a->SetLabelSize(0.08);
	a->SetLabelOffset(0.004);
	a->SetTickLength(0.025);
	a->SetLabelFont(42);
	a->SetTitleFont(42);
}
void pJetShape::yAxisSetup(TGaxis* a){
	a->SetTitleSize(0.1);
	a->SetTitleOffset(1);
	a->SetLabelOffset(0.004);
	a->SetTickLength(0.025);
	a->SetLabelSize(0.06);
	a->SetLabelFont(42);
	a->SetTitleFont(42);
	a->CenterTitle();
}

pJetShape::~pJetShape(){
//	delete c;
	delete tx;
	delete leg;
	delete tl;
	delete axis_x;
	delete axis_y;
	//	delete [] st;
	//	delete [] ratio;
}
