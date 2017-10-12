//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Mon Aug  7 06:26:14 2017 by ROOT version 6.08/05
// from TTree mixing_tree/
// found on file: /uscms/home/xiaowang/dataSet/sampleSet/Data2015_12.root
//////////////////////////////////////////////////////////

#ifndef dataTree_h
#define dataTree_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.
#include "vector"
#include "vector"
#include "vector"

class dataTree {
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
   vector<float>   *trkPt;
   vector<float>   *trkEta;
   vector<float>   *trkPhi;
   vector<int>     *trkAlgo;
   vector<bool>    *highPurity;
   Float_t         vz;
   Int_t           hiBin;
   Float_t         evtPlane_HF2;
   vector<float>   *pf_jteta;
   vector<float>   *pf_jtphi;
   vector<float>   *pf_jtpt;
   vector<float>   *pf_rawpt;
   vector<float>   *pf_corrpt;
   vector<float>   *pf_trackMax;
   vector<float>   *calo_jteta;
   vector<float>   *calo_jtphi;
   vector<float>   *calo_jtpt;
   vector<float>   *calo_rawpt;
   vector<float>   *calo_corrpt;
   vector<float>   *calo_trackMax;
   vector<float>   *trkDxy;
   vector<float>   *trkDxyError;
   vector<float>   *trkDz;
   vector<float>   *trkDzError;
   vector<float>   *trkPtError;
   vector<float>   *trkChi2;
   vector<int>     *trkNdof;
   vector<int>     *trkNHit;
   vector<int>     *trkNlayer;
   vector<float>   *pfEcal;
   vector<float>   *pfHcal;
   vector<float>   *trkMVALoose;
   vector<float>   *trkMVATight;
   Int_t           nPFpart;
   vector<int>     *nCSpart;
   vector<int>     *nCSpartGT1;
   vector<int>     *nCSpartGT2;
   vector<int>     *nCSpartGT2_id1;
   vector<int>     *nCSpartGT2_id145;
   vector<int>     *nPFpartPerJet;
   vector<int>     *nPFpartGT2;
   vector<int>     *nPFpartGT2_id1;
   vector<int>     *nPFpartGT2_id145;
   vector<int>     *pfId;
   vector<float>   *pfPt;
   vector<float>   *pfVsPtInitial;
   vector<float>   *pfEta;
   vector<float>   *pfPhi;
   vector<float>   *pf_discr_ssvHighEff;
   vector<float>   *pf_discr_ssvHighPur;
   vector<float>   *pf_discr_csvV1;
   vector<float>   *pf_discr_prob;
   vector<float>   *pf_svtxm;
   vector<float>   *pf_svtxpt;
   vector<float>   *pf_svtxmcorr;
   vector<float>   *pf_svtxdl;
   vector<float>   *pf_svtxdls;
   vector<float>   *calo_discr_ssvHighEff;
   vector<float>   *calo_discr_ssvHighPur;
   vector<float>   *calo_discr_csvV1;
   vector<float>   *calo_discr_prob;
   vector<float>   *calo_svtxm;
   vector<float>   *calo_svtxpt;
   vector<float>   *calo_svtxmcorr;
   vector<float>   *calo_svtxdl;
   vector<float>   *calo_svtxdls;

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
   TBranch        *b_trkAlgo;   //!
   TBranch        *b_highPurity;   //!
   TBranch        *b_vz;   //!
   TBranch        *b_hiBin;   //!
   TBranch        *b_evtPlane_HF2;   //!
   TBranch        *b_pf_jteta;   //!
   TBranch        *b_pf_jtphi;   //!
   TBranch        *b_pf_jtpt;   //!
   TBranch        *b_pf_rawpt;   //!
   TBranch        *b_pf_corrpt;   //!
   TBranch        *b_pf_trackMax;   //!
   TBranch        *b_calo_jteta;   //!
   TBranch        *b_calo_jtphi;   //!
   TBranch        *b_calo_jtpt;   //!
   TBranch        *b_calo_rawpt;   //!
   TBranch        *b_calo_corrpt;   //!
   TBranch        *b_calo_trackMax;   //!
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
   TBranch        *b_nPFpart;   //!
   TBranch        *b_nCSpart;   //!
   TBranch        *b_nCSpartGT1;   //!
   TBranch        *b_nCSpartGT2;   //!
   TBranch        *b_nCSpartGT2_id1;   //!
   TBranch        *b_nCSpartGT2_id145;   //!
   TBranch        *b_nPFpartPerJet;   //!
   TBranch        *b_nPFpartGT2;   //!
   TBranch        *b_nPFpartGT2_id1;   //!
   TBranch        *b_nPFpartGT2_id145;   //!
   TBranch        *b_pfId;   //!
   TBranch        *b_pfPt;   //!
   TBranch        *b_pfVsPtInitial;   //!
   TBranch        *b_pfEta;   //!
   TBranch        *b_pfPhi;   //!
   TBranch        *b_pf_discr_ssvHighEff;   //!
   TBranch        *b_pf_discr_ssvHighPur;   //!
   TBranch        *b_pf_discr_csvV1;   //!
   TBranch        *b_pf_discr_prob;   //!
   TBranch        *b_pf_svtxm;   //!
   TBranch        *b_pf_svtxpt;   //!
   TBranch        *b_pf_svtxmcorr;   //!
   TBranch        *b_pf_svtxdl;   //!
   TBranch        *b_pf_svtxdls;   //!
   TBranch        *b_calo_discr_ssvHighEff;   //!
   TBranch        *b_calo_discr_ssvHighPur;   //!
   TBranch        *b_calo_discr_csvV1;   //!
   TBranch        *b_calo_discr_prob;   //!
   TBranch        *b_calo_svtxm;   //!
   TBranch        *b_calo_svtxpt;   //!
   TBranch        *b_calo_svtxmcorr;   //!
   TBranch        *b_calo_svtxdl;   //!
   TBranch        *b_calo_svtxdls;   //!

   dataTree(TTree *tree=0);
   virtual ~dataTree();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

dataTree::dataTree(TTree *tree) : fChain(0) 
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("/uscms/home/xiaowang/dataSet/sampleSet/Data2015_12.root");
      if (!f || !f->IsOpen()) {
         f = new TFile("/uscms/home/xiaowang/dataSet/sampleSet/Data2015_12.root");
      }
      f->GetObject("mixing_tree",tree);

   }
   Init(tree);
}

dataTree::~dataTree()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t dataTree::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t dataTree::LoadTree(Long64_t entry)
{
// Set the environment to read one entry
   if (!fChain) return -5;
   Long64_t centry = fChain->LoadTree(entry);
   if (centry < 0) return centry;
   if (fChain->GetTreeNumber() != fCurrent) {
      fCurrent = fChain->GetTreeNumber();
      Notify();
   }
   return centry;
}

void dataTree::Init(TTree *tree)
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
   trkAlgo = 0;
   highPurity = 0;
   pf_jteta = 0;
   pf_jtphi = 0;
   pf_jtpt = 0;
   pf_rawpt = 0;
   pf_corrpt = 0;
   pf_trackMax = 0;
   calo_jteta = 0;
   calo_jtphi = 0;
   calo_jtpt = 0;
   calo_rawpt = 0;
   calo_corrpt = 0;
   calo_trackMax = 0;
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
   nCSpart = 0;
   nCSpartGT1 = 0;
   nCSpartGT2 = 0;
   nCSpartGT2_id1 = 0;
   nCSpartGT2_id145 = 0;
   nPFpartPerJet = 0;
   nPFpartGT2 = 0;
   nPFpartGT2_id1 = 0;
   nPFpartGT2_id145 = 0;
   pfId = 0;
   pfPt = 0;
   pfVsPtInitial = 0;
   pfEta = 0;
   pfPhi = 0;
   pf_discr_ssvHighEff = 0;
   pf_discr_ssvHighPur = 0;
   pf_discr_csvV1 = 0;
   pf_discr_prob = 0;
   pf_svtxm = 0;
   pf_svtxpt = 0;
   pf_svtxmcorr = 0;
   pf_svtxdl = 0;
   pf_svtxdls = 0;
   calo_discr_ssvHighEff = 0;
   calo_discr_ssvHighPur = 0;
   calo_discr_csvV1 = 0;
   calo_discr_prob = 0;
   calo_svtxm = 0;
   calo_svtxpt = 0;
   calo_svtxmcorr = 0;
   calo_svtxdl = 0;
   calo_svtxdls = 0;
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
   fChain->SetBranchAddress("trkAlgo", &trkAlgo, &b_trkAlgo);
   fChain->SetBranchAddress("highPurity", &highPurity, &b_highPurity);
   fChain->SetBranchAddress("vz", &vz, &b_vz);
   fChain->SetBranchAddress("hiBin", &hiBin, &b_hiBin);
   fChain->SetBranchAddress("evtPlane_HF2", &evtPlane_HF2, &b_evtPlane_HF2);
   fChain->SetBranchAddress("pf_jteta", &pf_jteta, &b_pf_jteta);
   fChain->SetBranchAddress("pf_jtphi", &pf_jtphi, &b_pf_jtphi);
   fChain->SetBranchAddress("pf_jtpt", &pf_jtpt, &b_pf_jtpt);
   fChain->SetBranchAddress("pf_rawpt", &pf_rawpt, &b_pf_rawpt);
   fChain->SetBranchAddress("pf_corrpt", &pf_corrpt, &b_pf_corrpt);
   fChain->SetBranchAddress("pf_trackMax", &pf_trackMax, &b_pf_trackMax);
   fChain->SetBranchAddress("calo_jteta", &calo_jteta, &b_calo_jteta);
   fChain->SetBranchAddress("calo_jtphi", &calo_jtphi, &b_calo_jtphi);
   fChain->SetBranchAddress("calo_jtpt", &calo_jtpt, &b_calo_jtpt);
   fChain->SetBranchAddress("calo_rawpt", &calo_rawpt, &b_calo_rawpt);
   fChain->SetBranchAddress("calo_corrpt", &calo_corrpt, &b_calo_corrpt);
   fChain->SetBranchAddress("calo_trackMax", &calo_trackMax, &b_calo_trackMax);
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
   fChain->SetBranchAddress("nPFpart", &nPFpart, &b_nPFpart);
   fChain->SetBranchAddress("nCSpart", &nCSpart, &b_nCSpart);
   fChain->SetBranchAddress("nCSpartGT1", &nCSpartGT1, &b_nCSpartGT1);
   fChain->SetBranchAddress("nCSpartGT2", &nCSpartGT2, &b_nCSpartGT2);
   fChain->SetBranchAddress("nCSpartGT2_id1", &nCSpartGT2_id1, &b_nCSpartGT2_id1);
   fChain->SetBranchAddress("nCSpartGT2_id145", &nCSpartGT2_id145, &b_nCSpartGT2_id145);
   fChain->SetBranchAddress("nPFpartPerJet", &nPFpartPerJet, &b_nPFpartPerJet);
   fChain->SetBranchAddress("nPFpartGT2", &nPFpartGT2, &b_nPFpartGT2);
   fChain->SetBranchAddress("nPFpartGT2_id1", &nPFpartGT2_id1, &b_nPFpartGT2_id1);
   fChain->SetBranchAddress("nPFpartGT2_id145", &nPFpartGT2_id145, &b_nPFpartGT2_id145);
   fChain->SetBranchAddress("pfId", &pfId, &b_pfId);
   fChain->SetBranchAddress("pfPt", &pfPt, &b_pfPt);
   fChain->SetBranchAddress("pfVsPtInitial", &pfVsPtInitial, &b_pfVsPtInitial);
   fChain->SetBranchAddress("pfEta", &pfEta, &b_pfEta);
   fChain->SetBranchAddress("pfPhi", &pfPhi, &b_pfPhi);
   fChain->SetBranchAddress("pf_discr_ssvHighEff", &pf_discr_ssvHighEff, &b_pf_discr_ssvHighEff);
   fChain->SetBranchAddress("pf_discr_ssvHighPur", &pf_discr_ssvHighPur, &b_pf_discr_ssvHighPur);
   fChain->SetBranchAddress("pf_discr_csvV1", &pf_discr_csvV1, &b_pf_discr_csvV1);
   fChain->SetBranchAddress("pf_discr_prob", &pf_discr_prob, &b_pf_discr_prob);
   fChain->SetBranchAddress("pf_svtxm", &pf_svtxm, &b_pf_svtxm);
   fChain->SetBranchAddress("pf_svtxpt", &pf_svtxpt, &b_pf_svtxpt);
   fChain->SetBranchAddress("pf_svtxmcorr", &pf_svtxmcorr, &b_pf_svtxmcorr);
   fChain->SetBranchAddress("pf_svtxdl", &pf_svtxdl, &b_pf_svtxdl);
   fChain->SetBranchAddress("pf_svtxdls", &pf_svtxdls, &b_pf_svtxdls);
   fChain->SetBranchAddress("calo_discr_ssvHighEff", &calo_discr_ssvHighEff, &b_calo_discr_ssvHighEff);
   fChain->SetBranchAddress("calo_discr_ssvHighPur", &calo_discr_ssvHighPur, &b_calo_discr_ssvHighPur);
   fChain->SetBranchAddress("calo_discr_csvV1", &calo_discr_csvV1, &b_calo_discr_csvV1);
   fChain->SetBranchAddress("calo_discr_prob", &calo_discr_prob, &b_calo_discr_prob);
   fChain->SetBranchAddress("calo_svtxm", &calo_svtxm, &b_calo_svtxm);
   fChain->SetBranchAddress("calo_svtxpt", &calo_svtxpt, &b_calo_svtxpt);
   fChain->SetBranchAddress("calo_svtxmcorr", &calo_svtxmcorr, &b_calo_svtxmcorr);
   fChain->SetBranchAddress("calo_svtxdl", &calo_svtxdl, &b_calo_svtxdl);
   fChain->SetBranchAddress("calo_svtxdls", &calo_svtxdls, &b_calo_svtxdls);
   Notify();
}

Bool_t dataTree::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void dataTree::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t dataTree::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef dataTree_cxx
