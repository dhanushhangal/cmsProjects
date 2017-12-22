
const int nPt = 9;
const int nCent = 5;
namespace input_raw2D{
		TString trk_tag[] = {"TrkPt0p7", "TrkPt1", "TrkPt2","TrkPt3","TrkPt4","TrkPt8","TrkPt12","TrkPt16","TrkPt20","TrkPt999"};
		TString cent_tag[]= {"Cent0","Cent10","Cent30","Cent50","Cent70","Cent100"};

		TFile *gengen_pb_f = TFile::Open("/data/kurt_histos/PbPb_5TeVMC_GenGenSube0_QuarkJets_fixMatch_fineBinTrkCorrs_withTrkCorrEtaSymmV2_finalJFF_noMix.root");

		TH2D* raw_sig[9][5];
		TH2D* raw_sig_pTweighted[9][5];
		TH2D* mixing [9][5];

		void get2DInput(TFile *f, TString cap){
				TH1D* h1; TString tmp; double njet;
				for(int i=0; i<nPt; ++i){
						for(int j=0; j<nCent; ++j){
								tmp = cap+"_all_jets_corrpT"+cent_tag[j]+"_"+cent_tag[j+1]\
									   +"_Pt100_Pt1000";
								h1 = (TH1D*) f->Get(tmp);
								njet = h1->Integral();
//								cout<<tmp<<endl;
								tmp = cap+"_hJetTrackSignalBackground"+cent_tag[j]+"_"+cent_tag[j+1]\
									   +"_Pt100_Pt1000_"+trk_tag[i]+"_"+trk_tag[i+1];
								raw_sig[i][j] = (TH2D*) f->Get(tmp);
								raw_sig[i][j]->Scale(1.0/njet);
								tmp = cap+"_hJetTrackSignalBackground_pTweighted"+cent_tag[j]+"_"+cent_tag[j+1]\
									   +"_Pt100_Pt1000_"+trk_tag[i]+"_"+trk_tag[i+1];
								raw_sig_pTweighted[i][j] = (TH2D*) f->Get(tmp);
								raw_sig_pTweighted[i][j]->Scale(1.0/njet);
								tmp = cap+"_hJetTrackME"+cent_tag[j]+"_"+cent_tag[j+1]\
									   +"_Pt100_Pt1000_"+trk_tag[i]+"_"+trk_tag[i+1];
						//		cout<<mixing[i][j]->GetName()<<endl;
								mixing[i][j] = (TH2D*) f->Get(tmp);
						}
				}
		}

		void clearInput(){
				for(int i=0; i<nPt; ++i){
						for(int j=0; j<nCent; ++j){
								delete raw_sig[i][j];
								delete mixing[i][j];
						}
				}
		}

		void get2DInput_GenGen(){ get2DInput(gengen_pb_f, "GenJet_GenTrack"); }
}

namespace signal2D {
		TH2D* sig[nPt][nCent];
		TH2D* raw[nPt][nCent];
		TH2D* mixing[nPt][nCent];
		TH2D* bkg[nPt][nCent];

		TFile *gengen_pb_f;

		void loadFile(){
				cout<<"reading the file.. "<<endl;
				gengen_pb_f = TFile::Open("/data/kurt_histos/PbPb_5TeVMC_GenGenSube0_QuarkJets_fixMatch_fineBinTrkCorrs_withTrkCorrEtaSymmV2_finalJFF_noMix.root");
		}
		void getHist(TFile *f, TString name){
				TString tmp;
				for(int i=0; i<nPt; ++i){
						for(int j=0; j<nCent; ++j){
								tmp = "signal_"+name+Form("_%d_%d",i,j);
								//cout<<tmp<<endl;
								sig[i][j]=(TH2D*) f->Get(tmp);
								tmp = "smoothed_mixing_"+name+Form("_%d_%d",i,j);
								mixing[i][j]=(TH2D*) f->Get(tmp);
								tmp = "raw_"+name+Form("_%d_%d",i,j);
								raw[i][j]=(TH2D*) f->Get(tmp);
								tmp = "bkg_"+name+Form("_%d_%d",i,j);
								bkg[i][j]=(TH2D*) f->Get(tmp);
						}
				}
		}
}
