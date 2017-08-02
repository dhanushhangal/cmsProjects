/*
   this .h file will load all the histograms we need to generate the root file for signal.
   rightnow I only try to load the inclusive jet and mixing event table.

*/

#define nhibin 4
#define nptbin 10

TString primaryTag[] = {"Data_hJetTrackSignalBackground", "Data_hJetTrackME"};
TString subprimaryTag[] = {"", "Leading", "SubLeading", "NonLeading"};
TString minorTag[] = {"","_pTweighted"};
TString centTag[] = {"Cent0","Cent10", "Cent30", "Cent50", "Cent100"};
TString jtPtTag[] = {"_Pt100", "_Pt300"};
TString trkPtTag[] = {"_TrkPt0p5", "_TrkPt0p7", "_TrkPt1", "_TrkPt2", "_TrkPt3", "_TrkPt4", "_TrkPt8", "_TrkPt12", "_TrkPt16", "_TrkPt20", "_TrkPt999"};
TString jtprimaryTag[] = {"Data_all_jets_corrpT", "only_leadingjets_corrpT", "only_subleadingjets_corrpT", "only_nonleadingjets_corrpT"};


TH2D* Signal[4][10]; //for gen gen signal
TH2D* Signal_pTweighted[4][10]; //for gen gen signal
TH2D* ME[4][10]; //for gen me
TH1D* jetDist[4];
TH2F* h2corr[4][10];

void getDataHist(TFile* f){
	TString stemp; 
	for(int jcent=0; jcent<4; jcent++){
		//get the jet distribution for getting the # of jets
		stemp = jtprimaryTag[0]+\
			centTag[jcent]+"_"+centTag[jcent+1]+\
			jtPtTag[0]+jtPtTag[1];
		jetDist[jcent]= (TH1D*)f->Get(stemp);

		for(int jtrkpt=0; jtrkpt<10; jtrkpt++){
			stemp = primaryTag[0]+subprimaryTag[0]+minorTag[0]+\
				centTag[jcent]+"_"+centTag[jcent+1]+\
				jtPtTag[0]+jtPtTag[1]+\
				trkPtTag[jtrkpt]+trkPtTag[jtrkpt+1];
			Signal[jcent][jtrkpt] = (TH2D*)f->Get(stemp)->Clone(stemp);

			stemp = primaryTag[0]+subprimaryTag[0]+minorTag[1]+\
				centTag[jcent]+"_"+centTag[jcent+1]+\
				jtPtTag[0]+jtPtTag[1]+\
				trkPtTag[jtrkpt]+trkPtTag[jtrkpt+1];
			Signal_pTweighted[jcent][jtrkpt] = (TH2D*)f->Get(stemp)->Clone(stemp);

			stemp = primaryTag[1]+subprimaryTag[0]+minorTag[0]+\
				centTag[jcent]+"_"+centTag[jcent+1]+\
				jtPtTag[0]+jtPtTag[1]+\
				trkPtTag[jtrkpt]+trkPtTag[jtrkpt+1];
			ME[jcent][jtrkpt] = (TH2D*)f->Get(stemp)->Clone(stemp);
			//substitute the mixing for 16<pt<300, cent:50-100% by the mixing of cent:30-50%
			/*
			*/
			if( jtrkpt >= 8 && jcent ==3){
				stemp = primaryTag[1]+subprimaryTag[0]+minorTag[0]+\
					// this line implement the substitution.
					centTag[jcent-1]+"_"+centTag[jcent]+\
					jtPtTag[0]+jtPtTag[1]+\
					trkPtTag[jtrkpt]+trkPtTag[jtrkpt+1];
				TString stemp2 = primaryTag[1]+subprimaryTag[0]+minorTag[0]+\
						 centTag[jcent]+"_"+centTag[jcent+1]+\
						 jtPtTag[0]+jtPtTag[1]+\
						 trkPtTag[jtrkpt]+trkPtTag[jtrkpt+1];
				ME[jcent][jtrkpt] = (TH2D*)f->Get(stemp)->Clone(stemp2);
			}
		}
	}
}

void getCorr(){
	TFile* f  = TFile::Open("/Users/tabris/Research/HIN_5.02TeV/analyzer/JetTrack2016/jet_shapes_result/Jet_Shapes_pTweighted.root");
	TString tmp;
	for(int i=1; i<10; ++i){
		for(int j=0; j<4; ++j){
			if(i==1) tmp = "Yield_BkgSub_RecoGen_"+centTag[j]+"_"+centTag[j+1]+"_TrkPt07_TrkPt1";
			else if(i<9)tmp = "Yield_BkgSub_RecoGen_"+centTag[j]+"_"+centTag[j+1]+trkPtTag[i]+trkPtTag[i+1];
			else tmp = "Yield_BkgSub_RecoGen_"+centTag[j]+"_"+centTag[j+1]+trkPtTag[i]+"_TrkPt300";
			h2corr[j][i]= (TH2F*)f->Get(tmp);
			h2corr[j][i]->Scale(1.0/h2corr[j][i]->GetXaxis()->GetBinWidth(1)/h2corr[j][i]->GetYaxis()->GetBinWidth(1));
		}
	}
}
