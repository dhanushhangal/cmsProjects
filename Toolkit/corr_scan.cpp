

// this will be the template for deriving the track correction table.
#include "inputTree.h"
#include "TH2F.h"
#include "TF1.h"
#include <iostream> 
#include <fstream> 
#include "TMath.h"
#include "TFile.h"
using namespace std;
#define nhibin 50
#define nptbin 10
//predefinition
int hibins[nhibin+1];

int increment = 4;
float ptbins[11] = {0.5, 0.7, 1, 2, 3, 4, 8, 12, 16, 20 ,300};


int main (int argc, char * argv[]){

	for( int k =0; k<nhibin+1; k++){
		hibins[k] = 0+ k*increment;
	}
	//	Float_t ptbins[9] = {0.5, 1,2, 3, 4, 8, 12, 16,20};
	//
	//reading the mc data list and get the mc data
	std::string weightF="VertexHiNcollFits.root";
	TFile* weightf = TFile::Open(weightF.c_str());
	TF1* xfit_vz = (TF1*)weightf->Get("xfit_vz");
	TF1* xfit_hi = (TF1*)weightf->Get("xfit_hi");
	std::string mcList = "mcList.txt";
	std::string mcpath="/data/kurtjung/JetTrackCorr_skims/5TeV_MC_PbPb_Pythia6Hydjet/";
	std::ifstream listf;
	std::string fileName;
	std::string dump;
	int nline = atoi(argv[1]); 
	listf.open(mcList.c_str());
	std::string saveF = Form("./output/corr_allpt_v9_%d.root", nline);
	for(int j=1; j<nline; j++)listf>>dump;
	listf>>fileName;
	std::string fileInput = mcpath+fileName;
	cout<<"processing file: "<<fileName<<endl;
	TFile *f;
	f=TFile::Open(fileInput.c_str());
	TTree * tree = (TTree*)f->Get("mixing_tree");
	inputTree *t = new inputTree(tree);
	TH2F* hvz[nhibin][10][2];
	TH2F* hall[nhibin][10][2];
	TH1F* vzDist;
	TString title;

	vzDist=new TH1F("vzDist", "vz distribution", 30, -15,15);
	for( int i =0; i<2; i++){
		for ( int j = 0; j< nhibin; j++){
			for ( int k=0; k<10; k++){
				if ( i == 0) title = Form("Gen (hiBin: %d - %d, %f <= pt < %f )",hibins[j], hibins[j+1], ptbins[k], ptbins[k+1]);
				else if( i ==1) title = Form("Reco with quality cuts (hiBin: %d - %d, %f <= pt < %f )",hibins[j], hibins[j+1], ptbins[k], ptbins[k+1]);

				hvz[j][k][i]= new TH2F(Form("hvz_%d_%d_%d",j,k,i),title ,100, -5, 5, 72, -TMath::Pi(), TMath::Pi()); 
				hun[j][k][i]= new TH2F(Form("hun_%d_%d_%d",j,k,i),title ,100, -5, 5, 72, -TMath::Pi(), TMath::Pi()); 
				hall[j][k][i]= new TH2F(Form("hall_%d_%d_%d",j,k,i),title ,100, -5, 5, 72, -TMath::Pi(), TMath::Pi()); 
				hun[j][k][i]->GetXaxis()->SetTitle("#eta");
				hun[j][k][i]->GetYaxis()->SetTitle("#phi");
				hun[j][k][i]->Sumw2();
				hvz[j][k][i]->GetXaxis()->SetTitle("#eta");
				hvz[j][k][i]->GetYaxis()->SetTitle("#phi");
				hvz[j][k][i]->Sumw2();
				hall[j][k][i]->GetXaxis()->SetTitle("#eta");
				hall[j][k][i]->GetYaxis()->SetTitle("#phi");
				hall[j][k][i]->Sumw2();
			}
		}
	}
	// calculating the weights---------------------------
	for ( int indx = 0; indx < 9; indx ++){
		if(nEntries[indx] ==0) continue;
		weights[indx] = (crossSection[indx]-crossSection[indx+1])/nEntries[indx];
		cout<<"["<<pthatbins[indx]<<","<<pthatbins[indx+1]<<"]: "<<weights[indx]<<endl;
	}
	Long64_t nentries = t->fChain->GetEntriesFast();
	for( Long64_t jentry=0; jentry < nentries; jentry++){
		if( jentry %10000 ==0) {cout<<jentry<<endl;
		}
		int hi= -1, hj = -1;
		t->GetEntry(jentry);

		if ( t->HBHENoiseFilterResultRun2Loose ==0) continue;
		if ( t->pcollisionEventSelection ==0) continue;
		if ( t->pprimaryVertexFilter ==0) continue;
		if ( t->phfCoincFilter3 ==0) continue;
		if( abs(t->vz) > 15) continue;

		float wvz = 1;
		float whi = 1;
		float wpthat = 1;
		// getting the pthat weight-------------------------------
		for ( int ibin =0; ibin < 9; ibin++){
			if ((float)t->pthat >= pthatbins[ibin] && (float)t->pthat< pthatbins[ibin+1]) {wpthat = weights[ibin];
			}
		}
		wpthat =1;  //for no pthat weight case
		//get vz weight -----------------
		wvz = xfit_vz->Eval(t->vz);
		whi = xfit_hi->Eval(t->hiBin);
		vzDist->Fill(t->vz);
		for ( int i = 0; i< nhibin; i++) if (int( t->hiBin) >= hibins[i] && int(t->hiBin) < hibins[i+1]) hi = i;
		if( hi < 0) continue;
		// filling the gen particles--------------------------
		for ( int j =0; j< t->pt->size(); j++){
			if( t->chg->at(j) ==0) continue;
			hj = -1;
			for ( int k = 0; k< nptbin; k++) if ( t->pt->at(j) >= ptbins[k] && t->pt->at(j) < ptbins[k+1]) hj = k;
			if ( hj <0) continue;
			hun[hi][hj][0]->Fill(t->eta->at(j), t->phi->at(j),wpthat);
			hvz [hi][hj][0]->Fill(t->eta->at(j), t->phi->at(j),wvz*wpthat);
			hall[hi][hj][0]->Fill(t->eta->at(j), t->phi->at(j),whi*wvz*wpthat);
		}
		//filling the reco ---------------------------------------
		for ( int j =0; j< t->trkPt->size(); j++){
			hj = -1;
			if (t->highPurity->at(j) ) {
				for (int k = 0; k< nptbin; k++){
					if ( t->trkPt->at(j) >= ptbins[k] && t->trkPt->at(j) < ptbins[k+1]) hj = k;
				}
				if ( hj < 0) continue;
				float Et = (t->pfHcal->at(j)+t->pfEcal->at(j))/TMath::CosH(t->trkEta->at(j));
				if(!(t->trkPt->at(j)<20 || (Et>0.5*t->trkPt->at(j)))) continue;
				if ( abs(t->trkDz->at(j)/t->trkDzError->at(j)) >=3) continue;
				if ( abs(t->trkDxy->at(j)/t->trkDxyError->at(j)) >=3) continue;
				if ( t->trkPtError->at(j)/t->trkPt->at(j)>=0.1) continue;
				if ( t->trkNHit->at(j)< 11) continue;
				if ( (float)t->trkChi2->at(j)/(float)t->trkNdof->at(j)/(float)t->trkNlayer->at(j)>0.15) continue;
				hvz[hi][hj][1]->Fill(t->trkEta->at(j), t->trkPhi->at(j),wvz*wpthat);
				hun[hi][hj][1]->Fill(t->trkEta->at(j), t->trkPhi->at(j),wpthat);
				hall[hi][hj][1]->Fill(t->trkEta->at(j), t->trkPhi->at(j),whi*wvz*wpthat);
			}
		}
	}
	TFile *wf = TFile::Open(saveF.c_str(),"recreate");

	for( int j =0; j<nhibin; j++){
		for ( int k = 0; k< nptbin; k++){
			hun[j][k][0]->Write();
			hun[j][k][1]->Write();
			hvz[j][k][0]->Write();
			hvz[j][k][1]->Write();
			hall[j][k][0]->Write();
			hall[j][k][1]->Write();
		}
	}
	vzDist->Write();
	wf->Close();
	return 0;
}
