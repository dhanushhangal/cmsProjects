

#include "/Users/tabris/cmsProjects/jetTrackCorrelation/jtShape.h"


void signalFinder(TString outFile){


	TFile *wf = TFile::Open(outFile,"recreate");
	jtShape* jtT = new jtShape();
	TH1D* PY_dr[nhibin][nptbin];
	TH1D* PY_dr_total[nhibin];
	TH1D* PY_eta[nhibin][nptbin];
	TH1D* PY_eta_total[nhibin];
	TH1D* PY_phi[nhibin][nptbin];
	TH1D* PY_phi_total[nhibin];
	TH1D* jetShape[nhibin][nptbin];
	TH1D* jetShape_total[nhibin];
	TH1D* jetShape_eta[nhibin][nptbin];
	TH1D* jetShape_phi[nhibin][nptbin];
	TH1D* jetShape_eta_total[nhibin];
	TH1D* jetShape_phi_total[nhibin];


	int nn =4;
	int xmin, xmax, ymin, ymax;
	float xbinwidth, ybinwidth;
	TH2D* Sig;
	for(int jcent =0;jcent<nhibin; jcent++){
		for(int jtrkpt=1;jtrkpt<nptbin;jtrkpt++){
			jetShape[jcent][jtrkpt] = new TH1D(Form("jetShape_%d_%d",jcent, jtrkpt), "",15,xdrbins);
			PY_dr[jcent][jtrkpt] = new TH1D(Form("PY_dr_%d_%d",jcent, jtrkpt), "",15,xdrbins);

			// get the signal for particle yield -----
			Sig = jtT->getSignal((TH2D*)Signal[jcent][jtrkpt],\
					(TH2D*) ME[jcent][jtrkpt], (TH1D*)jetDist[jcent], 1.5, 2.5);
			// ---------------------------------------
			// get the projection --------------------
			xmin = Sig->GetXaxis()->FindBin(-1);
			xmax = Sig->GetXaxis()->FindBin( 1);
			ymin = Sig->GetYaxis()->FindBin(-1);
			ymax = Sig->GetYaxis()->FindBin( 1);
			xbinwidth = Sig->GetXaxis()->GetBinWidth(1);
			ybinwidth = Sig->GetYaxis()->GetBinWidth(1);
			PY_eta[jcent][jtrkpt] = (TH1D*) Sig->ProjectionX(Form("PY_eta_%d_%d", jcent,jtrkpt),ymin,ymax,"e");
			PY_eta[jcent][jtrkpt]->Scale(ybinwidth);
			//PY_eta[jcent][jtrkpt]->Scale(1.0/PY_eta[jcent][jtrkpt]->GetBinWidth(1)); 
			PY_phi[jcent][jtrkpt] = (TH1D*) Sig->ProjectionY(Form("PY_phi_%d_%d", jcent,jtrkpt),xmin,xmax,"e");
			PY_phi[jcent][jtrkpt]->Scale(xbinwidth);
			if(jtrkpt==1){
				PY_eta_total  [jcent]=(TH1D*) PY_eta [jcent][jtrkpt]->Clone(Form("PY_eta_total_%d",jcent));
				PY_phi_total  [jcent]=(TH1D*) PY_phi [jcent][jtrkpt]->Clone(Form("PY_phi_total_%d",jcent));
				PY_dr_total   [jcent]=(TH1D*) PY_dr  [jcent][jtrkpt]->Clone(Form("PY_dr_total_%d",jcent));
			}
			else {
				PY_eta_total  [jcent] ->Add(PY_eta[jcent][jtrkpt]);
				PY_phi_total  [jcent] ->Add(PY_phi[jcent][jtrkpt]);
				PY_dr_total   [jcent] ->Add(PY_dr[jcent][jtrkpt]);
			}
			jtT->drJetShape(Sig,PY_dr[jcent][jtrkpt]);	

			// get the jet shape ---------------------
			Sig = jtT->getSignal((TH2D*)Signal_pTweighted[jcent][jtrkpt],\
					(TH2D*) ME[jcent][jtrkpt], (TH1D*)jetDist[jcent], 1.5, 2.5);
			xmin = Sig->GetXaxis()->FindBin(-1);
			xmax = Sig->GetXaxis()->FindBin( 1);
			ymin = Sig->GetYaxis()->FindBin(-1);
			ymax = Sig->GetYaxis()->FindBin( 1);
			xbinwidth = Sig->GetXaxis()->GetBinWidth(1);
			ybinwidth = Sig->GetYaxis()->GetBinWidth(1);
			// correction added here
			Sig->Add(h2corr[jcent][jtrkpt],-1);
			jtT->drJetShape(Sig,jetShape[jcent][jtrkpt]);	
			// do projection for jet shape -----------
			jetShape_eta[jcent][jtrkpt] = (TH1D*) Sig->ProjectionX(Form("jetShape_eta_%d_%d", jcent,jtrkpt),ymin,ymax, "e");
			jetShape_eta[jcent][jtrkpt]->Scale(ybinwidth);
			//jetShape_eta[jcent][jtrkpt]->Scale(1.0/PY_eta[jcent][jtrkpt]->GetBinWidth(1)); 
			jetShape_phi[jcent][jtrkpt] = (TH1D*) Sig->ProjectionY(Form("jetShape_phi_%d_%d", jcent,jtrkpt),xmin,xmax, "e");
			jetShape_phi[jcent][jtrkpt]->Scale(xbinwidth);
			//jetShape_phi[jcent][jtrkpt]->Scale(1.0/PY_phi[jcent][jtrkpt]->GetBinWidth(1)); 

			// get the grand projection --------------
			if(jtrkpt==1){
				jetShape_total[jcent]=(TH1D*) jetShape[jcent][jtrkpt]->Clone(Form("jetShape_total_%d",jcent));
				jetShape_eta_total[jcent]=(TH1D*) jetShape_eta[jcent][jtrkpt]->Clone(Form("jetShape_eta_total_%d",jcent));
				jetShape_phi_total[jcent]=(TH1D*) jetShape_phi[jcent][jtrkpt]->Clone(Form("jetShape_phi_total_%d",jcent));
			}
			else {
				jetShape_total[jcent] ->Add(jetShape[jcent][jtrkpt]); 
				jetShape_eta_total[jcent] ->Add(jetShape_eta[jcent][jtrkpt]); 
				jetShape_phi_total[jcent] ->Add(jetShape_phi[jcent][jtrkpt]); 
			}
		}

	//	for(int j=1;j<nptbin;j++){
		//	jetShape[jcent][j]->Scale(1.0/jetShape_total[jcent]->Integral("width"));
		//	PY_eta[jcent][j] ->Scale(1.0/PY_eta_total  [jcent]->Integral("width"));
		//	PY_phi[jcent][j] ->Scale(1.0/PY_phi_total  [jcent]->Integral("width"));
		//	PY_dr[jcent][j]  ->Scale(1.0/PY_dr_total   [jcent]->Integral("width"));
	//	}
	}

	for(int i=0;i<nhibin;i++){
		jetShape_total[i]->Write();
		jetShape_eta_total[i]->Write();
		jetShape_phi_total[i]->Write();
		PY_eta_total  [i]->Write();
		PY_phi_total  [i]->Write();
		PY_dr_total   [i]->Write();
		for(int j=1;j<nptbin;j++){
			jetShape[i][j]->Write();
			PY_eta [i][j]->Write();
			PY_phi [i][j]->Write();
			PY_dr  [i][j]->Write();
			jetShape_eta[i][j]->Write();
		}
	}
	wf->Close();
}

