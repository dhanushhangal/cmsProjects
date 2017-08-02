
#include "/Users/tabris/cmsProjects/Toolkit/stackHist.h"

#include "hallie_input.h"
bool donorm=0;
void subplot( stackHist* st, int ii, int nf);
TLatex tl = TLatex();
TString centLabel[] = {"Cent:0-10%", "Cent:10-30%", "Cent:30-50%", "Cent:50-100%"};
TString jetPtLabel[] = {"120<jetPt<150", "150<jetPt<200", "jetPt>200"};
TString legtxt[] = {"0.7<p_{T}^{assoc.}<1 GeV",
	"1<p_{T}^{assoc.}<2 GeV",
	"2<p_{T}^{assoc.}<3 GeV",
	"3<p_{T}^{assoc.}<4 GeV",
	"4<p_{T}^{assoc.}<8 GeV",
	"8<p_{T}^{assoc.}<12 GeV",
	"12<p_{T}^{assoc.}<16 GeV",
	"16<p_{T}^{assoc.}<20 GeV",
	"20<p_{T}^{assoc.}<999GeV"};

void plotShapes(){

	getInput_Hallie();
	TFile* f[3];
	f[0] = TFile::Open("jetShapeSignal_jtPt120_150.root");
	f[1] = TFile::Open("jetShapeSignal_jtPt150_200.root");
//	f[2] = TFile::Open("jetShapeSignal_jtPtAbove200.root");
	f[2] = TFile::Open("jetShapeSignal_alljtPt.root");
	//f[2] = TFile::Open("jetShapeSignal_jtPt200_999_meptrig60.root");
	stackHist *st[4][3];
	TH1D* jetShape[4][9][3];
	TH1D* jetShape_tot[4][3];

	for(int i=0; i<3;++i){
		for( int jcent=0; jcent<4;jcent++){
			st[jcent][i] = new stackHist(Form("st%d_%d",jcent, i));
			jetShape_tot[jcent][i]=(TH1D*)f[i]->Get(Form("jetShape_total_%d",jcent));
			jetShape_tot[jcent][i]->SetName(Form("jetShape_total_%d_%d",jcent,i));
			for(int jtrkpt=0;jtrkpt<9; jtrkpt++){
				jetShape[jcent][jtrkpt][i]=(TH1D*)f[i]->Get(Form("jetShape_%d_%d",jcent,jtrkpt+1));
				jetShape[jcent][jtrkpt][i]->SetName(Form("jetShape_%d_%d_%d",jcent,jtrkpt+1, i));
				if(donorm) jetShape[jcent][jtrkpt][i]->Scale(1.0/jetShape_tot[jcent][i]->Integral("width"));
				jetShape[jcent][jtrkpt][i]->GetYaxis()->SetTitleSize(1);
				st[jcent][i]->addHist((TH1*) jetShape[jcent][jtrkpt][i]);
			}
		}
	}

	TCanvas* c_tot = new TCanvas("c_tot","",2400,1600);
	c_tot->Divide(5,4,0,0);
	tl.SetTextSize(0.07);

	for(int jc=2; jc<6;++jc) {
		int ii = 5-jc;
		c_tot->cd(jc);
		subplot(st[ii][0], ii, 0);
		tl.DrawLatexNDC(0.65,0.8, centLabel[ii]);
	}
	for(int jc=7; jc<11;++jc) {
		int ii = 10-jc;
		c_tot->cd(jc);
		subplot(st[ii][1], ii, 1);
	}
	for(int jc=12; jc<16;++jc) {
		int ii = 15-jc;
		c_tot->cd(jc);
		subplot(st[ii][2], ii, 2);
	}

	//draw the ratio
	Color_t color[3];
	color[0]=kRed+1;
	color[1]=kAzure-3;
	color[2]=kGreen+3;
	TLegend *ll2 = new TLegend(0.1,0.2,0.7,0.8);
	TString legJetPt[3] = {"120<jetPt<150", "150<jetPt<200", "all jtpt"/*"jetPt>200"*/};
	TLine* l1 = new TLine(0, 1, 1.2,1);
	l1->SetLineStyle(2);
	l1->SetLineColor(kBlack);
	l1->SetLineWidth(4);
	if(donorm){
		for(int i=0;i<4;++i) jetShape_hallie[i]->Scale(1.0/jetShape_hallie[i]->Integral("width"));
	}
	for(int jc=17; jc<21;++jc) {
		c_tot->cd(jc);
		int ii = 20-jc;
		gPad->SetBottomMargin(0.2);
		for(int i=0; i<3;++i){
			if( ii == 0)ll2->AddEntry(jetShape_tot[0][i], legJetPt[i]);
			if(donorm) jetShape_tot[ii][i]->Scale(1.0/jetShape_tot[ii][i]->Integral("width"));
			jetShape_tot[ii][i]->SetAxisRange(0, 0.9,"x");
			jetShape_tot[ii][i]->Divide(jetShape_hallie[ii]);
			jetShape_tot[ii][i]->SetAxisRange(0.5, 2, "Y");
			jetShape_tot[ii][i]->SetStats(0);
			jetShape_tot[ii][i]->SetMarkerSize(1.5);
			jetShape_tot[ii][i]->SetMarkerStyle(20);
			jetShape_tot[ii][i]->SetMarkerColor(color[i]);
			jetShape_tot[ii][i]->SetLineColorAlpha(color[i],0.7);
			jetShape_tot[ii][i]->SetLineWidth(5);
			jetShape_tot[ii][i]->GetXaxis()->SetTitleSize(0.09);
			jetShape_tot[ii][i]->GetXaxis()->SetLabelSize(0.07);
			jetShape_tot[ii][i]->GetXaxis()->SetTitle("#Delta r");
			jetShape_tot[ii][i]->GetXaxis()->CenterTitle();
			jetShape_tot[ii][i]->GetYaxis()->SetLabelSize(0);
			jetShape_tot[ii][i]->Draw("same");
			l1->Draw("same");
		}
	}

	//draw legend
	c_tot->cd(11);
	TLegend * ll = st[1][0]->makeLegend(legtxt, 0.1,1, 1,0);
	ll->Draw();
	c_tot->cd(16);
	ll2->SetLineColorAlpha(kWhite, 0);
	ll2->SetFillColorAlpha(kWhite, 0);
	ll2->Draw();

	//add fake axis
	TF1 *f3;
	if(donorm) f3=new TF1("f3","log10(x)",0.001,50);
	else f3=new TF1("f3","log10(x)",0.5,5000);
	TGaxis* ax = new TGaxis(1,0,1,1,"f3",505,"G");
	ax->SetLabelSize(0.09);
	ax->SetTitleSize(0.1);
	ax->SetTitleOffset(1.3);
	ax->CenterTitle();
	ax->SetTitle("#rho(#Delta r)");
	c_tot->cd(1);
	ax->Draw();
	c_tot->cd(6);
	ax->Draw();
	c_tot->cd(11);
	ax->Draw();
	c_tot->cd(16);
	TGaxis* ax2 = new TGaxis(1,0.2,1,1,0.5, 2);
	ax2->SetLabelSize(0.09);
	ax2->SetTitleSize(0.1);
	ax2->SetTitleOffset(1.3);
	ax2->CenterTitle();
	ax2->SetTitle("jetPt/QM2017");
	ax2->Draw();
if(donorm) c_tot->SaveAs("pt_jetShapesForDifferentJetPt_normalized.png");
else c_tot->SaveAs("pt_jetShapesForDifferentJetPt_perJet.png");
}

void subplot( stackHist* st, int ii, int nf ){
	gPad->SetLogy();
	st->setRange(0,0.8,"x");
	if(donorm)st->setRange(0.001,50,"y");
	else st->setRange(0.5,5000,"y");
	st->setLabelSize(0.,"y");
	st->setLabelSize(0.);
	st->drawStack();
	if(ii==3) tl.DrawLatexNDC( 0.2, 0.8,jetPtLabel[nf]);
}
