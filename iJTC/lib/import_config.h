
const int nPt = 8;
const int nCent = 2;
namespace input_raw2D{
		TString trk_tag[] = {"TrkPt0.7", "TrkPt1", "TrkPt2","TrkPt3","TrkPt4","TrkPt8","TrkPt12","TrkPt16","TrkPt999"};
		TString cent_tag[]= {"Cent0","Cent30","Cent100"};

		TFile *recgen_pb_f = TFile::Open("/Users/tabris/cmsProjects/iJTC/dataSet/correlation/RecGen_JTC_200meptrig.root");
		TFile *recrec_pb_f = TFile::Open("/Users/tabris/cmsProjects/iJTC/dataSet/correlation/RecRec_JTC_200meptrig.root");
		TFile *gengen_pb_f = TFile::Open("/Users/tabris/cmsProjects/iJTC/dataSet/correlation/GenGen_JTC_200meptrig_3.root");
		TFile *genrec_pb_f = TFile::Open("/Users/tabris/cmsProjects/iJTC/dataSet/correlation/GenRec_JTC_200meptrig_3.root");

		TH2D* raw_sig[8][2];
		TH2D* mixing [8][2];
		void get2DInput(TFile *f, TString cap){
				TH1D* h1; TString tmp; double njet;
				for(int i=0; i<nPt; ++i){
						for(int j=0; j<nCent; ++j){
								tmp = cap+"_all_bjets_corrpT"+cent_tag[j]+"_"+cent_tag[j+1]\
									   +"_Pt120_Pt1000";
//								cout<<tmp<<endl;
								h1 = (TH1D*) f->Get(tmp);
								njet = h1->Integral();
								tmp = cap+"_hbJetTrackSignalBackground_"+cent_tag[j]+"_"+cent_tag[j+1]\
									   +"_Pt120_Pt1000_"+trk_tag[i]+"_"+trk_tag[i+1];
								raw_sig[i][j] = (TH2D*) f->Get(tmp);
								raw_sig[i][j]->Scale(1.0/njet);
								tmp = cap+"_hbJetTrackME"+cent_tag[j]+"_"+cent_tag[j+1]\
									   +"_Pt120_Pt1000_"+trk_tag[i]+"_"+trk_tag[i+1];
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
		void get2DInput_RecGen(){ get2DInput(recgen_pb_f, "RecoJet_GenTrack"); }
		void get2DInput_RecRec(){ get2DInput(recrec_pb_f, "RecoJet_RecoTrack"); }
		void get2DInput_GenGen(){ get2DInput(gengen_pb_f, "GenJet_GenTrack"); }
		void get2DInput_GenRec(){ get2DInput(genrec_pb_f, "GenJet_RecoTrack"); }
}

namespace signal2D {
		TH2D* sig[nPt][nCent];
		TH2D* raw[nPt][nCent];
		TH2D* mixing[nPt][nCent];
		TH2D* bkg[nPt][nCent];

		TFile *recgen_pb_f;
		TFile *recrec_pb_f;
		TFile *gengen_pb_f;
		TFile *genrec_pb_f;

		void loadFile(){
				cout<<"reading the file.. "<<endl;
				recgen_pb_f = TFile::Open("/Users/tabris/cmsProjects/iJTC/dataSet/correlation/rec_gen_JTCSignal.root");
				recrec_pb_f = TFile::Open("/Users/tabris/cmsProjects/iJTC/dataSet/correlation/rec_rec_JTCSignal.root");
				gengen_pb_f = TFile::Open("/Users/tabris/cmsProjects/iJTC/dataSet/correlation/gen_gen_JTCSignal.root");
				genrec_pb_f = TFile::Open("/Users/tabris/cmsProjects/iJTC/dataSet/correlation/gen_rec_JTCSignal.root");
		}
		void getHist(TFile *f, TString name){
				TString tmp;
				for(int i=0; i<7; ++i){
						for(int j=0; j<2; ++j){
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
		void getRecRec(){ getHist(recrec_pb_f, "rec_rec");}
		void getRecGen(){ getHist(recgen_pb_f, "rec_gen");}
}
