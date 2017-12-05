
#include "../lib/JTCSignalProducer.h"
#include "../lib/import_config.h"

void pullSignal (){
using namespace input_raw2D;
		const int nPt = 7; const int nCent =2 ;

		TString trkbin [] = {"1", "2", "3", "4", "8", "12", "16", "999"};
		TString centbin [] = {"0", "30", "100"};
		get2DInput_RecGen();
		JTCSignalProducer *sp1[7][2];
		TFile * wf = new TFile("/Users/tabris/cmsProjects/iJTC/dataSet/correlation/recgen_JTCSignal.root","recreate");
		TString title;
		for(int i=0; i<nPt ; ++i){
				for(int j=0; j<nCent ; ++j){
						title = "track pt in ["+trkbin[i]+", "+trkbin[i+1]\
								+"), cent in ["+centbin[j]+", "+centbin[j+1]+"), jet pt in [120, 1000]";
						raw_sig[i][j]->SetTitle(title);
						mixing [i][j]->SetTitle(title);
						sp1[i][j] = new JTCSignalProducer(raw_sig[i][j], mixing[i][j]);
						sp1[i][j]->getSignal(Form("rec_gen_%d_%d", i, j));
						sp1[i][j]->WriteTH2();
				}
		}
		wf->Close();
		clear();

		get2DInput_RecRec();
		JTCSignalProducer *sp2[7][2];
		wf = new TFile("/Users/tabris/cmsProjects/iJTC/dataSet/correlation/recrec_JTCSignal.root","recreate");
		for(int i=0; i<nPt ; ++i){
				for(int j=0; j<nCent ; ++j){
						title = "track pt in ["+trkbin[i]+", "+trkbin[i+1]\
								+"), cent in ["+centbin[j]+", "+centbin[j+1]+"), jet pt in [120, 1000]";
						raw_sig[i][j]->SetTitle(title);
						mixing [i][j]->SetTitle(title);
						sp2[i][j] = new JTCSignalProducer(raw_sig[i][j], mixing[i][j]);
						sp2[i][j]->getSignal(Form("rec_rec_%d_%d", i, j));
						sp2[i][j]->WriteTH2();
				}
		}
		wf->Close();
		clear();

		get2DInput_GenGen();
		JTCSignalProducer *sp3[7][2];
		wf = new TFile("/Users/tabris/cmsProjects/iJTC/dataSet/correlation/gengen_JTCSignal.root","recreate");
		for(int i=0; i<nPt ; ++i){
				for(int j=0; j<nCent ; ++j){
						title = "track pt in ["+trkbin[i]+", "+trkbin[i+1]\
								+"), cent in ["+centbin[j]+", "+centbin[j+1]+"), jet pt in [120, 1000]";
						raw_sig[i][j]->SetTitle(title);
						mixing [i][j]->SetTitle(title);
						sp3[i][j] = new JTCSignalProducer(raw_sig[i][j], mixing[i][j]);
						sp3[i][j]->getSignal(Form("gen_gen_%d_%d", i, j));
						sp3[i][j]->WriteTH2();
				}
		}
		wf->Close();
		clear();

		get2DInput_GenRec();
		JTCSignalProducer *sp4[7][2];
		wf = new TFile("/Users/tabris/cmsProjects/iJTC/dataSet/correlation/genrec_JTCSignal.root","recreate");
		for(int i=0; i<nPt ; ++i){
				for(int j=0; j<nCent ; ++j){
						title = "track pt in ["+trkbin[i]+", "+trkbin[i+1]\
								+"), cent in ["+centbin[j]+", "+centbin[j+1]+"), jet pt in [120, 1000]";
						raw_sig[i][j]->SetTitle(title);
						mixing [i][j]->SetTitle(title);
						sp4[i][j] = new JTCSignalProducer(raw_sig[i][j], mixing[i][j]);
						sp4[i][j]->getSignal(Form("gen_rec_%d_%d", i, j));
						sp4[i][j]->WriteTH2();
				}
		}
		wf->Close();
		clear();
/*
		*/
}
