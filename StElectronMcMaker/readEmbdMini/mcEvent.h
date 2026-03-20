//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Thu Jul 17 05:04:39 2025 by ROOT version 5.34/38
// from TTree mcT/mcT
// found on file: embproduction_OO_200GeV_2021xElectron_100_20250302x_22133010_raw_2000003.MuDst.root
//////////////////////////////////////////////////////////

#ifndef mcEvent_h
#define mcEvent_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.

// Fixed size dimensions of array or collections stored in the TTree if any.

class mcEvent {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

   // Declaration of leaf types
   Int_t           runId;
   Int_t           triggerId[4];
   Int_t           muEvtId;
   Float_t         muPriVertexX;
   Float_t         muPriVertexY;
   Float_t         muPriVertexZ;
   Float_t         muVpdVz;
   Int_t           muRefMult;
   Int_t           mugRefMult;
   Int_t           munBtofMatch;
   Float_t         zdcX;
   Int_t           mRefmult6;
   Float_t         mTotnMIP;
   Int_t           Centrality_16_refmult6;
   Int_t           Centrality_9_refmult6;
   Float_t         refmult_corr_refmult6;
   Float_t         reweight_refmult6;
   Int_t           Centrality_16_totnMIP;
   Int_t           Centrality_9_totnMIP;
   Float_t         refmult_corr_totnMIP;
   Float_t         reweight_totnMIP;
   Int_t           nMcTrks;
   Int_t           nMcVertices;
   Int_t           nMcE;
   Int_t           nRcE;
   Int_t           nMcTrack;
   Int_t           geantId[5];   //[nMcE]
   Int_t           mcTrackId[5];   //[nMcE]
   Short_t         mcCharge[5];   //[nMcE]
   Float_t         mcTrack_Vr[5];   //[nMcE]
   Int_t           mcTrack_par_Geantid[5];   //[nMcE]
   Float_t         mcPt[5];   //[nMcE]
   Float_t         mcEta[5];   //[nMcE]
   Float_t         mcPhi[5];   //[nMcE]
   Int_t           rcidtruth[5];   //[nRcE]
   Int_t           rcQaTruth[5];   //[nRcE]
   Int_t           rcflag[5];   //[nRcE]
   Short_t         rcCharge[5];   //[nRcE]
   Float_t         rcPt[5];   //[nRcE]
   Float_t         rcEta[5];   //[nRcE]
   Float_t         rcPhi[5];   //[nRcE]
   Int_t           rcNHitsFit[5];   //[nRcE]
   Int_t           rcNHitsPoss[5];   //[nRcE]
   Int_t           rcNHitsDedx[5];   //[nRcE]
   Float_t         rcDedx[5];   //[nRcE]
   Float_t         rcNSigmaE[5];   //[nRcE]
   Float_t         rcNSigmaPi[5];   //[nRcE]
   Float_t         rcNSigmaK[5];   //[nRcE]
   Float_t         rcNSigmaP[5];   //[nRcE]
   Float_t         rcDca[5];   //[nRcE]

   // List of branches
   TBranch        *b_runId;   //!
   TBranch        *b_triggerId;   //!
   TBranch        *b_muEvtId;   //!
   TBranch        *b_muPriVertexX;   //!
   TBranch        *b_muPriVertexY;   //!
   TBranch        *b_muPriVertexZ;   //!
   TBranch        *b_muVpdVz;   //!
   TBranch        *b_muRefMult;   //!
   TBranch        *b_mugRefMult;   //!
   TBranch        *b_munBtofMatch;   //!
   TBranch        *b_zdcX;   //!
   TBranch        *b_mRefmult6;   //!
   TBranch        *b_mTotnMIP;   //!
   TBranch        *b_Centrality_16_refmult6;   //!
   TBranch        *b_Centrality_9_refmult6;   //!
   TBranch        *b_refmult_corr_refmult6;   //!
   TBranch        *b_reweight_refmult6;   //!
   TBranch        *b_Centrality_16_totnMIP;   //!
   TBranch        *b_Centrality_9_totnMIP;   //!
   TBranch        *b_refmult_corr_totnMIP;   //!
   TBranch        *b_reweight_totnMIP;   //!
   TBranch        *b_nMcTrks;   //!
   TBranch        *b_nMcVertices;   //!
   TBranch        *b_nMcE;   //!
   TBranch        *b_nRcE;   //!
   TBranch        *b_nMcTrack;   //!
   TBranch        *b_geantId;   //!
   TBranch        *b_mcTrackId;   //!
   TBranch        *b_mcCharge;   //!
   TBranch        *b_mcTrack_Vr;   //!
   TBranch        *b_mcTrack_par_Geantid;   //!
   TBranch        *b_mcPt;   //!
   TBranch        *b_mcEta;   //!
   TBranch        *b_mcPhi;   //!
   TBranch        *b_rcidtruth;   //!
   TBranch        *b_rcQaTruth;   //!
   TBranch        *b_rcflag;   //!
   TBranch        *b_rcCharge;   //!
   TBranch        *b_rcPt;   //!
   TBranch        *b_rcEta;   //!
   TBranch        *b_rcPhi;   //!
   TBranch        *b_rcNHitsFit;   //!
   TBranch        *b_rcNHitsPoss;   //!
   TBranch        *b_rcNHitsDedx;   //!
   TBranch        *b_rcDedx;   //!
   TBranch        *b_rcNSigmaE;   //!
   TBranch        *b_rcNSigmaPi;   //!
   TBranch        *b_rcNSigmaK;   //!
   TBranch        *b_rcNSigmaP;   //!
   TBranch        *b_rcDca;   //!

   mcEvent(TTree *tree=0);
   virtual ~mcEvent();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef mcEvent_cxx
mcEvent::mcEvent(TTree *tree) : fChain(0) 
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("embproduction_OO_200GeV_2021xElectron_100_20250302x_22133010_raw_2000003.MuDst.root");
      if (!f || !f->IsOpen()) {
         f = new TFile("embproduction_OO_200GeV_2021xElectron_100_20250302x_22133010_raw_2000003.MuDst.root");
      }
      f->GetObject("mcT",tree);

   }
   Init(tree);
}

mcEvent::~mcEvent()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t mcEvent::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t mcEvent::LoadTree(Long64_t entry)
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

void mcEvent::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("runId", &runId, &b_runId);
   fChain->SetBranchAddress("triggerId", triggerId, &b_triggerId);
   fChain->SetBranchAddress("muEvtId", &muEvtId, &b_muEvtId);
   fChain->SetBranchAddress("muPriVertexX", &muPriVertexX, &b_muPriVertexX);
   fChain->SetBranchAddress("muPriVertexY", &muPriVertexY, &b_muPriVertexY);
   fChain->SetBranchAddress("muPriVertexZ", &muPriVertexZ, &b_muPriVertexZ);
   fChain->SetBranchAddress("muVpdVz", &muVpdVz, &b_muVpdVz);
   fChain->SetBranchAddress("muRefMult", &muRefMult, &b_muRefMult);
   fChain->SetBranchAddress("mugRefMult", &mugRefMult, &b_mugRefMult);
   fChain->SetBranchAddress("munBtofMatch", &munBtofMatch, &b_munBtofMatch);
   fChain->SetBranchAddress("zdcX", &zdcX, &b_zdcX);
   fChain->SetBranchAddress("mRefmult6", &mRefmult6, &b_mRefmult6);
   fChain->SetBranchAddress("mTotnMIP", &mTotnMIP, &b_mTotnMIP);
   fChain->SetBranchAddress("Centrality_16_refmult6", &Centrality_16_refmult6, &b_Centrality_16_refmult6);
   fChain->SetBranchAddress("Centrality_9_refmult6", &Centrality_9_refmult6, &b_Centrality_9_refmult6);
   fChain->SetBranchAddress("refmult_corr_refmult6", &refmult_corr_refmult6, &b_refmult_corr_refmult6);
   fChain->SetBranchAddress("reweight_refmult6", &reweight_refmult6, &b_reweight_refmult6);
   fChain->SetBranchAddress("Centrality_16_totnMIP", &Centrality_16_totnMIP, &b_Centrality_16_totnMIP);
   fChain->SetBranchAddress("Centrality_9_totnMIP", &Centrality_9_totnMIP, &b_Centrality_9_totnMIP);
   fChain->SetBranchAddress("refmult_corr_totnMIP", &refmult_corr_totnMIP, &b_refmult_corr_totnMIP);
   fChain->SetBranchAddress("reweight_totnMIP", &reweight_totnMIP, &b_reweight_totnMIP);
   fChain->SetBranchAddress("nMcTrks", &nMcTrks, &b_nMcTrks);
   fChain->SetBranchAddress("nMcVertices", &nMcVertices, &b_nMcVertices);
   fChain->SetBranchAddress("nMcE", &nMcE, &b_nMcE);
   fChain->SetBranchAddress("nRcE", &nRcE, &b_nRcE);
   fChain->SetBranchAddress("nMcTrack", &nMcTrack, &b_nMcTrack);
   fChain->SetBranchAddress("geantId", geantId, &b_geantId);
   fChain->SetBranchAddress("mcTrackId", mcTrackId, &b_mcTrackId);
   fChain->SetBranchAddress("mcCharge", mcCharge, &b_mcCharge);
   fChain->SetBranchAddress("mcTrack_Vr", mcTrack_Vr, &b_mcTrack_Vr);
   fChain->SetBranchAddress("mcTrack_par_Geantid", mcTrack_par_Geantid, &b_mcTrack_par_Geantid);
   fChain->SetBranchAddress("mcPt", mcPt, &b_mcPt);
   fChain->SetBranchAddress("mcEta", mcEta, &b_mcEta);
   fChain->SetBranchAddress("mcPhi", mcPhi, &b_mcPhi);
   fChain->SetBranchAddress("rcidtruth", rcidtruth, &b_rcidtruth);
   fChain->SetBranchAddress("rcQaTruth", rcQaTruth, &b_rcQaTruth);
   fChain->SetBranchAddress("rcflag", rcflag, &b_rcflag);
   fChain->SetBranchAddress("rcCharge", rcCharge, &b_rcCharge);
   fChain->SetBranchAddress("rcPt", rcPt, &b_rcPt);
   fChain->SetBranchAddress("rcEta", rcEta, &b_rcEta);
   fChain->SetBranchAddress("rcPhi", rcPhi, &b_rcPhi);
   fChain->SetBranchAddress("rcNHitsFit", rcNHitsFit, &b_rcNHitsFit);
   fChain->SetBranchAddress("rcNHitsPoss", rcNHitsPoss, &b_rcNHitsPoss);
   fChain->SetBranchAddress("rcNHitsDedx", rcNHitsDedx, &b_rcNHitsDedx);
   fChain->SetBranchAddress("rcDedx", rcDedx, &b_rcDedx);
   fChain->SetBranchAddress("rcNSigmaE", rcNSigmaE, &b_rcNSigmaE);
   fChain->SetBranchAddress("rcNSigmaPi", rcNSigmaPi, &b_rcNSigmaPi);
   fChain->SetBranchAddress("rcNSigmaK", rcNSigmaK, &b_rcNSigmaK);
   fChain->SetBranchAddress("rcNSigmaP", rcNSigmaP, &b_rcNSigmaP);
   fChain->SetBranchAddress("rcDca", rcDca, &b_rcDca);
   Notify();
}

Bool_t mcEvent::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void mcEvent::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t mcEvent::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef mcEvent_cxx
