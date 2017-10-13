
#include "../../lib/xTree.h"

class trackHist : public std::map<string, TH1F*>{
	public:
		TH1F* ptRes, *dzRes, *dxyRes, *dz, *dxy, *nhit;
	public:
		trackHist(TString name){
			ptRes = new TH1F(name+"ptRes", "", 100, 0, 0.07);
			ptRes->GetXaxis()->SetTitle("PtError/Pt");
			dzRes = new TH1F(name+"dzRes", "", 100, -5, 5);
			dzRes->GetXaxis()->SetTitle("Dz/DzError");
			dxyRes = new TH1F(name+"dxyRes", "", 100, -5, 5);
			dxyRes->GetXaxis()->SetTitle("Dxy/DxyError");
			dz = new TH1F(name+"dz",  "", 100, -0.1, 0.1);
			dz->GetXaxis()->SetTitle("Dz");
			dxy= new TH1F(name+"dxy", "", 100, -0.1, 0.1);
			dxy->GetXaxis()->SetTitle("Dxy");
			nhit= new TH1F(name+"nhit", "", 100, 0, 100);
			nhit->GetXaxis()->SetTitle("nhit");
			this->insert(std::pair<string, TH1F*> ("ptRes", ptRes));
			this->insert(std::pair<string, TH1F*> ("dzRes", dzRes));
			this->insert(std::pair<string, TH1F*> ("dxyRes", dxyRes));
			this->insert(std::pair<string, TH1F*> ("dz", dz));
			this->insert(std::pair<string, TH1F*> ("dxy",dxy));
			this->insert(std::pair<string, TH1F*> ("nhit",nhit));
		};
		void fill(xTree &, int);
};

void trackHist::fill(xTree &t, int j){
	ptRes ->Fill(t.trkPtError->at(j)/t.trkPt->at(j));
	dzRes ->Fill(t.trkDz->at(j)/t.trkDzError->at(j));
	dxyRes->Fill(t.trkDxy->at(j)/t.trkDxyError->at(j));
	dz->Fill(t.trkDz->at(j));
	dxy->Fill(t.trkDxy->at(j));
	nhit->Fill(t.trkNHit->at(j));
}

bool highPurityCut(xTree &t, int j){
//	if(t.trkNHit->at(j)<13) return false;
	if(fabs( t.trkPtError->at(j)/t.trkPt->at(j))>0.05) return false;
	//if(fabs( t.trkDz->at(j)/t.trkDzError->at(j))>3) return false;
	//if(fabs( t.trkDxy->at(j)/t.trkDxyError->at(j))>3) return false;
	if((t.trkChi2->at(j)/t.trkNdof->at(j)/t.trkNlayer->at(j))>0.15) return false;
	return true;
}

void highPurity_check(){
	TFile *f = TFile::Open("../../dataSet/MC_cymbal_tune_1.root");
	auto t = new xTree("mixing_tree",f, 1, 0); 
	trackHist raw("raw");
	trackHist highPurity("highPurity");
	trackHist cutsOn("cuts");

	Long64_t nev = t->tree->GetEntriesFast();
	for(Long64_t jev = 0; jev<nev; ++jev){
		t->GetEntry(jev);
		if(jev %1000==0) cout<<"processing "<<jev<<".."<<endl;
		for(int j=0; j<t->trkPt->size(); ++j){
			raw.fill(*t, j);	
			if(t->highPurity->at(j)==1) highPurity.fill(*t, j);
			if(highPurityCut(*t, j)) cutsOn.fill(*t,j);
		}
	}
	TCanvas* c = new TCanvas("c", "", 1600, 1200);
	c->Divide(3,2);
	c->cd(1);
	highPurity["ptRes"]->SetLineColor(kRed);
	cutsOn    ["ptRes"]->SetLineColor(kGreen+2);
	raw       ["ptRes"]->Draw();
	highPurity["ptRes"]->Draw("same");
	cutsOn    ["ptRes"]->Draw("same");
	c->cd(2);
	highPurity["dzRes"]->SetLineColor(kRed);
	cutsOn    ["dzRes"]->SetLineColor(kGreen+2);
	raw       ["dzRes"]->Draw();
	highPurity["dzRes"]->Draw("same");
	cutsOn    ["dzRes"]->Draw("same");
	c->cd(3);
	highPurity["dxyRes"]->SetLineColor(kRed);
	cutsOn    ["dxyRes"]->SetLineColor(kGreen+2);
	raw       ["dxyRes"]->Draw();
	highPurity["dxyRes"]->Draw("same");
	cutsOn    ["dxyRes"]->Draw("same");
	c->cd(4);
	highPurity["dz"]->SetLineColor(kRed);
	cutsOn    ["dz"]->SetLineColor(kGreen+2);
	raw       ["dz"]->Draw();
	highPurity["dz"]->Draw("same");
	cutsOn    ["dz"]->Draw("same");
	c->cd(5);
	highPurity["dxy"]->SetLineColor(kRed);
	cutsOn    ["dxy"]->SetLineColor(kGreen+2);
	raw       ["dxy"]->Draw();
	highPurity["dxy"]->Draw("same");
	cutsOn    ["dxy"]->Draw("same");
	c->cd(6);
	highPurity["nhit"]->SetLineColor(kRed);
	cutsOn    ["nhit"]->SetLineColor(kGreen+2);
	raw       ["nhit"]->Draw();
	highPurity["nhit"]->Draw("same");
	cutsOn    ["nhit"]->Draw("same");
}

