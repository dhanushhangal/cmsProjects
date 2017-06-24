

#ifndef PLOTSCH_H
#define PLOTSCH_H
#ifndef TH1_H
#define TH1_H
#include <TH1.h>
#include <TString.h>
#endif 

class plotSch{

	public: 
		float ymin;
		float ymax;
		float xmin;
		float xmax;
		int isCustomized;

		plotSch();
		TPad* EfficiencyPlot(std::string labelden, TH1* hden, std::string labelnum1, TH1* hnum1, std::string labelnum2 = "", TH1* hnum2 = NULL, int opt =0, TString errorOpt = "");
		void EfficiencyTXT(TPad* pad, TString label, float x, float y, int n =0);
		void Efficiency95Area(TPad* pad, float xmin, float xmax);
		void SetRatioRange(float min, float max);
		void SetPlotRange(float min, float max);
		void addPlot(TPad* pad, TF1* f, int n);

		void legendConfig();
};


plotSch::plotSch(){
	isCustomized=0;
	ymin=0;
	ymax=1;
}

TPad* plotSch::EfficiencyPlot(std::string labelden, TH1* hden, std::string labelnum1, TH1* hnum1, std::string labelnum2, TH1* hnum2, int opt, TString errorOpt ){
	TH1* hratio1;
	TH1* hratio2;
	hden->SetAxisRange(0, 1.8*hden->GetMaximum(), "y");
	hden->SetMarkerSize(1.4);
	hden->SetMarkerStyle(21);
	hden->SetMarkerColor(kBlue-5);
	hden->SetLabelSize(0.05, "Y");
	hden->SetLabelSize(0.05, "X");
	hnum1->SetMarkerSize(0.9);
	hnum1->SetMarkerStyle(20);
	hnum1->SetMarkerColor(kRed+1);
	hnum1->SetTitle("");
	hnum1->SetLabelSize(0.05, "Y");
	hnum1->SetLabelSize(0.05, "X");
	hnum1->GetXaxis()->SetTitleOffset(0.8);
	hnum1->GetXaxis()->CenterTitle();
	hnum1->GetXaxis()->SetTitleSize(0.06);
	hratio1=(TH1*)hnum1->Clone();
	if(isCustomized) {
		hden->SetAxisRange(xmin, xmax, "X");
		hratio1->SetAxisRange(xmin, xmax, "X");
	}
	hratio1->Divide(hnum1, hden, 1, 1, errorOpt);
	if( hnum2 !=NULL){
		hnum2->SetMarkerSize(0.9);
		hnum2->SetMarkerStyle(20);
		hnum2->SetTitle("");
		hnum2->SetMarkerColor(kBlue-3);
		hratio2=(TH1*)hnum2->Clone();
		hratio2->Divide(hnum2, hden, 1 ,1, errorOpt);
	}

	TPad* pad = (TPad*) gPad;
	pad->Divide(1, 2, 0,0);
	pad->cd(1);
	hden->SetStats(0);
	hden->Draw();
	hnum1->Draw("same");
	if( hnum2 !=NULL) hnum2->Draw("same");
	TLegend *ltemp = new TLegend(0.15,0.67,0.6,0.97);
//	ltemp->SetLineColorAlpha(kWhite,0);
	ltemp->SetLineColor(kWhite);
	ltemp->SetFillColorAlpha(kWhite, 0);
	ltemp->AddEntry(hden, labelden.c_str());
	ltemp->AddEntry(hnum1, labelnum1.c_str());
	if( hnum2 != NULL)ltemp->AddEntry(hnum2, labelnum2.c_str());
	ltemp->Draw();

	pad->cd(2);
	hratio1->SetStats(0);
	if( isCustomized)
		hratio1->SetAxisRange(ymin, ymax, "Y");
	else
		hratio1->SetAxisRange(0, 1.8*hratio1->GetMaximum(), "Y");
	hratio1->Draw();
	if(opt ==1)hratio2->Draw("same");
	TLegend *ltemp2 = new TLegend(0.15,0.71,0.8,0.95);
//	ltemp2->SetLineColorAlpha(kWhite,0);
	ltemp2->SetLineColor(kWhite);
	ltemp2->SetFillColorAlpha(kWhite, 0);
	ltemp2->AddEntry(hratio1, Form("%s/%s",labelnum1.c_str(), labelden.c_str()));
	if( opt ==1)ltemp2->AddEntry(hratio2, Form("%s/%s",labelnum2.c_str(), labelden.c_str()));
	ltemp2->Draw();
	return (TPad*)pad;
}

void plotSch::EfficiencyTXT(TPad* pad, TString label, float x, float y, int n ){
	TPad * ptemp = (TPad*) gPad;
	if ( n>1 || n<0) return;
	pad->cd(1+n);
	TLatex* ltemp = new TLatex(x, y, label);
	ltemp->SetNDC();
	ltemp->Draw();
	if( ptemp !=NULL) ptemp->cd();
}

void plotSch::Efficiency95Area(TPad* pad, float xmin, float xmax){
	TPad * ptemp = (TPad*) gPad;
	pad->cd(2);
	TLine* lcent = new TLine( xmin, 1, xmax, 1);
	TLine* lupper= new TLine( xmin, 1.05, xmax, 1.05);
	TLine* llower= new TLine( xmin, .95, xmax, .95);
	lcent->SetLineStyle(8);
	lupper->SetLineStyle(8);
	llower->SetLineStyle(8);
	lcent->Draw("same");
	lupper->Draw("same");
	llower->Draw("same");
}
void plotSch::addPlot(TPad* pad, TF1* f, int n){
	TPad * ptemp = (TPad*) gPad;
	if ( n>1 || n<0) return;
	pad->cd(1+n);
	f->Draw("same");
	ptemp->cd();
}

void plotSch::SetRatioRange(float min, float max){
	isCustomized=1;
	ymin = min;
	ymax = max;
	return; 
}
void plotSch::SetPlotRange(float min, float max){
	isCustomized=1;
	xmin = min;
	xmax = max;
	return; 
}
#endif
