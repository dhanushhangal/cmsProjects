
#ifndef jtShape_H
#include "jtShape.h"
#endif

namespace kurt_reg{
	TFile *GenGen_MC_pp_f ;
	TFile *RecRec_MC_pp_f ;
	TFile *GenGen_MC_pp_f_calo5jet ;
	TFile *GenGen_MC_pp_f_calo3jet ;
	TFile *GenGen_MC_pb_f ;
	TFile *RecRec_MC_pb_f ;
	TFile *GenGen_MC_pb_f_sub0 ;
	void loadConfig(){
		std::cout<<"loading kurt's configuration.."<<std::endl;
		//pp input 
		GenGen_MC_pp_f_calo5jet =TFile::Open("/Users/tabris/cmsProjects/inclusiveJetTrackCorrelation2015/dataSet/correlation/pp_5TeVMC_GenGen_PFJets_R0p5_finalJFF_noMix.root");	
		GenGen_MC_pp_f_calo3jet =TFile::Open("/Users/tabris/cmsProjects/inclusiveJetTrackCorrelation2015/dataSet/correlation/pp_5TeVMC_GenGen_PFJets_R0p3_finalJFF_noMix.root");	
		GenGen_MC_pp_f =TFile::Open("/Users/tabris/cmsProjects/inclusiveJetTrackCorrelation2015/dataSet/correlation/pp_Pythia6MC_GenGen_withMix_inclJetBinning_finalJFFs.root");	
		RecRec_MC_pp_f =TFile::Open("/Users/tabris/cmsProjects/inclusiveJetTrackCorrelation2015/dataSet/correlation/pp_Pythia6MC_RecoReco_withMix_inclJetBinning_finalJFFs.root");	
		// pb mc input 
		GenGen_MC_pb_f =TFile::Open("/Users/tabris/cmsProjects/inclusiveJetTrackCorrelation2015/dataSet/correlation/PbPb_5TeVMC_withMix_GenGen_CymbalTune_fullCymbalCorrs_inclJetBinning.root");	
		RecRec_MC_pb_f =TFile::Open("/Users/tabris/cmsProjects/inclusiveJetTrackCorrelation2015/dataSet/correlation/PbPb_5TeVMC_withMix_RecoReco_CymbalTune_fullCymbalCorrs_inclJetBinning.root");	
		GenGen_MC_pb_f_sub0 =TFile::Open("/Users/tabris/cmsProjects/inclusiveJetTrackCorrelation2015/dataSet/correlation/PbPb_5TeVMC_GenGenSube0_fineBinTrkCorrs_withTrkCorrEtaSymmV2_noJFF_noMix.root");	
	}
	TString trk_tag[] = {"TrkPt0p7", "TrkPt1", "TrkPt2","TrkPt3","TrkPt4","TrkPt8","TrkPt12","TrkPt16","TrkPt20","TrkPt999"};
	TString cent_tag[]= {"Cent0","Cent10","Cent30","Cent50","Cent70","Cent100"};
	TH1D* GenJet_pp;	
	TH2D* GenGen_MC_pp_sig[9];	
	TH2D* GenGen_MC_pp_mix[9];	
	TH1D* GenJet_pb[5];	
	TH2D* GenGen_MC_pb_sig[9][5];	
	TH2D* GenGen_MC_pb_mix[9][5];	
	TH2D* RecRec_MC_pb_sig[9][5];
	TH2D* RecRec_MC_pb_mix[9][5];
	void getGenGen_pb(){
		float trkBinWidth[] = {0.3, 1, 1,1, 4, 4, 4, 4, 50}; 
		TString stmp;
		for(int j=0; j<5; ++j){
			stmp = "GenJet_GenTrack_all_jets_corrpT"+cent_tag[j]+"_"+cent_tag[j+1]+"_Pt100_Pt1000";
			GenJet_pb[j]= (TH1D*)GenGen_MC_pb_f->Get(stmp);
			for(int i=0;i<9; ++i){
				stmp = "GenJet_GenTrack_hJetTrackME"+cent_tag[j]+"_"+cent_tag[j+1]+"_Pt100_Pt1000_"
					+trk_tag[i]+"_"+trk_tag[i+1];
				GenGen_MC_pb_mix[i][j]=(TH2D*)GenGen_MC_pb_f->Get(stmp);
				stmp = "GenJet_GenTrack_hJetTrackSignalBackground"+cent_tag[j]+"_"+cent_tag[j+1]+"_Pt100_Pt1000_"
					+trk_tag[i]+"_"+trk_tag[i+1];
				GenGen_MC_pb_sig[i][j]=(TH2D*)GenGen_MC_pb_f->Get(stmp);
				GenGen_MC_pb_sig[i][j]->Scale(1./trkBinWidth[i]);
			}
		}
		//combined the last two bins together
		GenJet_pb[3]->Add(GenJet_pb[4]);
		for(int i=0;i<9; ++i){
			//combined the last two bins together
			GenGen_MC_pb_sig[i][3]->Add(GenGen_MC_pb_sig[i][4]);
			for(int j=0; j<4; ++j){
				//adding the cent50-70 and cent70-100 together:
				GenGen_MC_pb_sig[i][j]->Scale(1.0/GenJet_pb[j]->Integral());
			}
		}
	}

	void getGenGen_pb_sub0(){
		float trkBinWidth[] = {0.3, 1, 1,1, 4, 4, 4, 4, 50}; 
		TString stmp;
		for(int j=0; j<5; ++j){
			stmp = "GenJet_GenTrack_all_jets_corrpT"+cent_tag[j]+"_"+cent_tag[j+1]+"_Pt100_Pt1000";
			GenJet_pb[j]= (TH1D*)GenGen_MC_pb_f_sub0->Get(stmp);
			for(int i=0;i<9; ++i){
				stmp = "GenJet_GenTrack_hJetTrackME"+cent_tag[j]+"_"+cent_tag[j+1]+"_Pt100_Pt1000_"
					+trk_tag[i]+"_"+trk_tag[i+1];
				GenGen_MC_pb_mix[i][j]=(TH2D*)GenGen_MC_pp_f->Get(stmp);
				stmp = "GenJet_GenTrack_hJetTrackSignalBackground"+cent_tag[j]+"_"+cent_tag[j+1]+"_Pt100_Pt1000_"
					+trk_tag[i]+"_"+trk_tag[i+1];
				GenGen_MC_pb_sig[i][j]=(TH2D*)GenGen_MC_pb_f_sub0->Get(stmp);
				GenGen_MC_pb_sig[i][j]->Scale(1./trkBinWidth[i]);
			}
		}
		GenJet_pb[3]->Add(GenJet_pb[4]);
		for(int i=0;i<9; ++i){
			//combined the last two bins together
			GenGen_MC_pb_sig[i][3]->Add(GenGen_MC_pb_sig[i][4]);
			for(int j=0; j<4; ++j){
				//adding the cent50-70 and cent70-100 together:
				GenGen_MC_pb_sig[i][j]->Scale(1.0/GenJet_pb[j]->Integral());
			}
		}
	}

	void getGenGen_pp(){
		TString stmp;
		float trkBinWidth[] = {0.3, 1, 1,1, 4, 4, 4, 4, 50}; 
		stmp = "GenJet_GenTrack_all_jets_corrpT"+cent_tag[0]+"_"+cent_tag[1]+"_Pt100_Pt1000";
		GenJet_pp= (TH1D*)GenGen_MC_pp_f->Get(stmp);
		for(int i=0;i<9; ++i){
			stmp = "GenJet_GenTrack_hJetTrackME"+cent_tag[0]+"_"+cent_tag[1]+"_Pt100_Pt1000_"
				+trk_tag[i]+"_"+trk_tag[i+1];
			GenGen_MC_pp_mix[i]=(TH2D*)GenGen_MC_pp_f->Get(stmp);
			stmp = "GenJet_GenTrack_hJetTrackSignalBackground"+cent_tag[0]+"_"+cent_tag[1]+"_Pt100_Pt1000_"
				+trk_tag[i]+"_"+trk_tag[i+1];
			GenGen_MC_pp_sig[i]=(TH2D*)GenGen_MC_pp_f->Get(stmp);
			GenGen_MC_pp_sig[i]->Scale(1.0/GenJet_pp->Integral()/trkBinWidth[i]);
		}
	}

	void getGenGen_pp_calo5jet(){
		TString stmp;
		float trkBinWidth[] = {0.3, 1, 1,1, 4, 4, 4, 4, 50}; 
		stmp = "GenJet_GenTrack_all_jets_corrpT"+cent_tag[0]+"_"+cent_tag[1]+"_Pt100_Pt1000";
		GenJet_pp= (TH1D*)GenGen_MC_pp_f_calo5jet->Get(stmp);
		for(int i=0;i<9; ++i){
			stmp = "GenJet_GenTrack_hJetTrackME"+cent_tag[0]+"_"+cent_tag[1]+"_Pt100_Pt1000_"
				+trk_tag[i]+"_"+trk_tag[i+1];
			GenGen_MC_pp_mix[i]=(TH2D*)GenGen_MC_pp_f->Get(stmp);
			stmp = "GenJet_GenTrack_hJetTrackSignalBackground"+cent_tag[0]+"_"+cent_tag[1]+"_Pt100_Pt1000_"
				+trk_tag[i]+"_"+trk_tag[i+1];
			cout<<stmp<<endl;
			GenGen_MC_pp_sig[i]=(TH2D*)GenGen_MC_pp_f_calo5jet->Get(stmp);
			GenGen_MC_pp_sig[i]->Scale(1.0/GenJet_pp->Integral()/trkBinWidth[i]);
		}
	}

	void getGenGen_pp_calo3jet(){
		TString stmp;
		float trkBinWidth[] = {0.3, 1, 1,1, 4, 4, 4, 4, 50}; 
		stmp = "GenJet_GenTrack_all_jets_corrpT"+cent_tag[0]+"_"+cent_tag[1]+"_Pt100_Pt1000";
		GenJet_pp= (TH1D*)GenGen_MC_pp_f_calo3jet->Get(stmp);
		for(int i=0;i<9; ++i){
			stmp = "GenJet_GenTrack_hJetTrackME"+cent_tag[0]+"_"+cent_tag[1]+"_Pt100_Pt1000_"
				+trk_tag[i]+"_"+trk_tag[i+1];
			GenGen_MC_pp_mix[i]=(TH2D*)GenGen_MC_pp_f->Get(stmp);
			stmp = "GenJet_GenTrack_hJetTrackSignalBackground"+cent_tag[0]+"_"+cent_tag[1]+"_Pt100_Pt1000_"
				+trk_tag[i]+"_"+trk_tag[i+1];
			cout<<stmp<<endl;
			GenGen_MC_pp_sig[i]=(TH2D*)GenGen_MC_pp_f_calo3jet->Get(stmp);
			GenGen_MC_pp_sig[i]->Scale(1.0/GenJet_pp->Integral()/trkBinWidth[i]);
		}
	}

	void getParticleYield_MC_GenGen_pb(TString name){
		const double xdrbins[16] = {0,0.05,0.1,0.15,0.2,0.25,0.3,0.35,0.4,0.45,0.5,0.6,0.7,0.8,1., 1.2};
		getGenGen_pb();
		getGenGen_pp();
		TFile *wf = TFile::Open(name, "recreate");
		TH1D* py_dr[9][5];
		TH2D* sig;
		jtShape* jtT = new jtShape();
		for(int i=0;i<9; ++i){
			for(int j=0; j<4; ++j){
				py_dr[i][j]= new TH1D(Form("particleYield_pb_%d_%d",i, j), "",15,xdrbins);
				//GenGen_MC_pb_sig[0][0]->Draw("colz");
			//	sig =jtT->getSignal((TH2D*)GenGen_MC_pb_sig[i][j],\
			//			(TH2D*) GenGen_MC_pb_mix[i][j], (TH1D*)GenJet_pb[j], 1.5, 2.5);	
			//	jtT->drJetShape(sig,py_dr[i][j]);
				sig =jtT->getSignal((TH2D*)GenGen_MC_pb_sig[i][j],\
						(TH2D*) GenGen_MC_pb_mix[i][j],1, 1.5, 2.5);	
				jtT->drJetShape(sig,py_dr[i][j]);
				py_dr[i][j]->Write();
			}
			//py_dr[i][4]= new TH1D(Form("particleYield_pp_%d",i), "",15,xdrbins);
			////sig =jtT->getSignal((TH2D*)GenGen_MC_pp_sig[i],\
			////		(TH2D*) GenGen_MC_pp_mix[i], (TH1D*)GenJet_pp, 1.5, 2.5);	
			//sig =jtT->getSignal((TH2D*)GenGen_MC_pp_sig[i],\
			//		(TH2D*) GenGen_MC_pp_mix[i], 1, 1.5, 2.5);	
			//jtT->drJetShape(sig,py_dr[i][4]);
			//py_dr[i][4]->Write();
		}
	}
}

namespace hallie_reg{
	TString trk_tag[] = {"TrkPt0p7", "TrkPt1", "TrkPt2","TrkPt3","TrkPt4","TrkPt8","TrkPt12","TrkPt16","TrkPt20","TrkPt999"};
	TString cent_tag[]= {"Cent0","Cent10","Cent30","Cent50","Cent70","Cent100"};
}
