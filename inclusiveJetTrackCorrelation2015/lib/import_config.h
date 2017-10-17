
#ifndef jtShape_H
#include "jtShape.h"
#endif

TH2D* pb_sig[9][5];
TH2D* pb_me [9][5];
TH1D* jet_pt[5];
namespace kurt_reg{
	TFile *GenGen_MC_pp_f ;
	TFile *RecRec_MC_pp_f ;
	TFile *GenGen_MC_pp_f_calo5jet ;
	TFile *GenGen_MC_pp_f_calo3jet ;
	TFile *GenGen_MC_pb_f ;
	TFile *RecRec_MC_pb_f ;
	TFile *GenGen_MC_pb_f_sub0 ;
	TFile *data_pb_f ;
	void loadConfig(){
		std::cout<<"loading kurt's configuration.."<<std::endl;
		//pp input 
		data_pb_f =TFile::Open("/Users/tabris/cmsProjects/inclusiveJetTrackCorrelation2015/dataSet/correlation/PbPb_5TeVData_fixMix_fineBinTrkCorrs_withTrkCorrEtaSymmV2_finalJFFs.root");	
		//data_pb_f =TFile::Open("/Users/tabris/cmsProjects/inclusiveJetTrackCorrelation2015/dataSet/correlation/PbPb_5TeVData_fineBinTrkCorrs_withMix_finalJFFs.root");	
		//data_pb_f =TFile::Open("/Users/tabris/cmsProjects/inclusiveJetTrackCorrelation2015/dataSet/correlation/PbPb_Data_withMix_CymbalTune_fullCymbalCorrs_inclJetBinning_withPtWeightME.root");	
		GenGen_MC_pp_f_calo5jet =TFile::Open("/Users/tabris/cmsProjects/inclusiveJetTrackCorrelation2015/dataSet/correlation/pp_5TeVMC_GenGen_PFJets_R0p5_finalJFF_noMix.root");	
		GenGen_MC_pp_f_calo3jet =TFile::Open("/Users/tabris/cmsProjects/inclusiveJetTrackCorrelation2015/dataSet/correlation/pp_5TeVMC_GenGen_PFJets_R0p3_finalJFF_noMix.root");	
		GenGen_MC_pp_f =TFile::Open("/Users/tabris/cmsProjects/inclusiveJetTrackCorrelation2015/dataSet/correlation/pp_Pythia6MC_GenGen_withMix_inclJetBinning_finalJFFs.root");	
		RecRec_MC_pp_f =TFile::Open("/Users/tabris/cmsProjects/inclusiveJetTrackCorrelation2015/dataSet/correlation/pp_Pythia6MC_RecoReco_withMix_inclJetBinning_finalJFFs.root");	
		// pb mc input 
		GenGen_MC_pb_f =TFile::Open("/Users/tabris/cmsProjects/inclusiveJetTrackCorrelation2015/dataSet/correlation/PbPb_5TeVMC_withMix_GenGen_CymbalTune_fullCymbalCorrs_inclJetBinning.root");	
		RecRec_MC_pb_f =TFile::Open("/Users/tabris/cmsProjects/inclusiveJetTrackCorrelation2015/dataSet/correlation/PbPb_5TeVMC_withMix_RecoReco_CymbalTune_fullCymbalCorrs_inclJetBinning.root");	
		//GenGen_MC_pb_f_sub0 =TFile::Open("/Users/tabris/cmsProjects/inclusiveJetTrackCorrelation2015/dataSet/correlation/PbPb_5TeVMC_GenGenSube0_fineBinTrkCorrs_withTrkCorrEtaSymmV2_noJFF_noMix.root");	
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
			//cout<<stmp<<endl;
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

	void getPbData(){
		TString stmp;
		float trkBinWidth[] = {0.3, 1, 1,1, 4, 4, 4, 4, 50}; 
		for(int j=0; j<5; ++j){
			stmp = "Data_all_jets_corrpT"+cent_tag[j]+"_"+cent_tag[j+1]+"_Pt100_Pt1000";
			jet_pt[j]= (TH1D*)data_pb_f->Get(stmp);
			for(int i=0;i<9; ++i){
				stmp = "Data_hJetTrackME"+cent_tag[j]+"_"+cent_tag[j+1]+"_Pt100_Pt1000_"
					+trk_tag[i]+"_"+trk_tag[i+1];
				pb_me[i][j]=(TH2D*)data_pb_f->Get(stmp);
				stmp = "Data_hJetTrackSignalBackground"+cent_tag[j]+"_"+cent_tag[j+1]+"_Pt100_Pt1000_"
					+trk_tag[i]+"_"+trk_tag[i+1];
				pb_sig[i][j]=(TH2D*)data_pb_f->Get(stmp);
				pb_sig[i][j]->Scale(1./trkBinWidth[i]);
			}
		}

		jet_pt[3]->Add(jet_pt[4]);
		for(int i=0;i<9; ++i){
			//combined the last two bins together
			pb_sig[i][3]->Add(pb_sig[i][4]);
			for(int j=0; j<4; ++j){
				//adding the cent50-70 and cent70-100 together:
				pb_sig[i][j]->Scale(1.0/jet_pt[j]->Integral());
			}
		}
	}

}

namespace hallie_reg{
	TFile * py_f = new TFile("/Users/tabris/cmsProjects/inclusiveJetTrackCorrelation2015/dataSet/Jet_Shapes_geoCorr.root");
	TFile * py_proj_f = new TFile("/Users/tabris/cmsProjects/inclusiveJetTrackCorrelation2015/dataSet/Particle_Yields.root");
	TFile * js_f = new TFile("/Users/tabris/Research/HIN_5.02TeV/JetTrack2016/jet_shapes_result/Jet_Shapes_pTweighted_FineDr.root");
	TString trk_tag[] = {"TrkPt07", "TrkPt1", "TrkPt2","TrkPt3","TrkPt4","TrkPt8","TrkPt12","TrkPt16","TrkPt20","TrkPt300"};
	TString cent_tag[]= {"Cent0","Cent10","Cent30","Cent50","Cent100"};

	TH1D* py_dr[9][5];
	TH1D* py_dr_err[9][5];
	TH1D* py_dr_all[5];
	TH1D* py_dr_err_all[5];
	TH1D* integral[5];
	TH1D* integral_syst[5];
	TH1D* diff[4];
	TH1D* diff_syst[4];
	TString tmp;

	TH1D* js_dr[9][5];
	TH1D* js_dr_err[9][5];
	TH1D* js_dr_all[5];
	TH1D* js_dr_err_all[5];

	TH1D* py_deta    [8][5];
	TH1D* py_dphi    [8][5];
	TH1D* py_deta_err[8][5];
	TH1D* py_dphi_err[8][5];

	TH1D* py_deta_err_all[5];
	TH1D* py_dphi_err_all[5];

	void getPY_proj(){
		for(int i=0;i<8; ++i){
			tmp = "Proj_dEta_pp_"+trk_tag[i]+"_"+trk_tag[i+1]+"_Rebin";
			py_deta[i][4]=(TH1D*)py_proj_f->Get(tmp);
			tmp = "Proj_dPhi_pp_"+trk_tag[i]+"_"+trk_tag[i+1]+"_Rebin";
			py_dphi[i][4]=(TH1D*)py_proj_f->Get(tmp);
			tmp = "dEta_Syst_pp_Cent0_Cent10_"+trk_tag[i]+"_"+trk_tag[i+1];
			py_deta_err[i][4]=(TH1D*)py_proj_f->Get(tmp);
			tmp = "dPhi_Syst_pp_Cent0_Cent10_"+trk_tag[i]+"_"+trk_tag[i+1];
			//cout<<tmp<<endl;
			py_dphi_err[i][4]=(TH1D*)py_proj_f->Get(tmp);
			for(int j=0; j<4; ++j){
				tmp = "Proj_dEta_PbPb_"+cent_tag[j]+"_"+cent_tag[j+1]+"_"+trk_tag[i]+"_"+trk_tag[i+1]+"_Rebin";
				py_deta[i][j]=(TH1D*)py_proj_f->Get(tmp);
				tmp = "Proj_dPhi_PbPb_"+cent_tag[j]+"_"+cent_tag[j+1]+"_"+trk_tag[i]+"_"+trk_tag[i+1]+"_Rebin";
				py_dphi[i][j]=(TH1D*)py_proj_f->Get(tmp);
				tmp = "dEta_Syst_PbPb_"+cent_tag[j]+"_"+cent_tag[j+1]+"_"+trk_tag[i]+"_"+trk_tag[i+1];
				py_deta_err[i][j]=(TH1D*)py_proj_f->Get(tmp);
				tmp = "dPhi_Syst_PbPb_"+cent_tag[j]+"_"+cent_tag[j+1]+"_"+trk_tag[i]+"_"+trk_tag[i+1];
				py_dphi_err[i][j]=(TH1D*)py_proj_f->Get(tmp);
			}
		}
	}

	void getJS(){
		for(int i=0;i<9; ++i){
			tmp = "JetShape2_Yield_BkgSub_pTweightedInclusive_pp_"+
				trk_tag[i]+"_"+trk_tag[i+1];
			js_dr[i][4]=(TH1D*)js_f->Get(tmp);
			tmp = "Jet_Shape_SystErr_pp_"+
				trk_tag[i]+"_"+trk_tag[i+1];
			js_dr_err[i][4]=(TH1D*)js_f->Get(tmp);
			for(int j=0;j<4; ++j){
				tmp = "JetShape2_Yield_BkgSub_pTweightedInclusive_"+cent_tag[j]+"_"+cent_tag[j+1]+"_"+
					trk_tag[i]+"_"+trk_tag[i+1];
				js_dr[i][j]=(TH1D*)js_f->Get(tmp);
				tmp = "Jet_Shape_SystErr_"+cent_tag[j]+"_"+cent_tag[j+1]+"_"+
					trk_tag[i]+"_"+trk_tag[i+1];
				js_dr_err[i][j]=(TH1D*)js_f->Get(tmp);
			}
		}
		for(int i=0;i<4; ++i){
			tmp = "JetShape2_Yield_BkgSub_pTweightedInclusive_"+cent_tag[i]+"_"+cent_tag[i+1]+"_"+
				trk_tag[9]+"_";
			js_dr_all[i]=(TH1D*)js_f->Get(tmp);
			tmp = "Jet_Shape_SystErr_"+cent_tag[i]+"_"+cent_tag[i+1]+"_"+
				trk_tag[9]+"_";
			js_dr_err_all[i]=(TH1D*)js_f->Get(tmp);
		//	tmp = "Integral_PbPb"+cent_tag[i]+"_"+cent_tag[i+1];
		//	integral[i]=(TH1D*)py_f->Get(tmp);
		//	tmp = "Integral_PbPb_Syst"+cent_tag[i]+"_"+cent_tag[i+1];
		//	integral_syst[i]=(TH1D*)py_f->Get(tmp);
		//	tmp = "Integral_Diff_"+cent_tag[i]+"_"+cent_tag[i+1];
		//	diff[i]=(TH1D*)py_f->Get(tmp);
		//	tmp = "Integral_Diff_Syst"+cent_tag[i]+"_"+cent_tag[i+1];
		//	diff_syst[i]=(TH1D*)py_f->Get(tmp);
		}
		//integral[4]=(TH1D*)py_f->Get("Integral_pp");
		//integral_syst[4]=(TH1D*)py_f->Get("Integral_pp_Syst");
		tmp = "JetShape2_Yield_BkgSub_pTweightedInclusive_pp_"+
			trk_tag[9]+"_";
		js_dr_all[4]=(TH1D*)js_f->Get(tmp);
		tmp = "Jet_Shape_SystErr_pp_"+
			trk_tag[9]+"_";
		js_dr_err_all[4]=(TH1D*)js_f->Get(tmp);
	}

	void getPY(){
		for(int i=0;i<9; ++i){
			tmp = "JetShape2_Yield_BkgSub_Inclusive_pp_"+
				trk_tag[i]+"_"+trk_tag[i+1];
			py_dr[i][4]=(TH1D*)py_f->Get(tmp);
			tmp = "Jet_Shape_SystErr_pp_"+
				trk_tag[i]+"_"+trk_tag[i+1];
			py_dr_err[i][4]=(TH1D*)py_f->Get(tmp);
			for(int j=0;j<4; ++j){
				tmp = "JetShape2_Yield_BkgSub_Inclusive_"+cent_tag[j]+"_"+cent_tag[j+1]+"_"+
					trk_tag[i]+"_"+trk_tag[i+1];
				py_dr[i][j]=(TH1D*)py_f->Get(tmp);
				tmp = "Jet_Shape_SystErr_"+cent_tag[j]+"_"+cent_tag[j+1]+"_"+
					trk_tag[i]+"_"+trk_tag[i+1];
				py_dr_err[i][j]=(TH1D*)py_f->Get(tmp);
			}
		}
		for(int i=0;i<4; ++i){
			tmp = "JetShape2_Yield_BkgSub_Inclusive_"+cent_tag[i]+"_"+cent_tag[i+1]+"_"+
				trk_tag[9]+"_";
			py_dr_all[i]=(TH1D*)py_f->Get(tmp);
			tmp = "Jet_Shape_SystErr_"+cent_tag[i]+"_"+cent_tag[i+1]+"_"+
				trk_tag[9]+"_";
			py_dr_err_all[i]=(TH1D*)py_f->Get(tmp);
			tmp = "Integral_PbPb"+cent_tag[i]+"_"+cent_tag[i+1];
			integral[i]=(TH1D*)py_f->Get(tmp);
			tmp = "Integral_PbPb_Syst"+cent_tag[i]+"_"+cent_tag[i+1];
			integral_syst[i]=(TH1D*)py_f->Get(tmp);
			tmp = "Integral_Diff_"+cent_tag[i]+"_"+cent_tag[i+1];
			diff[i]=(TH1D*)py_f->Get(tmp);
			tmp = "Integral_Diff_Syst"+cent_tag[i]+"_"+cent_tag[i+1];
			diff_syst[i]=(TH1D*)py_f->Get(tmp);
		}
		integral[4]=(TH1D*)py_f->Get("Integral_pp");
		integral_syst[4]=(TH1D*)py_f->Get("Integral_pp_Syst");
		tmp = "JetShape2_Yield_BkgSub_Inclusive_pp_"+
			trk_tag[9]+"_";
		py_dr_all[4]=(TH1D*)py_f->Get(tmp);
		tmp = "Jet_Shape_SystErr_pp_"+
			trk_tag[9]+"_";
		py_dr_err_all[4]=(TH1D*)py_f->Get(tmp);
	}

	TFile *bkgSub_f = TFile::Open("/Users/tabris/cmsProjects/inclusiveJetTrackCorrelation2015/dataSet/correlation/hallie/PbPb_Inclusive_Correlations.root");
	TH2D* py[9][4];
	TH2D* raw2D[9][4];
	TH2D* mix[9][4];
	TH2D* bkg[9][4];
	TH2D* py_nobkgSub[9][4];
	void get2DHist(){
		for(int i=0;i<4; ++i){
			for(int j=0;j<9; ++j){
				tmp = "Yield_BkgSub_"+cent_tag[i]+"_"+cent_tag[i+1]+"_Pt100_Pt1000_"+
					trk_tag[j]+"_"+trk_tag[j+1];
				//				cout<<tmp<<endl;
				py[j][i]=(TH2D*)bkgSub_f->Get(tmp);
				tmp = "Raw_Yield_"+cent_tag[i]+"_"+cent_tag[i+1]+"_Pt100_Pt1000_"+
					trk_tag[j]+"_"+trk_tag[j+1];
				raw2D[j][i]=(TH2D*)bkgSub_f->Get(tmp);
				tmp = "Mixed_Event_"+cent_tag[i]+"_"+cent_tag[i+1]+"_Pt100_Pt1000_"+
					trk_tag[j]+"_"+trk_tag[j+1];
				mix[j][i]=(TH2D*)bkgSub_f->Get(tmp);
				tmp = "SummedBkg_"+cent_tag[i]+"_"+cent_tag[i+1]+"_Pt100_Pt1000_"+
					trk_tag[j]+"_"+trk_tag[j+1];
				bkg[j][i]=(TH2D*)bkgSub_f->Get(tmp);
				tmp = "Yield_PbPb_"+cent_tag[i]+"_"+cent_tag[i+1]+"_Pt100_Pt1000_"+
					trk_tag[j]+"_"+trk_tag[j+1];
				py_nobkgSub[j][i]=(TH2D*)bkgSub_f->Get(tmp);
			}
		}
	}
}
