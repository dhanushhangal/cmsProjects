
#include "../../lib/inputTree.h"
namespace {
	template<int N> struct PowN {
		template<typename T> static T op(T t) { return PowN<N/2>::op(t)*PowN<(N+1)/2>::op(t);}
	};
	template<> struct PowN<0> {
		template<typename T>
			static T op(T t) { return T(1);}
	};
	template<>
		struct PowN<1> {
			template<typename T>
				static T op(T t) { return t;}
		};
	template<>
		struct PowN<2> {
			template<typename T>
				static T op(T t) { return t*t;}
		};

	template<typename T>
		T powN(T t, int n) {
			switch(n) {
				case 4: return PowN<4>::op(t); // the only one that matters
				case 3: return PowN<3>::op(t); // and this
				case 8: return PowN<8>::op(t); // used in conversion????
				case 2: return PowN<2>::op(t);
				case 5: return PowN<5>::op(t);
				case 6: return PowN<6>::op(t);
				case 7: return PowN<7>::op(t);
				case 0: return PowN<0>::op(t);
				case 1: return PowN<1>::op(t);
				default : return std::pow(t,T(n)); 
			}
		}
}

void test(){
	using namespace std;
	//cout<<powN(1.5,3)<<endl;
	TChain *t = new TChain("mixing_tree");
	t->Add("/uscms_data/d3/xiaowang/sampleSet/MC_cymbal_tune_1.root");
	t->Add("/uscms_data/d3/xiaowang/sampleSet/MC_cymbal_tune_2.root");
	TH1F* dptRes1 = new TH1F("dptRes1", "", 100, 0, 0.05);
	TH1F* dptRes2 = new TH1F("dptRes2", "", 100, 0, 0.05);
	dptRes2->SetLineColor(kRed);
	dptRes1->GetXaxis()->SetTitle("PtError/Pt");

	TH1F* dz1 = new TH1F("dz1", "", 100, -0.2, 0.2);
	TH1F* dz2 = new TH1F("dz2", "", 100, -0.2, 0.2);
	dz2->SetLineColor(kRed);
	dz1->GetXaxis()->SetTitle("trkDz");

	TCanvas *c = new TCanvas("c","", 1200, 400);
	c->Divide(4,1);
	c->cd(1);
	t->Draw("trkPtError/trkPt>>dptRes1" );
	t->Draw("trkPtError/trkPt>>dptRes2","highPurity ==1", "same");

	c->cd(2);
	t->Draw("(trkDz/trkDzError)>>dz1" );
	t->Draw("(trkDz/trkDzError)>>dz2","highPurity ==1", "same");

	TH1F* dxy1 = new TH1F("dxy1", "", 100, -0.2, 0.2);
	TH1F* dxy2 = new TH1F("dxy2", "", 100, -0.2, 0.2);
	dxy2->SetLineColor(kRed);
	dxy1->GetXaxis()->SetTitle("trkDxy");
	c->cd(3);
	t->Draw("trkDxy>>dxy1" );
	t->Draw("trkDxy>>dxy2","highPurity ==1", "same");
	//t->Draw("trkDz>>dz2","highPurity ==1", "same");
	/*
	*/
}
