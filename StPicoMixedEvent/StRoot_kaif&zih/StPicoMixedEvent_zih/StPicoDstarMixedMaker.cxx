// ****************************************************
// *                                                  *
// *  Authors: Yuanjing Ji                            *
// *           Guannan Xie <guannanxie@lbl.gov>       *
// *           Mustafa Mustafa <mmustafa@lbl.gov>     *
// *                                                  *
// ****************************************************

#include <iostream>
#include <fstream>
#include <string>
#include <cmath>

#include "TFile.h"
#include "TMath.h"
#include "TClonesArray.h"
#include "TTree.h"
#include "TNtuple.h"

#include "StEvent/StDcaGeometry.h"
#include "StPhysicalHelixD.hh"
#include "phys_constants.h"
#include "StPicoEvent/StPicoBTofPidTraits.h"
#include "StPicoEvent/StPicoETofPidTraits.h"
#include "StBTofUtil/tofPathLength.hh"
#include "StPicoDstMaker/StPicoDstMaker.h"
#include "StPicoEvent/StPicoDst.h"
#include "StPicoEvent/StPicoEvent.h"
#include "StPicoEvent/StPicoTrack.h"
#include "StPicoEvent/StPicoEpdHit.h"
#include "StPicoEvent/StPicoBEmcPidTraits.h"
#include "StPicoEvent/StPicoMtdPidTraits.h"
#include "StPicoDstarMixedMaker.h"
#include "StAnaCuts.h"
// #include "calmean.h"
#include "TLorentzVector.h"
#include "TVector3.h"
#include "TRandom.h"
#include "TRandom3.h"
#ifndef DEBUG
#define DEBUG 1
#endif
// Mixed Event
//mMagBins=2; mCenBins=9; mVzBins=20; mEveBins=12; mMaxEventsInBuffer=500;
// const Int_t mMagBins = 2; //1;
const Int_t mCenBins = 9;
const Int_t mVzBins = 20; //10;
const Int_t mEveBins = 12; //12;

const Int_t mMaxEventsInBuffer = 100; //50;
const Int_t mMaxElectrons = 30;
const Float_t mPhiVCutMRange = 0.2;
//Float_t current_EQx[mMaxElectrons],current_EQy[mMaxElectrons];

Int_t current_nEPlus,current_nEMinus;
TLorentzVector current_ePlus[mMaxElectrons];
TLorentzVector current_eMinus[mMaxElectrons];

Int_t magBufferPointer, cenBufferPointer, vzBufferPointer, eveBufferPointer;
Int_t nEventsInBuffer[mCenBins][mVzBins][mEveBins];
Bool_t bufferFullFlag[mCenBins][mVzBins][mEveBins];
Int_t buffer_nEPlus[mCenBins][mVzBins][mEveBins][mMaxEventsInBuffer];
Int_t buffer_nEMinus[mCenBins][mVzBins][mEveBins][mMaxEventsInBuffer];
TLorentzVector buffer_ePlus[mCenBins][mVzBins][mEveBins][mMaxEventsInBuffer][mMaxElectrons];
TLorentzVector buffer_eMinus[mCenBins][mVzBins][mEveBins][mMaxEventsInBuffer][mMaxElectrons];

TRandom3* myRandom;
TRandom3* myRandom_phi;
float bfield = 0;
// Double_t Mee_bins[] = {0, 0.02, 0.04, 0.06, 0.08, 0.1, 0.12, 0.16, 0.2, 0.25, 0.3, 0.35, 0.4, 0.45, 0.5, 0.55, 0.6, 0.65, 0.7, 0.76, 0.77, 0.78, 0.8, 0.83, 0.92, 0.99, 1.01, 1.03, 1.05, 1.1, 1.2, 1.5, 1.8, 2.2, 2.6, 2.8, 2.98, 3.03, 3.08, 3.11, 3.15, 3.2, 4.0};
// int N_Mee_bins = sizeof(Mee_bins)/sizeof(Mee_bins[0])-1;

ClassImp(StPicoDstarMixedMaker)
  StPicoDstarMixedMaker::StPicoDstarMixedMaker(char const * name, TString const inputFilesList, TString const outFileBaseName, StPicoDstMaker* picoDstMaker):
    StMaker(name), mPicoDstMaker(picoDstMaker),
    mInputFilesList(inputFilesList), mOutFileBaseName(outFileBaseName),mRunbyRunQA(true)
{}

Int_t StPicoDstarMixedMaker::Init()
{
  mOutFileBaseName = mOutFileBaseName.ReplaceAll(".root", "");
  // -------------- USER VARIABLES -------------------------
  mFile = new TFile(mOutFileBaseName+".QA.root", "RECREATE");
  //mFile_RunID = new TFile(mOutFileBaseName+".RunID.root","RECREATE");
  //initialize trees
  initHists();

  return kStOK;
}
//-----------------------------------------------------------------------------
StPicoDstarMixedMaker::~StPicoDstarMixedMaker()
{}
//-----------------------------------------------------------------------------
void StPicoDstarMixedMaker::initHists()
{
  int totalNum = 129;//200GeV RFF+FF OO 2021 mb
  // int totalNum = 59;//200GeV RFF OO 2021 mb

  TH1::SetDefaultSumw2(kTRUE);
  
  //char name_RunID[100];

  if (mRunbyRunQA)
  {
    ifstream readnum;
    readnum.open(mRunNumList);

    int tmp=0;
    if (DEBUG) cout<<"start initial run number..."<<endl;
    for (int i=0;i<totalNum;i++)
    {
      readnum>>tmp;
      runnum.insert(pair<int,int>(tmp,i));
      if (DEBUG) cout <<"run number : " << tmp <<" id :" <<runnum[tmp] <<endl;
    }
    readnum.close();
  }

  myRandom = new TRandom3();
  myRandom_phi = new TRandom3();
  memset(nEventsInBuffer,0,sizeof(nEventsInBuffer));  
  memset(bufferFullFlag,0,sizeof(bufferFullFlag));  
  memset(buffer_nEPlus,0,sizeof(buffer_nEPlus));  
  memset(buffer_nEMinus,0,sizeof(buffer_nEMinus));  

  // mRefMultCorrUtil = CentralityMaker::instance()->getRefMult6Corr();
  mRefMultCorrUtil = CentralityMaker::instance()->getTotnMIPCorr();
  mRefMultCorrUtil->setVerbose(kFALSE);

  TFile* file = TFile::Open(mRecenterFile.c_str());
  if (!file->IsOpen())
  {
    cout << "Error: Cannot read recenter file!!!" << endl;
    // return kStWarn;
  }
  for (int ic=0;ic<9;ic++)
  {
    pQxRecenter_etaplus_zplus[ic] = (TProfile*)file->Get(Form("pQxRecenter_etaplus_zplus_%d",ic));
    pQxRecenter_etaplus_zplus[ic]->SetDirectory(0);
    pQyRecenter_etaplus_zplus[ic] = (TProfile*)file->Get(Form("pQyRecenter_etaplus_zplus_%d",ic));
    pQyRecenter_etaplus_zplus[ic]->SetDirectory(0);    
    pQxRecenter_etaplus_zminus[ic] = (TProfile*)file->Get(Form("pQxRecenter_etaplus_zminus_%d",ic));
    pQxRecenter_etaplus_zminus[ic]->SetDirectory(0);
    pQyRecenter_etaplus_zminus[ic] = (TProfile*)file->Get(Form("pQyRecenter_etaplus_zminus_%d",ic));
    pQyRecenter_etaplus_zminus[ic]->SetDirectory(0);
    pQxRecenter_etaminus_zplus[ic] = (TProfile*)file->Get(Form("pQxRecenter_etaminus_zplus_%d",ic));
    pQxRecenter_etaminus_zplus[ic]->SetDirectory(0);
    pQyRecenter_etaminus_zplus[ic] = (TProfile*)file->Get(Form("pQyRecenter_etaminus_zplus_%d",ic));
    pQyRecenter_etaminus_zplus[ic]->SetDirectory(0);
    pQxRecenter_etaminus_zminus[ic] = (TProfile*)file->Get(Form("pQxRecenter_etaminus_zminus_%d",ic));
    pQxRecenter_etaminus_zminus[ic]->SetDirectory(0);
    pQyRecenter_etaminus_zminus[ic] = (TProfile*)file->Get(Form("pQyRecenter_etaminus_zminus_%d",ic));
    pQyRecenter_etaminus_zminus[ic]->SetDirectory(0);
  }  
  file->Close();

  file = TFile::Open(mShiftFile.c_str());
  if (!file->IsOpen())
  {
    cout << "Error: Cannot read shift file!!!" << endl;
    // return kStWarn;
  }
  for (int ic=0;ic<9;ic++)
  {
    pSinEta_Shuai[ic] = (TProfile*)file->Get(Form("pSinEta_Shuai_Cent%d",ic));
    pSinEta_Shuai[ic]->SetDirectory(0);
    pCosEta_Shuai[ic] = (TProfile*)file->Get(Form("pCosEta_Shuai_Cent%d",ic));
    pCosEta_Shuai[ic]->SetDirectory(0);
  }  
  file->Close();
  
  for(int i=0; i<9; i++)
  {
    hEP_Shuai_Raw[i] = new TH1F(Form("hEP_Shuai_Raw_%d",i),";#Psi_{2}^{Shuai};Count", 140, -PI/12., PI+PI/12.);
    hEP_Shuai_Recenter[i] = new TH1F(Form("hEP_Shuai_Recenter_%d",i),";#Psi_{2}^{Shuai};Count", 140, -PI/12., PI+PI/12.);
    hEP_Shuai_Shift[i] = new TH1F(Form("hEP_Shuai_Shift_%d",i),";#Psi_{2}^{Shuai};Count", 140, -PI/12., PI+PI/12.);
    hEP_Shuai_Raw_woele[i] = new TH1F(Form("hEP_Shuai_Raw_woele_%d",i),";#Psi_{2}^{Shuai} (w/o ele);Count", 140, -PI/12., PI+PI/12.);
    hEP_Shuai_Recenter_woele[i] = new TH1F(Form("hEP_Shuai_Recenter_woele_%d",i),";#Psi_{2}^{Shuai} (w/o ele);Count", 140, -PI/12., PI+PI/12.);
    hEP_Shuai_Shift_woele[i] = new TH1F(Form("hEP_Shuai_Shift_woele_%d",i),";#Psi_{2}^{Shuai} (w/o ele);Count", 140, -PI/12., PI+PI/12.);
  }

  phiVcut = new TF1("phiVcut","0.84326*exp((-49.4819)*x)+(-0.996609)*x+(0.19801)",0.0,1.0);

  // event hists
  hevt = new TH1F("hevt","hevt",totalNum,0,totalNum);
  hevt_cen = new TH1F("hevt_cen","hevt_cen",17,-1.5,15.5);
  hevt_cen_reweight = new TH1F("hevt_cen_reweight","hevt_cen_reweight",17,-1.5,15.5);
  // hRefMult6 = new TH1F("hRefMult6","hRefMult6",250,0,250);
  // hTotnMIP = new TH1F("hTotnMIP","hTotnMIP",1000,0,1000);
    
  // hpassevtcut = new TH1F("hpassevtcut","pass event cut", 8, -0.5, 7.5);
  // hpassevtcut->GetXaxis()->SetBinLabel(1, "All");
  // hpassevtcut->GetXaxis()->SetBinLabel(2, "Good Events");
  // hpassevtcut->GetXaxis()->SetBinLabel(3, "passVz");
  // hpassevtcut->GetXaxis()->SetBinLabel(4, "passVzVpdVz");
  // hpassevtcut->GetXaxis()->SetBinLabel(5, "passVerr");
  // hpassevtcut->GetXaxis()->SetBinLabel(6, "passVr");
  // hpassevtcut->GetXaxis()->SetBinLabel(7, "notPileUp");
  // hpassevtcut->GetXaxis()->SetBinLabel(8, "0-80%");
    
  hpassevtcut = new TH1F("hpassevtcut","pass event cut", 7, -0.5, 6.5);
  hpassevtcut->GetXaxis()->SetBinLabel(1, "All");
  hpassevtcut->GetXaxis()->SetBinLabel(2, "Good Events");
  hpassevtcut->GetXaxis()->SetBinLabel(3, "passVz");
  hpassevtcut->GetXaxis()->SetBinLabel(4, "passVerr");
  hpassevtcut->GetXaxis()->SetBinLabel(5, "passVr");
  hpassevtcut->GetXaxis()->SetBinLabel(6, "notPileUp");
  hpassevtcut->GetXaxis()->SetBinLabel(7, "0-80%");

  for(int i=1; i<hevt_cen->GetNbinsX()+1; i++)
  {
      if(i==1){
        hevt_cen->GetXaxis()->SetBinLabel(i, "80-100%");
        hevt_cen_reweight->GetXaxis()->SetBinLabel(i, "80-100%");
      }
      else{
        hevt_cen->GetXaxis()->SetBinLabel(i, Form("%d-%d%%", 85-5*i, 90-5*i));
        hevt_cen_reweight->GetXaxis()->SetBinLabel(i, Form("%d-%d%%", 85-5*i, 90-5*i));
      }
  }

  // hVxVyVz = new TH3F("hVxVyVz","VxVyVz;Vx(cm);Vy(cm);Vz(cm)",250,-5,5,250,-5,5, 400,-200,200);
  // h_Vx_Vy = new TH2F("h_Vx_Vy","Vertex XY",1400,-7,7,1400,-7,7);
  // hVr = new TH1F("hVr","Vr;Vr(cm);Counts",400,0,4);
  // hVz = new TH1F("hVz","Vz;Vz(cm);Counts",400,-200,200);
  // hVpdVz = new TH1F("hVpdVz","VpdVz;VpdVz(cm);Counts",400,-200,200);
  // hVzVpdVz = new TH1F("hVzVpdVz","VpdVz-Vz;VpdVz-Vz(cm);Counts",2000,-200,200);
  // h_Vz_VpdVz = new TH2F("h_Vz_VpdVz","Vz vs VpdVz;Vz(cm);VpdVz(cm)",400,-200,200,400,-200,200);

  // hnTofMatvsRef= new TH2F("hnTofMatvsRef","RefMul VS nTofmatch;RefMul;nTofMatch",250,0,250,250,0,250);
  // hrefmult = new TH1F("hrefmult","hrefmult",250,0,250);
  // hDCA_Eta_NHitsFit = new TH3F("hDCA_Eta_NHitsFit","hDCA_Eta_NHitsFit;DCA;Eta;NHitsFit",50,0,5,60,-1.5,1.5,90,0,90);

  // //tracl hists
  // hnHitsFit = new TH1F("hnHitsFit","nHitsFit;nHitsFit",180,-90,90);
  // hnHitsPoss = new TH1F("hnHitsPoss","nHitsPoss;nHitsPoss",180,-90,90);
  // hnHitsDedx = new TH1F("hnHitsDedx","nHitsDedx;nHitsDedx",180,-90,90);
  // h_nHitsDedx_p = new TH2F("h_nHitsDedx_p","nHitsDedx vs p*charge;p*charge (GeV/c);nHitsDedx",400,-10,10,90,0,90);
  // hinvBetavsP = new TH2F("hinvBetavsP","#frac{1}{#beta} vs p;p(GeV/c);#frac{1}{#beta}",300,0,3,5000,0,5);
  // hdEdx = new TH2F("hdEdx","dEdx vs p*charge;p*charge(GeV/c);dEdx",200,-2,2,400,0,25);
  // h_m2 = new TH1F("h_m2","mass^{2};mass^{2}",2000,-0.5,1.5);
  // h_mTpc = new TH2F("h_mTpc","mass^{2} vs p*charge;p*charge(GeV/c);mass square (GeV/c^{2})^{2}",2000,-2,2,1200,0,1.2);
  // hpDca = new TH1F("hpDca","pDca",50,0,5);
  // hppt = new TH1F("hppt","primary p_{T};hpt;primary p_{T}(GeV/c)",240,0,12);
  // hpEta = new TH1F("hpEta","primary Eta;Eta;#eta",80,-2,2);
  // hpPhi = new TH1F("hpPhi","primary Phi;Phi;#phi",80,-4,4);
  // h_pT_Eta = new TH2F("h_pT_Eta","p_{T}*charge vs #eta;p_{T}*charge (GeV/c);#eta",400,-10,10,500,-2.5,2.5);
  // h_pT_Phi = new TH2F("h_pT_Phi","p_{T}*charge vs #phi;p_{T}*charge (GeV/c);#phi",400,-10,10,800,-4,4);

  // h_pT_TOFmatch = new TH1F("h_pT_TOFmatch","p_{T} of TOF matched tracks;p_{T}(GeV/c)",240,0,12);

  // h_pT_electrons = new TH1F("h_pT_electrons","RFF p_{T} of electrons;p_{T}(GeV/c)",240,0,12);
  // h_eta_electrons = new TH1F("h_eta_electrons","RFF #eta of electrons;#eta",40,-1,1);
  // h_eta_electrons_w_PhiV_Cut = new TH1F("h_eta_electrons_w_PhiV_Cut","RFF #eta of electrons with phiV cut;#eta",40,-1,1);
  // h_phi_electrons = new TH1F("h_phi_electrons","RFF #phi of electrons;#phi",80,-4,4);
  // h_phi_electrons_w_PhiV_Cut = new TH1F("h_phi_electrons_w_PhiV_Cut","RFF #phi of electrons with phiV cut;#phi",80,-4,4);

  // h_nSigmaElectron_P_tpc = new TH2F("h_nSigmaElectron_P_tpc","nSigmaElectron_P;P(GeV/c);nSigmaElectron",300,0,3,4000,-20,20);
  // h_nSigmaPion_P_tpc = new TH2F("h_nSigmaPion_P_tpc","nSigmaPion_P;P(GeV/c);nSigmaPion",300,0,3,4000,-20,20);
  // h_nSigmaKaon_P_tpc = new TH2F("h_nSigmaKaon_P_tpc","nSigmaKaon_P;P(GeV/c);nSigmaKaon",300,0,3,4000,-20,20);
  // h_nSigmaProton_P_tpc = new TH2F("h_nSigmaProton_P_tpc","nSigmaProton_P;P(GeV/c);nSigmaProton",300,0,3,4000,-20,20);
  // h_nSigmaElectron_P = new TH2F("h_nSigmaElectron_P","nSigmaElectron_P;P(GeV/c);nSigmaElectron",300,0,3,4000,-20,20);

  // hMeeCountPhiV = new TH2F("hMeePhiV","hMee vs phiV;Mee(GeV/c^{2});phiV",40000,0,4,100,0,1);
  // hMeeCount_wo_PhiV_Cut = new TH1F("hMee_wo_PhiV_Cut","hMee without phiV cut;Mee(GeV/c^{2})",40000,0,4);
  // hMeeCount_w_PhiV_Cut = new TH1F("hMee_w_PhiV_Cut","hMee with phiV cut;Mee(GeV/c^{2})",40000,0,4);

  hMeeCountPtCen = new TH3F("hMeePtTPCTrks","hMee vs p_{T} vs Cen;Mee(GeV/c^{2});p_{T};Cen",800,0,4,120,0,6,16,0,16);
  hMeeCountPtCen_like1 = new TH3F("hMeePtTPCTrks_like1","hMee vs p_{T} vs Cen like sign electron;Mee(GeV/c^{2});p_{T};Cen",800,0,4,120,0,6,16,0,16);
  hMeeCountPtCen_like2 = new TH3F("hMeePtTPCTrks_like2","hMee vs p_{T} vs Cen like sign positron;Mee(GeV/c^{2});p_{T};Cen",800,0,4,120,0,6,16,0,16);

  hMeeMixPtCen = new TH3F("hMeeMix_Pt_Cen","hMeeMix vs p_{T} vs Cen;Mee(GeV/c^{2});p_{T};Cen",800,0,4,120,0,6,16,0,16);
  hMeeMixPtCen_like1 = new TH3F("hMeeMixlike1_Pt_Cen","hMeeMix vs p_{T} vs Cen like sign electron;Mee(GeV/c^{2});p_{T};Cen",800,0,4,120,0,6,16,0,16);
  hMeeMixPtCen_like2 = new TH3F("hMeeMixlike2_Pt_Cen","hMeeMix vs p_{T} vs Cen like sign positron;Mee(GeV/c^{2});p_{T};Cen",800,0,4,120,0,6,16,0,16);

}
//-----------------------------------------------------------------------------
Int_t StPicoDstarMixedMaker::Finish()
{
  mFile->cd();

  hevt->Write();
  hpassevtcut->Write();
  hevt_cen->Write();
  hevt_cen_reweight->Write();
  // hRefMult6->Write();
  // hTotnMIP->Write();
  
  //write the hists  
  // //event QA
  // hVxVyVz->Write();
  // hVz->Write();
  // hVpdVz->Write();
  // hVr->Write();
  // hVzVpdVz->Write();
  // h_Vz_VpdVz->Write();
  // h_Vx_Vy->Write();
  // hnTofMatvsRef->Write(); 
  // hrefmult->Write();
  // hDCA_Eta_NHitsFit->Write();

  // for(int ic=0;ic<9;ic++)
  // {
  //   hEP_Shuai_Raw[ic]->Write();
  //   hEP_Shuai_Recenter[ic]->Write();
  //   hEP_Shuai_Shift[ic]->Write();
  //   hEP_Shuai_Raw_woele[ic]->Write();
  //   hEP_Shuai_Recenter_woele[ic]->Write();
  //   hEP_Shuai_Shift_woele[ic]->Write();
  // }

  // //tracl level QA
  // hnHitsFit->Write();
  // hnHitsPoss->Write();
  // hnHitsDedx->Write();
  // h_nHitsDedx_p->Write();
  // hinvBetavsP->Write();
  // hdEdx->Write();
  // h_m2->Write();
  // h_mTpc->Write();
  // hpDca->Write();
  // hppt->Write();
  // hpEta->Write();
  // hpPhi->Write();
  // h_pT_Eta->Write();
  // h_pT_Phi->Write();

  // h_pT_TOFmatch->Write();

  // h_pT_electrons->Write();
  // h_eta_electrons->Write();
  // h_eta_electrons_w_PhiV_Cut->Write();
  // h_phi_electrons->Write();
  // h_phi_electrons_w_PhiV_Cut->Write();

  // h_nSigmaElectron_P_tpc->Write();
  // h_nSigmaPion_P_tpc->Write();
  // h_nSigmaKaon_P_tpc->Write();
  // h_nSigmaProton_P_tpc->Write();  
  // h_nSigmaElectron_P->Write();

  // hMeeCountPhiV->Write();
  // hMeeCount_wo_PhiV_Cut->Write();
  // hMeeCount_w_PhiV_Cut->Write();

  hMeeCountPtCen->Write();
  hMeeCountPtCen_like1->Write();
  hMeeCountPtCen_like2->Write();

  hMeeMixPtCen->Write();
  hMeeMixPtCen_like1->Write();
  hMeeMixPtCen_like2->Write();

  mFile->Close();

  return kStOK;
}
//-----------------------------------------------------------------------------
Int_t StPicoDstarMixedMaker::Make()
{
  //cout<<"star make"<<endl;

  ParticleInfo_Electron particleinfo;
  vector<ParticleInfo_Electron> electroninfo;
  vector<ParticleInfo_Electron> positroninfo;

  // StMemStat mem;
  if (!mPicoDstMaker)
  {
    LOG_WARN << " StPicoDstarMixedMaker - No PicoDstMaker! Skip! " << endm;
    return kStWarn;
  }
  StPicoDst const* picoDst = mPicoDstMaker->picoDst();
  if (!picoDst)
  {
    LOG_WARN << "StPicoDstarMixedMaker - No PicoDst! Skip! " << endm;
    return kStWarn;
  }
  // -------------- USER ANALYSIS -------------------------
  StPicoEvent const * picoEvent = picoDst->event();

  //trigger
  if (!isGoodTrigger(picoEvent)) return 0;    
  mRunId = picoEvent->runId();
  hevt->Fill(runnum[mRunId]);

  TVector3 pVtx = picoEvent->primaryVertex();  
  mVx = pVtx.x();
  mVy = pVtx.y();
  mVz = pVtx.z();
  mVr = sqrt(mVx*mVx+mVy*mVy);    
  mVpdVz = picoEvent->vzVpd();
  
  electroninfo.clear();
  positroninfo.clear();

  //event and track level QA
  hpassevtcut->Fill(0);
  if (!isBadrun(mRunId))
  {
    hpassevtcut->Fill(1); //bad run list   
  
    mRefmult6 = getRefmult6(picoDst, picoEvent);
    mTotnMIP = getTotnMIP(picoDst);    
    mRefMultCorrUtil->init(mRunId);
    // mRefMultCorrUtil->initEvent(mRefmult6, mVz);
    mRefMultCorrUtil->initEvent(mTotnMIP, mVz);
    // hRefMult6->Fill(mRefmult6);
    // hTotnMIP->Fill(mTotnMIP);

    int mCen9 = mRefMultCorrUtil->getCentralityBin9();
    int mCen16 = mRefMultCorrUtil->getCentralityBin16();
    double reWeight = mRefMultCorrUtil->getWeight();
    
    // bool vzcut = fabs(mVz) < anaCuts::vz;
    // bool vzVpdVzcut = fabs(mVz - mVpdVz) < anaCuts::vzVpdVz;
    // bool verrcut = !(fabs(mVx) < anaCuts::Verror && fabs(mVy) < anaCuts::Verror && fabs(mVz) < anaCuts::Verror);
    // bool vrcut =  sqrt(TMath::Power(mVx, 2) + TMath::Power(mVy, 2)) <=  anaCuts::Vr ;
    // bool NotPileUp = !mRefMultCorrUtil->isPileUpEvent(mRefmult6, picoEvent->nBTOFMatch(), mVz, mTotnMIP);
    // bool cen_0_80 = mCen16 > -1 && mCen16 < 16;

    // if (vzcut) hpassevtcut->Fill(2);
    // if (vzcut && vzVpdVzcut ) hpassevtcut->Fill(3);
    // if (vzcut && vzVpdVzcut && verrcut ) hpassevtcut->Fill(4);
    // if (vzcut && vzVpdVzcut && verrcut && vrcut ) hpassevtcut->Fill(5);
    // if (vzcut && vzVpdVzcut && verrcut && vrcut && NotPileUp ) hpassevtcut->Fill(6);
    // if (vzcut && vzVpdVzcut && verrcut && vrcut && NotPileUp && cen_0_80) hpassevtcut->Fill(7);
    
    bool vzcut = fabs(mVz) < anaCuts::vz;
    bool verrcut = !(fabs(mVx) < anaCuts::Verror && fabs(mVy) < anaCuts::Verror && fabs(mVz) < anaCuts::Verror);
    bool vrcut =  sqrt(TMath::Power(mVx, 2) + TMath::Power(mVy, 2)) <=  anaCuts::Vr ;
    bool NotPileUp = !mRefMultCorrUtil->isPileUpEvent(mRefmult6, picoEvent->nBTOFMatch(), mVz, mTotnMIP);
    bool cen_0_80 = mCen16 > -1 && mCen16 < 16;

    if (vzcut) hpassevtcut->Fill(2);
    if (vzcut && verrcut ) hpassevtcut->Fill(3);
    if (vzcut && verrcut && vrcut ) hpassevtcut->Fill(4);
    if (vzcut && verrcut && vrcut && NotPileUp ) hpassevtcut->Fill(5);
    if (vzcut && verrcut && vrcut && NotPileUp && cen_0_80) hpassevtcut->Fill(6);

    // // Histrograms for event selection cut QA: start
    // hVxVyVz->Fill(mVx,mVy,mVz); 
    // hVz->Fill(mVz);
    // h_Vx_Vy->Fill(mVx,mVy);
    // hVr->Fill(sqrt(mVy*mVy+mVx*mVx));
    // if(fabs(mVpdVz+999)>1e-2 && fabs(mVpdVz)<1000)
    // {
    //   hVpdVz->Fill(mVpdVz);
    //   h_Vz_VpdVz->Fill(mVz,mVpdVz);
    //   hVzVpdVz->Fill(mVpdVz-mVz);
    // }

    // hnTofMatvsRef->Fill(picoEvent->refMult(),picoEvent->nBTOFMatch());
    // hrefmult->Fill(picoEvent->refMult());
    // // Histrograms for event selection cut QA: end

    if (isGoodEvent(picoEvent) && NotPileUp)
    {
      hevt_cen->Fill(mCen16); 
      hevt_cen_reweight->Fill(mCen16, reWeight);
      if(!cen_0_80) return kStOK; 
      if(getShiftedEP()==-1) return kStOK;
      bfield = picoEvent->bField();

      // if(mMagBins==2)
      // {
      //   if(bfield<0.) magBufferPointer = 0;
      //   if(bfield>0.) magBufferPointer = 1;
      // }
      // else magBufferPointer = 0;
      // if(magBufferPointer!=0 && magBufferPointer!=1) return kStOK;

      cenBufferPointer = mCen9;
      if(cenBufferPointer<0 || cenBufferPointer>=mCenBins) return kStOK;
;
      vzBufferPointer = (Int_t)((mVz+anaCuts::vz)/(2*anaCuts::vz)*mVzBins);
      if(vzBufferPointer<0 || vzBufferPointer>=mVzBins) return kStOK;

      eveBufferPointer = (Int_t)(mEP_Psi2_woele/TMath::Pi()*mEveBins);
      if(eveBufferPointer<0 || eveBufferPointer>=mEveBins) return kStOK;

      current_nEPlus = 0;
      current_nEMinus = 0;

      int nTracks = picoDst->numberOfTracks();
      for (int itrack=0;itrack<nTracks;itrack++)
      {
        StPicoTrack* trk = picoDst->track(itrack);
        TVector3 mom = trk->pMom();
        
        Float_t mgDCAs = trk->gDCA(pVtx.x(),pVtx.y(),pVtx.z());       

        bool isprimary = trk->isPrimary();
        bool goodtrack = isGoodTrack(trk,trk->gDCA(mVx,mVy,mVz));      
        
        double nSigmaE_Corr_Factor = nSigmaECorr(mom, mCen16);
        double nSigmaE_corrected = trk->nSigmaElectron()-nSigmaE_Corr_Factor;  

        double beta = getTofBeta(trk);
        bool tofmatch = (beta!=std::numeric_limits<float>::quiet_NaN()) && beta>0;  

        if (!isprimary) continue;

        // // Histrograms for track quality cut QA: start
        // if(fabs(mgDCAs+999)>1e-2) hpDca->Fill(trk->gDCA(mVx,mVy,mVz));
        // hppt->Fill(mom.Perp());
        // hpEta->Fill(mom.Eta());
        // hpPhi->Fill(mom.Phi());
        // h_pT_Eta->Fill(mom.Perp()*trk->charge(),mom.Eta());
        // h_pT_Phi->Fill(mom.Perp()*trk->charge(),mom.Phi());
        
        // hnHitsFit->Fill(trk->nHitsFit()*trk->charge());
        // hnHitsPoss->Fill(trk->nHitsPoss()*trk->charge());
        // hnHitsDedx->Fill(trk->nHitsDedx()*trk->charge());
        // h_nHitsDedx_p->Fill(mom.Mag()*trk->charge(),trk->nHitsDedx());
        // hDCA_Eta_NHitsFit->Fill(mgDCAs,trk->gMom().Eta(),trk->nHitsFit());
        // // Histrograms for track quality cut QA: end
        
        if (!goodtrack) continue;
        // track with
        // 1. |DCA| < 1cm
        // 2. |\eta| < 1
        // 3. nHitsFit > 40
        // 4. nHitsFit/nHitsMax > 0.52
        // 5. nHitsDedx > 30
        // 6. pT > 0.2 GeV/c
    
        // hdEdx->Fill(mom.Mag()*trk->charge(),trk->dEdx());
        // h_m2->Fill(pow(mom.Mag()*1.0/beta,2)*(1-beta*beta));
        // h_mTpc->Fill(mom.Mag()*trk->charge(),pow(mom.Mag()*1.0/beta,2)*(1-beta*beta));
        
        // h_nSigmaElectron_P_tpc->Fill(mom.Mag(),nSigmaE_corrected);
        // h_nSigmaPion_P_tpc->Fill(mom.Mag(),trk->nSigmaPion());
        // h_nSigmaKaon_P_tpc->Fill(mom.Mag(),trk->nSigmaKaon());
        // h_nSigmaProton_P_tpc->Fill(mom.Mag(),trk->nSigmaProton());
        
        // Electrons identification
        bool isTOFElectron = false;
        bool isTPCElectron = false;
        bool isBEMCElectron = false;
        // bool realTofMatch = false;
        bool isElectron = false;

        // TOF cuts
        if (tofmatch) 
        { 
          int index2tof = trk->bTofPidTraitsIndex();
          StPicoBTofPidTraits const* const tofPid = mPicoDstMaker->picoDst()->btofPidTraits(index2tof);
          int tofid = tofPid->btofCellId();

          if(isGoodBTofCell(tofid) && fabs(tofPid->btofYLocal())<1.8)
          {
            // hinvBetavsP->Fill(mom.Mag(),1./beta);
            // h_pT_TOFmatch->Fill(mom.Perp());     
            // realTofMatch = true;  
            if(fabs(1.0/beta - 1) < 0.025)
            {
              isTOFElectron = true;
              // h_nSigmaElectron_P->Fill(mom.Mag(),nSigmaE_corrected);
            }
          }
        }

        int bemcId = trk->bemcPidTraitsIndex();
        bool bemcmatch = trk->isBemcMatchedTrack();
        double E0 = 0;
        double p_track = mom.Mag();
        if(bemcId >= 0)
        {
          StPicoBEmcPidTraits * bemctrait = picoDst->bemcPidTraits(bemcId);
          E0 = bemctrait->bemcE0();
          // E_bemc = bemctrait->bemcE();
          // bemcZ = bemctrait->bemcZDist();
          // bemcPhi = bemctrait->bemcPhiDist();
          // bemcTowerPhi = bemctrait->btowPhiDist();
          // bemcTowerEta = bemctrait->btowEtaDist();
          // bemcSmdN_eta = bemctrait->bemcSmdNEta();
          // bemcSmdN_phi = bemctrait->bemcSmdNPhi();
          isBEMCElectron = (E0/p_track > 0.5) && (E0/p_track < 1.5) && bemcmatch;
        }
        // // TPC cuts
        // if (mom.Mag()>1) isTPCElectron = nSigmaE_corrected<3 && nSigmaE_corrected>-1.25;
        // else isTPCElectron = nSigmaE_corrected<3 && nSigmaE_corrected>(2.3*mom.Mag()-3.55);


        if(mom.Pt()<=1.5)
        {
          // Aoke's cuts
          if (mom.Mag()>0.8) isTPCElectron =  nSigmaE_corrected<3 && nSigmaE_corrected>-1.5;
          else isTPCElectron = nSigmaE_corrected<3 && nSigmaE_corrected>(2.5*mom.Mag()-3.5);
          isElectron = isTPCElectron && isTOFElectron;
        }
        else{
          if(tofmatch)
          {
            isElectron = nSigmaE_corrected<3 && nSigmaE_corrected>-1.5 && isTOFElectron;;
          }
          else if (bemcmatch && !tofmatch)
          {
            isElectron = isTPCElectron && isBEMCElectron;
          }
        }

        // // Electrons need both TOF and TPC cuts
        // if (isTOFElectron && isTPCElectron) 
        if (isElectron) 
        {          
          if(trk->charge()<0 && tofmatch)  // electron
          {
            particleinfo.charge = trk->charge();
            particleinfo.pt = mom.Perp();
            particleinfo.eta = mom.Eta();
            particleinfo.phi = mom.Phi();
            particleinfo.p = mom.Mag();
            particleinfo.nSigmaE = nSigmaE_corrected;
            particleinfo.beta = beta;
            particleinfo.energy = sqrt(pow(M_electron,2.0)+pow(mom.Mag(),2.0));
            particleinfo.p1 = mom.X();
            particleinfo.p2 = mom.Y();
            particleinfo.p3 = mom.Z();
            particleinfo.PhiV_Cut = false;
            // particleinfo.isPure_Mee_lt_0p015 = false;
            electroninfo.push_back(particleinfo);

            current_eMinus[current_nEMinus].SetPx(mom.x());
            current_eMinus[current_nEMinus].SetPy(mom.y());
            current_eMinus[current_nEMinus].SetPz(mom.z());
            current_eMinus[current_nEMinus].SetE(sqrt(pow(M_electron,2.0)+pow(mom.Mag(),2.0)));
            current_nEMinus++;
          }     
          if(trk->charge()>0 && tofmatch) // positron
          {
            particleinfo.charge = trk->charge();
            particleinfo.pt = mom.Perp();
            particleinfo.eta = mom.Eta();
            particleinfo.phi = mom.Phi();
            particleinfo.p = mom.Mag();
            particleinfo.nSigmaE = nSigmaE_corrected;
            particleinfo.beta = beta;
            particleinfo.energy = sqrt(pow(M_electron,2.0)+pow(mom.Mag(),2.0));
            particleinfo.p1 = mom.X();
            particleinfo.p2 = mom.Y();
            particleinfo.p3 = mom.Z();
            particleinfo.PhiV_Cut = false;
            // particleinfo.isPure_Mee_lt_0p015 = false;
            positroninfo.push_back(particleinfo);

            current_ePlus[current_nEPlus].SetPx(mom.x());
            current_ePlus[current_nEPlus].SetPy(mom.y());
            current_ePlus[current_nEPlus].SetPz(mom.z());
            current_ePlus[current_nEPlus].SetE(sqrt(pow(M_electron,2.0)+pow(mom.Mag(),2.0)));
            current_nEPlus++;
          }
        }
      }

      int x=0;
      int y=0;
      int num_electron = electroninfo.size();
      int num_positron = positroninfo.size();
      // float inv_mass=0;
      TVector3 momentum_particle;
      TLorentzVector eepair(0,0,0,0);
      TLorentzVector particle1_4V(0,0,0,0);
      TLorentzVector particle2_4V(0,0,0,0);
      
      for(x=0;x<num_electron;x++)
      {
        particle1_4V.SetPx(electroninfo[x].p1);
        particle1_4V.SetPy(electroninfo[x].p2);
        particle1_4V.SetPz(electroninfo[x].p3);
        particle1_4V.SetE(electroninfo[x].energy);
        for(y=x+1;y<num_electron;y++)
        {
          particle2_4V.SetPx(electroninfo[y].p1);
          particle2_4V.SetPy(electroninfo[y].p2);
          particle2_4V.SetPz(electroninfo[y].p3);
          particle2_4V.SetE(electroninfo[y].energy);
          eepair = particle1_4V + particle2_4V;   
          
          Double_t angleV = phiVAngle(particle1_4V,particle2_4V,-1,-1);
          Double_t angleVcut = phiVcut->Eval(eepair.M()); 

          if(fabs(eepair.Rapidity())<=1)
          {            
            if( (angleV>=angleVcut && eepair.M()<mPhiVCutMRange) || eepair.M()>=mPhiVCutMRange )
            {
              hMeeCountPtCen_like1->Fill(eepair.M(),eepair.Perp(),mCen16, reWeight);
            }  
          }
        }
      }

      for(x=0;x<num_positron;x++)
      {
        particle1_4V.SetPx(positroninfo[x].p1);
        particle1_4V.SetPy(positroninfo[x].p2);
        particle1_4V.SetPz(positroninfo[x].p3);
        particle1_4V.SetE(positroninfo[x].energy);
        for(y=x+1;y<num_positron;y++)
        {
          particle2_4V.SetPx(positroninfo[y].p1);
          particle2_4V.SetPy(positroninfo[y].p2);
          particle2_4V.SetPz(positroninfo[y].p3);
          particle2_4V.SetE(positroninfo[y].energy);
          eepair = particle1_4V + particle2_4V;
          
          Double_t angleV = phiVAngle(particle1_4V,particle2_4V,1,1);
          Double_t angleVcut = phiVcut->Eval(eepair.M());

          if(fabs(eepair.Rapidity())<=1)
          {            
            if( (angleV>=angleVcut && eepair.M()<mPhiVCutMRange) || eepair.M()>=mPhiVCutMRange )
            {
              hMeeCountPtCen_like2->Fill(eepair.M(),eepair.Perp(),mCen16, reWeight);
            }
          }
        }
      }

      for(x=0;x<num_positron;x++)
      {
        particle1_4V.SetPx(positroninfo[x].p1);
        particle1_4V.SetPy(positroninfo[x].p2);
        particle1_4V.SetPz(positroninfo[x].p3);
        particle1_4V.SetE(positroninfo[x].energy);
        for(y=0;y<num_electron;y++)
        {
          particle2_4V.SetPx(electroninfo[y].p1);
          particle2_4V.SetPy(electroninfo[y].p2);
          particle2_4V.SetPz(electroninfo[y].p3);
          particle2_4V.SetE(electroninfo[y].energy);
          eepair = particle1_4V + particle2_4V;

          Double_t angleV = phiVAngle(particle1_4V,particle2_4V,1,-1);
          Double_t angleVcut = phiVcut->Eval(eepair.M());
          
          if(fabs(eepair.Rapidity())<=1)
          {
            // hMeeCountPhiV->Fill(eepair.M(),angleV, reWeight);
            // hMeeCount_wo_PhiV_Cut->Fill(eepair.M(), reWeight);
            if( (angleV>=angleVcut && eepair.M()<mPhiVCutMRange) || eepair.M()>=mPhiVCutMRange )
            {
              positroninfo[x].PhiV_Cut = true;
              electroninfo[y].PhiV_Cut = true;
              // hMeeCount_w_PhiV_Cut->Fill(eepair.M(), reWeight);
              hMeeCountPtCen->Fill(eepair.M(),eepair.Perp(),mCen16, reWeight);
            }   
          }
        }
      }

      // for(x=0;x<num_positron;x++)
      // {        
      //   h_pT_electrons->Fill(positroninfo[x].pt, reWeight);
      //   h_eta_electrons->Fill(positroninfo[x].eta, reWeight);
      //   h_phi_electrons->Fill(positroninfo[x].phi, reWeight);
      //   if(positroninfo[x].PhiV_Cut) 
      //   {
      //     h_eta_electrons_w_PhiV_Cut->Fill(positroninfo[x].eta, reWeight);
      //     h_phi_electrons_w_PhiV_Cut->Fill(positroninfo[x].phi, reWeight);
      //   }
      // }
      // for(y=0;y<num_electron;y++)
      // {
      //   h_pT_electrons->Fill(electroninfo[y].pt, reWeight);
      //   h_eta_electrons->Fill(electroninfo[y].eta, reWeight);
      //   h_phi_electrons->Fill(electroninfo[y].phi, reWeight);
      //   if(electroninfo[y].PhiV_Cut)
      //   {          
      //     h_eta_electrons_w_PhiV_Cut->Fill(electroninfo[y].eta, reWeight);
      //     h_phi_electrons_w_PhiV_Cut->Fill(electroninfo[y].phi, reWeight);
      //   }
      // }

      // Event Mixing
      for(Int_t iBufferEvent=0;iBufferEvent<nEventsInBuffer[cenBufferPointer][vzBufferPointer][eveBufferPointer];iBufferEvent++)
      {
        for(x=0;x<current_nEPlus;x++)
        {
          for(y=0;y<buffer_nEMinus[cenBufferPointer][vzBufferPointer][eveBufferPointer][iBufferEvent];y++)
          {
            eepair = current_ePlus[x] + buffer_eMinus[cenBufferPointer][vzBufferPointer][eveBufferPointer][iBufferEvent][y];
            Double_t angleV = phiVAngle(current_ePlus[x],buffer_eMinus[cenBufferPointer][vzBufferPointer][eveBufferPointer][iBufferEvent][y],1,-1);
            Double_t angleVcut = phiVcut->Eval(eepair.M());
            if(fabs(eepair.Rapidity())<=1)
            {
              if( (angleV>angleVcut && eepair.M()<mPhiVCutMRange) || eepair.M()>=mPhiVCutMRange )
              {
                hMeeMixPtCen->Fill(eepair.M(),eepair.Perp(),mCen16, reWeight);
              }
            }
          }
        }        
        for(x=0;x<current_nEMinus;x++)
        {
          for(y=0;y<buffer_nEPlus[cenBufferPointer][vzBufferPointer][eveBufferPointer][iBufferEvent];y++)
          {
            eepair = current_eMinus[x] + buffer_ePlus[cenBufferPointer][vzBufferPointer][eveBufferPointer][iBufferEvent][y];
            Double_t angleV = phiVAngle(current_eMinus[x],buffer_ePlus[cenBufferPointer][vzBufferPointer][eveBufferPointer][iBufferEvent][y],-1,1);
            Double_t angleVcut = phiVcut->Eval(eepair.M());
            if(fabs(eepair.Rapidity())<=1)
            {
              if( (angleV>angleVcut && eepair.M()<mPhiVCutMRange) || eepair.M()>=mPhiVCutMRange )
              {
                hMeeMixPtCen->Fill(eepair.M(),eepair.Perp(),mCen16, reWeight);
              }
            }
          }
        }          
        for(x=0;x<current_nEPlus;x++)
        {
          for(y=0;y<buffer_nEPlus[cenBufferPointer][vzBufferPointer][eveBufferPointer][iBufferEvent];y++)
          {
            eepair = current_ePlus[x] + buffer_ePlus[cenBufferPointer][vzBufferPointer][eveBufferPointer][iBufferEvent][y];
            Double_t angleV = phiVAngle(current_ePlus[x],buffer_ePlus[cenBufferPointer][vzBufferPointer][eveBufferPointer][iBufferEvent][y],1,1);
            Double_t angleVcut = phiVcut->Eval(eepair.M());
            if(fabs(eepair.Rapidity())<=1)
            {
              if( (angleV>angleVcut && eepair.M()<mPhiVCutMRange) || eepair.M()>=mPhiVCutMRange )
              {
                hMeeMixPtCen_like2->Fill(eepair.M(),eepair.Perp(),mCen16, reWeight);
              }
            }
          }
        }        
        for(x=0;x<current_nEMinus;x++)
        {
          for(y=0;y<buffer_nEMinus[cenBufferPointer][vzBufferPointer][eveBufferPointer][iBufferEvent];y++)
          {
            eepair = current_eMinus[x] + buffer_eMinus[cenBufferPointer][vzBufferPointer][eveBufferPointer][iBufferEvent][y];
            Double_t angleV = phiVAngle(current_eMinus[x],buffer_eMinus[cenBufferPointer][vzBufferPointer][eveBufferPointer][iBufferEvent][y],-1,-1);
            Double_t angleVcut = phiVcut->Eval(eepair.M());
            if(fabs(eepair.Rapidity())<=1)
            {
              if( (angleV>angleVcut && eepair.M()<mPhiVCutMRange) || eepair.M()>=mPhiVCutMRange )
              {
                hMeeMixPtCen_like1->Fill(eepair.M(),eepair.Perp(),mCen16, reWeight);
              }
            }
          }
        }
      }
      copyCurrentToBuffer();
    } //Good Event
  }

  // cout<<"end make"<<endl;
  return kStOK;
}

bool StPicoDstarMixedMaker::isGoodTrigger(StPicoEvent const* const picoEvent) const
{
  for (auto trg : anaCuts::triggers)
  {
    if (picoEvent->isTrigger(trg)) return true;
  }
  return false;
}

bool StPicoDstarMixedMaker::isGoodQaTrack(StPicoTrack const* const trk) const
{
  TVector3 vtx = mPicoDstMaker->picoDst()->event()->primaryVertex();
  return trk->gPt() > anaCuts::qaGPt && 
    fabs(trk->gMom().Eta())<anaCuts::qaEta &&
    fabs(trk->nHitsFit()) >= anaCuts::qaNHitsFit && 
    fabs(trk->nHitsFit()*1.0/trk->nHitsMax())>anaCuts::qanHitsFitRatio &&
    fabs(trk->nHitsDedx())>=anaCuts::qaNHitsDedx && 
    fabs(trk->gDCA(vtx.x(),vtx.y(),vtx.z()))<=anaCuts::qaDca;
}

bool StPicoDstarMixedMaker::isGoodTrack(StPicoTrack const* trk, float dca) const
{
  TVector3 vtx = mPicoDstMaker->picoDst()->event()->primaryVertex();
  return trk->gPt() > anaCuts::GPt && 
    fabs(trk->gMom().Eta())<anaCuts::Eta &&
    fabs(trk->nHitsFit()) >= anaCuts::NHitsFit && 
    fabs(trk->nHitsFit()*1.0/trk->nHitsMax())>=anaCuts::NHitsFitRatio &&
    fabs(trk->nHitsDedx())>=anaCuts::NHitsDedx && 
    fabs(trk->gDCA(vtx.x(),vtx.y(),vtx.z()))<=anaCuts::Dca;
}

// bool StPicoDstarMixedMaker::isTrack_eta_1_1p5(StPicoTrack const* trk, float dca) const
// {
//   TVector3 vtx = mPicoDstMaker->picoDst()->event()->primaryVertex();
//   return 
//     fabs(trk->gMom().Eta())<1.5 && fabs(trk->gMom().Eta())>=1 &&
//     fabs(trk->nHitsFit()) >= anaCuts::NHitsFit && 
//     fabs(trk->nHitsFit()*1.0/trk->nHitsMax())>=anaCuts::NHitsFitRatio &&
//     fabs(trk->nHitsDedx())>=anaCuts::NHitsDedx && 
//     fabs(trk->gDCA(vtx.x(),vtx.y(),vtx.z()))<=anaCuts::Dca;
//   // return true;
// }

// bool StPicoDstarMixedMaker::isTrack_wo_pTCut(StPicoTrack const* trk, float dca) const
// {
//   TVector3 vtx = mPicoDstMaker->picoDst()->event()->primaryVertex();
//   return 
//     fabs(trk->gMom().Eta())<1 &&
//     fabs(trk->nHitsFit()) >= anaCuts::NHitsFit && 
//     fabs(trk->nHitsFit()*1.0/trk->nHitsMax())>=anaCuts::NHitsFitRatio &&
//     fabs(trk->nHitsDedx())>=anaCuts::NHitsDedx && 
//     fabs(trk->gDCA(vtx.x(),vtx.y(),vtx.z()))<=anaCuts::Dca;
//   // return true;
// }

bool StPicoDstarMixedMaker::isGoodQaEvent(StPicoEvent const* const picoEvent) const
{
  TVector3 pVtx = picoEvent->primaryVertex();
  return fabs(pVtx.z()) < anaCuts::qavz &&
    !(fabs(pVtx.x()) < anaCuts::qaVerror && fabs(pVtx.y()) < anaCuts::qaVerror && fabs(pVtx.z()) < anaCuts::qaVerror) &&
    sqrt(TMath::Power(pVtx.x(), 2) + TMath::Power(pVtx.y(), 2)) <=  anaCuts::qaVr;
}

bool StPicoDstarMixedMaker::isGoodEvent(StPicoEvent const* const picoEvent) const
{
  TVector3 pVtx = picoEvent->primaryVertex();
  return fabs(pVtx.z()) < anaCuts::vz &&
    // fabs(pVtx.z()-picoEvent->vzVpd()) < anaCuts::vzVpdVz &&
    !(fabs(pVtx.x()) < anaCuts::Verror && fabs(pVtx.y()) < anaCuts::Verror && fabs(pVtx.z()) < anaCuts::Verror) &&
    sqrt(TMath::Power(pVtx.x(), 2) + TMath::Power(pVtx.y(), 2)) <=  anaCuts::Vr;
}

float StPicoDstarMixedMaker::getTofBeta(StPicoTrack const* const trk) const
{
  int index2tof = trk->bTofPidTraitsIndex();
  float beta = std::numeric_limits<float>::quiet_NaN();
  if (index2tof >= 0)
  {
    StPicoBTofPidTraits const* const tofPid = mPicoDstMaker->picoDst()->btofPidTraits(index2tof);
    if (tofPid)
    {
      beta = tofPid->btofBeta();
      if (beta < 1e-4)
      {
        TVector3 const vtx3 = mPicoDstMaker->picoDst()->event()->primaryVertex();
        StThreeVectorF vtx(vtx3.x(),vtx3.y(),vtx3.z());
        TVector3 const btofHitPos3 = tofPid->btofHitPos();
        StThreeVectorF btofHitPos(btofHitPos3.x(),btofHitPos3.y(),btofHitPos3.z());
        StPicoPhysicalHelix helix = trk->helix(mPicoDstMaker->picoDst()->event()->bField());
        float L = tofPathLength(&vtx, &btofHitPos, helix.curvature());
        float tof = tofPid->btof();
        if (tof > 0) beta = L / (tof * (C_C_LIGHT / 1.e9));
        else beta = std::numeric_limits<float>::quiet_NaN();
      }
    }
  } 
  return beta;
}

Double_t StPicoDstarMixedMaker::phiVAngle(TLorentzVector e1, TLorentzVector e2, Int_t q1, Int_t q2) const
{
	Double_t pt1 = e1.Pt();
	Double_t eta1 = e1.Eta();
	Double_t phi1 = e1.Phi();

	Double_t pt2 = e2.Pt();
	Double_t eta2 = e2.Eta();
	Double_t phi2 = e2.Phi();

	TVector3 e1Mom,e2Mom;
	if(q1>0&&q2<0){
		e2Mom.SetPtEtaPhi(pt1,eta1,phi1);//e+
		e1Mom.SetPtEtaPhi(pt2,eta2,phi2);//e-
	}else if(q1<0&&q2>0){
		e2Mom.SetPtEtaPhi(pt2,eta2,phi2);//e+
		e1Mom.SetPtEtaPhi(pt1,eta1,phi1);//e-
	}else if(q1==q2&&TMath::Abs(q1)==1){
		Double_t ran = myRandom_phi->Uniform(-1,1);
		if(ran>0){
			e2Mom.SetPtEtaPhi(pt1,eta1,phi1);
			e1Mom.SetPtEtaPhi(pt2,eta2,phi2);
		}
		else{
			e2Mom.SetPtEtaPhi(pt2,eta2,phi2);
			e1Mom.SetPtEtaPhi(pt1,eta1,phi1);
		}
	}else return -1;
	Double_t mN = 0.;
	if(bfield<0.) mN = -1.;
	if(bfield>0.) mN = 1.;

	TVector3 pu=e1Mom+e2Mom;
	TVector3 pv=e1Mom.Cross(e2Mom);
	TVector3 pw=pu.Cross(pv);
	TVector3 pnz(0.,0.,mN);
	TVector3 pwc=pu.Cross(pnz);

	Double_t angleV = pw.Angle(pwc);

	return angleV;
}

void StPicoDstarMixedMaker::copyCurrentToBuffer()
{
  // cout<<"magBufferPointer: "<<magBufferPointer<<" "<<"cenBufferPointer: "<<cenBufferPointer<<" "<<"vzBufferPointer: "<<vzBufferPointer<<endl;
	if(nEventsInBuffer[cenBufferPointer][vzBufferPointer][eveBufferPointer]>=mMaxEventsInBuffer) 
  {
    bufferFullFlag[cenBufferPointer][vzBufferPointer][eveBufferPointer] = kTRUE;
  }
  // cout<<"nEventsInBuffer: "<<nEventsInBuffer[cenBufferPointer][vzBufferPointer]<<endl;
  Int_t eventPointer = -1;
	if(bufferFullFlag[cenBufferPointer][vzBufferPointer][eveBufferPointer])
  {
    // cout<<"Random star"<<endl;
    eventPointer = (Int_t)myRandom->Uniform(0,mMaxEventsInBuffer-1.e-6);
    // cout<<"Random: "<<eventPointer<<endl;
	}
  else
  {
		eventPointer = nEventsInBuffer[cenBufferPointer][vzBufferPointer][eveBufferPointer];
	}

  // cout<<"eventPointer: "<<eventPointer<<"current_nEPlus: "<<current_nEPlus<<"current_nEMinus: "<<current_nEMinus<<endl;
	buffer_nEPlus[cenBufferPointer][vzBufferPointer][eveBufferPointer][eventPointer] = current_nEPlus;
	for(Int_t i=0;i<current_nEPlus;i++)
  {
		buffer_ePlus[cenBufferPointer][vzBufferPointer][eveBufferPointer][eventPointer][i] = current_ePlus[i];
	}

	buffer_nEMinus[cenBufferPointer][vzBufferPointer][eveBufferPointer][eventPointer] = current_nEMinus;
	for(Int_t i=0;i<current_nEMinus;i++)
  {
		buffer_eMinus[cenBufferPointer][vzBufferPointer][eveBufferPointer][eventPointer][i] = current_eMinus[i];
	}

	if(nEventsInBuffer[cenBufferPointer][vzBufferPointer][eveBufferPointer]<mMaxEventsInBuffer)
  {
		nEventsInBuffer[cenBufferPointer][vzBufferPointer][eveBufferPointer]++;
	}
}

double StPicoDstarMixedMaker::nSigmaECorr(TVector3 mom, int mCen16) const
{
  double Corr = 0;

  for(int i_cen=0; i_cen<8; i_cen++)
  {
    if(mCen16 >= i_cen*2 && mCen16 < (i_cen+1)*2)
    {
      for(int i=0; i<20; i++)
      {
        if(mom.Eta()>=-1.0 + 0.1*i && mom.Eta()<-1.0 + 0.1*(i+1))
        {
          Corr = Corr + etaCorr[i_cen][i];
        }
      }
      for(int i=0; i<64; i++)
      {
        if(mom.Phi()>=-3.2 + 0.1*i && mom.Phi()<-3.2 + 0.1*(i+1))
        {
          Corr = Corr + phiCorr[i_cen][i] - PhiAverageCorr[i_cen];
        }
      }
    }
  }  

  return Corr;
}

Int_t StPicoDstarMixedMaker::getRefmult6(StPicoDst const* const picoDst, StPicoEvent const* const picoEvent) const
{
  Int_t refMult6 = 0;
  for ( UInt_t iTrk=0; iTrk<picoDst->numberOfTracks(); iTrk++ )
  {
    StPicoTrack* track = picoDst->track( iTrk );
    if ( !track ) continue;
    if ( !track->isPrimary() ) continue;
    if ( TMath::Abs( track->pMom().Eta() ) > 1.5 ) continue;
    if ( track->pPt() >= 2.0 ) continue;
    if ( track->pPt() <= 0.2 ) continue;
    if ( track->gDCA(picoEvent->primaryVertex()).Mag() >= 3. ) continue;
    if ( track->nHitsFit() <= 15 ) continue;
    if ( (Double_t)track->nHitsFit() / track->nHitsPoss() <= 0.52 ) continue;
    refMult6++;
  }
  // cout << "RefMult6: " << refMult6 << endl;
  return refMult6;
}

Double_t StPicoDstarMixedMaker::getTotnMIP(StPicoDst const* const picoDst) const
{
  Double_t totnMIP = 0.;
  Int_t nEpdHits = picoDst->numberOfEpdHits();
  for(Int_t iHit=0; iHit<nEpdHits; iHit++)
  {
    StPicoEpdHit *epdHit = picoDst->epdHit(iHit);
    if( !epdHit ) continue;
    if (epdHit->nMIP() > 6.) totnMIP += 6.;
    else if (epdHit->nMIP() < 0.3) continue;
    else totnMIP += epdHit->nMIP();
  }
  // cout << "Total nMIP: " << totnMIP << endl;
  return totnMIP;
}

int StPicoDstarMixedMaker::getShiftedEP()
{
  int nTracks = mPicoDstMaker->picoDst()->numberOfTracks();
  int mCen9 = mRefMultCorrUtil->getCentralityBin9();
  int mCen16 = mRefMultCorrUtil->getCentralityBin16();

  double Qx_Raw = 0.0, Qy_Raw = 0.0;
  double Qx_Recenter = 0.0, Qy_Recenter = 0.0;
  double Qx_Raw_woele = 0.0, Qy_Raw_woele = 0.0;
  double Qx_Recenter_woele = 0.0, Qy_Recenter_woele = 0.0;

  int numOfTrks = 0, numOfTrks_woele = 0;

  for(int itrack=0; itrack<nTracks; itrack++)
  {
    StPicoTrack* trk = mPicoDstMaker->picoDst()->track(itrack);
    if (!trk)
    {
      cout<<"Warning: input track is NAN!!!"<<endl;
      return kStWarn;
    }
    bool isprimary = trk->isPrimary();
    if (!isprimary) continue;
    bool goodtrack = isGoodTrack(trk,trk->gDCA(mVx,mVy,mVz));
    if (!goodtrack) continue;

    TVector3 mom = trk->pMom();
    double phi = mom.Phi();
    float eta = mom.Eta();
    float pt = mom.Perp();        
    
    double nSigmaE_Corr_Factor = nSigmaECorr(mom, mCen16);
    double nSigmaE_corrected = trk->nSigmaElectron()-nSigmaE_Corr_Factor;  
    bool isTPCElectron = kFALSE;
    if (mom.Mag()>1) isTPCElectron = nSigmaE_corrected<3 && nSigmaE_corrected>-1.25;
    else isTPCElectron = nSigmaE_corrected<3 && nSigmaE_corrected>(2.3*mom.Mag()-3.55);

    if (pt>anaCuts::ptMax_Re || pt<anaCuts::ptMin_Re) continue; 
    double qx = cos(2*phi)*pt;
    double qy = sin(2*phi)*pt;

    double qx_corr = 0.0, qy_corr = 0.0;
    if(eta>0.0 && mVz>0.0)
    {
      qx_corr = pQxRecenter_etaplus_zplus[mCen9]->GetBinContent(runnum[mRunId]+1);
      qy_corr = pQyRecenter_etaplus_zplus[mCen9]->GetBinContent(runnum[mRunId]+1);
    }
    else if(eta>0.0 && mVz<0.0)
    {
      qx_corr = pQxRecenter_etaplus_zminus[mCen9]->GetBinContent(runnum[mRunId]+1);
      qy_corr = pQyRecenter_etaplus_zminus[mCen9]->GetBinContent(runnum[mRunId]+1);
    }
    else if(eta<0.0 && mVz>0.0)
    {
      qx_corr = pQxRecenter_etaminus_zplus[mCen9]->GetBinContent(runnum[mRunId]+1);
      qy_corr = pQyRecenter_etaminus_zplus[mCen9]->GetBinContent(runnum[mRunId]+1);
    }
    else if(eta<0.0 && mVz<0.0)
    {
      qx_corr = pQxRecenter_etaminus_zminus[mCen9]->GetBinContent(runnum[mRunId]+1);
      qy_corr = pQyRecenter_etaminus_zminus[mCen9]->GetBinContent(runnum[mRunId]+1);
    }
    
    Qx_Raw = Qx_Raw + qx;
    Qy_Raw = Qy_Raw + qy;
    Qx_Recenter = Qx_Recenter + qx - qx_corr;
    Qy_Recenter = Qy_Recenter + qy - qy_corr;
    numOfTrks++;

    if(!isTPCElectron)
    {
      Qx_Raw_woele = Qx_Raw_woele + qx;
      Qy_Raw_woele = Qy_Raw_woele + qy;
      Qx_Recenter_woele = Qx_Recenter_woele + qx - qx_corr;
      Qy_Recenter_woele = Qy_Recenter_woele + qy - qy_corr;
      numOfTrks_woele++;
    }
  }
  if( numOfTrks==0 || numOfTrks_woele==0 ) return -1;
  if( Qx_Recenter==0.0 || Qy_Recenter==0.0 ) return -1;
  
  TVector2 Qvec_Raw(Qx_Raw, Qy_Raw);
  TVector2 Qvec_Recenter(Qx_Recenter, Qy_Recenter);
  TVector2 Qvec_Raw_woele(Qx_Raw_woele, Qy_Raw_woele);
  TVector2 Qvec_Recenter_woele(Qx_Recenter_woele, Qy_Recenter_woele);

  double EP_Raw = Qvec_Raw.Phi()*0.5;
  double EP_Recenter = Qvec_Recenter.Phi()*0.5;
  double EP_Raw_woele = Qvec_Raw_woele.Phi()*0.5;
  double EP_Recenter_woele = Qvec_Recenter_woele.Phi()*0.5;

  hEP_Shuai_Raw[mCen9]->Fill(EP_Raw);
  hEP_Shuai_Recenter[mCen9]->Fill(EP_Recenter);
  hEP_Shuai_Raw_woele[mCen9]->Fill(EP_Raw_woele);
  hEP_Shuai_Recenter_woele[mCen9]->Fill(EP_Recenter_woele);

  double EP_Shifted = getEventPlaneShift(EP_Recenter, mCen9);
  if(EP_Shifted < 0) EP_Shifted += TMath::Pi();
  if(EP_Shifted > TMath::Pi()) EP_Shifted -= TMath::Pi();
  double EP_Shifted_woele = getEventPlaneShift(EP_Recenter_woele, mCen9);
  if(EP_Shifted_woele < 0) EP_Shifted_woele += TMath::Pi();
  if(EP_Shifted_woele > TMath::Pi()) EP_Shifted_woele -= TMath::Pi();

  hEP_Shuai_Shift[mCen9]->Fill(EP_Shifted);
  hEP_Shuai_Shift_woele[mCen9]->Fill(EP_Shifted_woele);

  mEP_Psi2_woele = EP_Shifted;

  return 0;
}

double StPicoDstarMixedMaker::getEventPlaneShift(double EP_Re, int cent) const
{
  double EP_Sh = EP_Re;
  for (int i=1; i<21; i++)
  {
    double meansin = pSinEta_Shuai[cent]->GetBinContent(i);
    double meancos = pCosEta_Shuai[cent]->GetBinContent(i);
    EP_Sh += (1./(double)i)*(-1.*meansin*std::cos(i*2.*EP_Re) + meancos*std::sin(i*2.*EP_Re));
  }
  return EP_Sh;
}

// // Codes that no longer used
// int StPicoDstarMixedMaker::numberOfTPCTracks(StPicoDst const* const picoDst) const
// {
//   int nTracks = picoDst->numberOfTracks();
//   // cout << "Number of Tracks: " << nTracks << endl;
//   int nTPCTracks = 0;
//   for (int itrack=0;itrack<nTracks;itrack++)
//   {
//     StPicoTrack* trk = picoDst->track(itrack);
//     if ( !trk->isPrimary() ) continue;
//     TVector3 mom = trk->pMom();
//     if ( mom.Perp()>0.2 && mom.Perp()<2.0 && fabs(mom.Eta())<1.5 ) nTPCTracks++;
//   }
//   // cout << "Number of TPC Tracks: " << nTPCTracks << endl;
//   return nTPCTracks;
// }

// bool StPicoDstarMixedMaker::isPileUpEvent(int nTPCTracks, int nBTOFMatch) const
// {
//   double b0 = 10.805229670974072;
//   double b1 = 4.222122255649001;
//   double b2 = -0.059023457533258925;
//   double b3 = 0.0007297279166792341;
//   double b4 = -3.730248246986408e-06;
//   double c0 = -22.14994423125864;
//   double c1 = 2.295827717755825;
//   double c2 = -0.007304309390513328;
//   double c3 = 9.397986288422607e-05;
//   double c4 = -9.080150075680224e-07;

//   double TPCTracksUpper = b0 + b1*nBTOFMatch + b2*pow(nBTOFMatch, 2) + b3*pow(nBTOFMatch, 3) + b4*pow(nBTOFMatch, 4);
//   double TPCTracksLower = c0 + c1*nBTOFMatch + c2*pow(nBTOFMatch, 2) + c3*pow(nBTOFMatch, 3) + c4*pow(nBTOFMatch, 4);

//   if (nTPCTracks<TPCTracksUpper && nTPCTracks>TPCTracksLower) return false;
//   else return true;
// }

// int StPicoDstarMixedMaker::getCentralityBin(int nTPCTracks) const
// {  
//   const int TPCTrk_cent[16] = {13, 15, 17, 19, 22, 24, 27, 31, 35, 40, 46, 52, 60, 69, 80, 95};
//   for(int cent=0; cent<16; cent++)
//   {
//     if(nTPCTracks<TPCTrk_cent[cent]) return cent -1;
//   }
//   return 15;
// }