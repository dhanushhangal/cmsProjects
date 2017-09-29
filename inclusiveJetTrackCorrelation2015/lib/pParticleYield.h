
#include "stackHist.h"

class pParticleYield: public TCanvas{
	public :
		pParticleYield(TString name, TString title="", float w=2600, float h=1200): TCanvas(name,title, w, h){
			pname = name;
			c=(TPad*)gPad;
			c->SetMargin(0.2, 0.05, 0.25, 0.2);
			c->Divide(5,2,0,0);
			gStyle->SetOptStat(0);
			for(int i=0; i<5; ++i) st[i]=new stackHist(Form("st_%d",i));
			for(int i=0; i<4; ++i) diff[i]=new stackHist(Form("diff_%d",i));
			tl= new TLine();
			tl->SetLineStyle(2);
			tx= new TLatex();
			c_diff= new TCanvas("c_diff", "", 1600, 2500);
		}
		void addHist(TH1* h, int jcent);
		void initHist();
		void addDiff(TH1** h, int num, int jcent);
		void getDiff(); 
		void draw();
		void addLegend(TString *);
		void drawLabels();

	public : 
		TPad * c;
		TString pname;
		stackHist *st[5];
		stackHist *diff[4];
		TLine * tl;
		TLatex * tx;
		TLegend* leg;
		TString* legTxt;
		TGaxis* axis_x;
		TGaxis *axis_y;
		TCanvas* c_diff;
		float xmin=0, xmax=0.99;
		float y1min=-4, y1max=40, y2min=-1.9, y2max=1.9;	
};

void pParticleYield::addHist(TH1* h, int jcent){
	st[jcent]->addHist((TH1*) h);	
}

void pParticleYield::addDiff(TH1** h, int num, int jcent){
	diff[jcent]->addDiff((TH1**) h, num);	
}

void pParticleYield::getDiff(){
	int nn=int(st[4]->hist_trunk.size());
	TH1D** htmp;
	htmp = new TH1D*[nn];
	c_diff->Divide(4,nn, 0, 0);
	for(int jcent=0;jcent<4; ++jcent){
		for(int n=0; n<nn;++n){
			htmp[n]= (TH1D*)st[jcent]->hist_trunk.at(n)->Clone();
			htmp[n]->Add(st[4]->hist_trunk.at(n),-1);
			c_diff->cd((n+1)*4-jcent);
			htmp[n]->SetMarkerStyle(20);
			htmp[n]->SetMarkerSize(0.8);
			htmp[n]->SetAxisRange(-3,3,"y");
			htmp[n]->Draw();
		}
		diff[jcent]->addDiff((TH1**) htmp, nn);	
	//	addDiff((TH1**) htmp, nn, jcent);
	}
	c_diff->SaveAs(pname+"_Difference.png");
	delete [] htmp;
}

void pParticleYield::initHist(){
	for(int i=0;i<5;++i){
		st[i]->setRange(xmin,xmax,"x");
		st[i]->setRange(y1min,y1max,"y");
	}
	for(int i=0;i<4;++i){
		diff[i]->setRange(xmin,xmax,"xd");
		diff[i]->setRange(y2min,y2max,"yd");
		//diff[i]->setLabelSize(0);
		diff[i]->setLabelSize(0, "y");
		//diff[i]->hst_up->GetXaxis()->SetTitle("#Delta r");
	}
	/*
	*/
}

void pParticleYield::draw(){
	/*
	 */
	initHist();
	TH1* hf;
	 
	for(int jcent=0; jcent<4; ++jcent){
		//cout<<jcent<<endl;
		c->cd(5-jcent);
		st[jcent]->setLabelSize(0);
		st[jcent]->setLabelSize(0, "y");
		//gPad->SetLogy();
		st[jcent]->drawStack("r");
		//st[jcent]->drawStack();
	}
	c->cd(1);
	hf=st[4]->drawStack("r");
	hf->GetYaxis()->SetTitleSize(0.08);
	hf->GetYaxis()->SetTitleOffset(1.15);
	hf->GetYaxis()->SetTitle("Y=#frac{1}{N_{jet}} #frac{dN}{d#Delta r}");
	hf->GetXaxis()->CenterTitle();
	for(int jc=0; jc<4; ++jc){
		c->cd(10-jc);
	//	gPad->SetBottomMargin(0.2);
		hf=diff[jc]->drawDiff("r");
		hf->GetXaxis()->SetTitle("#Delta r");
		hf->GetXaxis()->SetTitleSize(0.08);
		hf->GetXaxis()->SetTitleOffset(0.8);
		hf->GetXaxis()->CenterTitle();
	}
	c->cd(6);
	leg = st[1]->makeLegend(legTxt, 0.05, 0.05,0.9, 0.85);
	leg->Draw();
	axis_y= new TGaxis(1    ,0.265, 1, 1, y2min, y2max,510, "");
	axis_x= new TGaxis(0.235, 1 , 1, 1, xmin, xmax  ,505, "");
	axis_y->SetTitle("Y_{PbPb}-Y_{pp}");
	axis_y->SetTitleSize(0.08);
	axis_y->SetTitleOffset(1.15);
	axis_y->SetLabelSize(0.06);
	axis_y->CenterTitle();
	axis_y->Draw();
//	axis_x->SetTitle("#Delta r");
//	axis_x->CenterTitle();
//	axis_x->SetTitleSize(0.08);
//	axis_x->SetTitleOffset(1.);
	axis_x->SetLabelSize(0.05);
	axis_x->Draw("same");
	drawLabels();
	c->Update();

}

void pParticleYield::addLegend(TString *ss){
	legTxt=ss;
}

void pParticleYield::drawLabels(){
	c->cd(1);
	tx->SetTextFont(63);
	tx->SetTextSizePixels(20);
	tx->SetLineColor(kWhite);
//	tx->DrawLatexNDC(0.3, 0.8, "CMS");
	tx->SetTextFont(53);
//	tx->DrawLatexNDC(0.45, 0.8, "Preliminary");

	tx->SetLineColor(kBlack);
	tx->SetTextFont(63);
	tx->SetTextSize(30);
	tx->DrawLatexNDC(0.28, 0.9, "pp reference");
	c->cd(2);
	tx->DrawLatexNDC(0.08, 0.9, "PbPb Cent.50-100%");
	c->cd(3);
	tx->DrawLatexNDC(0.08, 0.9, "PbPb Cent.30-50%");
	c->cd(4);
	tx->DrawLatexNDC(0.08, 0.9, "PbPb Cent.10-30%");
	c->cd(5);
	tx->DrawLatexNDC(0.08, 0.9, "PbPb Cent.0-10%");
}
