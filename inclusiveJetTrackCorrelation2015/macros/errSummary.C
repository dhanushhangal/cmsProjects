
#include "../lib/import_config.h"
#include "../lib/JTCSystError.h"
#include "../lib/systErrorTable.h"
#include "../lib/read_residual_corr.h"
using namespace hallie_reg;

void errSummary(){
		TString trk_tag[] = {"0.7 < p_{T}^{track} GeV < 1 ", "1 < p_{T}^{track} GeV < 2", 
				"2 < p_{T}^{track} GeV < 3", "3 < p_{T}^{track} GeV < 4", 
				"4 < p_{T}^{track} GeV < 8", 
				"8 < p_{T}^{track} GeV < 12", "12 < p_{T}^{track} GeV < 16","16 < p_{T}^{track} GeV < 20", 
				"p_{T}^{track} GeV > 20"};

		py_proj_f = TFile::Open("/Users/tabris/Research/HIN_5.02TeV/JetTrack2016/particle_yields/Particle_Yields.root");	
		getAllErr_proj(8);
		getPYErr_dr();
		TLatex tx;
		tx.SetTextSize(.1);
		readResidualCorr();
		JTCSystError* pb_deta_err[9][4];
		JTCSystError* pb_dphi_err[9][4];
		JTCSystError* pb_dr_err[9][4];
		JTCSystError* pp_dphi_err[9];
		JTCSystError* pp_deta_err[9];
		JTCSystError* pp_dr_err[9];
		auto ceta = new JTCSystCanvas("ceta", "", 8, 5);
		auto cphi = new JTCSystCanvas("cphi", "", 8, 5);
		auto cpb_dr = new JTCSystCanvas("cpb_dr", "", 8, 5);
		for(int i=0; i<8; ++i){
				//dr 
				cout<<endl;
				cout<<py_dr_syst_err[4][i][4]->GetName()<<endl;
				pp_dr_err[i]=new JTCSystError();
				pp_dr_err[i]->bkg_err=pp_err[i];		
				pp_dr_err[i]->jff_err= jff_err;		
				pp_dr_err[i]->spill_err= 1;	
				pp_dr_err[i]->rela_err= rela_err_pp;	
				pp_dr_err[i]->getAllError(py_dr_syst_err[0][i][4], py_dr_syst_err[4][i][4], NULL, 1);
				pp_dr_err[i]->error->GetXaxis()->SetTitle("dr");
				pp_dr_err[i]->error->GetXaxis()->SetTitleSize(0.1);
				pp_dr_err[i]->error->GetXaxis()->CenterTitle();
				cpb_dr->addError(pp_dr_err[i],i+1, 1);

				pp_dphi_err[i]=new JTCSystError();
				pp_dphi_err[i]->bkg_err=pp_err[i];		
				pp_dphi_err[i]->jff_err= jff_err;		
				pp_dphi_err[i]->spill_err= 1;	
				pp_dphi_err[i]->rela_err= rela_err_pp;	
				pp_dphi_err[i]->getAllError(dphi_syst_err[0][i][3],pp_dphi_jff[i]);
				pp_deta_err[i]=new JTCSystError();
				pp_deta_err[i]->bkg_err=pp_err[i];		
				pp_deta_err[i]->jff_err= jff_err;		
				pp_deta_err[i]->spill_err= 1;	
				pp_deta_err[i]->rela_err= rela_err_pp;	
				pp_deta_err[i]->getAllError(deta_syst_err[0][i][3],pp_deta_jff[i]);
				cphi->addError(pp_dphi_err[i],i+1, 1);
				ceta->addError(pp_deta_err[i],i+1, 1);
				for(int j=0; j<4; ++j){
						pb_dr_err[i][j]=new JTCSystError();
						pb_dr_err[i][j]->bkg_err=pb_err[i][j];		
						pb_dr_err[i][j]->jff_err= jff_err;		
						pb_dr_err[i][j]->spill_err= spill_err[i];	
						pb_dr_err[i][j]->rela_err= rela_err_pb;	
						pb_dr_err[i][j]->getAllError(py_dr_syst_err[0][i][j],py_dr_syst_err[4][i][j], py_dr_syst_err[3][i][j], 1);
						pb_dr_err[i][j]->error->GetXaxis()->SetTitle("dr");
						pb_dr_err[i][j]->error->GetXaxis()->SetTitleSize(0.1);
						pb_dr_err[i][j]->error->GetXaxis()->CenterTitle();

						cpb_dr->addError(pb_dr_err[i][j],i+1, 5-j);

						pb_deta_err[i][j]=new JTCSystError();
						pb_deta_err[i][j]->bkg_err=pb_err[i][j];		
						pb_deta_err[i][j]->jff_err= jff_err;		
						pb_deta_err[i][j]->spill_err= spill_err[i];	
						pb_deta_err[i][j]->rela_err= rela_err_pb;	
						pb_deta_err[i][j]->getAllError(deta_syst_err[0][i][j],pb_deta_jff[i][j], pb_deta_spill[i][j]);
						pb_deta_err[i][j]->error->GetXaxis()->SetTitle("d#eta");
						pb_deta_err[i][j]->error->GetXaxis()->CenterTitle();
						pb_deta_err[i][j]->error->GetXaxis()->SetTitleSize(0.1);

						pb_dphi_err[i][j]=new JTCSystError();
						pb_dphi_err[i][j]->bkg_err=pb_err[i][j];		
						pb_dphi_err[i][j]->jff_err= jff_err;		
						pb_dphi_err[i][j]->spill_err= spill_err[i];	
						pb_dphi_err[i][j]->rela_err= rela_err_pb;	
						pb_dphi_err[i][j]->getAllError(dphi_syst_err[0][i][j],pb_dphi_jff[i][j], pb_dphi_spill[i][j]);
						ceta->addError(pb_deta_err[i][j],i+1, 5-j);
						cphi->addError(pb_dphi_err[i][j],i+1, 5-j);
				}
		}
		ceta->drawSummary(-2.5, 2.49);
		cphi->drawSummary(-1.5, 1.49);
		cpb_dr->drawSummary(0, 0.99);
		TString cent_tag[] = {"PbPb(0-10%)", "PbPb(10-30%)", "PbPb(30-50%)", "PbPb(50-100%)", "pp reference"};
		for(int i=0; i<8; ++i){
				if(i==7 ) tx.SetTextSize(0.08);
				else tx.SetTextSize(0.1);
				ceta->cd(1+5*i);
				tx.DrawLatexNDC(0.4, 0.78, trk_tag[i]);
				cphi->cd(1+5*i);
				tx.DrawLatexNDC(0.4, 0.78, trk_tag[i]);
				cpb_dr->cd(i*5+1);
				tx.DrawLatexNDC(0.4, 0.78, trk_tag[i]);
				if(i==0 ){
						for(int j=0;j<5; j++){
								cpb_dr->cd(5-j);
								tx.DrawLatexNDC(0.4,0.9, cent_tag[j]);
						}
				}
		}
		ceta->SaveAs("py_deta_err_summary.pdf");
		cphi->SaveAs("py_dphi_err_summary.pdf");
		cpb_dr->SaveAs("py_dr_err_summary.pdf");
		TFile *wf = TFile::Open("/Users/tabris/cmsProjects/inclusiveJetTrackCorrelation2015/dataSet/yield_proj_syst_err.root","recreate");
		//TFile *wf = TFile::Open("/Users/tabris/cmsProjects/inclusiveJetTrackCorrelation2015/dataSet/yield_proj_syst_err_old.root","recreate");
		for(int i=0; i<8; ++i){
				for(int j=0; j<4; ++j){
						pb_deta_err[i][j]->error->SetName(Form("Yield_dEta_Pb_Syst_Error_%d_%d",i,j));
						pb_dphi_err[i][j]->error->SetName(Form("Yield_dPhi_Pb_Syst_Error_%d_%d",i,j));
						pb_deta_err[i][j]->error->Write();
						pb_dphi_err[i][j]->error->Write();
						pb_dr_err[i][j]->error->SetName(Form("Yield_dr_Pb_Syst_Error_%d_%d",i,j));
						pb_dr_err[i][j]->error->Write();
				}
				pp_dphi_err[i]->error->SetName(Form("Yield_dPhi_pp_Syst_Error_%d",i));
				pp_dphi_err[i]->error->Write();
				pp_deta_err[i]->error->SetName(Form("Yield_dEta_pp_Syst_Error_%d",i));
				pp_deta_err[i]->error->Write();
				pp_dr_err[i]->error->SetName(Form("Yield_dr_pp_Syst_Error_%d",i));
				pp_dr_err[i]->error->Write();
		}


		//add reference 
		py_f  = TFile::Open("/Users/tabris/cmsProjects/inclusiveJetTrackCorrelation2015/dataSet/Jet_Shapes_v3.root");
		getPY_dr();
		for(int i=0; i<8; ++i){
				for(int j=0; j<5; ++j){
						for(int k=1; k<py_dr_err[i][j]->GetNbinsX()+1; ++k){
								py_dr_err[i][j]->SetBinContent(k, py_dr_err[i][j]->GetBinError(k));
								py_dr_err[i][j]->SetBinError(k, 0);
						}
						cpb_dr->cd(i*5+5-j);
						py_dr_err[i][j]->SetLineColor(kRed-6);
						py_dr_err[i][j]->SetLineStyle(2);
						py_dr_err[i][j]->SetLineWidth(2);
						py_dr_err[i][j]->Draw("same");
				}
		}
		TLegend * l1  = new TLegend(0.4, 0.2, 0.9, 0.7);
		l1->SetFillColor(0);
		l1->SetLineColor(0);
		TLegend * l2  = new TLegend(0.4, 0.3, 0.9, 0.8);
		l2->SetFillColor(0);
		l2->SetLineColor(0);
		l1->AddEntry(cpb_dr->getErr(1,1)->error, "New Syst. Error");
		l1->AddEntry(py_dr_err[1][1], "Old Syst. Error");
		l2->AddEntry(cpb_dr->getErr(1,1)->sig, "Rela. Error");
		l2->AddEntry(cpb_dr->getErr(1,1)->bkg, "ME+BG");
		l2->AddEntry(cpb_dr->getErr(1,1)->jff, "Jff residual");
		l2->AddEntry(cpb_dr->getErr(1,1)->spill, "Spill-over");
		cpb_dr->cd(1);
		l1->Draw();
		cpb_dr->cd(2);
		l2->Draw();
		cpb_dr->SaveAs("py_dr_err_summary_overlay.pdf");
}
