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
		TBranch        *b_genPt;   //!
		TBranch        *b_genEta;   //!
		TBranch        *b_genPhi;   //!
		TBranch        *b_genCharge;   //!

int isMC;

		mTree(TTree *tree=0, int isMC=0);
		virtual ~mTree();
		virtual Int_t    Cut(Long64_t entry);
		virtual Int_t    GetEntry(Long64_t entry);
		virtual Long64_t LoadTree(Long64_t entry);
		virtual void     Init(TTree *tree);
};

mTree::mTree(TTree *tree, int key) : fChain(0) 
{
isMC = key;
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
	trkDz = 0;
	trkDxy = 0;
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
	fChain->SetBranchAddress("trkDz", &trkDz, &b_trkDz);
	fChain->SetBranchAddress("trkDxy", &trkDxy, &b_trkDxy);
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
