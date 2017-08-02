
#include "/Users/tabris/Research/HIN_5.02TeV/analyzer/anPlot/stylePackage.h"
#define nhibin 4
#define nptbin 10

TH1D* jetShape[nhibin][3];
TH1D* jetShape_tot[nhibin][3];
TH1D* jetShape_eta[nhibin][3];
TH1D* PY_eta  [nhibin][3];
TH1D* PY_eta_total[nhibin][3];
TH1D* PY_phi  [nhibin][3];
TFile * f[3];

TString ptbin[] = {"0.5<Pt<0.7", "0.7<Pt<1", "1<Pt<2", "2<Pt<3", "3<Pt<4", "4<Pt8","8<Pt12", " 12<Pt16", "16<Pt<20", "20<Pt<300"};
void plotTogether(){

	f[0] = TFile::Open("jetShapeSignal_jtPt120_150.root");
	f[1] = TFile::Open("jetShapeSignal_jtPt150_200.root");
	f[2] = TFile::Open("jetShapeSignal_jtPtAbove200.root");
	//f[2] = TFile::Open("jetShapeSignal_jtPt200_999_meptrig60.root");

	Color_t color[3];
	float colorAlpha[3];
	color[0]=kRed+1;
	color[1]=kAzure-3;
	color[2]=kGreen+3;
	colorAlpha[0]=0.6;
	colorAlpha[1]=0.6;
	colorAlpha[2]=0.6;
	TCanvas* c[4][10];
	TString label[3] = {"120<jtpt<150", "150<jtpt<200", "200<jtpt<999"};
	TString centLab[4] = {"cent:0-10%", "cent:10-30%", "cent:30-50%", "cent:50-100%"};
	TLegend* tl = new TLegend(0.6,0.6,0.95,0.8);
	TLatex* tlx = new TLatex();
	TLine *basel = new TLine();
	stylePackage* sp = new stylePackage();
	gStyle->SetPadBottomMargin(0.12);
	gStyle->SetPadRightMargin(0.04);
	gStyle->SetPadLeftMargin(0.14);
	basel->SetLineStyle(3);
	for(int jtrkpt = 1; jtrkpt<10; ++jtrkpt){
		for(int i=0;i<3; i++){
			for( int jcent=0; jcent<nhibin;jcent++){
				jetShape_tot[jcent][i]=(TH1D*)f[i]->Get(Form("jetShape_total_%d",jcent));
				PY_eta_total[jcent][i]=(TH1D*)f[i]->Get(Form("PY_eta_total_%d",jcent));
				PY_eta[jcent][i]      =(TH1D*)f[i]->Get(Form("PY_eta_%d_%d",jcent, jtrkpt));
				PY_eta[jcent][i]      =(TH1D*)f[i]->Get(Form("PY_eta_%d_%d",jcent, jtrkpt));
				PY_phi[jcent][i]      =(TH1D*)f[i]->Get(Form("PY_phi_%d_%d",jcent, jtrkpt));
				jetShape[jcent][i]    =(TH1D*)f[i]->Get(Form("jetShape_%d_%d",jcent,jtrkpt));
				jetShape_eta[jcent][i]=(TH1D*)f[i]->Get(Form("jetShape_eta_%d_%d",jcent, jtrkpt));

				PY_eta[jcent][i]->Scale(1.0/5);
				PY_eta_total[jcent][i]->Scale(1.0/5);
				PY_phi[jcent][i]->Scale(1.0/5);
				jetShape_eta[jcent][i]->Scale(1.0/5);

				PY_eta[jcent][i]->Rebin(5);
				PY_phi[jcent][i]->Rebin(5);
				jetShape_eta[jcent][i]->Rebin(5);
				PY_eta_total[jcent][i]->Rebin(5);
			}
		}
		/*
		   c[0][jtrkpt] = new TCanvas("c1", "", 1500,400);
		   c[0][jtrkpt]->Divide(4,1,0,0);
		   for( int j=3;j>-1;j--){
		   tl->SetLineColor(kWhite);
		   for( int i=0;i<3;i++){
		   c[0]->cd(4-j);
		   sp->histConfig_classic(PY_eta[j][i],color[i], colorAlpha[i]);
		   PY_eta[j][i]->SetAxisRange(-2,2, "X");
		   PY_eta[j][i]->SetAxisRange(-3,15, "Y");
		   PY_eta[j][i]->GetYaxis()->SetTitle("Y = #frac{1}{N_{jet}}#frac{dN}{d#Delta#eta}");
		   PY_eta[j][i]->GetXaxis()->SetTitle("#Delta#eta");
		   PY_eta[j][i]->Draw("same");
//sp->histConfig_classic(PY_eta_total[j][i],color[i]);
//PY_eta_total[j][i]->SetAxisRange(-2,2, "X");
//PY_eta_total[j][i]->SetAxisRange(-3,60, "Y");
//PY_eta_total[j][i]->GetYaxis()->SetTitle("Y = #frac{1}{N_{jet}}#frac{dN}{d#Delta#eta}");
//PY_eta_total[j][i]->GetXaxis()->SetTitle("#Delta#eta");
//PY_eta_total[j][i]->Draw("same");
basel->DrawLine(-2,0, 2,0);
if( j==3){
tl->AddEntry(PY_eta[j][i], label[i]);
}
tlx ->DrawLatexNDC(0.7,0.9,centLab[j]);	
}
if( j==3){
tl->Draw();
tlx->DrawLatexNDC(0.23,0.9, ptbin[jtrkpt]);
	//tlx->DrawLatexNDC(0.23,0.9, "total");
	}
	}
	*/

c[1][jtrkpt] = new TCanvas(Form("c2_%d",jtrkpt),"",1500,400);
c[1][jtrkpt]->Divide(4,1,0,0);
for( int j=3;j>-1;j--){
	tl->SetLineColor(kWhite);
	for( int i=0;i<3;i++){
		c[1][jtrkpt]->cd(4-j);
		sp->histConfig_classic(jetShape_eta[j][i],color[i], colorAlpha[i]);
		jetShape_eta[j][i]->SetAxisRange(-2,2, "X");
		jetShape_eta[j][i]->SetAxisRange(-3,10, "Y");
		jetShape_eta[j][i]->Draw("same");
		basel->DrawLine(-2,0, 2,0);
		jetShape_eta[j][i]->GetYaxis()->SetTitle("#rho(#Delta#eta)");
		jetShape_eta[j][i]->GetXaxis()->SetTitle("#Delta#eta");
		//	if( j==3){
		//		tl->AddEntry(PY_eta[j][i], label[i]);
		//	}
		tlx ->DrawLatexNDC(0.7,0.9,centLab[j]);	
	}
	if( j==3){
		tl->Draw();
		tlx->DrawLatexNDC(0.23,0.9, ptbin[jtrkpt]);
	}
}
c[1][jtrkpt]->SaveAs(Form("jetShape_eta_%d.png", jtrkpt));
}
/*
   c[2] = new TCanvas("c3","",1500,400);
   c[2]->Divide(4,1,0,0);
   for( int j=3;j>-1;j--){
   tl->SetLineColor(kWhite);
   for( int i=0;i<3;i++){
   c[2]->cd(4-j);
   sp->histConfig_classic(jetShape[j][i],color[i], colorAlpha[i]);
   jetShape[j][i]->SetAxisRange(0,0.9, "X");
   jetShape[j][i]->SetAxisRange(0.01, 3000, "Y");
   gPad->SetLogy();
   jetShape[j][i]->GetYaxis()->SetTitle("#rho(#Deltar)");
   jetShape[j][i]->GetXaxis()->SetTitle("#Deltar");
   jetShape[j][i]->Draw("same");
//	if( j==3){
//		tl->AddEntry(PY_eta[j][i], label[i]);
//	}
tlx ->DrawLatexNDC(0.7,0.9,centLab[j]);	
}
if( j==3){
tl->Draw();
tlx->DrawLatexNDC(0.23,0.9, ptbin[jtrkpt]);
}
}
c[3] = new TCanvas("c4","",1500,400);
c[3]->Divide(4,1,0,0);
for( int j=3;j>-1;j--){
tl->SetLineColor(kWhite);
for( int i=0;i<3;i++){
c[3]->cd(4-j);
sp->histConfig_classic(jetShape_tot[j][i],color[i], colorAlpha[i]);
jetShape_tot[j][i]->SetAxisRange(0,0.9, "X");
jetShape_tot[j][i]->SetAxisRange(1, 4000, "Y");
jetShape_tot[j][i]->GetYaxis()->SetTitle("#rho(#Deltar)");
jetShape_tot[j][i]->GetXaxis()->SetTitle("#Deltar");
gPad->SetLogy();
jetShape_tot[j][i]->Draw("same");
tlx ->DrawLatexNDC(0.7,0.9,centLab[j]);	
}
if( j==3){
tl->Draw();
tlx->DrawLatexNDC(0.23,0.9, "total");
}
}
*/
}
