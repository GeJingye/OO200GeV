#ifndef StPicoDstarMixedMaker_H
#define StPicoDstarMixedMaker_H

/* **************************************************
 *  A Maker to read a StPicoEvent and StPicoDstarEvent
 *  simultaneously and do analysis.
 *
 *  Authors: Guannan Xie <guannanxie@lbl.gov>
 *           Mustafa Mustafa <mmustafa@lbl.gov>
 *
 * **************************************************
 */
 /* **************************************************************************************
  * read PicoDst document about OO 200GeV collision for produciton within iTPC acceptance*
  * **************************************************************************************
  */
#include "TChain.h"
#include "StMaker.h"
#include "StThreeVectorF.hh"
#include "StPicoEvent/StPicoDst.h"
#include "../StRefMultCorr/StRefMultCorr.h"
#include "../StRefMultCorr/CentralityMaker.h"
#include "TProfile.h"
#include "TH1.h"
#include "TH2.h"
#include "TH3.h"
#include "TF1.h"
#include "TLorentzVector.h"
class TString;
class TFile;
class TNtuple;
class StPicoTrack;
class StPicoDstMaker;
class StPicoEvent;
class ParticleInfo;

class ParticleInfo
{
  public:
	Int_t charge;
	Float_t pt;
	Float_t eta;
	Float_t phi;
	Float_t p;
	Float_t nSigmaE;
	Float_t nSigmaEcorr;
	// Float_t nSigmaPi;
	// Float_t nSigmaK;
	// Float_t nSigmaP;
	Float_t beta;
	// Float_t betaElectron;
	// Float_t dBetaElectron;
	Float_t energy;
	Float_t p1;
	Float_t p2;
	Float_t p3;
	Bool_t isPhotonicE;//isPhotonicE
    Bool_t isCurlingE;
    Bool_t isCloneE;
	Bool_t isPureE;
};

class StPicoDstarMixedMaker : public StMaker
{
  public:
    StPicoDstarMixedMaker(Char_t const *name, TString const inputFilesList, TString const outBaseName, StPicoDstMaker* picoDstMaker);
    virtual ~StPicoDstarMixedMaker();

    virtual Int_t Init();
    virtual Int_t Make();
    virtual Int_t Finish();

  private:
    StPicoDstMaker* mPicoDstMaker;
	StRefMultCorr* mRefMultCorrUtil;
	TString mInputFilesList;
	TString mOutFileBaseName;

    StPicoDstarMixedMaker() {}
    void initHists();
    Bool_t isGoodTrigger(StPicoEvent const*) const;
    Bool_t isGoodEvent(StPicoEvent const* const picoEvent) const;
    Bool_t isGoodTrack(StPicoTrack const* trk, StPicoEvent const* const picoEvent) const;

    Float_t getTofBeta(StPicoTrack const* const trk) const;
    Float_t getETofBeta(StPicoTrack const* const trk) const;
	Double_t getPhiVAngle(TLorentzVector e1, TLorentzVector e2, Int_t q1, Int_t q2) const;
    Int_t getRefmult6(StPicoDst const* const picoDst, StPicoEvent const* const picoEvent) const;
    Double_t getTotnMIP(StPicoDst const* const picoDst) const;
	Double_t getNSigmaECorr(TVector3 mom) const;
	Double_t getNSigmaPiKPCorr(Int_t num_variable, TVector3 mom) const;
	void copyCurrentToBuffer();
    
    // -------------- USER variables -------------------------
    // add your member variables here. 
    // Remember that ntuples size can be really big, use histograms where appropriate 

  public:
    void setRunNumList(string list){
      mRunNumList = list;
    }
    void getBadruns(string inputFileName);
    void getBadTofCells(string inputFileName);
    Bool_t isBadrun(Int_t runId);
    Bool_t isGoodBTofCell(Int_t const tofid);

  private: 
    TFile* mFile;
	std::map<Int_t, Int_t> mrunnum;
	string mRunNumList;
    vector<Int_t> mBadRun;
    vector<Int_t> mBadTofCell;
    // Event level
    Int_t  mRunId;
    // primaryVertex vertex
    Float_t mVx;
    Float_t mVy;
    Float_t mVr;
    Float_t mVz;
    Float_t mVpdVz;

	Float_t mBfield;
    Int_t mRefmult6;
    Float_t mTotnMIP;
	Int_t mCen16,mCen9;

    TF1* fphiVcut;
    
    TH1F* h_RunNum;
    TH1F* h_passEvtcut;
	TH1D* h_passTrkcut;//float最大能表示16777215，所以用TH1D
    TH1F* h_cen;
	TH1F* h_cen__reWeight;
    TH1F* h_RefMult6;
    TH1F* h_TotnMIP;

    
    // Histograms
    // event QA
	TH2F* h_Vx_Vy;
	TH1F* h_Vr;
    TH1F* h_Vz;
    TH1F* h_VpdVz;
    TH2F* h_VpdVz_Vz;
    TH1F* h_VpdVzmVz;
    TH2F* h_nTofMat_RefMul;
    TH1F* h_RefMult;

    // track level QA
    TH1F* h_nHitsFit;
    TH1F* h_nHitsPoss;
    TH1F* h_nHitsDEdx;
	TH3F* h_nHitsFit_Pt_Eta;
    TH3F* h_nHitsDEdx_Pt_Eta;
	TH3F* h_pDca_Pt_Eta;
    TH2F* h_dEdx_Pc;
	TH1F* h_m2;
    TH2F* h_m2_Pc;
	TH2F* h_nSigmaElectron_P;
	TH2F* h_nSigmaEcorr_P;
	TH2F* h_nSigmaPion_P;
	TH2F* h_nSigmaPicorr_P;
	TH2F* h_nSigmaKaon_P;
	TH2F* h_nSigmaProton_P;

    TH1F* h_pDca;
    TH1F* h_ppT;
	TH1F* h_pP;
	TH2F* h_pP_ppT;
	TH1F* h_gPt;
    TH1F* h_pEta;
    TH1F* h_pPhi;
    TH2F* h_ppTc_pEta;
    TH2F* h_ppTc_pPhi;

    TH2F* h_invBeta_P__TOFMatch;
    TH2F* h_invBeta_P__eTOFMatch;
    TH1F* h_pT__TOFMatch;
	TH1F* h_Eta__TOFMatch;
	TH1F* h_Phi__TOFMatch;
    TH1F* h_pT__eTOFMatch;
	TH1F* h_Eta__eTOFMatch;
	TH1F* h_Phi__eTOFMatch;
    // calibrate
	TH3F* h_Pt_Cen_nSigmaE;
	TH3F* h_Eta_Cen_nSigmaE;
	TH3F* h_Phi_Cen_nSigmaE;
	TH3F* h_Pt_Cen_nSigmaEcorr;
	TH3F* h_Eta_Cen_nSigmaEcorr;
	TH3F* h_Phi_Cen_nSigmaEcorr;
	TH3F* h_Pt_Cen_nSigmaPion;
	TH3F* h_Eta_Cen_nSigmaPion;
	TH3F* h_Phi_Cen_nSigmaPion;
	TH3F* h_Pt_Cen_nSigmaPicorr;
	TH3F* h_Eta_Cen_nSigmaPicorr;
	TH3F* h_Phi_Cen_nSigmaPicorr;
	TH3F* h_Pt_Cen_nSigmaE__PureE;
	TH3F* h_Eta_Cen_nSigmaE__PureE;
	TH3F* h_Phi_Cen_nSigmaE__PureE;
	// group 1 2 3
	TH2F* h_nSigmaElectron_P__1;
	TH2F* h_nSigmaElectron_P__2;
	TH2F* h_nSigmaElectron_P__3;
    TH2F* h_nSigmaElectron_P__4;
	TH2F* h_nSigmaElectron_P__TOFMatch;
	TH2F* h_nSigmaElectron_P__eTOFMatch;
	TH2F* h_nSigmaElectron_P__PID_2;
	TH2F* h_nSigmaElectron_P__PID_3;
	TH2F* h_nSigmaElectron_P__EIDcut_1;
	TH2F* h_nSigmaElectron_P__EIDcut_2;
	TH2F* h_nSigmaElectron_Eta__EIDcut_2;
	TH2F* h_nSigmaElectron_P__EIDcut_3;
	TH2F* h_nSigmaElectron_Eta__EIDcut_3_lowP;
	TH2F* h_nSigmaElectron_Eta__EIDcut_3_highP;
	TH2F* h_nSigmaElectron_P__EIDcut_4;
	TH2F* h_nSigmaElectron_P__EIDcut_total;
    TH2F* h_nSigmaKaon_P__2;
	TH2F* h_nSigmaPion_P__3;
    TH2F* h_nSigmaKaon_P__3;
    TH2F* h_nSigmaProton_P__2;
	// phiV cut
	TH2F* h_e_p__Number_Same;
	TH2F* h_e_p__Number_Mix;
	TH1F* h_pT__positrons;
	TH1F* h_eta__positrons;
	TH1F* h_phi__positrons;
	TH1F* h_pT__positrons_w_PhiV_Cut;
	TH1F* h_eta__positrons_w_PhiV_Cut;
	TH1F* h_phi__positrons_w_PhiV_Cut;
	TH1F* h_pT__electrons;
	TH1F* h_eta__electrons;
	TH1F* h_phi__electrons;
	TH1F* h_pT__electrons_w_PhiV_Cut;
	TH1F* h_eta__electrons_w_PhiV_Cut;
	TH1F* h_phi__electrons_w_PhiV_Cut;

    // pair 信息
    TH1F* h_Qinv__likemm;
	TH1F* h_Qinv__likepp;
	TH1F* h_Qinv__unlikeSame;
	TH1F* h_Qinv__likemmMixed;
	TH1F* h_Qinv__likeppMixed;
	TH1F* h_Qinv__unlikeMixed;
	TH1F* h_DeltaPt__unlikeSame;
	TH1F* h_DeltaEta__unlikeSame;
    TH1F* h_SumEta__unlikeSame;
	TH1F* h_DeltaPhi__unlikeSame;
    TH1F* h_DeltaR__unlikeSame;
	TH1F* h_DeltaPt__likepp;
	TH1F* h_DeltaEta__likepp;
	TH1F* h_DeltaPhi__likepp;
    TH1F* h_DeltaR__likepp;
	TH1F* h_DeltaPt__likemm;
	TH1F* h_DeltaEta__likemm;
	TH1F* h_DeltaPhi__likemm;
	TH2F* h_DeltaEta_DeltaPhi__likemm;
	TH2F* h_DeltaEta_DeltaPhi__likepp;
    TH1F* h_DeltaR__likemm;
    TH1F* h_DeltaR__unlikeMixed;
    TH1F* h_DeltaR__likeppMixed;
    TH1F* h_DeltaR__likemmMixed;
	TH1F* h_DeltaPt__unlikeMixed;
	TH1F* h_DeltaEta__unlikeMixed;
	TH1F* h_DeltaPhi__unlikeMixed;
    TH1F* h_Phi__likemm_deltaPhi_0p2;
    TH1F* h_Eta__likemm_deltaPhi_0p2;
    TH1F* h_Pt__likemm_deltaPhi_0p2;
    TH1F* h_Phi__likepp_deltaPhi_0p2;
    TH1F* h_Eta__likepp_deltaPhi_0p2;
    TH1F* h_Pt__likepp_deltaPhi_0p2;
    TH1F* h_Phi__unlike_deltaPhi_3p0;
    TH1F* h_Eta__unlike_deltaPhi_3p0;
    TH1F* h_DeltaEta__likemm_deltaPhi_0p2;
    TH1F* h_DeltaPt__likemm_deltaPhi_0p2;
    TH1F* h_DeltaEta__likepp_deltaPhi_0p2;
    TH1F* h_DeltaPt__likepp_deltaPhi_0p2;
    TH1F* h_Mee__unlike_deltaPhi_3p0;
    TH1F* h_Mee__likemm_deltaPhi_0p2;
    TH1F* h_Mee__likepp_deltaPhi_0p2;

    TH1F* h_Rapidity__unlikeSame;
    TH2F* h_Mee_PhiV__unlikeSame;
    TH1F* h_Mee__unlikeSame;
    TH1F* h_Mee__unlikeSame__w_PhiV_Cut;

    TH3F* h_Mee_Pt_Cen__unlikeSame;
    TH3F* h_Mee_Pt_Cen__likemm;
    TH3F* h_Mee_Pt_Cen__likepp;
    TH3F* h_Mee_Pt_Cen__unlikeSame_Ro;
    TH3F* h_Mee_Pt_Cen__likemm_Ro;
    TH3F* h_Mee_Pt_Cen__likepp_Ro;
    TH3F* h_Mee_Pt_Cen__unlikeMixed;
    TH3F* h_Mee_Pt_Cen__likemmMixed;
    TH3F* h_Mee_Pt_Cen__likeppMixed;

    const Float_t M_electron=0.000511;// GeV
    const Float_t M_positron=0.000511;// GeV
    const Float_t M_pion=0.139570;// GeV
    const Float_t M_kaon=0.493677;// GeV
    const Float_t M_proton=0.938272;// GeV

    ClassDef(StPicoDstarMixedMaker, 1)
};

inline void StPicoDstarMixedMaker::getBadruns(string inputFileName)
{
    ifstream fin(inputFileName.c_str());
    if(!fin)
    {
      cout <<"no Bad runs list" << endl;
      return;
    }
    cout << "  " << inputFileName << flush;

    Int_t runId = 0 ;
    while( fin >> runId )
    {
      mBadRun.push_back(runId);
    }
    cout << "get Bad runs list: [OK]" << endl;
}

inline void StPicoDstarMixedMaker::getBadTofCells(string inputFileName)
{
    ifstream fin(inputFileName.c_str());
    if(!fin)
    {
      cout << "no Bad BTOF Cells list" << endl;
      return;
    }
    cout << "  " << inputFileName << flush;

    Int_t runId = 0 ;
    while( fin >> runId )
    {
      mBadTofCell.push_back(runId);
    }
    cout << "get Bad BTOF Cells list: [OK]" << endl;
}

inline Bool_t StPicoDstarMixedMaker::isBadrun(Int_t runId)
{
    vector<Int_t>::iterator iter = std::find(mBadRun.begin(), mBadRun.end(), runId);
    return ( iter != mBadRun.end() ) ;
}

inline Bool_t StPicoDstarMixedMaker::isGoodBTofCell(Int_t const tofid)
{
  if(std::count(mBadTofCell.begin(),mBadTofCell.end(),tofid)>0) return kFALSE;
  else return kTRUE;
}

#endif
