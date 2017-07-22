//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Fri Jul 21 19:23:01 2017 by ROOT version 6.08/02
// from TTree mixing_tree/
// found on file: Data2015_25.root
//////////////////////////////////////////////////////////

#ifndef inTree_h
#define inTree_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.
#include "vector"

class inTree {
	public :
		TTree          *fChain;   //!pointer to the analyzed TTree or TChain
		Int_t           fCurrent; //!current Tree number in a TChain

		// Fixed size dimensions of array or collections stored in the TTree if any.
		// Declaration of leaf types
		Int_t           HLT_ak4CaloJet80;
		Int_t           HLT_ak4CaloJet100;
		Int_t           HLT_ak4CaloJet80_Prescale;
		Int_t           HLT_ak4CaloJet100_Prescale;
		Int_t           HBHENoiseFilterResult;
		Int_t           HBHENoiseFilterResultRun1;
		Int_t           HBHENoiseFilterResultRun2Loose;
		Int_t           HBHENoiseFilterResultRun2Tight;
		Int_t           HBHEIsoNoiseFilterResult;
		Int_t           pcollisionEventSelection;
		Int_t           pprimaryVertexFilter;
		Int_t           pclusterCompatibilityFilter;
		Int_t           phfCoincFilter1;
		Int_t           phfCoincFilter2;
		Int_t           phfCoincFilter3;
		Int_t           phfCoincFilter4;
		Int_t           phfCoincFilter5;
		Float_t         vz;
		Float_t         pthat;
		Int_t           hiBin;
		vector<float>   *trkPt;
		vector<float>   *trkEta;
		vector<float>   *trkPhi;
		vector<bool>    *highPurity;
		vector<float>   *trkChi2;
		vector<int>     *trkNdof;
		vector<int>     *trkNHit;
		vector<int>     *trkNlayer;
		vector<float>   *pfEcal;
		vector<float>   *pfHcal;
		vector<float>   *trkMVALoose;
		vector<float>   *trkMVATight;
		vector<float>   *trkDxy;
		vector<float>   *trkDxyError;
		vector<float>   *trkDz;
		vector<float>   *trkDzError;
		vector<float>   *trkPtError;
		vector<float>   *pt;
		vector<float>   *eta;
		vector<float>   *phi;
		vector<float>   *chg;

		// List of branches
		TBranch        *b_HLT_ak4CaloJet80;   //!
		TBranch        *b_HLT_ak4CaloJet100;   //!
		TBranch        *b_HLT_ak4CaloJet80_Prescale;   //!
		TBranch        *b_HLT_ak4CaloJet100_Prescale;   //!
		TBranch        *b_HBHENoiseFilterResult;   //!
		TBranch        *b_HBHENoiseFilterResultRun1;   //!
		TBranch        *b_HBHENoiseFilterResultRun2Loose;   //!
		TBranch        *b_HBHENoiseFilterResultRun2Tight;   //!
		TBranch        *b_HBHEIsoNoiseFilterResult;   //!
		TBranch        *b_pcollisionEventSelection;   //!
		TBranch        *b_pprimaryVertexFilter;   //!
		TBranch        *b_pclusterCompatibilityFilter;   //!
		TBranch        *b_phfCoincFilter1;   //!
		TBranch        *b_phfCoincFilter2;   //!
		TBranch        *b_phfCoincFilter3;   //!
		TBranch        *b_phfCoincFilter4;   //!
		TBranch        *b_phfCoincFilter5;   //!
		TBranch        *b_trkPt;   //!
		TBranch        *b_trkEta;   //!
		TBranch        *b_trkPhi;   //!
		TBranch        *b_highPurity;   //!
		TBranch        *b_vz;   //!
		TBranch        *b_pthat;   //!
		TBranch        *b_hiBin;   //!
		TBranch        *b_trkDxy;   //!
		TBranch        *b_trkDxyError;   //!
		TBranch        *b_trkDz;   //!
		TBranch        *b_trkDzError;   //!
		TBranch        *b_trkPtError;   //!
		TBranch        *b_trkChi2;   //!
		TBranch        *b_trkNdof;   //!
		TBranch        *b_trkNHit;   //!
		TBranch        *b_trkNlayer;   //!
		TBranch        *b_pfEcal;   //!
		TBranch        *b_pfHcal;   //!
		TBranch        *b_trkMVALoose;   //!
		TBranch        *b_trkMVATight;   //!

		TBranch        *b_pt;   //!
		TBranch        *b_eta;   //!
		TBranch        *b_phi;   //!
		TBranch        *b_chg;   //!

		inTree(TTree *tree=0, bool isMC = 0);
		virtual ~inTree();
		virtual Int_t    GetEntry(Long64_t entry);
		virtual void     Init(TTree *, bool);
};

inTree::inTree(TTree *tree, bool isMC) : fChain(0) 
{
	// if parameter tree is not specified (or zero), connect the file
	// used to generate this class and read the Tree.
	Init(tree, isMC);
if ( isMC)std::cout<<"MC tree is ready"<<std::endl;
else std::cout<<"data tree is ready"<<std::endl;
}

inTree::~inTree()
{
	if (!fChain) return;
	delete fChain->GetCurrentFile();
}

Int_t inTree::GetEntry(Long64_t entry)
{
	// Read contents of entry.
	if (!fChain) return 0;
	return fChain->GetEntry(entry);
}

void inTree::Init(TTree *tree, bool isMC)
{
	// The Init() function is called when the selector needs to initialize
	// a new tree or chain. Typically here the branch addresses and branch
	// pointers of the tree will be set.
	// It is normally not necessary to make changes to the generated
	// code, but the routine can be extended by the user if needed.
	// Init() will be called many times when running on PROOF
	// (once per file to be processed).

	// Set object pointer
	trkPt = 0;
	trkEta = 0;
	trkPhi = 0;
	highPurity = 0;
	trkDxy = 0;
	trkDxyError = 0;
	trkDz = 0;
	trkDzError = 0;
	trkPtError = 0;
	trkChi2 = 0;
	trkNdof = 0;
	trkNHit = 0;
	trkNlayer = 0;
	pfEcal = 0;
	pfHcal = 0;
	trkMVALoose = 0;
	trkMVATight = 0;
	chg=0;
	pt=0;
	eta=0;
	phi=0;

	// Set branch addresses and branch pointers
	if (!tree) return;
	fChain = tree;
	fCurrent = -1;
	fChain->SetMakeClass(1);

	fChain->SetBranchAddress("HLT_ak4CaloJet80", &HLT_ak4CaloJet80, &b_HLT_ak4CaloJet80);
	fChain->SetBranchAddress("HLT_ak4CaloJet100", &HLT_ak4CaloJet100, &b_HLT_ak4CaloJet100);
	fChain->SetBranchAddress("HLT_ak4CaloJet80_Prescale", &HLT_ak4CaloJet80_Prescale, &b_HLT_ak4CaloJet80_Prescale);
	fChain->SetBranchAddress("HLT_ak4CaloJet100_Prescale", &HLT_ak4CaloJet100_Prescale, &b_HLT_ak4CaloJet100_Prescale);
	fChain->SetBranchAddress("HBHENoiseFilterResult", &HBHENoiseFilterResult, &b_HBHENoiseFilterResult);
	fChain->SetBranchAddress("HBHENoiseFilterResultRun1", &HBHENoiseFilterResultRun1, &b_HBHENoiseFilterResultRun1);
	fChain->SetBranchAddress("HBHENoiseFilterResultRun2Loose", &HBHENoiseFilterResultRun2Loose, &b_HBHENoiseFilterResultRun2Loose);
	fChain->SetBranchAddress("HBHENoiseFilterResultRun2Tight", &HBHENoiseFilterResultRun2Tight, &b_HBHENoiseFilterResultRun2Tight);
	fChain->SetBranchAddress("HBHEIsoNoiseFilterResult", &HBHEIsoNoiseFilterResult, &b_HBHEIsoNoiseFilterResult);
	fChain->SetBranchAddress("pcollisionEventSelection", &pcollisionEventSelection, &b_pcollisionEventSelection);
	fChain->SetBranchAddress("pprimaryVertexFilter", &pprimaryVertexFilter, &b_pprimaryVertexFilter);
	fChain->SetBranchAddress("pclusterCompatibilityFilter", &pclusterCompatibilityFilter, &b_pclusterCompatibilityFilter);
	fChain->SetBranchAddress("phfCoincFilter1", &phfCoincFilter1, &b_phfCoincFilter1);
	fChain->SetBranchAddress("phfCoincFilter2", &phfCoincFilter2, &b_phfCoincFilter2);
	fChain->SetBranchAddress("phfCoincFilter3", &phfCoincFilter3, &b_phfCoincFilter3);
	fChain->SetBranchAddress("phfCoincFilter4", &phfCoincFilter4, &b_phfCoincFilter4);
	fChain->SetBranchAddress("phfCoincFilter5", &phfCoincFilter5, &b_phfCoincFilter5);
	fChain->SetBranchAddress("trkPt", &trkPt, &b_trkPt);
	fChain->SetBranchAddress("trkEta", &trkEta, &b_trkEta);
	fChain->SetBranchAddress("trkPhi", &trkPhi, &b_trkPhi);
	fChain->SetBranchAddress("highPurity", &highPurity, &b_highPurity);
	fChain->SetBranchAddress("vz", &vz, &b_vz);
	fChain->SetBranchAddress("hiBin", &hiBin, &b_hiBin);
	fChain->SetBranchAddress("trkDxy", &trkDxy, &b_trkDxy);
	fChain->SetBranchAddress("trkDxyError", &trkDxyError, &b_trkDxyError);
	fChain->SetBranchAddress("trkDz", &trkDz, &b_trkDz);
	fChain->SetBranchAddress("trkDzError", &trkDzError, &b_trkDzError);
	fChain->SetBranchAddress("trkPtError", &trkPtError, &b_trkPtError);
	fChain->SetBranchAddress("trkChi2", &trkChi2, &b_trkChi2);
	fChain->SetBranchAddress("trkNdof", &trkNdof, &b_trkNdof);
	fChain->SetBranchAddress("trkNHit", &trkNHit, &b_trkNHit);
	fChain->SetBranchAddress("trkNlayer", &trkNlayer, &b_trkNlayer);
	fChain->SetBranchAddress("pfEcal", &pfEcal, &b_pfEcal);
	fChain->SetBranchAddress("pfHcal", &pfHcal, &b_pfHcal);
	fChain->SetBranchAddress("trkMVALoose", &trkMVALoose, &b_trkMVALoose);
	fChain->SetBranchAddress("trkMVATight", &trkMVATight, &b_trkMVATight);

	if(isMC){
	fChain->SetBranchAddress("pthat", &pthat, &b_pthat);
		fChain->SetBranchAddress("pt", &pt  , &b_pt);
		fChain->SetBranchAddress("eta", &eta, &b_eta);
		fChain->SetBranchAddress("phi", &phi, &b_phi);
		fChain->SetBranchAddress("chg", &chg, &b_chg);
	}
/*
*/
}


#endif // #ifdef inTree_cxx
