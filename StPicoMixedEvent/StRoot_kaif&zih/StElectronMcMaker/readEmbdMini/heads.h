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

TFile   *outfile;
TH1F    *h_EvtCounter;

Int_t mRoundIndex;
Int_t mNBadRuns;

TH1F* hMc_Vz;
TH2F* hMc_VxVy;
TH1F* hRc_Vz;
TH2F* hRc_VxVy;
TH2F* hVz_McVsRc;
TH1F* hRefMultCorr;
TH1F* hcent9;
TH1F* hcent16;
TH2F* h_nEfracVsCent9;
TH2F* h_nEfracVsCent16;
TH2F* h_nEvsNRefmult;
TH2F* h_nEfracVsCent9_aftwt;
TH2F* h_nEfracVsCent16_aftwt;
TH2F* h_nEvsNRefmult_aftwt;

//check embd quality
TH3F* hMc_ComnHitsPtEta;
TH3F* hRc_ComnHitsPtEta;
TH3F* hRc_ComnHitsnHitsFitPt;

//to compare to Real Data QA
TH3F* h3d_nHitsFit_vsPtEta;
TH3F* h3d_nHitsDedx_vsPtEta;
TH3F* h3d_nHitsFrac_vsPtEta;
TH3F* h3d_Dca_vsPtEta;
TH3F* h3d_Phi_vsPtEta;
//QA for tracking syst. errors
const int nCentBins = 9;
TH3F* h3d_nHitsFit_vsPtEta_Cen[nCentBins];
TH3F* h3d_nHitsDedx_vsPtEta_Cen[nCentBins];
TH3F* h3d_nHitsFrac_vsPtEta_Cen[nCentBins];
TH3F* h3d_Dca_vsPtEta_Cen[nCentBins];

//the input eta and pt weights to weight embd to data
TFile *infile_PtEtaWT;
TH1F* H_Pt_WT;
TH1F* H_Eta_WT;

TH3F* h3d_nHitsFit_vsPtEta_wted;
TH3F* h3d_nHitsDedx_vsPtEta_wted;
TH3F* h3d_nHitsFrac_vsPtEta_wted;
TH3F* h3d_Dca_vsPtEta_wted;
TH3F* h3d_Phi_vsPtEta_wted;

//for tracking efficiency
const int nCuts     = 5; //0:Acc., 1:Acc.+nHitsFit, 2: Acc.+nHitsFit+nHitDedx, 3:Acc.+nHitsFit+nHitDedx+Dca
TH3F* h3d_Mc_PtEtaPhi_perCent[nCentBins];
TH3F* h3d_Rc_PtEtaPhi_perCent[nCentBins][nCuts];
TH2F* h_PtDiff4Res[nCentBins];
TH2F* h_PtDiff4Res_aftnorm[nCentBins];


