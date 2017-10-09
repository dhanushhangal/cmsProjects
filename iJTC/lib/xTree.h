

#ifndef xTree_H
#define xTree_H

class xTree{
	public : 
		xTree(TString name,TFile *f, bool isMC, bool ispp);
		void tree_basic_build();
		void tree_MC_build();
		void tree_pp_build();
		void tree_pb_build();
		void GetEntry(Long64_t);
	public : 
		TTree* tree;
		bool isMC, ispp;

		vector<float> *trkEta=0, *trkPhi=0, *trkPt=0, *jteta=0, *jtphi=0, *jtpt=0, *rawpt=0, *corrpt=0;
		vector<UChar_t> *trkAlgo=0;
		vector<bool> *highPurity=0;
		vector<float> *trackMax=0, *trkDxy=0, *trkDxyError=0, *trkDz=0, *trkDzError=0, *trkPtError=0, *pfEcal=0, *pfHcal=0, *trkMVALoose=0, *trkMVATight=0, *trkNdof=0, *trkChi2=0;
		vector<int> *pfId=0, *trkNHit=0, *trkNlayer=0;
		Int_t hiBin = -999;
		Float_t pthat = -999;
		Float_t vz = -999;
		vector<int> *sube=0, *chg=0;
		vector<float> *gpt=0, *gphi=0, *geta=0, *pPt=0, *pPhi=0, *pEta=0, *geneta=0, *genphi=0, *genpt=0;
		Int_t HBHEFilter, collisionEventSelection, phfCoincFilter, pprimaryVertexFilter;
};

xTree::xTree(TString name,TFile *f, bool is_MC, bool is_pp){
	isMC=is_MC;
	ispp=is_pp;
	tree = (TTree*) f->Get(name);
	tree->SetBranchStatus("*",0);
	tree_basic_build();	
	if( isMC) tree_MC_build();
	if( ispp) tree_pp_build();
	else tree_pb_build();
}

void xTree::tree_basic_build(){
	tree->SetBranchStatus("trkPt", 1);
	tree->SetBranchStatus("trkEta",1);
	tree->SetBranchStatus("trkPhi",1);
	tree->SetBranchStatus("trkAlgo", 1);
	tree->SetBranchStatus("vz", 1);

	tree->SetBranchAddress("trkPt", &trkPt);
	tree->SetBranchAddress("trkEta", &trkEta);
	tree->SetBranchAddress("trkPhi", &trkPhi);
	tree->SetBranchAddress("trkAlgo", &trkAlgo);
	tree->SetBranchAddress("vz", &vz);

	if(!ispp) {
		tree->SetBranchStatus("hiBin",1);
		tree->SetBranchAddress("hiBin", &hiBin);
	}

	tree->SetBranchStatus("calo_jteta" , 1);
	tree->SetBranchStatus("calo_jtphi" , 1);
	tree->SetBranchStatus("calo_jtpt"  , 1);
	tree->SetBranchStatus("calo_rawpt" , 1);
	tree->SetBranchStatus("calo_corrpt", 1);

	tree->SetBranchAddress("calo_jteta", &jteta);
	tree->SetBranchAddress("calo_jtphi", &jtphi);
	tree->SetBranchAddress("calo_jtpt", &jtpt);
	tree->SetBranchAddress("calo_rawpt", &rawpt);
	tree->SetBranchAddress("calo_corrpt", &corrpt);

	tree->SetBranchStatus("calo_trackMax", 1);
	tree->SetBranchStatus("trkDxy", 1);
	tree->SetBranchStatus("trkDxyError", 1);
	tree->SetBranchStatus("trkDz", 1);
	tree->SetBranchStatus("trkDzError", 1);
	tree->SetBranchStatus("trkPtError", 1);
	tree->SetBranchStatus("trkNHit", 1);
	tree->SetBranchStatus("trkNlayer", 1);
	tree->SetBranchStatus("trkNdof",1);
	tree->SetBranchStatus("trkChi2",1);

	tree->SetBranchStatus("pfEcal",1);
	tree->SetBranchStatus("pfHcal",1);

	tree->SetBranchAddress("calo_trackMax", &trackMax);
	tree->SetBranchAddress("trkDxy", &trkDxy);
	tree->SetBranchAddress("trkDxyError", &trkDxyError);
	tree->SetBranchAddress("trkDz", &trkDz);
	tree->SetBranchAddress("trkDzError", &trkDzError);
	tree->SetBranchAddress("trkPtError", &trkPtError);
	tree->SetBranchAddress("trkNHit",&trkNHit);
	tree->SetBranchAddress("trkNlayer",&trkNlayer);
	tree->SetBranchAddress("trkNdof",&trkNdof);
	tree->SetBranchAddress("trkChi2",&trkChi2);

	tree->SetBranchAddress("pfEcal",&pfEcal);
	tree->SetBranchAddress("pfHcal",&pfHcal);
	tree->SetBranchStatus("HBHENoiseFilterResultRun2Loose",1);
	tree->SetBranchAddress("HBHENoiseFilterResultRun2Loose",&HBHEFilter);
}

void xTree::tree_MC_build(){
	tree->SetBranchAddress("pthat", &pthat);
	tree->SetBranchStatus("pthat", 1);

	tree->SetBranchStatus("pt"  , 1);
	tree->SetBranchStatus("phi" , 1);
	tree->SetBranchStatus("eta" , 1);
	tree->SetBranchStatus("chg" , 1);
	tree->SetBranchStatus("sube", 1);
	tree->SetBranchStatus("pPt" , 1);
	tree->SetBranchStatus("pPhi", 1);
	tree->SetBranchStatus("pEta", 1);
	tree->SetBranchStatus("geneta",1);
	tree->SetBranchStatus("genphi",1);
	tree->SetBranchStatus("genpt", 1);

	//gen particle axis:
	tree->SetBranchAddress("pt", &gpt);
	tree->SetBranchAddress("phi", &gphi);
	tree->SetBranchAddress("eta", &geta);
	tree->SetBranchAddress("chg", &chg);
	// embeding information
	tree->SetBranchAddress("sube", &sube);
	tree->SetBranchAddress("pPt", &pPt);
	tree->SetBranchAddress("pPhi", &pPhi);
	tree->SetBranchAddress("pEta", &pEta);
	//gen jet axis:
	tree->SetBranchAddress("geneta", &geneta);
	tree->SetBranchAddress("genphi", &genphi);
	tree->SetBranchAddress("genpt", &genpt);
}
void xTree::GetEntry(Long64_t jentry){
	tree->GetEntry(jentry);
}

void xTree::tree_pp_build(){
	tree->SetBranchStatus("pPAprimaryVertexFilter",1);
	tree->SetBranchAddress("pPAprimaryVertexFilter",&collisionEventSelection);
}

void xTree::tree_pb_build(){
	tree->SetBranchStatus("highPurity", 1);
	tree->SetBranchAddress("highPurity", &highPurity);
	tree->SetBranchStatus("pcollisionEventSelection",1);
	tree->SetBranchAddress("pcollisionEventSelection",&collisionEventSelection);
	tree->SetBranchStatus("phfCoincFilter3",1);
	tree->SetBranchAddress("phfCoincFilter3",&phfCoincFilter);
	tree->SetBranchStatus("pprimaryVertexFilter",1);
	tree->SetBranchAddress("pprimaryVertexFilter",&pprimaryVertexFilter);
}
#endif
