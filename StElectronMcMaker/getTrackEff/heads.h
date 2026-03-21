#include "TH1.h"
#include "TH2.h"
#include "TH3.h"
#include "TFile.h"
#include "TObject.h"
#include "TTree.h"
#include "TBranch.h"
#include "TSystem.h"
#include "TChain.h"
#include "TVector2.h"
#include "TVector3.h"
#include "TLorentzVector.h"
#include "TProfile.h"
#include "TString.h"
#include <iostream>

const int mBadRuns[]=
{
	22130029,
	22130030,
	22130031,
	22130046,
	22130047,
	22131011,
	22131012,
	22131013,
	22131014,
	22131015,
	22131021,
	22131033,
	22131035,
	22132014,
	22133005,
	22133007,
	22133009,
	22133018,
	22133019,
	22133020,
	22133021,
	22133022,
	22133025,
	22133031,
	22133032,
	22133037,
	22133038,
	22135030,
	22135039
};

TFile   *outfile;
TH1F    *h_EvtCounter;

Int_t mRoundIndex;
Int_t mNBadRuns;

const int nCentBins = 9;
const int nCuts     = 5; //0:Acc., 1:Acc.+nHitsFit, 2: Acc.+nHitsFit+nHitDedx, 3:Acc.+nHitsFit+nHitDedx+Dca

const int CentLow[nCentBins] = {0, 5, 10, 20, 30, 40, 50, 60, 70}; // 9 Centrality Bins
const int CentHig[nCentBins] = {5, 10, 20, 30, 40, 50, 60, 70, 80}; // 9 Centrality Bins

const int mCentBinLow[nCentBins] = {0, 0, 2, 3, 4, 5, 6, 7, 8}; // 9 Centrality Bins
const int mCentBinHigh[nCentBins] = {2, 2, 3, 4, 5, 6, 7, 8, 9}; // 9 Centrality Bins

TH3F* h3d_Mc_PtEtaPhi_perCent_eplus[nCentBins];
TH3F* h3d_Rc_PtEtaPhi_perCent_McValues_eplus[nCentBins][nCuts];
TH3F* h3d_Rc_PtEtaPhi_perCent_RcValues_eplus[nCentBins][nCuts];

TH3F* h3d_Mc_PtEtaPhi_perCent_eminus[nCentBins];
TH3F* h3d_Rc_PtEtaPhi_perCent_McValues_eminus[nCentBins][nCuts];
TH3F* h3d_Rc_PtEtaPhi_perCent_RcValues_eminus[nCentBins][nCuts];
// TH1F* h1d_pT_Mc[nCentBins];
// TH1F* h1d_pT_Rc[nCentBins];
TH2F* h_PtDiff4Res[nCentBins];
// TH2F* h2d_McpT_RcpT[nCentBins];
// TH2F* h2d_pT_RcminusMc[nCentBins];
