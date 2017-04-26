
#ifndef HISTCOLLECTION_H
#define  HISTCOLLECTION_H

enum typl{ t_jtpt, t_eta, t_phi,t_dphi, t_eta_phi_correlation};

class histCollection {
	public: 
		std::vector<TH1F*> *th1_vec;
		std::vector<TH2D*> *th2_vec;
		std::map<TString, TH1F*> Th1f;
		std::map<TString, TH2D*> Th2d;
		TH1F* th1p;
		TH2D* th2p;
		TFile* wf;

		histCollection(){
			th1_vec = new std::vector<TH1F*>();
			th2_vec = new std::vector<TH2D*>();
		}
		void addHist(TString name, typl tp, TString title="");
		void saveAll(TString f_name);
};

#endif

void histCollection::addHist(TString name, typl tp, TString title){
	switch (tp){
		case t_jtpt:  th1p = new TH1F(name, title , 100, 0., 1000.);
			      th1p->Sumw2(); 
			      th1_vec->push_back(th1p);
			      Th1f[name]=th1_vec->back();
			      return;
		case t_eta: th1p = new TH1F(name, title , 100, -5., 5.);
			      th1p->Sumw2(); 
			      th1_vec->push_back(th1p);
			      Th1f[name]=th1_vec->back();
			      return;
		case t_phi: th1p = new TH1F(name, title ,72, -TMath::Pi(), TMath::Pi());
			      th1p->Sumw2(); 
			      th1_vec->push_back(th1p);
			      Th1f[name]=th1_vec->back();
			      return;
		case t_dphi:th1p = new TH1F(name, title ,100, -0.5*TMath::Pi(), 1.5*TMath::Pi());
			      th1p->Sumw2(); 
			      th1_vec->push_back(th1p);
			      Th1f[name]=th1_vec->back();
			      return;
		case t_eta_phi_correlation:
			      th2p = new TH2D(name, title, 100,-5,5,100,-0.5*TMath::Pi(),1.5*TMath::Pi());
			      th2p->Sumw2(); 
			      th2_vec->push_back(th2p);
			      Th2d[name]=th2_vec->back();
			      return;
		default : return;
	}
}

void histCollection::saveAll(TString f_name){
	wf = TFile::Open(f_name,"recreate");
	for(std::vector<TH1F*>::iterator it=th1_vec->begin(); it!=th1_vec->end();++it){
		(*it)->Write();
	}
	for(std::vector<TH2D*>::iterator it=th2_vec->begin(); it!=th2_vec->end();++it){
		(*it)->Write();
	}
}


