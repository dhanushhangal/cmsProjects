
#ifndef CANVASX
#define CANVASX
//in this header file, I just collect all the plot scheme I used to avoid the repeat working.
//
//========================================================================
//base class for all plots.===============================================
//========================================================================
class canvasX {
	protected:

		TCanvas* xc;
	public: 
		canvasX(TString name, TString title, double width, double height)
		{
			xc=new TCanvas(name, title, width, height);
		}
		//virtual TPad* getPad()=0;
		virtual TCanvas* getCanvas();
		virtual void Draw(TString opt)=0;
};
TCanvas *canvasX::getCanvas(){
	return (TCanvas*) xc;
}
//end of definition of base class ========================================
#endif

#ifndef XSTANRDARD
#define XSTANRDARD
class xStandard : public canvasX{
	public:
		std::vector<TH1*> * hist_vec;
		TLegend* lg;
	public:
		xStandard(TString name, TString title= "", double width = 600,double height=500);
		void addEntry(TH1* hnum, int marker, Color_t color, TString leg=""); 
		void histConfig(TH1* h); 
		void padConfig(TPad* p); 
		void Draw(TString opt=""); 
};

xStandard::xStandard(TString name, TString title = "", double width = 600,double height=500): 
	canvasX(name, title, width, height)
{
	hist_vec = new std::vector<TH1*>();	
	lg = new TLegend(0.65, 0.75, 0.85,0.85);
	lg->SetFillColor(0); 
	lg->SetBorderSize(0);
}

void xStandard::addEntry(TH1* h, int marker, Color_t color, TString leg=""){
	TH1* htmp = (TH1*) h->Clone();
	htmp->SetMarkerStyle(marker);
	htmp->SetMarkerColor(color);
	htmp->SetLineColor(color);
	hist_vec->push_back(htmp);
	lg->AddEntry(htmp, leg,"lp");
}

void xStandard::histConfig(TH1* h ){
	h->SetStats(0);
	h->GetXaxis()->SetTickLength(0.04);
	h->GetYaxis()->SetTickLength(0.04);
	h->GetXaxis()->CenterTitle(true);
	h->GetXaxis()->SetLabelSize(0.05);
	h->GetYaxis()->SetLabelSize(0.05);
	h->GetXaxis()->SetTitleSize(0.05);
	h->GetYaxis()->SetTitleSize(0.05);
}

void xStandard::padConfig(TPad*p ){
}

void xStandard::Draw(TString opt){
	xc->cd();
	opt = opt+"same";
	padConfig((TPad*) gPad);
	for(auto &it : *hist_vec){
		histConfig(it);
		it->Draw(opt);
	}	
	lg->Draw();
}

#endif
//--------------------------------
// the plot for efficiency -------
// The class to plot the efficiency for trakcing or other stuff. 
// The TGraphAsymmErrors has been used for getting the efficiency (ratio).
// The Poisson Divide has been set for default.
//--------------------------------
class xEfficiency: public canvasX{
	public:
		TString divideOpt="";
	//	std::vector <TH1*>* hist_vec;
		TH1* hframe;
		std::vector <TGraphAsymmErrors*>* eff_vec;
		double xMax=0, xMin=0, yMax= 1, yMin=0;
	public:
		xEfficiency(TString name, TString title="", double width=600, double height=500);
		void getEff(TH1* hnum, TH1* hden, TString opt="pois",int marker=24, Color_t color=4);
//		TAxis* getAxis(TString opt);
		void Draw(TString opt ="apc");
};
xEfficiency::xEfficiency(TString name, TString title, double width, double height):
       	canvasX(name, title,  width, height)
{
	eff_vec = new vector<TGraphAsymmErrors*> ();
	xc->SetLeftMargin(0.14);
	xc->SetRightMargin(0.085);
	xc->SetTopMargin(0.1);
	xc->SetBottomMargin(0.13);
}
void xEfficiency::getEff(TH1* hnum, TH1* hden, TString opt, int marker, Color_t color){
	TGraphAsymmErrors* x_eff = new TGraphAsymmErrors();
	if( opt == "bayes") x_eff ->BayesDivide(hnum, hden);
	else x_eff->Divide(hnum, hden, opt);
	x_eff->SetMarkerStyle(marker);
	x_eff->SetMarkerColor(color);
	x_eff->SetLineColor(color);
	eff_vec->push_back(x_eff);
}
//TAxis* xEfficiency::getAxis(TString ax){
//	if ( ax == "x") return xc->GetXaxis();
//	else if(ax == "y") return xc->GetYaxis();
//	else return NULL;
//}
void xEfficiency::Draw(TString opt){
	xc->SetGrid();
	for(auto &it : *eff_vec ){
		it->SetMaximum(yMax);
		it->SetMinimum(yMin);
		it->Draw(opt);
	}
}
// end of definition for xEfficiency -------------------------------
//------------------------------------------------------------------
//-----------------       xRatioPlot      -------------------------- 
//the canvas has been divide into two un-equal size sub pads, upper and lower one.
//Upper one for showing one or two pairs of histograms together and the
//lower one for showing the ratio for each pairs. 
//(The lower pad will be a little bit small in height comparing to the upper one). 
//------------------------------------------------------------------
class xRatioPlot : public canvasX{
	public:
		TPad* p_upper;
		TPad* p_lower;
		std::vector<TH1*>* hnum_vec;
		std::vector<TH1*>* hden_vec;
		std::vector<TH1*>* ratio_vec;
		TLegend * lg;
	public :
		xRatioPlot(TString name, TString title="", double cwidth=600, double cheight=650);
		void addHistoPair(TString name, TH1* hnum, TH1* hden, TString opt = "");
		void Draw(TString opt ="");
		void histConfig(TH1* hist);
		void ratioConfig(TH1* hist);

};

xRatioPlot::xRatioPlot(TString name, TString title, double cwidth, double cheight):
       canvasX (name, title, cwidth, cheight)	
{
	hnum_vec = new std::vector<TH1*>();
	hden_vec = new std::vector<TH1*>();
	ratio_vec = new std::vector<TH1*>();
	p_upper = new TPad("p_upper","",0.02, 0.45, 1,1);
	p_lower = new TPad("p_lower","",0.02, 0.07,1 ,0.45);
	p_upper->SetRightMargin(0.08);
	p_lower->SetRightMargin(0.08);
	p_lower->SetTopMargin(0);
	p_upper->SetBottomMargin(0);
	p_lower->SetBottomMargin(0.2);
	p_lower->SetTickx(1);
	p_upper->Draw();
	p_lower->Draw();
	lg = new TLegend(0.25, 0.1, 0.75,0.35);
	lg->SetFillColor(0); 
	lg->SetBorderSize(0);
}

void xRatioPlot::addHistoPair(TString name, TH1* hnum, TH1* hden, TString opt){
	TH1* tmp;
	tmp = (TH1*) hnum->Clone(name+"_num");
	histConfig(tmp);
	hnum_vec->push_back(tmp);
	tmp = (TH1*) hden->Clone(name+"_den");
	histConfig(tmp);
	hden_vec->push_back(tmp);
	tmp = (TH1*) hden->Clone(name+"_ratio");
	tmp->Divide(hnum, hden, 1, 1, opt);
	tmp->SetMarkerColor(kBlack);
	tmp->SetLineColor(kBlack);
	ratioConfig(tmp);
	ratio_vec->push_back(tmp);
}

void xRatioPlot::Draw(TString opt){
	p_upper->cd();
	for(auto & it : *hnum_vec) it->Draw("same");
	for(auto & it : *hden_vec) it->Draw("same");
	lg->Draw();
	p_lower->cd();
	for(auto & it : *ratio_vec) it->Draw("same");
}
void xRatioPlot::histConfig(TH1* hist){
	hist->SetStats(0);
	hist->GetXaxis()->SetTickLength(0.04);
	hist->GetYaxis()->SetTickLength(0.03);
	hist->GetXaxis()->CenterTitle(true);
	hist->GetXaxis()->SetLabelSize(0.05);
	hist->GetYaxis()->SetLabelSize(0.05);
}
void xRatioPlot::ratioConfig(TH1* hist){
	hist->SetStats(0);
	hist->GetXaxis()->SetTickLength(0.06);
	hist->GetYaxis()->SetTickLength(0.03);
	hist->GetXaxis()->CenterTitle(true);
	hist->GetXaxis()->SetLabelSize(0.075);
	hist->GetYaxis()->SetLabelSize(0.075);
	hist->GetXaxis()->SetTitleSize(0.08);
}
//end of xRatioPlot --------------------------------------------------------
/*
*/
void ratioCompare(TH1* hnum, TH1* hden, TString xtitle="", TString ytitle="", TString ctitle="", TPad* x_pad=(TPad*)gPad){

	//set the configuration on the TPad and histograms -----------------
	x_pad->cd();
	TPad* x_upper = new TPad("upper","",0.0, 0.3, 1,1);
	TPad* x_lower = new TPad("lower","",0.0, 0.0,1 ,0.3);
	x_upper->Draw();
	x_lower->Draw();
	TGraphAsymmErrors* ratio = new TGraphAsymmErrors();
	//ratio->BayesDivide(hnum,hden);
	ratio->Divide(hnum,hden,"pois");
	//------------------------------------------------------------------
	hnum->SetMarkerStyle(24);
	hnum->SetMarkerColor(4);
	hnum->SetLineColor(4);
	hden->SetMarkerStyle(25);
	hden->SetMarkerColor(2);
	hden->SetLineColor(2);
	ratio->SetMarkerStyle(25);
	ratio->SetMarkerColor(2);
	ratio->SetLineColor(2);
	//------------------------------------------------------------------
	
	x_upper->cd();
	hnum->Draw("pc"); hden->Draw("pcsame");
	x_upper->Update();

	x_lower->cd();
	ratio->Draw("ALP");
}
