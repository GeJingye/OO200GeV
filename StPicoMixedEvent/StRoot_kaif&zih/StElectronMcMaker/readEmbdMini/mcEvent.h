//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Tue Oct 15 09:51:28 2019 by ROOT version 5.34/30
// from TTree mcT/mcT
// found on file: /star/data01/pwg/zaochen/analysis/Run50/Run50_27GeV/out_embd/out_Electron_200/emb27GeV_production_2050xElectron_200_20193201x_19131046_raw_3500013.myminimc.root
//////////////////////////////////////////////////////////

#ifndef mcEvent_h
#define mcEvent_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.

// Fixed size dimensions of array or collections stored in the TTree if any.

class mcEvent 
{
	public :
		TTree          *fChain;   //!pointer to the analyzed TTree or TChain
		Int_t           fCurrent; //!current Tree number in a TChain

		// Declaration of leaf types
		Int_t           runId;
		Int_t           triggerId;
		Float_t         zdcX;
		Int_t           mcEvtId;
		Float_t         mcVertexX;
		Float_t         mcVertexY;
		Float_t         mcVertexZ;
		Int_t           rcEvtId;
		Float_t         Ranking;
		Float_t         rcVertexX;
		Float_t         rcVertexY;
		Float_t         rcVertexZ;
		Float_t         rcVpdVz;
		Int_t           rcRefMult;
		Float_t         rcRefMultCorr;
		Float_t         reWeight;
		Int_t           cent9;
		Int_t           cent16;
		Int_t           nMcTrks;
		Int_t           nMcE;
		Int_t           geantId[50];   //[nMcE]
		Int_t           mcTrkKey[50];   //[nMcE]
		Int_t           parentGeantId[50];   //[nMcE]
		Int_t           mcParentTrkKey[50];   //[nMcE]
		Float_t         mcPt[50];   //[nMcE]
		Float_t         mcEta[50];   //[nMcE]
		Float_t         mcPhi[50];   //[nMcE]
		Float_t         mcPtFirst[50];   //[nMcE]
		Float_t         mcEtaFirst[50];   //[nMcE]
		Float_t         mcPhiFirst[50];   //[nMcE]
		Float_t         mcPtLast[50];   //[nMcE]
		Float_t         mcEtaLast[50];   //[nMcE]
		Float_t         mcPhiLast[50];   //[nMcE]
		Int_t           rcTrkKey[50];   //[nMcE]
		Float_t         rcPtFirst[50];   //[nMcE]
		Float_t         rcEtaFirst[50];   //[nMcE]
		Float_t         rcPhiFirst[50];   //[nMcE]
		Float_t         rcPtLast[50];   //[nMcE]
		Float_t         rcEtaLast[50];   //[nMcE]
		Float_t         rcPhiLast[50];   //[nMcE]
		Int_t           rcNHitsFit[50];   //[nMcE]
		Int_t           rcNHitsPoss[50];   //[nMcE]
		Int_t           rcNHitsDedx[50];   //[nMcE]
		Int_t           rcNHitsCommon[50];   //[nMcE]
		Float_t         rcDedx[50];   //[nMcE]
		Float_t         rcNSigmaE[50];   //[nMcE]
		Float_t         rcNSigmaPi[50];   //[nMcE]
		Float_t         rcNSigmaK[50];   //[nMcE]
		Float_t         rcNSigmaP[50];   //[nMcE]
		Float_t         rcDca[50];   //[nMcE]
		Float_t         rcDca1[50];   //[nMcE]
		Float_t         rcDca2[50];   //[nMcE]
		Float_t         rcDca3[50];   //[nMcE]
		Float_t         rcDca4[50];   //[nMcE]
		Float_t         rcDca5[50];   //[nMcE]
		Float_t         rcDca6[50];   //[nMcE]

		// List of branches
		TBranch        *b_runId;   //!
		TBranch        *b_triggerId;   //!
		TBranch        *b_zdcX;   //!
		TBranch        *b_mcEvtId;   //!
		TBranch        *b_mcVertexX;   //!
		TBranch        *b_mcVertexY;   //!
		TBranch        *b_mcVertexZ;   //!
		TBranch        *b_rcEvtId;   //!
		TBranch        *b_Ranking;   //!
		TBranch        *b_rcVertexX;   //!
		TBranch        *b_rcVertexY;   //!
		TBranch        *b_rcVertexZ;   //!
		TBranch        *b_rcVpdVz;   //!
		TBranch        *b_rcRefMult;   //!
		TBranch        *b_rcRefMultCorr;   //!
		TBranch        *b_gWeigth;   //!
		TBranch        *b_cent9;   //!
		TBranch        *b_cent16;   //!
		TBranch        *b_nMcTrks;   //!
		TBranch        *b_nMcE;   //!
		TBranch        *b_geantId;   //!
		TBranch        *b_mcTrkKey;   //!
		TBranch        *b_parentGeantId;   //!
		TBranch        *b_mcParentTrkKey;   //!
		TBranch        *b_mcPt;   //!
		TBranch        *b_mcEta;   //!
		TBranch        *b_mcPhi;   //!
		TBranch        *b_mcPtFirst;   //!
		TBranch        *b_mcEtaFirst;   //!
		TBranch        *b_mcPhiFirst;   //!
		TBranch        *b_mcPtLast;   //!
		TBranch        *b_mcEtaLast;   //!
		TBranch        *b_mcPhiLast;   //!
		TBranch        *b_rcTrkKey;   //!
		TBranch        *b_rcPtFirst;   //!
		TBranch        *b_rcEtaFirst;   //!
		TBranch        *b_rcPhiFirst;   //!
		TBranch        *b_rcPtLast;   //!
		TBranch        *b_rcEtaLast;   //!
		TBranch        *b_rcPhiLast;   //!
		TBranch        *b_rcNHitsFit;   //!
		TBranch        *b_rcNHitsPoss;   //!
		TBranch        *b_rcNHitsDedx;   //!
		TBranch        *b_rcNHitsCommon;   //!
		TBranch        *b_rcDedx;   //!
		TBranch        *b_rcNSigmaE;   //!
		TBranch        *b_rcNSigmaPi;   //!
		TBranch        *b_rcNSigmaK;   //!
		TBranch        *b_rcNSigmaP;   //!
		TBranch        *b_rcDca;   //!
		TBranch        *b_rcDca1;   //!
		TBranch        *b_rcDca2;   //!
		TBranch        *b_rcDca3;   //!
		TBranch        *b_rcDca4;   //!
		TBranch        *b_rcDca5;   //!
		TBranch        *b_rcDca6;   //!

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
		TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("/star/data01/pwg/zaochen/analysis/Run50/Run50_27GeV/out_embd/out_Electron_200/emb27GeV_production_2050xElectron_200_20193201x_19131046_raw_3500013.myminimc.root");
		if (!f || !f->IsOpen()) {
			f = new TFile("/star/data01/pwg/zaochen/analysis/Run50/Run50_27GeV/out_embd/out_Electron_200/emb27GeV_production_2050xElectron_200_20193201x_19131046_raw_3500013.myminimc.root");
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
	fChain->SetBranchAddress("triggerId", &triggerId, &b_triggerId);
	fChain->SetBranchAddress("zdcX", &zdcX, &b_zdcX);
	fChain->SetBranchAddress("mcEvtId", &mcEvtId, &b_mcEvtId);
	fChain->SetBranchAddress("mcVertexX", &mcVertexX, &b_mcVertexX);
	fChain->SetBranchAddress("mcVertexY", &mcVertexY, &b_mcVertexY);
	fChain->SetBranchAddress("mcVertexZ", &mcVertexZ, &b_mcVertexZ);
	fChain->SetBranchAddress("rcEvtId", &rcEvtId, &b_rcEvtId);
	fChain->SetBranchAddress("Ranking", &Ranking, &b_Ranking);
	fChain->SetBranchAddress("rcVertexX", &rcVertexX, &b_rcVertexX);
	fChain->SetBranchAddress("rcVertexY", &rcVertexY, &b_rcVertexY);
	fChain->SetBranchAddress("rcVertexZ", &rcVertexZ, &b_rcVertexZ);
	fChain->SetBranchAddress("rcVpdVz", &rcVpdVz, &b_rcVpdVz);
	fChain->SetBranchAddress("rcRefMult", &rcRefMult, &b_rcRefMult);
	fChain->SetBranchAddress("rcRefMultCorr", &rcRefMultCorr, &b_rcRefMultCorr);
	fChain->SetBranchAddress("reWeight", &reWeight, &b_gWeigth);
	fChain->SetBranchAddress("cent9", &cent9, &b_cent9);
	fChain->SetBranchAddress("cent16", &cent16, &b_cent16);
	fChain->SetBranchAddress("nMcTrks", &nMcTrks, &b_nMcTrks);
	fChain->SetBranchAddress("nMcE", &nMcE, &b_nMcE);
	fChain->SetBranchAddress("geantId", geantId, &b_geantId);
	fChain->SetBranchAddress("mcTrkKey", mcTrkKey, &b_mcTrkKey);
	fChain->SetBranchAddress("parentGeantId", parentGeantId, &b_parentGeantId);
	fChain->SetBranchAddress("mcParentTrkKey", mcParentTrkKey, &b_mcParentTrkKey);
	fChain->SetBranchAddress("mcPt", mcPt, &b_mcPt);
	fChain->SetBranchAddress("mcEta", mcEta, &b_mcEta);
	fChain->SetBranchAddress("mcPhi", mcPhi, &b_mcPhi);
	fChain->SetBranchAddress("mcPtFirst", mcPtFirst, &b_mcPtFirst);
	fChain->SetBranchAddress("mcEtaFirst", mcEtaFirst, &b_mcEtaFirst);
	fChain->SetBranchAddress("mcPhiFirst", mcPhiFirst, &b_mcPhiFirst);
	fChain->SetBranchAddress("mcPtLast", mcPtLast, &b_mcPtLast);
	fChain->SetBranchAddress("mcEtaLast", mcEtaLast, &b_mcEtaLast);
	fChain->SetBranchAddress("mcPhiLast", mcPhiLast, &b_mcPhiLast);
	fChain->SetBranchAddress("rcTrkKey", rcTrkKey, &b_rcTrkKey);
	fChain->SetBranchAddress("rcPtFirst", rcPtFirst, &b_rcPtFirst);
	fChain->SetBranchAddress("rcEtaFirst", rcEtaFirst, &b_rcEtaFirst);
	fChain->SetBranchAddress("rcPhiFirst", rcPhiFirst, &b_rcPhiFirst);
	fChain->SetBranchAddress("rcPtLast", rcPtLast, &b_rcPtLast);
	fChain->SetBranchAddress("rcEtaLast", rcEtaLast, &b_rcEtaLast);
	fChain->SetBranchAddress("rcPhiLast", rcPhiLast, &b_rcPhiLast);
	fChain->SetBranchAddress("rcNHitsFit", rcNHitsFit, &b_rcNHitsFit);
	fChain->SetBranchAddress("rcNHitsPoss", rcNHitsPoss, &b_rcNHitsPoss);
	fChain->SetBranchAddress("rcNHitsDedx", rcNHitsDedx, &b_rcNHitsDedx);
	fChain->SetBranchAddress("rcNHitsCommon", rcNHitsCommon, &b_rcNHitsCommon);
	fChain->SetBranchAddress("rcDedx", rcDedx, &b_rcDedx);
	fChain->SetBranchAddress("rcNSigmaE", rcNSigmaE, &b_rcNSigmaE);
	fChain->SetBranchAddress("rcNSigmaPi", rcNSigmaPi, &b_rcNSigmaPi);
	fChain->SetBranchAddress("rcNSigmaK", rcNSigmaK, &b_rcNSigmaK);
	fChain->SetBranchAddress("rcNSigmaP", rcNSigmaP, &b_rcNSigmaP);
	fChain->SetBranchAddress("rcDca", rcDca, &b_rcDca);
	fChain->SetBranchAddress("rcDca1", rcDca1, &b_rcDca1);
	fChain->SetBranchAddress("rcDca2", rcDca2, &b_rcDca2);
	fChain->SetBranchAddress("rcDca3", rcDca3, &b_rcDca3);
	fChain->SetBranchAddress("rcDca4", rcDca4, &b_rcDca4);
	fChain->SetBranchAddress("rcDca5", rcDca5, &b_rcDca5);
	fChain->SetBranchAddress("rcDca6", rcDca6, &b_rcDca6);
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
