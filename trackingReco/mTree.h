//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Sat Jul 22 14:52:32 2017 by ROOT version 6.02/13
// from TTree trackingMiniTree/
// found on file: /uscms/home/xiaowang/dataSet/skims/part1.root
//////////////////////////////////////////////////////////

#ifndef mTree_h
#define mTree_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.
#include "vector"

class mTree {
	public :
		TTree          *fChain;   //!pointer to the analyzed TTree or TChain
		Int_t           fCurrent; //!current Tree number in a TChain

		// Fixed size dimensions of array or collections stored in the TTree if any.

		// Declaration of leaf types
		Int_t           hiBin;
		Float_t         vz;
		vector<float>   *trkPt;
		vector<float>   *trkEta;
		vector<float>   *trkPhi;
		vector<float>   *trkDz;
		vector<float>   *trkDxy;
		vector<float>   *trkDzError;
		vector<float>   *trkDxyError;
		vector<float>   *trkChi2;
		vector<float>   *trkPtError;
		vector<float>   *pfHcal;
		vector<float>   *pfEcal;
		vector<int>     *trkNdof;
		vector<int>     *trkNHit;
		vector<int>     *trkNlayer;
		vector<float>   *genPt;
		vector<float>   *genEta;
		vector<float>   *genPhi;
		vector<float>   *genCharge;

		// List of branches
		TBranch        *b_hiBin;   //!
		TBranch        *b_vz;   //!
		TBranch        *b_trkPt;   //!
		TBranch        *b_trkEta;   //!
		TBranch        *b_trkPhi;   //!
		TBranch        *b_trkDz;   //!
		TBranch        *b_trkDxy;   //!
		TBranch        *b_trkDzError;   //!
		TBranch        *b_trkDxyError;   //!
		TBranch        *b_trkPtError;   //!
		TBranch        *b_trkChi2;   //!
		TBranch        *b_pfEcal;   //!
		TBranch        *b_pfHcal;   //!
		TBranch        *b_trkNdof;   //!
		TBranch        *b_trkNHit;   //!
		TBranch        *b_trkNlayer;   //!
		TBranch        *b_genPt;   //!
		TBranch        *b_genEta;   //!
		TBranch        *b_genPhi;   //!
		TBranch        *b_genCharge;   //!

		int isMC;
		int trackType;

		mTree(TTree *tree=0, int isMC=0);
		virtual ~mTree();
		virtual Int_t    Cut(Long64_t entry);
		virtual Int_t    GetEntry(Long64_t entry);
		virtual Long64_t LoadTree(Long64_t entry);
		virtual void     Init(TTree *tree);
};

mTree::mTree(TTree *tree, int key, int isfullTrack) : fChain(0) 
{
	isMC = key;
	trackType = isfullTrack;
	// if parameter tree is not specified (or zero), connect the file
	// used to generate this class and read the Tree.
	if (tree == 0) {
		TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("/uscms/home/xiaowang/dataSet/skims/part1.root");
		if (!f || !f->IsOpen()) {
			f = new TFile("/uscms/home/xiaowang/dataSet/skims/part1.root");
		}
		f->GetObject("trackingMiniTree",tree);

	}
	Init(tree);
}

mTree::~mTree()
{
	if (!fChain) return;
	delete fChain->GetCurrentFile();
}

Int_t mTree::GetEntry(Long64_t entry)
{
	// Read contents of entry.
	if (!fChain) return 0;
	return fChain->GetEntry(entry);
}
Long64_t mTree::LoadTree(Long64_t entry)
{
	// Set the environment to read one entry
	if (!fChain) return -5;
	Long64_t centry = fChain->LoadTree(entry);
	if (centry < 0) return centry;
	if (fChain->GetTreeNumber() != fCurrent) {
		fCurrent = fChain->GetTreeNumber();
	}
	return centry;
}

void mTree::Init(TTree *tree)
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
	trkPtError = 0;
	trkDz = 0;
	trkDxy = 0;
	trkDzError = 0;
	trkDxyError = 0;
	trkChi2 = 0;
	pfEcal = 0;
	pfHcal = 0;
	trkNdof = 0;
	trkNHit = 0;
	trkNlayer = 0;

	genPt = 0;
	genEta = 0;
	genPhi = 0;
	genCharge = 0;
	// Set branch addresses and branch pointers
	if (!tree) return;
	fChain = tree;
	fCurrent = -1;
	fChain->SetMakeClass(1);

	fChain->SetBranchAddress("hiBin", &hiBin, &b_hiBin);
	fChain->SetBranchAddress("vz", &vz, &b_vz);
	fChain->SetBranchAddress("trkPt", &trkPt, &b_trkPt);
	fChain->SetBranchAddress("trkEta", &trkEta, &b_trkEta);
	fChain->SetBranchAddress("trkPhi", &trkPhi, &b_trkPhi);
	if(trackType){
		fChain->SetBranchAddress("trkDz", &trkDz, &b_trkDz);
		fChain->SetBranchAddress("trkDxy", &trkDxy, &b_trkDxy);
		fChain->SetBranchAddress("trkDzError", &trkDzError, &b_trkDzError);
		fChain->SetBranchAddress("trkDxyError", &trkDxyError, &b_trkDxyError);
		fChain->SetBranchAddress("trkPtError", &trkPtError, &b_trkPtError);
		fChain->SetBranchAddress("trkChi2", &trkChi2, &b_trkChi2);
		fChain->SetBranchAddress("trkNdof", &trkNdof, &b_trkNdof);
		fChain->SetBranchAddress("trkNHit", &trkNHit, &b_trkNHit);
		fChain->SetBranchAddress("trkNlayer", &trkNlayer, &b_trkNlayer);
		fChain->SetBranchAddress("pfEcal", &pfEcal, &b_pfEcal);
		fChain->SetBranchAddress("pfHcal", &pfHcal, &b_pfHcal);
	}
	if(isMC){
		fChain->SetBranchAddress("genPt", &genPt, &b_genPt);
		fChain->SetBranchAddress("genEta", &genEta, &b_genEta);
		fChain->SetBranchAddress("genPhi", &genPhi, &b_genPhi);
		fChain->SetBranchAddress("genCharge", &genCharge, &b_genCharge);
	}
}


Int_t mTree::Cut(Long64_t entry)
{
	// This function may be called from Loop.
	// returns  1 if entry is accepted.
	// returns -1 otherwise.
	return 1;
}
#endif // #ifdef mTree_cxx
