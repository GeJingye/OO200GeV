/* **************************************************
 *
 *  Authors: Yuanjing Ji
 Guannan Xie <guannanxie@lbl.gov>
 *           Mustafa Mustafa <mmustafa@lbl.gov>
 *
 * **************************************************
 */

#include <iostream>
#include <fstream>
#include <string>
#include <cmath>

#include "TFile.h"
#include "TMath.h"
#include "TClonesArray.h"
#include "TTree.h"
#include "TNtuple.h"
#include "TLorentzVector.h"
#include "TVector3.h"
#include "TRandom.h"
#include "TRandom3.h"

#include "StEvent/StDcaGeometry.h"
#include "StPhysicalHelixD.hh"
#include "phys_constants.h"
#include "StBTofUtil/tofPathLength.hh"
#include "StPicoDstMaker/StPicoDstMaker.h"

#include "StPicoEvent/StPicoBTofPidTraits.h"
#include "StPicoEvent/StPicoETofPidTraits.h"
#include "StPicoEvent/StPicoDst.h"
#include "StPicoEvent/StPicoEvent.h"
#include "StPicoEvent/StPicoTrack.h"
#include "StPicoEvent/StPicoBEmcPidTraits.h"
#include "StPicoEvent/StPicoMtdPidTraits.h"

#include "StPicoDstarMixedMaker.h"
#include "StAnaCuts.h" 
#include "calmean.h"

#include "StMemStat.h"

//让代码里的调试输出开关处于“打开”状态。
#ifndef DEBUG
#define DEBUG 1
#endif

TRandom3* myRandom_phi;
float Bfield = 0;

/* 
ClassImp() 是 ROOT 框架中的一个宏，用于向 ROOT 的 RTTI（运行时类型识别）系统注册类，使得该类：
支持 ROOT 的 I / O 系统（如 TFile::Write()、TFile::Read()）
支持 反射机制（如 TClass::GetClass("ClassName")）
支持 交互式命令行（CLING）识别
支持 TTree::Branch()` 自动序列化
*/
ClassImp(StPicoDstarMixedMaker)

//成员初始化函数的类外实现
StPicoDstarMixedMaker::StPicoDstarMixedMaker(char const * name, TString const inputFilesList, TString const outFileBaseName, StPicoDstMaker* picoDstMaker):
    StMaker(name),
	mInputFilesList(inputFilesList),
	mOutFileBaseName(outFileBaseName),
	mPicoDstMaker(picoDstMaker),
	mRunbyRunQA(true)
{}

//-----------------------------------------------------------------------------
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
void StPicoDstarMixedMaker::initHists(){
  //本次要处理的run的个数；	
  //int TotalRunNum = 170;//9.2GeV 
  //int TotalRunNum = 661;//9.2GeV 2020b
  //int TotalRunNum = 403;//9.2GeV 2020c
  //int TotalRunNum = 335;//9.2GeV 2020c 20200807
  //int TotalRunNum = 420;//9.2GeV 2020c 20200807
  //int TotalRunNum = 508;//9.2GeV 2020c 20200828
  //int TotalRunNum = 141;//7.7GeV 2020 20200911
  //int TotalRunNum = 1127;//19.6GeV 2020c
  //int TotalRunNum = 1950;//11.5GeV 
  //int TotalRunNum = 29;//19.5GeV fixed target
  //int TotalRunNum = 14;//54GeV 2020 reproduction test
  //int TotalRunNum = 547;//54GeV 2020 reproduction 20201008
  //int TotalRunNum = 53;//54GeV 2020 reproduction 20201008
  //int TotalRunNum = 13;//200 GeV SL20d AuAu200
  //int TotalRunNum = 567;//7.7GeV 2021
  //int TotalRunNum = 562;//7.7GeV 2021
  //int TotalRunNum = 62;//200GeV OO 2021 mb
  //int TotalRunNum = 75;//200GeV OO 2021 ct
  //int TotalRunNum = 170;//200GeV 17p3 2021
  //int TotalRunNum = 37;//200GeV 17p3 20210628
  //int TotalRunNum = 156;// 19p6 20210805
  //int TotalRunNum = 30;// 19p6 20210805 SL21cB
  //int TotalRunNum = 1145;// 19p6 20211221
  int TotalRunNum = 81;// 3.2 GeV
  //int TotalRunNum = 31;// 3.5 GeV
  
  //char name_RunID[100];

  if (mRunbyRunQA){
    ifstream readnum;
    readnum.open(mRunNumList);
    int tmp=0;
    if (DEBUG) cout<<"start initial run number..."<<endl;
    for (int i=0;i<TotalRunNum;i++){
      readnum>>tmp;
      mrunnum.insert(pair<int,int>(tmp,i));//std::pair<T1, T2> 把两个不一定同类型的值粘在一起，形成一个新的对象。
      
      // sprintf(name_RunID,"hinvBetavsP_%d",tmp);
      // hinvBetavsP_RunID[i] = new TH2F(name_RunID,"#frac{1}{#beta} vs P;P(GeV/c);#frac{1}{#beta}",300,0,3,200,0.5,1.5);
       //memset(name_RunID,0,sizeof(name_RunID));

      if (DEBUG) cout <<"run number : " << tmp <<" id :" <<mrunnum[tmp] <<endl;
    }
    readnum.close();
  }

  myRandom_phi = new TRandom3();
  // event level QA
  hevt = new TH1D("hevt","hevt",TotalRunNum,0,TotalRunNum);
  // hevt_21217018 = new TH1D("hevt_21217018","hevt_21217018",2,0,2);
  hevtcut = new TH1D("hevtcut","hevtcut",TotalRunNum,0,TotalRunNum);
  hevtbadcut = new TH1D("hevtbadcut","Events after remove bad run;Run;Counts",TotalRunNum,0,TotalRunNum);
  hpassevtcut = new TH1D("hpassevtcut","pass event cut",6  , -0.5 , 5.5 );
  //run by run QA
  if (mRunbyRunQA){ 
    pVpdVz = new TProfile("VpdVz","VpdVz vs runId;runId;VpdVz(cm)",TotalRunNum,0,TotalRunNum);
    pVzVpdVz = new TProfile("VzVpdVz","VzVpdVz vs runId;runId;VpdVz-Vz(cm)",TotalRunNum,0,TotalRunNum);
    pRefmult = new TProfile("Refmult","Refmult vs runId;runId;Refmult",TotalRunNum,0,TotalRunNum);
    pVpdHitEast = new TProfile("pVpdHitEast","pVpdHitEast;runId;pVpdHitEast",TotalRunNum,0,TotalRunNum);
    pVpdHitWest = new TProfile("pVpdHitWest","pVpdHitWest;runId;pVpdHitWest",TotalRunNum,0,TotalRunNum);
    pVz  = new TProfile("pVz","pVz;runId;pVz(cm)",TotalRunNum,0,TotalRunNum);
    pVx = new TProfile("pVx","pVx;runId;pVx(cm)",TotalRunNum,0,TotalRunNum);
    pVy = new TProfile("pVy","pVy;runId;pVy(cm)",TotalRunNum,0,TotalRunNum);
    pVr = new TProfile("pVr","pVr;runId;pVr(cm)",TotalRunNum,0,TotalRunNum);
    //track level QA
    pTof = new TProfile("Tof","1/#beta vs runId;runId;1/#beta",TotalRunNum,0,TotalRunNum);
    pDedx = new TProfile("Dedx","dEdx vs runId;runId;dEdx",TotalRunNum,0,TotalRunNum);
    //pRMSDedx = new TProfile("RMSDedx","RMSdEdx vs runId;runId;RMSdEdx",TotalRunNum,0,TotalRunNum);
    pgDCA = new TProfile("gDCA","gDCA vs runId;runId;global DCA(cm)",TotalRunNum,0,TotalRunNum);
    ppDCA = new TProfile("pDCA","pDCA vs runId;runId;primary DCA(cm)",TotalRunNum,0,TotalRunNum);
    pgPt = new TProfile("gPt","global Pt vs runId;runId;global p_{T}(GeV/c)",TotalRunNum,0,TotalRunNum);
    pgPhi = new TProfile("gPhi","global Phi vs runId;runId;gPhi",TotalRunNum,0,TotalRunNum);
    pgEta = new TProfile("gEta","global Eta vs runId;runId;Eta",TotalRunNum,0,TotalRunNum);
    pNFits = new TProfile("NFits","NHitsFit vs runId;runId;nHitsFit",TotalRunNum,0,TotalRunNum);
    ppPt = new TProfile("pPt","primary Pt vs runId;runId;primary p_{T}(GeV/c)",TotalRunNum,0,TotalRunNum);
    ppEta = new TProfile("pEta","primary Eta vs runId;runId;pEta",TotalRunNum,0,TotalRunNum);
    ppPhi = new TProfile("pPhi","primary Phi vs runId;runId;pPhi",TotalRunNum,0,TotalRunNum);
    p_nSigmaE = new TProfile("p_nSigmaE","primary nSigmaE vs runId;runId;n#sigma_{e}",TotalRunNum,0,TotalRunNum);
    p_nSigmaPion = new TProfile("p_nSigmaPion","primary nSigmaPion vs runId;runId;n#sigma_{#pi}",TotalRunNum,0,TotalRunNum);
    p_nSigmaKaon = new TProfile("p_nSigmaKaon","primary nSigmaKaon vs runId;runId;n#sigma_{k}",TotalRunNum,0,TotalRunNum);
    p_nSigmaProton = new TProfile("p_nSigmaProton","primary nSigmaProton vs runId;runId;n#sigma_{p}",TotalRunNum,0,TotalRunNum);
    p_nTofMat = new TProfile("p_nTofMat","nTofMat vs runId;runId;nTofMat",TotalRunNum,0,TotalRunNum);
    
    p_nSigmaTofE = new TProfile("p_nSigmaTofE","primary nSigmaTofE vs runId;runId;nSigmaTof_{e}",TotalRunNum,0,TotalRunNum);
    p_nSigmaTofPion = new TProfile("p_nSigmaTofPion","primary nSigmaTofPion vs runId;runId;nSigmaTof_{#pi}",TotalRunNum,0,TotalRunNum);
    p_nSigmaTofKaon = new TProfile("p_nSigmaTofKaon","primary nSigmaTofKaon vs runId;runId;nSigmaTof_{k}",TotalRunNum,0,TotalRunNum);
    p_nSigmaTofProton = new TProfile("p_nSigmaTofProton","primary nSigmaTofProton vs runId;runId;nSigmaTof_{p}",TotalRunNum,0,TotalRunNum);
    p_btofYLocal = new TProfile("p_btofYLocal","btofYLocal vs runId;runId;btofYLocal",TotalRunNum,0,TotalRunNum);
    p_btofZLocal = new TProfile("p_btofZLocal","btofZLocal vs runId;runId;btofZLocal",TotalRunNum,0,TotalRunNum);

    p_nETofHits = new TProfile("p_nETofHits","ETof Hits vs runId;runId;nETofHits",TotalRunNum,0,TotalRunNum);
    p_nBemcPidTraits = new TProfile("p_nBemcPidTraits","nBemcPidTraits vs runId;runId;nBemcPidTraits",TotalRunNum,0,TotalRunNum);
    p_nMtdHits = new TProfile("p_nMtdHits","nMtdHits vs runId;runId;nMtdHits",TotalRunNum,0,TotalRunNum);
    p_nMtdPidTraits = new TProfile("p_nMtdPidTraits","nMtdPidTraits vs runId;runId;nMtdPidTraits",TotalRunNum,0,TotalRunNum);
    p_ETof_beta = new TProfile("p_ETof_beta","ETof beta vs runId;runId;1/beta(ETof)",TotalRunNum,0,TotalRunNum);
    p_ETof_deltaX = new TProfile("p_ETof_deltaX","ETof delta X vs runId;runId;#Delta X (ETof)",TotalRunNum,0,TotalRunNum);
    p_ETof_deltaY = new TProfile("p_ETof_deltaY","ETof delta Y vs runId;runId;#Delta Y (ETof)",TotalRunNum,0,TotalRunNum);
    h_ETof_beta = new TH1F("h_ETof_beta","ETof beta;1/beta(ETof);Counts",1100,-10,100);
    h_ETof_deltaX = new TH1F("h_ETof_deltaX","ETof delta X;#Delta X (ETof);Counts",2000,-100,100);
    h_ETof_deltaY = new TH1F("h_ETof_deltaY","ETof delta Y;#Delta Y (ETof);Counts",2000,-100,100);
    
  }
   //event QA
    hVxVyVz = new TH3F("hVxVyVz","VxVyVz;Vx(cm);Vy(cm);Vz(cm)",100,-2,2,100,-2,2,240,0,240);
    hVz = new TH1F("hVz","Vz;Vz(cm);Counts",240,0,240);
    hVpdVz = new TH1F("hVpdVz","VpdVz;VpdVz(cm);Counts",840,-200,240);
    h_Vz_VpdVz = new TH2F("h_Vz_VpdVz","Vz vs VpdVz;Vz(cm);VpdVz(cm)",240,0,240,840,-200,240);
    hVr = new TH1F("hVr","Vr;Vr(cm);Counts",300,0,3);
    hVzVpdVz = new TH1F("hVzVpdVz","Vz-VpdVz(cm)",2000,-200,200);
    h_Vx_Vy = new TH2F("h_Vx_Vy","Vertex XY",600,-3,3,600,-3,3);
    hnEvsEtavsVz = new TH2F("hnEvsEtavsVz","nElectron;#eta;Vz(cm)",100,-2.5,2.5,240,0,240);
    hnEvsPhivsVz = new TH2F("hnEvsPhivsVz","nElectron;#phi;Vz(cm)",100,-1*TMath::Pi(),TMath::Pi(),240,0,240);
    //hnTofMulvsRef = new TH2F("hnTofMulvsRef","hnTofMul vs Ref;btofTrayMultiplicity;refMult",2000,0,2000,900,0,900); 
    hnTofMulvsRef = new TH2F("hnTofMulvsRef","RefMul vs nTofMul;RefMul;btofTrayMultiplicity",900,0,900,2000,0,2000); 
    //hnTofMatvsRef= new TH2F("hnTofMatvsRef","nTofmatch VS Refmult;nTofMatch;refMult",900,0,900,900,0,900);
    hnTofMatvsRef= new TH2F("hnTofMatvsRef","RefMul VS nTofmatch;RefMul;nTofMatch",900,0,900,900,0,900);
    hnTofHitvsRef= new TH2F("hnTofHitvsRef","hnTofHit vs Ref;nTofHits;refMult",900,0,900,900,0,900);
    hrefmult = new TH1F("hrefmult","hrefmult",700,0,700);

    h_Vz_btofZLocal = new TH2F("h_Vz_btofZLocal"," ;Vz;btofZLocal",840,0,240,1000,-5,5);
    h_Vz_btofYLocal = new TH2F("h_Vz_btofYLocal"," ;Vz;btofYLocal",840,0,240,1000,-5,5);

    //tracl level QA
    hnHitsFit = new TH1F("hnHitsFit","nHitsFit;nHitsFit",180,-90,90);
    hnHitsPoss = new TH1F("hnHitsPoss","nHitsPoss;nHitsPoss",180,-90,90);
    hnHitsDedx = new TH1F("hnHitsDedx","nHitsDedx;nHitsDedx",180,-90,90);
    h_nHitsDedx_p = new TH2F("h_nHitsDedx_p","nHitsDedx vs p*charge;p*charge (GeV/c);nHitsDedx",400,-10,10,90,0,90);
    h_p_E0 = new TH1F("h_p_E0","#frac{p}{E0};#frac{p}{E0}",40,0,4);
    ph_p_E0 = new TProfile("ph_p_E0","#frac{p}{E0};RunId;#frac{p}{E0}",TotalRunNum,0,TotalRunNum);
    h_bemcdz = new TH1F("h_bemcdz","bemcdz;bemcdz",40,-20,20);
    ph_bemcdz = new TProfile("ph_bemcdz","bemcdz;RunId;bemcdz",TotalRunNum,0,TotalRunNum);
    h_bemcDphi = new TH1F("h_bemcDphi","bemcDphi;bemcDphi",100,-0.5,0.5);
    ph_bemcDphi = new TProfile("ph_bemcDphi","bemcDphi;RunId;bemcDphi",TotalRunNum,0,TotalRunNum);
    h_nSMDphi = new TH1F("h_nSMDphi","nSMDphi;nSMDphi",20,-10,10);
    ph_nSMDphi = new TProfile("ph_nSMDphi","nSMDphi;RunId;nSMDphi",TotalRunNum,0,TotalRunNum);
    h_nSMDeta = new TH1F("h_nSMDeta","nSMDeta;nSMDeta",20,-10,10);
    ph_nSMDeta = new TProfile("ph_nSMDeta","nSMDeta;RunId;nSMDeta",TotalRunNum,0,TotalRunNum);
    h_btowPhiDz = new TH1F("h_btowPhiDz","btowPhiDz;btowPhiDz",1000,-50,50);
    ph_btowPhiDz = new TProfile("ph_btowPhiDz","btowPhiDz;RunId;btowPhiDz",TotalRunNum,0,TotalRunNum);
    h_btowEtaDz = new TH1F("h_btowEtaDz","btowEtaDz;btowEtaDz",1000,-50,50);
    ph_btowEtaDz = new TProfile("ph_btowEtaDz","btowEtaDz;RunId;btowEtaDz",TotalRunNum,0,TotalRunNum);
    
    h_mtdDeltaY = new TH1F("h_mtdDeltaY","#Delta Y(mtd);#Delta Y",1000,-50,50);
    ph_mtdDeltaY = new TProfile("ph_mtdDeltaY","#Delta Y(mtd);RunId;#Delta Y",TotalRunNum,0,TotalRunNum);
    h_mtdDeltaZ = new TH1F("h_mtdDeltaZ","#Delta Z(mtd);#Delta Z",1000,-50,50);
    ph_mtdDeltaZ = new TProfile("ph_mtdDeltaZ","#Delta Z(mtd);RunId;#Delta Z",TotalRunNum,0,TotalRunNum);
    h_mtdDeltaTOF = new TH1F("h_mtdDeltaTOF","#Delta TOF(mtd);#Delta TOF",20000,-2000,2000);
    ph_mtdDeltaTOF = new TProfile("ph_mtdDeltaTOF","#Delta TOF(mtd);RunId;#Delta TOF",TotalRunNum,0,TotalRunNum);

    // hpDca = new TH1F("hpDca","pDca",50,0,5);
    hgDca = new TH1F("hgDca","gDca",50,0,5);
    hpDca = new TH1F("hpDca","pDca",50,0,5);
    hinvBetavsP = new TH2F("hinvBetavsP","#frac{1}{#beta} vs p;p(GeV/c);#frac{1}{#beta}",300,0,3,200,0.5,1.5);
    // hinvBetavsY;
    hdEdx = new TH2F("hdEdx","dEdx vs p*charge;p*charge(GeV/c);dEdx",200,-2,2,400,0,25);
    h_mTpc = new TH2F("h_mTpc","mass^{2} vs p*charge;p*charge(GeV/c);mass square (GeV/c^{2})^{2}",200,-2,2,120,0,1.2);
    hNsigEvsinvBeta = new TH3F("hNsigEvsinvBeta","nSigmaE vs 1/#beta;nSigmaE;1/#beta;p",200,-10,10,100,0.8,1.2,100,0.15,2.5);
    hpt = new TH1F("hpt","hpt;p_{T}(GeV/c)",240,0,12);
    hGpt = new TH1F("hGpt","hGpt;global p_{T}(GeV/c)",240,0,12);
    hEta = new TH1F("hEta","Eta;#eta",60,-1.5,1.5);
    hPhi = new TH1F("hPhi","Phi;#phi",80,-4,4);
    h_pT_Eta = new TH2F("h_pT_Eta","p_{T}*charge vs #eta;p_{T}*charge (GeV/c);#eta",400,-10,10,60,-2.5,2.5);
    h_pT_Phi = new TH2F("h_pT_Phi","p_{T}*charge vs #phi;p_{T}*charge (GeV/c);#phi",400,-10,10,160,-4,4);
    h_EtavsPhi_Pos = new TH2F("h_EtavsPhi_Pos","Positive Tracks #eta vs #phi;#eta;#phi",60,-2.5,2.5,160,-4,4);
    h_EtavsPhi_Neg = new TH2F("h_EtavsPhi_Neg","Negative Tracks #eta vs #phi;#eta;#phi",60,-2.5,2.5,160,-4,4);
    hBadTofId = new TH3F("hBadTofId","hBadTofId;tray;module;cell",125,-0.5,124.5,33,-0.5,32.5,7,-0.5,6.5);

    //invariant mass electron
    //hMeeCount = new TH1F("hMee","hMee;Count;Mee(GeV/c^{2})",400,0,4);
    hMeeCount = new TH1F("hMee","hMee;Mee(GeV/c^{2})",40000,0,4);

    hMee_phiV = new TH1F("hMee_phiV","phiV;phiV",100,0,1);

    hMeeCount_like1 = new TH1F("hMee_like1","hMee like sign electron;Mee(GeV/c^{2})",40000,0,4);
    hMeeCount_like2 = new TH1F("hMee_like2","hMee like sign positron;Mee(GeV/c^{2})",40000,0,4);
    hMeeCountPt = new TH2F("hMeePt","hMee vs p_{T};Mee(GeV/c^{2});p_{T}",40000,0,4,200,0,10);

    hMeeCountPhiV = new TH2F("hMeePhiV","hMee vs phiV;Mee(GeV/c^{2});phiV",40000,0,4,100,0,1);

    hMeeCountPt_like1 = new TH2F("hMeePt_like1","hMee vs p_{T} like sign electron;Mee(GeV/c^{2});p_{T}",40000,0,4,200,0,10);
    hMeeCountPt_like2 = new TH2F("hMeePt_like2","hMee vs p_{T} like sign positron;Mee(GeV/c^{2});p_{T}",40000,0,4,200,0,10);
    h_nSigmaElectron_P = new TH2F("h_nSigmaElectron_P","nSigmaElectron_P;P(GeV/c);nSigmaElectron",300,0,3,100,-5,5);
    h_nSigmaElectron_P_tpc = new TH2F("h_nSigmaElectron_P_tpc","nSigmaElectron_P;P(GeV/c);nSigmaElectron",300,0,3,100,-5,5);

    TofId = new TH1F("TofId","1.13<1/#beta<1.24 0.3<p<0.5;TofId",23100,0,23100);
    TofId_nSigmaPi = new TH1F("TofId_nSigmaPi","1.13<1/#beta<1.24 0.3<p<0.5;nSigmaPi",200,-10,10);
    //tof module id
    /*ModuleId_1 = new TH1F("ModuleId 1","0.8<1/#beta<0.9 0.4<P;ModuleId",40,0,40);
    TofId_1 = new TH1F("TofId 1","0.8<1/#beta<0.9 0.4<P;TofId",23100,0,23100);
    TrayId_1 = new TH1F("TrayId 1","0.8<1/#beta<0.9 0.4<P;TrayId",130,0,130);
    ModuleId_2 = new TH1F("ModuleId 2","0.82<1/#beta<0.9 0.4<P;ModuleId",40,0,40);
    TofId_2 = new TH1F("TofId 2","0.8<1/#beta<0.9 0.4<P;TofId",23100,0,23100);
    ModuleId_3 = new TH1F("ModuleId 3","0.82<1/#beta<0.88 0.4<P;ModuleId",40,0,40);
    TofId_3 = new TH1F("TofId 3","0.8<1/#beta<0.9 0.4<P;TofId",23100,0,23100);
    ModuleId_4 = new TH1F("ModuleId 4","0.84<1/#beta<0.88 0.4<P;ModuleId",40,0,40);
    TofId_4 = new TH1F("TofId 4","0.8<1/#beta<0.9 0.4<P;TofId",23100,0,23100);
    ModuleId_5 = new TH1F("ModuleId 5","0.84<1/#beta<0.9 & 0.4<P;ModuleId",40,0,40);
    TofId_5 = new TH1F("TofId 5","0.8<1/#beta<0.9 0.4<P;TofId",23100,0,23100);*/
}
//-----------------------------------------------------------------------------
Int_t StPicoDstarMixedMaker::Finish()
{
    mFile->cd();
    //write the hists
    //event QA
    hVxVyVz->Write();
    hVz->Write();
    hVpdVz->Write();
    hVr->Write();
    hVzVpdVz->Write();
    hnEvsEtavsVz->Write();
    hnEvsPhivsVz->Write();
    hnTofMulvsRef->Write(); 
    hnTofMatvsRef->Write();
    hnTofHitvsRef->Write();

    hevt->Write();
    h_Vx_Vy->Write();
    h_Vz_VpdVz->Write();
    hevtcut->Write();
    hevtbadcut->Write();
    hpassevtcut->Write(); 
    hrefmult->Write();
    hNsigEvsinvBeta->Write();
    //tracl level QA
    hnHitsFit->Write();
    // hpDca->Write();
    hgDca->Write();
    hpDca->Write();
    hinvBetavsP->Write();
    TofId->Write();
    TofId_nSigmaPi->Write();
    // hinvBetavsY->Write();
    hdEdx->Write();
    h_mTpc->Write();
    hpt->Write();
    hGpt->Write();
    hEta->Write();
    hPhi->Write();
    hnHitsPoss->Write();
    hnHitsDedx->Write();
    h_nHitsDedx_p->Write();
    h_pT_Eta->Write();
    h_pT_Phi->Write();
    h_EtavsPhi_Pos->Write();
    h_EtavsPhi_Neg->Write();
    hBadTofId->Write();
    
    h_Vz_btofZLocal->Write();
    h_Vz_btofYLocal->Write();

  hMeeCount->Write();

  hMee_phiV->Write();

  hMeeCount_like1->Write();
  hMeeCount_like2->Write();
  hMeeCountPt->Write();

  hMeeCountPhiV->Write();

  hMeeCountPt_like1->Write();
  hMeeCountPt_like2->Write();
  h_nSigmaElectron_P->Write();
  h_nSigmaElectron_P_tpc->Write();
  h_p_E0->Write();
  h_bemcdz->Write();
  h_bemcDphi->Write();
  h_nSMDphi->Write();
  h_nSMDeta->Write();
  h_btowPhiDz->Write();
  h_btowEtaDz->Write();
  h_mtdDeltaY->Write();
  h_mtdDeltaZ->Write();
  h_mtdDeltaTOF->Write();
  ph_p_E0->Write();
  ph_bemcdz->Write();
  ph_bemcDphi->Write();
  ph_nSMDphi->Write();
  ph_nSMDeta->Write();
  ph_btowPhiDz->Write();
  ph_btowEtaDz->Write();
  ph_mtdDeltaY->Write();
  ph_mtdDeltaZ->Write();
  ph_mtdDeltaTOF->Write();

    /*ModuleId_1->Write();
    TofId_1->Write();
    TrayId_1->Write();
    ModuleId_2->Write();
    TofId_2->Write();
    ModuleId_3->Write();
    TofId_3->Write();
    ModuleId_4->Write();
    TofId_4->Write();
    ModuleId_5->Write();
    TofId_5->Write();*/
  if (mRunbyRunQA) {
    pVpdVz->Write();
    pVzVpdVz->Write();
    pRefmult->Write();
    pVpdHitEast->Write();
    pVpdHitWest->Write();
    pVz->Write();
    pVx->Write();
    pVy->Write();
    pVr->Write();
    pTof->Write(); 
    pDedx->Write();
    pgDCA->Write();
    ppDCA->Write();
    pgPt->Write();
    pgPhi->Write();
    pgEta->Write();
    pNFits->Write();
    ppPt->Write();
    ppEta->Write();
    ppPhi->Write();
    p_nSigmaE->Write();
    p_nSigmaPion->Write();
    p_nSigmaKaon->Write();
    p_nSigmaProton->Write();

    p_nTofMat->Write();
    p_nSigmaTofE->Write();
    p_nSigmaTofPion->Write();
    p_nSigmaTofKaon->Write();
    p_nSigmaTofProton->Write();
    p_btofYLocal->Write();
    p_btofZLocal->Write();

    p_nETofHits->Write();
    p_nBemcPidTraits->Write();
    p_nMtdHits->Write();
    p_nMtdPidTraits->Write();
    p_ETof_beta->Write();
    p_ETof_deltaX->Write();
    p_ETof_deltaY->Write();
    h_ETof_beta->Write();
    h_ETof_deltaX->Write();
    h_ETof_deltaY->Write();
  }
  mFile->Close();

  /*mFile_RunID->cd();
  int x_RunID=0;
  for(x_RunID=0;x_RunID<78;x_RunID++)
  {
    hinvBetavsP_RunID[x_RunID]->Write();
  }
  mFile_RunID->Close();*/

  return kStOK;
}
//-----------------------------------------------------------------------------
Int_t StPicoDstarMixedMaker::Make()
{
  cout<<"star make"<<endl;
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
  
  //if(mRunId < 22057001) return 0;   

  hevt->Fill(mrunnum[mRunId]);

  //if(mRunId < 22043001) return 0;// form 20210212

    TVector3 pVtx = picoEvent->primaryVertex();
  if (mRunbyRunQA && isGoodQaEvent(picoEvent) ){ 
    //primary vertex
    // StThreeVectorF pVtx = picoEvent->primaryVertex();
    cout<<"star runbyrun QA"<<endl;
    mVx = pVtx.x();
    mVy = pVtx.y();
    mVz = pVtx.z();
    mVr = sqrt(mVx*mVx+mVy*mVy);
    
    mVpdVz = picoEvent->vzVpd();
    cout<<"runbyrun VpdVz: "<<mVpdVz<<endl;
    cout<<"runbyrun Vz: "<<mVz<<endl;
    mRefmult = picoEvent->refMult();
    mVpdHitEast = picoEvent->nVpdHitsEast();
    mVpdHitWest = picoEvent->nVpdHitsWest();
    if (DEBUG) cout<<"start filling "<<mRunId<<" "<<mrunnum[mRunId]<<endl; 
    if(fabs(mVpdVz) < 200){//fill event level profile
    pVpdVz->Fill(mrunnum[mRunId],mVpdVz);
    pVzVpdVz->Fill(mrunnum[mRunId],mVpdVz-mVz);
    }
    pRefmult->Fill(mrunnum[mRunId],mRefmult);
    pVpdHitEast->Fill(mrunnum[mRunId],mVpdHitEast);
    pVpdHitWest->Fill(mrunnum[mRunId],mVpdHitWest);
    pVx->Fill(mrunnum[mRunId],mVx);
    pVy->Fill(mrunnum[mRunId],mVy);
    pVz->Fill(mrunnum[mRunId],mVz);
    pVr->Fill(mrunnum[mRunId],mVr);

    p_nTofMat->Fill(mrunnum[mRunId],picoEvent->nBTOFMatch());
    p_nETofHits->Fill(mrunnum[mRunId],picoDst->numberOfETofHits());
    p_nBemcPidTraits->Fill(mrunnum[mRunId],picoDst->numberOfBEmcPidTraits());
    p_nMtdPidTraits->Fill(mrunnum[mRunId],picoDst->numberOfMtdPidTraits());
    p_nMtdHits->Fill(mrunnum[mRunId],picoDst->numberOfMtdHits());
    //track level 
    

    electroninfo.clear();
    positroninfo.clear();  
  


    int nTracks = picoDst->numberOfTracks();
    //if (DEBUG)  cout << nTracks <<endl;
    //global
    for (int itrack=0;itrack<nTracks;itrack++){
      StPicoTrack* trk = picoDst->track(itrack);
      bool goodQAtrack = isGoodQaTrack(trk); 
      if (!goodQAtrack) continue;
      if (!(fabs(trk->gDCA(pVtx.x(),pVtx.y(),pVtx.z()))<anaCuts::qaDca)) continue;
      bool isprimary = trk->isPrimary();
      double ptot = trk->gMom(pVtx, picoEvent->bField()).Mag();
      float beta = getTofBeta(trk);
      bool tofmatch =( beta!=std::numeric_limits<float>::quiet_NaN() )&& beta>0;
      bool tofQaPion = false;
      if (tofmatch) {
        float beta_pi = ptot / sqrt(ptot * ptot + M_PION_PLUS * M_PION_PLUS);
        tofQaPion = fabs(1. / beta - 1. / beta_pi) < anaCuts::qaTofPion;
      }
      bool tpcQaPion = fabs(trk->nSigmaPion()) < anaCuts::qaTpcPion;
      //global
      //runbyrunQA
      //pid performance 
      //if (tofQaPion && ptot<1) pTof->Fill(runnum[mRunId],(1./beta)); 
      //if (tpcQaPion && ptot<0.5) {
      //  pDedx->Fill(runnum[mRunId],trk->dEdx());
     // }
      //global tracking performance
      pgPt->Fill(mrunnum[mRunId],trk->gMom().Perp());
      pgDCA->Fill(mrunnum[mRunId],trk->gDCA(pVtx.x(),pVtx.y(),pVtx.z()));
      pgPhi->Fill(mrunnum[mRunId],trk->gMom().Phi());
      pgEta->Fill(mrunnum[mRunId],trk->gMom().Eta());

      //primary 
      if (isprimary){
        ppDCA->Fill(mrunnum[mRunId],trk->gDCA(pVtx.x(),pVtx.y(),pVtx.z()));
        if(tofmatch){pTof->Fill(mrunnum[mRunId],(1./beta));} 
        pDedx->Fill(mrunnum[mRunId],trk->dEdx());
        pNFits->Fill(mrunnum[mRunId],trk->nHitsFit()); 
      
        ppPt->Fill(mrunnum[mRunId],trk->pMom().Perp());
        ppEta->Fill(mrunnum[mRunId],trk->pMom().Eta());
        ppPhi->Fill(mrunnum[mRunId],trk->pMom().Phi());
        if(fabs(pow(trk->pMom().Mag()*sqrt(1-beta*beta)*1.0/beta,2) - 0.019)<0.003 && fabs(trk->nSigmaPion())<5){
           p_nSigmaPion->Fill(mrunnum[mRunId],trk->nSigmaPion());
        }
        if(fabs(pow(trk->pMom().Mag()*sqrt(1-beta*beta)*1.0/beta,2) - 0.243)<0.005 && fabs(trk->nSigmaKaon())<5){
           p_nSigmaKaon->Fill(mrunnum[mRunId],trk->nSigmaKaon());
        }
        if(fabs(pow(trk->pMom().Mag()*sqrt(1-beta*beta)*1.0/beta,2) - 0.879)<0.02 && fabs(trk->nSigmaProton())<5){
           p_nSigmaProton->Fill(mrunnum[mRunId],trk->nSigmaProton());
        }
        


      int pbemcId = trk->bemcPidTraitsIndex();
      if(pbemcId>=0){
         StPicoBEmcPidTraits * pbemctrait = picoDst->bemcPidTraits(pbemcId);
         if(pbemctrait){
            float pnSMDphi = pbemctrait->bemcSmdNPhi();
            ph_nSMDphi->Fill(mrunnum[mRunId],pnSMDphi);
            float pnSMDeta = pbemctrait->bemcSmdNEta();
            ph_nSMDeta->Fill(mrunnum[mRunId],pnSMDeta);
            float pbtowPhiDz = pbemctrait->btowPhiDist();
            ph_btowPhiDz->Fill(mrunnum[mRunId],pbtowPhiDz);
            float pbtowEtaDz = pbemctrait->btowEtaDist();
            ph_btowEtaDz->Fill(mrunnum[mRunId],pbtowEtaDz);
            float pbemcdz = pbemctrait->bemcZDist();
            ph_bemcdz->Fill(mrunnum[mRunId],pbemcdz);
            float pbemcDphi = pbemctrait->bemcPhiDist();
            ph_bemcDphi->Fill(mrunnum[mRunId],pbemcDphi);
            float pE0 = pbemctrait->bemcE0(); 
            ph_p_E0->Fill(mrunnum[mRunId],trk->gMom().Mag()*1.0/pE0); 
           }
      }
    int pmtdId = trk->mtdPidTraitsIndex();
    if(pmtdId>=0){
       StPicoMtdPidTraits * pmtdtrait = picoDst->mtdPidTraits(pmtdId);
       if(pmtdtrait){
          float pmtdDeltaY = pmtdtrait->deltaY();
          ph_mtdDeltaY->Fill(mrunnum[mRunId],pmtdDeltaY);
          float pmtdDeltaZ = pmtdtrait->deltaZ();
          ph_mtdDeltaZ->Fill(mrunnum[mRunId],pmtdDeltaZ);
          float pmtdDeltaTOF = pmtdtrait->deltaTimeOfFlight();
          ph_mtdDeltaTOF->Fill(mrunnum[mRunId],pmtdDeltaTOF);
       }
    }

    int pbtofId = trk->bTofPidTraitsIndex();
    if(pbtofId>=0 && tofmatch){
       StPicoBTofPidTraits * pbtoftrait = picoDst->btofPidTraits(pbtofId);
       if(pbtoftrait){
         // float pbtofnSigmaE = pbtoftrait->nSigmaElectron();
          float pbtofnSigmaPion = pbtoftrait->nSigmaPion();
          float pbtofnSigmaKaon = pbtoftrait->nSigmaKaon();
          float pbtofnSigmaProton = pbtoftrait->nSigmaProton();
          float pbtofYLocal = pbtoftrait->btofYLocal();
          float pbtofZLocal = pbtoftrait->btofZLocal();
          //float pbtofdeltaY = pbtoftrait->deltaY(); 
          //cout<<"btofdeltaY: "<<pbtofdeltaY<<endl;

          p_btofYLocal->Fill(mrunnum[mRunId],pbtofYLocal);  
          p_btofZLocal->Fill(mrunnum[mRunId],pbtofZLocal);  

          h_Vz_btofZLocal->Fill(mVz,pbtofZLocal);
          h_Vz_btofYLocal->Fill(mVz,pbtofYLocal);

        if(fabs(pow(trk->pMom().Mag()*sqrt(1-beta*beta)*1.0/beta,2) - 0.019)<0.003 && fabs(trk->nSigmaPion())<5){
           p_nSigmaTofPion->Fill(mrunnum[mRunId],pbtofnSigmaPion);
        }
        if(fabs(pow(trk->pMom().Mag()*sqrt(1-beta*beta)*1.0/beta,2) - 0.243)<0.005 && fabs(trk->nSigmaKaon())<5){
           p_nSigmaTofKaon->Fill(mrunnum[mRunId],pbtofnSigmaKaon);
        }
        if(fabs(pow(trk->pMom().Mag()*sqrt(1-beta*beta)*1.0/beta,2) - 0.879)<0.02 && fabs(trk->nSigmaProton())<5){
           p_nSigmaTofProton->Fill(mrunnum[mRunId],pbtofnSigmaProton);
        }
       }
    }
     
    int petofId = trk->eTofPidTraitsIndex();
    if(petofId>=0){
      StPicoETofPidTraits * petoftrait = picoDst->etofPidTraits(petofId);
      //int etofmatch = petoftrait->matchFlag();         
      if(petoftrait){
         float petofbeta = 0;
         float petofdeltaX = 0;
         float petofdeltaY = 0; 
         petofbeta = petoftrait->beta(); 
         petofdeltaX = petoftrait->deltaX(); 
         petofdeltaY = petoftrait->deltaY(); 
         int etofmatch = petoftrait->matchFlag();         
     
         if(etofmatch == 1 || etofmatch == 2){
            if(petofbeta != 0){p_ETof_beta->Fill(mrunnum[mRunId],1.0/petofbeta);}
            p_ETof_deltaX->Fill(mrunnum[mRunId],petofdeltaX);
            p_ETof_deltaY->Fill(mrunnum[mRunId],petofdeltaY);
            h_ETof_beta->Fill(1.0/petofbeta);
            h_ETof_deltaX->Fill(petofdeltaX);
            h_ETof_deltaY->Fill(petofdeltaY);
         }
      
      }
    }


      }
    }
  } //runbyrun QA 

  //event and track level QA
  hpassevtcut->Fill(0);
  if (!isBadrun(mRunId)){
  hpassevtcut->Fill(1); //bad run list
  // bool vzcut = fabs(pVtx.z()) < 30;
  bool vzcut = fabs(pVtx.z()) < 202 && fabs(pVtx.z())>198;
  bool verrcut = !(fabs(pVtx.x()) < anaCuts::Verror && fabs(pVtx.y()) < anaCuts::Verror &&
        fabs(pVtx.z()) < anaCuts::Verror);
  bool vrcut =  sqrt(TMath::Power(pVtx.x()+0.3, 2) + TMath::Power(pVtx.y()+2.0, 2)) <=  anaCuts::Vr ;
  // bool vpdvzcut = fabs(pVtx.z() - picoEvent->vzVpd()) < 3;
  bool vpdvzcut = true;
  if (vzcut) hpassevtcut->Fill(2);
  if (vzcut &&  vrcut) hpassevtcut->Fill(3);
  // if (vzcut && vrcut  &&  vpdvzcut ) hpassevtcut->Fill(4);
  if (vzcut && vrcut  &&  vpdvzcut && verrcut ) hpassevtcut->Fill(4);
  bool refusepileup = picoEvent->refMult()<picoEvent->btofTrayMultiplicity()*0.36+45;
  bool refusebadtof = picoEvent->refMult()>picoEvent->btofTrayMultiplicity()*0.28-115;
  bool passCentralityCut = refusepileup && refusebadtof  && verrcut && vrcut && fabs(pVtx.z()) < 10; 
  if (passCentralityCut) hrefmult->Fill(picoEvent->refMult());
  if (isGoodEvent(picoEvent)){
    // StThreeVectorF pVtx = picoEvent->primaryVertex();
    
    
   // if(mRunId < 22106001) return 0;   

    cout<<"star event QA"<<endl;
    TVector3 pVtx = picoEvent->primaryVertex();
    mVx = pVtx.x();
    mVy = pVtx.y();
    mVz = pVtx.z();

    Bfield  = picoEvent->bField();

    h_Vx_Vy->Fill(mVx,mVy);
    mVpdVz = picoEvent->vzVpd();
    hevtcut->Fill(mrunnum[mRunId]);
    hVz->Fill(mVz);
    hVpdVz->Fill(mVpdVz);
    hVxVyVz->Fill(mVx,mVy,mVz);
    hVr->Fill(sqrt(mVy*mVy+mVx*mVx));
    hVzVpdVz->Fill(mVpdVz-mVz);
    h_Vz_VpdVz->Fill(mVz,mVpdVz);  

    //hnTofMulvsRef->Fill(picoEvent->btofTrayMultiplicity(),picoEvent->refMult());  
    hnTofMulvsRef->Fill(picoEvent->refMult(),picoEvent->btofTrayMultiplicity());  
    //hnTofMatvsRef->Fill(picoEvent->nBTOFMatch(),picoEvent->refMult());  
    hnTofMatvsRef->Fill(picoEvent->refMult(),picoEvent->nBTOFMatch());  
    double ntofhits = 0;
//    int ntrack_tof_hits =0; 
    int nTracks = picoDst->numberOfTracks();
    for (int itrack=0;itrack<nTracks;itrack++){
      StPicoTrack* trk = picoDst->track(itrack);
      hgDca->Fill(trk->gDCA(mVx,mVy,mVz));
      bool isprimary = trk->isPrimary();
      bool goodtrack = isGoodTrack(trk,trk->gDCA(mVx,mVy,mVz));
      if (!goodtrack) continue;
      if (!isprimary) continue;
      
      hpDca->Fill(trk->gDCA(mVx,mVy,mVz));

      int bemcId = trk->bemcPidTraitsIndex();
      if(bemcId>=0){
         StPicoBEmcPidTraits * bemctrait = picoDst->bemcPidTraits(bemcId);
         if(bemctrait){
            float nSMDphi = bemctrait->bemcSmdNPhi();
            h_nSMDphi->Fill(nSMDphi);
            float nSMDeta = bemctrait->bemcSmdNEta();
            h_nSMDeta->Fill(nSMDeta);
            float btowPhiDz = bemctrait->btowPhiDist();
            h_btowPhiDz->Fill(btowPhiDz);
            float btowEtaDz = bemctrait->btowEtaDist();
            h_btowEtaDz->Fill(btowEtaDz);
            float bemcdz = bemctrait->bemcZDist();
            h_bemcdz->Fill(bemcdz);
            float bemcDphi = bemctrait->bemcPhiDist();
            h_bemcDphi->Fill(bemcDphi);
            float E0 = bemctrait->bemcE0(); 
            h_p_E0->Fill(trk->gMom().Mag()*1.0/E0); 
           }
       
      }
     
    int mtdId = trk->mtdPidTraitsIndex();
    if(mtdId>=0){
       StPicoMtdPidTraits * mtdtrait = picoDst->mtdPidTraits(mtdId);
       if(mtdtrait){
          float mtdDeltaY = mtdtrait->deltaY();
          h_mtdDeltaY->Fill(mtdDeltaY);
          float mtdDeltaZ = mtdtrait->deltaZ();
          h_mtdDeltaZ->Fill(mtdDeltaZ);
          float mtdDeltaTOF = mtdtrait->deltaTimeOfFlight();
          h_mtdDeltaTOF->Fill(mtdDeltaTOF);
          //cout<<"mtdDeltaTOF: "<<mtdDeltaTOF<<endl;
       }
    }


      // StThreeVectorF mom = trk->pMom(); 
      TVector3 mom = trk->pMom(); 
      hpt->Fill(mom.Perp());
      hGpt->Fill(trk->gMom().Perp());
      hPhi->Fill(mom.Phi());
      hEta->Fill(mom.Eta());
      h_pT_Eta->Fill(mom.Perp()*trk->charge(),mom.Eta());
      h_pT_Phi->Fill(mom.Perp()*trk->charge(),mom.Phi());
      if(trk->charge()>0){
         h_EtavsPhi_Pos->Fill(mom.Eta(),mom.Phi());
      }else{
         h_EtavsPhi_Neg->Fill(mom.Eta(),mom.Phi());
      }
      // hpDca->Fill(trk->pDca(mVx,mVy,mVz));
      hnHitsFit->Fill(trk->nHitsFit()*trk->charge());
      hnHitsPoss->Fill(trk->nHitsPoss()*trk->charge());
      hnHitsDedx->Fill(trk->nHitsDedx()*trk->charge());
      h_nHitsDedx_p->Fill(mom.Mag()*trk->charge(),trk->nHitsDedx());
      
      double beta = getTofBeta(trk);
      bool tofmatch = (beta!=std::numeric_limits<float>::quiet_NaN()) && beta>0;
      //choose inclusive electron
      // bool isTPCElectron =  trk->nSigmaElectron()<2 && trk->nSigmaElectron()>0.75;
      bool isTPCElectron=0;
      if (mom.Mag()>0.8) isTPCElectron =  trk->nSigmaElectron()<2 && trk->nSigmaElectron()>-0.75;
      else isTPCElectron = trk->nSigmaElectron()<2 && trk->nSigmaElectron()>(3*mom.Mag()-3.15);
      bool isTOFElectron = tofmatch?fabs(1./beta-1.)<0.025:false;
  
       h_nSigmaElectron_P_tpc->Fill(mom.Mag(),trk->nSigmaElectron());      

        if (isTOFElectron && isTPCElectron) {
        hnEvsEtavsVz->Fill(mom.Eta(),mVz); 
        hnEvsPhivsVz->Fill(mom.Phi(),mVz);
        
        p_nSigmaE->Fill(mrunnum[mRunId],trk->nSigmaElectron());
        //p_nSigmaTofE->Fill(runnum[mRunId],pbtofnSigmaE);
        
        if(trk->charge()<0 && tofmatch)
         {
            particleinfo.charge = trk->charge();
//            cout<<"charge: "<<trk->charge()<<endl;
            particleinfo.pt = mom.Perp();
            particleinfo.eta = mom.Eta();
            particleinfo.phi = mom.Phi();
            particleinfo.p = mom.Mag();
            particleinfo.nSigmaPi = trk->nSigmaPion();
            particleinfo.beta = beta;
            particleinfo.energy = sqrt(pow(M_electron,2.0)+pow(mom.Mag(),2.0));
            particleinfo.p1 = mom.X();
            particleinfo.p2 = mom.Y();
            particleinfo.p3 = mom.Z();
            electroninfo.push_back(particleinfo);
//            cout<<"debug01"<<endl;
            /* current_eMinus[current_nEMinus].SetPx(mom.x());
             current_eMinus[current_nEMinus].SetPy(mom.y());
             current_eMinus[current_nEMinus].SetPz(mom.z());
             current_eMinus[current_nEMinus].SetE(sqrt(pow(M_electron,2.0)+pow(mom.Mag(),2.0)));
             current_nEMinus++;*/
         }     

        if(trk->charge()>0 && tofmatch)
         {
            particleinfo.charge = trk->charge();
//            cout<<"charge: "<<trk->charge()<<endl;
            particleinfo.pt = mom.Perp();
            particleinfo.eta = mom.Eta();
            particleinfo.phi = mom.Phi();
            particleinfo.p = mom.Mag();
            particleinfo.nSigmaPi = trk->nSigmaPion();
            particleinfo.beta = beta;
            particleinfo.energy = sqrt(pow(M_electron,2.0)+pow(mom.Mag(),2.0));
            particleinfo.p1 = mom.X();
            particleinfo.p2 = mom.Y();
            particleinfo.p3 = mom.Z();
            positroninfo.push_back(particleinfo);
//            cout<<"debug02"<<endl;
             /*current_ePlus[current_nEPlus].SetPx(mom.x());
             current_ePlus[current_nEPlus].SetPy(mom.y());
             current_ePlus[current_nEPlus].SetPz(mom.z());
             current_ePlus[current_nEPlus].SetE(sqrt(pow(M_electron,2.0)+pow(mom.Mag(),2.0)));
             current_nEPlus++;*/
         }

         //current_nE++;
      }
      
      if (tofmatch) {
        ntofhits++;
        hinvBetavsP->Fill(mom.Mag(),1./beta);
        
        if(fabs(1.0/beta - 1) < 0.025)
        {
         h_nSigmaElectron_P->Fill(mom.Mag(),trk->nSigmaElectron()); 
        }       

       // hinvBetavsP_RunID[runnum[mRunId]]->Fill(mom.Mag(),1./beta);

    //    bool istoftrack = trk->isTofTrack();
    //    if(istoftrack && 0.8<1.0/beta && 1.0/beta<0.9 && mom.Mag()>0.4){
    //        ntrack_tof_hits = trk->numberOfBTofHits();
    //        for(int itrack_tof_hits=0;itrack_tof_hits<ntrack_tof_hits;itrack_tof_hits++)
    //           {
    //            StPicoBTofHit* tofhit = trk->btofHit(itrack_tof_hits);
    //            ModuleId_1->Fill(tofhit->module()); 
    //           }
    //   }

        int index2tof = trk->bTofPidTraitsIndex();
        StPicoBTofPidTraits const* const tofPid = mPicoDstMaker->picoDst()->btofPidTraits(index2tof);
        if (1./beta<0.88 && mom.Mag()>0.8 && mom.Mag()<1.5) {
          int tofid = tofPid->btofCellId();
        //  TofId_1->Fill(tofid);
          hBadTofId->Fill(tofid/192+1,(tofid%192)/6+1,tofid%6+1);
        }

        if(1.0/beta>1.13 && 1.0/beta<1.24 && mom.Mag()>0.3 && mom.Mag()<0.5)
         {
            int tofid = tofPid->btofCellId();
            TofId->Fill(tofid);
            TofId_nSigmaPi->Fill(trk->nSigmaPion());
            //TrayId_1->Fill(tofid/192+1);
            //ModuleId_1->Fill((tofid%192)/6+1);
         }

         
        if (isTOFElectron && isTPCElectron) {

        p_nSigmaTofE->Fill(mrunnum[mRunId],tofPid->nSigmaElectron());
        }
       /* if(1.0/beta>0.8 && 1.0/beta<0.9 && mom.Mag()>0.4 && mom.Mag()<3)
         {
            int tofid = tofPid->btofCellId();
            TofId_1->Fill(tofid);
            TrayId_1->Fill(tofid/192+1);
            ModuleId_1->Fill((tofid%192)/6+1);
         }

        if(1.0/beta>0.82 && 1.0/beta<0.9 && mom.Mag()>0.4 && mom.Mag()<3)
         {
           int  tofid = tofPid->btofCellId();
          TofId_2->Fill(tofid);
            ModuleId_2->Fill((tofid%192)/6+1);
         }
        if(1.0/beta>0.82 && 1.0/beta<0.88 && mom.Mag()>0.4 && mom.Mag()<3)
         {
           int  tofid = tofPid->btofCellId();
          TofId_3->Fill(tofid);
            ModuleId_3->Fill((tofid%192)/6+1);
         }
        if(1.0/beta>0.84 && 1.0/beta<0.88 && mom.Mag()>0.4 && mom.Mag()<3)
         {
           int   tofid = tofPid->btofCellId();
          TofId_4->Fill(tofid);
            ModuleId_4->Fill((tofid%192)/6+1);
         }
        if(1.0/beta>0.84 && 1.0/beta<0.9 && mom.Mag()>0.4 && mom.Mag()<3)
         {
           int  tofid = tofPid->btofCellId();
          TofId_5->Fill(tofid);
            ModuleId_5->Fill((tofid%192)/6+1);
         }*/
        hNsigEvsinvBeta->Fill(trk->nSigmaElectron(),1./beta,mom.Mag());
      }
      hdEdx->Fill(mom.Mag()*trk->charge(),trk->dEdx());
      h_mTpc->Fill(mom.Mag()*trk->charge(),pow(mom.Mag()*sqrt(1-beta*beta)*1.0/beta,2));
    }
    hnTofHitvsRef->Fill(ntofhits,picoEvent->refMult());
    
      int x=0;
      int y=0;
      int num_electron = electroninfo.size();
      int num_positron = positroninfo.size();
      float inv_mass=0;
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
                    
                    //if(eepair.Perp()<0.2){hMeeCount_like1->Fill(eepair.M());}
                    hMeeCount_like1->Fill(eepair.M());
                    hMeeCountPt_like1->Fill(eepair.M(),eepair.Perp());
//                    cout<<"debug03"<<endl;
                    //if(eepair.Perp()<=10){hMeelike1_Pt_Cent->Fill(eepair.Perp(),mCentrality,eepair.M());}
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
                    //if(eepair.Perp()<0.2){hMeeCount_like2->Fill(eepair.M());}
                    hMeeCount_like2->Fill(eepair.M());
                    hMeeCountPt_like2->Fill(eepair.M(),eepair.Perp());
                    //if(eepair.Perp()<=10){hMeelike2_Pt_Cent->Fill(eepair.Perp(),mCentrality,eepair.M());}
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

                    if(eepair.M()>0.065 && eepair.M()<0.075){
                      hMee_phiV->Fill(angleV);
                    }
                    
                    hMeeCountPhiV->Fill(eepair.M(),angleV);

                    //if(eepair.Perp()<0.2){hMeeCount->Fill(eepair.M());}
                    hMeeCount->Fill(eepair.M());
                    hMeeCountPt->Fill(eepair.M(),eepair.Perp());
                    //if(eepair.Perp()<=10){hMee_Pt_Cent->Fill(eepair.Perp(),mCentrality,eepair.M());}
                  }
         }


  } //Good Event
 }
  cout<<"end make"<<endl;
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
bool StPicoDstarMixedMaker::isGoodTrack(StPicoTrack const* trk, float dca) const
{
  // StThreeVectorF const vtx = mPicoDstMaker->picoDst()->event()->primaryVertex();
  return trk->gPt() > anaCuts::GPt && fabs(trk->nHitsFit()) >= anaCuts::NHitsFit && 
    fabs(trk->gMom().Eta())<anaCuts::Eta &&
    // fabs(trk->nHitsFit()*1.0/trk->nHitsMax())>=anaCuts::NHitsFit2Poss &&
    fabs(trk->nHitsDedx())>=anaCuts::NHitsDedx && fabs(dca)<=anaCuts::Dca;
    // fabs(trk->nHitsDedx())>=anaCuts::NHitsDedx &&
    // fabs(trk->gDCA(vtx.x() , vtx.y(), vtx.z() )) <= anaCuts::Dca;
}
bool StPicoDstarMixedMaker::isGoodQaTrack(StPicoTrack const* const trk) const
{
  // StThreeVectorF vtx = mPicoDstMaker->picoDst()->event()->primaryVertex();
  return trk->gPt() > anaCuts::qaGPt && fabs(trk->nHitsFit()) >= anaCuts::qaNHitsFit && 
    fabs(trk->gMom().Eta())<anaCuts::qaEta &&
    // fabs(trk->nHitsFit()*1.0/trk->nHitsMax())>=anaCuts::NHitsFit2Poss &&
    // fabs(trk->nHitsDedx())>=anaCuts::qaNHitsDedx && fabs(trk->gDCA(vtx.x(),vtx.y(),vtx.z()))<=anaCuts::qaDca;
    fabs(trk->nHitsDedx())>=anaCuts::qaNHitsDedx;
}
bool StPicoDstarMixedMaker::isGoodQaEvent(StPicoEvent const* const picoEvent) const
{
  TVector3 pVtx = picoEvent->primaryVertex();
  // StThreeVectorF pVtx = picoEvent->primaryVertex();
  return fabs(pVtx.z()) < anaCuts::qavz &&
    // fabs(pVtx.z() - picoEvent->vzVpd()) < anaCuts::qavzVpdVz &&
    //!(fabs(pVtx.x()) < anaCuts::qaVerror && fabs(pVtx.y()) < anaCuts::qaVerror &&
    //    fabs(pVtx.z()) < anaCuts::qaVerror) &&
    sqrt(TMath::Power(pVtx.x(), 2) + TMath::Power(pVtx.y(), 2)) <=  anaCuts::qaVr;
}
bool StPicoDstarMixedMaker::isGoodEvent(StPicoEvent const* const picoEvent) const
{
  TVector3 pVtx = picoEvent->primaryVertex();
  // StThreeVectorF pVtx = picoEvent->primaryVertex();
  return fabs(pVtx.z()) < anaCuts::vz_high &&
         fabs(pVtx.z()) > anaCuts::vz_low &&
    // fabs(pVtx.z() - picoEvent->vzVpd()) < anaCuts::vzVpdVz &&
   // !(fabs(pVtx.x()) < anaCuts::Verror && fabs(pVtx.y()) < anaCuts::Verror &&
     //   fabs(pVtx.z()) < anaCuts::Verror) &&
    sqrt(TMath::Power(pVtx.x()+0.3, 2) + TMath::Power(pVtx.y()+2, 2)) <=  anaCuts::Vr;
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
        // StThreeVectorF const vtx = mPicoDstMaker->picoDst()->event()->primaryVertex();
        TVector3 const vtx3 = mPicoDstMaker->picoDst()->event()->primaryVertex();
        StThreeVectorF vtx(vtx3.x(),vtx3.y(),vtx3.z());
        TVector3 const btofHitPos3 = tofPid->btofHitPos();
        // StThreeVectorF const btofHitPos = tofPid->btofHitPos();
        StThreeVectorF btofHitPos(btofHitPos3.x(),btofHitPos3.y(),btofHitPos3.z());
        StPicoPhysicalHelix helix = trk->helix(mPicoDstMaker->picoDst()->event()->bField());
        // StPhysicalHelixD helix = trk->helix(mPicoDstMaker->picoDst()->event()->bField());
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


