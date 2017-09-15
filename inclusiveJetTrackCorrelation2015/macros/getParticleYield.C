
#include "../lib/import_config.h"
#include "../lib/pParticleYield.h"
#include "../lib/signalFactory.h"
#include "../lib/signalFactory_pp.h"
using namespace kurt_reg;

void getParticleYield(){
	loadConfig();

	//	getParticleYield_MC_GenGen_pb("GenGen_pb_particleYield.root");
	//TFile *f = TFile::Open("GenGen_pb_particleYield.root");
	TH1D* py_dr[9][5];
	/* debug part
	*/
	float trkbin[] = {0.7, 1, 2, 3, 4, 8, 12, 16, 20, 999};
	float centbin[]= {0, 10, 30, 50, 100};
	getGenGen_pb();
	getGenGen_pp();
	TFile *wf = TFile::Open("GenGen_ParticleYield.root", "recreate");
	/*
	auto sf = new signalFactory();
	sf->signal->init_setup(9, trkbin, 4, centbin);
	sf->signal->hd4= new TH2D*[40];
	sf->mixing->init_setup(9, trkbin, 4, centbin);
	sf->mixing->hd4= new TH2D*[40];
	for(int i=0; i<9; ++i){
		for(int j=0; j<4; ++j){
			sf->signal->hd4[j*10+i]=(TH2D*)GenGen_MC_pb_sig[i][j];
			sf->mixing->hd4[j*10+i]=(TH2D*)GenGen_MC_pb_mix[i][j];
			TString tmp = sf->signal->ztitle.at(i)+sf->signal->wtitle.at(j);
			sf->signal->hd4[j*10+i]->SetTitle(tmp);
		}
	}
	sf->getSignal("GenGenParticleYield");
	*/
	//sf->Write();

	float ppcent[] = {0,100};
	auto sf_pp = new signalFactory_pp();
	sf_pp->signal->init_setup(9, trkbin, 1, ppcent);
	sf_pp->signal->hd4= new TH2D*[10];
	sf_pp->mixing->init_setup(9, trkbin, 1, ppcent);
	sf_pp->mixing->hd4= new TH2D*[10];
	for(int i=0; i<9; ++i){
		sf_pp->signal->hd4[i]=(TH2D*)GenGen_MC_pp_sig[i];
		sf_pp->mixing->hd4[i]=(TH2D*)GenGen_MC_pp_mix[i];
		TString tmp = sf_pp->signal->ztitle.at(i);
		sf_pp->signal->hd4[i]->SetTitle(tmp);
	}
	sf_pp->getSignal("GenGenParticleYield");


}
