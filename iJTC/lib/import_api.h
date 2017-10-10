

namespace kurt_reg{

	TFile *pp_data_f;
	void loadConfig(){
		std::cout<<"loading kurt's configuration.."<<std::endl;
		//pp input 
		pp_data_f =TFile::Open("/Users/tabris/cmsProjects/iJTC/dataSet/Data_pp_5TeV_JTC_40meptrig.root");	
	}

	TString trk_tag[] = {"TrkPt0p7", "TrkPt1", "TrkPt2","TrkPt3","TrkPt4","TrkPt8","TrkPt12","TrkPt16","TrkPt20","TrkPt999"};
	TString cent_tag[]= {"Cent0","Cent10","Cent30","Cent50","Cent100"};
	TString jetPt_tag[]= {"Pt100","Pt150","Pt200","Pt1000"};

	TH1F *data_pp_jet[4][3];
	// the jet data_pp_XX [sig/mix][trkpt][centbin][jetpt];
	TH2D *data_pp_py[2][9][4][3];
	TH2D *data_pp_js[2][9][4][3];
	void get_pp_data(){
		float trkBinWidth[] = {0.3, 1, 1,1, 4, 4, 4, 4, 50}; 
		TString stmp;
		for(int j=0; j<4; ++j){
			for(int k=0; k<3; ++k){
				stmp = "Data_all_jets_corrpT"+cent_tag[j]+"_"+cent_tag[j+1]
					+"_"+jetPt_tag[k]+"_"+jetPt_tag[k+1];
				data_pp_jet[j][k]= (TH1F*)pp_data_f->Get(stmp);
				for(int i=0;i<9; ++i){
					stmp = "Data_hJetTrackME"+cent_tag[j]+"_"+cent_tag[j+1]
						+"_"+jetPt_tag[k]+"_"+jetPt_tag[k+1]+"_"+
						+trk_tag[i]+"_"+trk_tag[i+1];
					data_pp_py[1][i][j][k]=(TH2D*)pp_data_f->Get(stmp);
					stmp = "Data_hJetTrackSignalBackground"+cent_tag[j]+"_"+cent_tag[j+1]
						+"_"+jetPt_tag[k]+"_"+jetPt_tag[k+1]+"_"+
						+trk_tag[i]+"_"+trk_tag[i+1];
					data_pp_py[0][i][j][k]=(TH2D*)pp_data_f->Get(stmp);
//				cout<<stmp<<endl;
					data_pp_py[0][i][j][k]->Scale(1./trkBinWidth[i]/data_pp_jet[j][k]->Integral());

					stmp = "Data_hJetTrackME_pTweighted"+cent_tag[j]+"_"+cent_tag[j+1]
						+"_"+jetPt_tag[k]+"_"+jetPt_tag[k+1]+"_"+
						+trk_tag[i]+"_"+trk_tag[i+1];
					data_pp_js[1][i][j][k]=(TH2D*)pp_data_f->Get(stmp);
					stmp = "Data_hJetTrackSignalBackground_pTweighted"+cent_tag[j]+"_"+cent_tag[j+1]
						+"_"+jetPt_tag[k]+"_"+jetPt_tag[k+1]+"_"+
						+trk_tag[i]+"_"+trk_tag[i+1];
					data_pp_js[0][i][j][k]=(TH2D*)pp_data_f->Get(stmp);
					data_pp_js[0][i][j][k]->Scale(1./trkBinWidth[i]/data_pp_jet[j][k]->Integral());
				}
			}
		}
	}
}

