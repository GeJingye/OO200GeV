#ifndef StPicoDstarMixedMaker_h
#define StPicoDstarMixedMaker_h

/* **************************************************
 *  A Maker to read a StPicoEvent and StPicoDstarEvent
 *  simultaneously and do analysis.
 *
 *  Authors: Guannan Xie <guannanxie@lbl.gov>
 *           Mustafa Mustafa <mmustafa@lbl.gov>
 *
 * **************************************************
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


const double etaCorr[8][20] = 
{
 {1.13877, 1.04923, 0.710773, 0.414649, -0.0321807, -0.26596, -0.641656, -0.890378, -0.985173, -1.126, -1.05834, -0.906711, -0.757652, -0.459274, -0.118759, 0.155855, 0.489225, 0.805639, 0.997073, 1.05951},
 {1.15565, 0.925616, 0.708409, 0.395575, 0.00163209, -0.299634, -0.515818, -0.908757, -1.03, -1.01072, -1.01852, -0.926232, -0.713519, -0.514964, -0.192768, 0.175521, 0.532425, 0.757125, 0.939195, 1.37085},
 {1.02623, 1.03749, 0.630063, 0.361894, 0.0434215, -0.351991, -0.579348, -0.790651, -1.00179, -1.14478, -1.05117, -0.990214, -0.690814, -0.430004, -0.111031, 0.189025, 0.51749, 0.752075, 1.11737, 1.01032},
 {1.15381, 0.869568, 0.66003, 0.403814, 0.0549524, -0.2703, -0.580425, -0.790577, -1.02077, -1.12707, -1.03619, -0.96376, -0.725199, -0.477682, -0.200653, 0.165797, 0.525758, 0.800787, 0.980626, 1.25356},
 {1.22072, 0.954147, 0.670658, 0.354593, 0.0438945, -0.308818, -0.523904, -0.813536, -1.00427, -1.09289, -1.02329, -0.921369, -0.71094, -0.464872, -0.159844, 0.174581, 0.495894, 0.742782, 0.998511, 1.32225},
 {1.24898, 0.940487, 0.67223, 0.358221, 0.0312774, -0.276794, -0.559973, -0.792032, -1.00129, -1.08323, -1.0401, -0.899318, -0.727168, -0.455791, -0.12586, 0.165967, 0.472968, 0.778234, 1.02252, 1.38896},
 {1.19066, 0.95118, 0.679281, 0.378808, 0.0382586, -0.277997, -0.564533, -0.837063, -1.0202, -1.0877, -1.02664, -0.926954, -0.699056, -0.453378, -0.163139, 0.15837, 0.483363, 0.785403, 1.02657, 1.36677},
 {1.29561, 0.945692, 0.681653, 0.375245, 0.0455246, -0.278552, -0.57146, -0.82279, -1.00434, -1.08152, -1.03246, -0.93454, -0.70738, -0.466531, -0.146205, 0.154341, 0.480298, 0.782615, 1.04015, 1.19135},
};

const double phiCorr[8][64] = 
{
 {-0.11284, -0.232304, 0.00589941, -0.175022, -0.112672, -0.333041, -0.248924, -0.0772527, 0.152678, -0.357699, -0.330014, -0.100139, -0.0527093, -0.196981, -0.183298, -0.299709, -0.164887, 0.093222, 0.145521, -0.056967, -0.111056, -0.19322, -0.156671, -0.054952, -0.0345147, -0.0630587, -0.399333, -0.184433, -0.112429, -0.129166, 0.038272, -0.0917096, -0.182912, -0.0272267, 0.183803, 0.187725, -0.0543206, -0.158248, -0.163905, -0.0450071, -0.0701144, 0.0643654, -0.254659, -0.11779, -0.0773751, 0.0138421, -0.184573, -0.261407, -0.103412, 0.135108, 0.1463, -0.251043, -0.2838, -0.25673, -0.0566395, -0.256952, -0.196355, -0.265201, -0.33483, -0.250487, -0.176901, -0.138779, -0.328791, -0.352228},
 {-0.163121, -0.053478, -0.188846, -0.220726, -0.286108, -0.44886, -0.194289, 0.0161437, -0.0583847, -0.25105, -0.373883, -0.257256, -0.0565644, -0.164015, -0.293561, -0.213435, -0.22684, 0.0389408, 0.0472745, -0.00896126, -0.158367, -0.195348, -0.20474, -0.152682, 0.0704356, -0.192341, -0.232742, -0.0844584, 0.0144092, 0.0682486, -0.0229936, -0.0881033, -0.0451467, -0.0393852, 0.214681, 0.141791, -0.0622045, -0.165715, -0.192339, -0.0528561, 0.00446745, -0.153567, -0.221348, -0.159577, -0.138581, 0.0300989, -0.0736428, -0.375914, 0.00593157, -0.12491, -0.0212865, -0.120343, -0.223216, -0.257603, -0.145122, -0.103888, -0.162954, -0.505107, -0.25846, -0.179594, -0.229981, -0.147108, -0.316694, -0.195517},
 {-0.226679, -0.106794, -0.0435003, -0.10549, -0.231466, -0.276974, 0.00873606, 0.00404522, -0.150661, -0.279596, -0.375255, -0.2189, -0.0514276, 0.0995574, -0.0648955, -0.291306, -0.198277, -0.00840594, 0.0393265, 0.0129071, -0.169806, -0.136392, -0.166372, -0.0547929, 0.0487871, -0.186656, -0.226119, -0.337858, -0.127111, 0.0739659, 0.065261, -0.167874, -0.0401956, -0.222113, 0.139271, 0.361963, -0.0111616, -0.119417, -0.133585, 0.0670999, 0.0235782, -0.0168036, -0.312314, -0.147996, -0.0400311, 0.0306866, -0.170527, -0.207171, -0.256949, 0.0803783, -0.131134, -0.0126937, -0.244315, 0.0492682, -0.185606, -0.184135, -0.330548, -0.228943, -0.339479, -0.227454, -0.0426994, -0.275799, -0.316085, -0.160221},
 {-0.251006, -0.0922764, -0.00752426, -0.222253, -0.362718, -0.259176, -0.270349, -0.122625, -0.201433, -0.186196, -0.324682, -0.268088, -0.135987, -0.122672, -0.119755, -0.205253, -0.17166, -0.00842706, -0.00249371, 0.0183219, -0.212402, -0.240757, -0.177091, -0.170176, 0.00940934, -0.129572, -0.175694, -0.0869524, -0.0475537, 0.156698, 0.0165805, 0.0447954, -0.110465, -0.0172469, 0.139665, 0.242532, -0.145469, -0.0964798, -0.039954, -0.0719663, -0.0187959, -0.129931, -0.216957, -0.239408, -0.136251, 0.0692026, -0.0705542, -0.166449, -0.102363, -0.0654787, -0.0842612, -0.159619, -0.318996, -0.268923, -0.219688, -0.269416, -0.292597, -0.316248, -0.311107, -0.150354, -0.269687, -0.178154, -0.248936, -0.187648},
 {-0.196524, -0.0195845, -0.00335295, -0.148796, -0.227142, -0.242758, -0.195416, 0.0140668, -0.0833217, -0.187684, -0.306126, -0.282865, -0.208802, -0.0623762, -0.163688, -0.210898, -0.193455, -0.0277964, 0.0124164, 0.0335456, -0.206314, -0.274728, -0.227727, -0.0644723, 0.000609655, -0.13906, -0.211275, -0.210611, -0.0644079, 0.0138719, 0.0640164, -0.0960215, -0.103641, 7.63876e-05, 0.108223, 0.0805066, -0.0466794, -0.0981636, -0.0925812, 0.0644285, -0.0137872, -0.0694668, -0.190561, -0.19414, -0.143238, -0.0722723, -0.120997, -0.228305, -0.182884, -0.0497482, -0.0417466, -0.0453452, -0.153289, -0.290358, -0.178444, -0.192458, -0.273299, -0.355674, -0.312194, -0.243684, -0.280912, -0.287083, -0.317492, -0.320264},
 {-0.208452, -0.0888174, -0.0167921, -0.151158, -0.270861, -0.283577, -0.163066, -0.00331697, -0.0883874, -0.207209, -0.278636, -0.330743, -0.152399, -0.118564, -0.107873, -0.135224, -0.183612, -0.0555863, 0.0801835, -0.0355451, -0.106286, -0.247769, -0.20017, -0.107181, 0.0432923, -0.156161, -0.254843, -0.192961, -0.128178, 0.0676384, 0.0289071, -0.100051, -0.156021, -0.0536301, 0.0927824, 0.165582, -0.042361, -0.219591, -0.132368, 0.0103485, -0.00239894, -0.15885, -0.244616, -0.196904, -0.0899476, -0.000339335, -0.122534, -0.13056, -0.172387, -0.0271737, -0.166074, -0.19048, -0.245153, -0.171012, -0.302276, -0.222505, -0.254723, -0.317722, -0.303643, -0.141223, -0.192253, -0.22218, -0.296859, -0.278566},
 {-0.285591, -0.0826779, 0.00401203, -0.100027, -0.200918, -0.229819, -0.183155, -0.107476, -0.129588, -0.225297, -0.345661, -0.242866, -0.0979808, -0.108842, -0.137485, -0.217928, -0.205202, -0.0501376, 0.00392701, -0.00888051, -0.11295, -0.192388, -0.19317, -0.188396, -0.098647, -0.16569, -0.223169, -0.322228, -0.0587308, 0.0700177, -0.0249787, -0.06731, -0.0841107, -0.0227021, 0.126526, 0.118805, -0.0382478, -0.126875, -0.149237, -0.0544699, -0.0628789, -0.11823, -0.180669, -0.186468, -0.0883503, -0.0498316, -0.147243, -0.214747, -0.109031, 0.00266815, -0.0644261, -0.15588, -0.151451, -0.226188, -0.201783, -0.142174, -0.243216, -0.344618, -0.313411, -0.223025, -0.238067, -0.207546, -0.230263, -0.251531},
 {-0.211249, -0.0651862, -0.0157197, -0.0876736, -0.178473, -0.234956, -0.17012, -0.0572875, -0.174979, -0.239189, -0.319963, -0.282611, -0.160062, -0.0992396, -0.166256, -0.210071, -0.211607, -0.0250179, 0.0112845, 0.0364213, -0.159543, -0.221612, -0.191711, -0.100944, -0.0435091, -0.115611, -0.286062, -0.242086, -0.0309578, 0.0792093, 0.0466773, -0.079648, -0.0969507, -0.0114835, 0.138349, 0.122545, -0.0453625, -0.16295, -0.146016, -0.0321351, -0.00463731, -0.1293, -0.237082, -0.205542, -0.131927, -0.0734227, -0.102163, -0.146695, -0.19024, -0.0771011, -0.0935595, -0.162498, -0.164865, -0.24016, -0.181546, -0.225942, -0.24071, -0.3425, -0.336291, -0.220834, -0.176327, -0.235632, -0.302743, -0.26153},
};

const double PhiAverageCorr[8] = {-0.128874, -0.137669, -0.116487, -0.137203, -0.136909, -0.139672, -0.139186, -0.139453};

struct ParticleInfo_Electron
{
    Int_t charge;
    Float_t pt;
    Float_t eta;
    Float_t phi;
    Float_t p;
    Float_t nSigmaE;
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
    bool PhiV_Cut;
    // bool isPure_Mee_lt_0p015;
};

//vector<ParticleInfo> ElectronInfo;

class StPicoDstarMixedMaker : public StMaker
{
  public:
    StPicoDstarMixedMaker(char const * name, TString const inputFilesList,
        TString const outBaseName, StPicoDstMaker* picoDstMaker);
    virtual ~StPicoDstarMixedMaker();

    virtual Int_t Init();
    virtual Int_t Make();
    virtual Int_t Finish();
  private:

    StPicoDstarMixedMaker() {}
    void initHists();

    bool isGoodTrigger(StPicoEvent const*) const;
    bool isGoodQaEvent(StPicoEvent const* const picoEvent) const;
    bool isGoodEvent(StPicoEvent const* const picoEvent) const;
    bool isGoodQaTrack(StPicoTrack const* const trk) const;
    bool isGoodTrack(StPicoTrack const* trk,float dca) const;
    float getTofBeta(StPicoTrack const* const trk) const;
    void copyCurrentToBuffer();
    double nSigmaECorr(TVector3 mom, int mCentrality) const;
    Double_t phiVAngle(TLorentzVector e1, TLorentzVector e2, Int_t q1, Int_t q2) const;
    Int_t getRefmult6(StPicoDst const* const picoDst, StPicoEvent const* const picoEvent) const;
    Double_t getTotnMIP(StPicoDst const* const picoDst) const;
    int getShiftedEP();
    double getEventPlaneShift(double EP_Re, int cent) const;

    StPicoDstMaker* mPicoDstMaker;
    StRefMultCorr* mRefMultCorrUtil;
    TString mInputFilesList;
    TString mOutFileBaseName;
    
    // -------------- USER variables -------------------------
    // add your member variables here. 
    // Remember that ntuples size can be really big, use histograms where appropriate 

  public:
    void setRunNumList(string list){
      mRunNumList = list;
    }
    void setRunbyRunQA(bool b){
      mRunbyRunQA = b;
    }

    void getBadruns(string inputFileName);
    void getBadTofCells(string inputFileName);
    void setRecenterFile(string recenterfilename);
    void setShiftFile(string shiftfilename);
    bool isBadrun(Int_t runId);
    bool isGoodBTofCell(int const tofid);

  private: 
    TFile* mFile;
    std::map<int,int> runnum;
    string mRunNumList;
    vector<int> mBadRun;
    vector<int> mBadTofCell;
    string mRecenterFile;
    string mShiftFile;

    // EP, Shuai's method
    TProfile* pQxRecenter_etaplus_zplus[9];
    TProfile* pQyRecenter_etaplus_zplus[9];
    TProfile* pQxRecenter_etaplus_zminus[9];
    TProfile* pQyRecenter_etaplus_zminus[9];
    TProfile* pQxRecenter_etaminus_zplus[9];
    TProfile* pQyRecenter_etaminus_zplus[9];
    TProfile* pQxRecenter_etaminus_zminus[9];
    TProfile* pQyRecenter_etaminus_zminus[9];

    TProfile* pSinEta_Shuai[9];
    TProfile* pCosEta_Shuai[9];

    //Event level
    int  mRunId;
    float mEP_Psi2;
    float mEP_Psi2_woele;
    
    //primaryVertex vertex
    float mVx;
    float mVy;
    float mVr;
    float mVz;
    float mVpdVz;    

    Int_t mRefmult6;
    Double_t mTotnMIP;

    TF1* phiVcut;
    
    TH1F* hevt;
    TH1F* hpassevtcut;
    TH1F* hevt_cen;
    TH1F* hevt_cen_reweight;
    TH1F* hRefMult6;
    TH1F* hTotnMIP;

    //Run by run QA
    bool mRunbyRunQA; 
    
    // Histograms
    //event QA
    TH3F* hVxVyVz;
    TH1F* hVz;
    TH1F* hVpdVz;
    TH2F* h_Vz_VpdVz;
    TH1F* hVr;
    TH1F* hVzVpdVz;
    TH2F* h_Vx_Vy;
    TH2F* hnTofMatvsRef;
    TH1F* hrefmult;
    TH3F* hDCA_Eta_NHitsFit;

    TH1F* hEP_Shuai_Raw[9];
    TH1F* hEP_Shuai_Recenter[9];
    TH1F* hEP_Shuai_Shift[9];
    TH1F* hEP_Shuai_Raw_woele[9];
    TH1F* hEP_Shuai_Recenter_woele[9];
    TH1F* hEP_Shuai_Shift_woele[9];

    //tracl level QA
    TH1F* hnHitsFit;
    TH1F* hnHitsPoss;
    TH1F* hnHitsDedx;
    TH2F* h_nHitsDedx_p;
    TH2F* hinvBetavsP;
    TH2F* hdEdx;
    TH1F* h_m2;
    TH2F* h_mTpc;
    TH1F* hpDca;
    TH1F* hppt;
    TH1F* hpEta;
    TH1F* hpPhi;    
    TH2F* h_pT_Eta;
    TH2F* h_pT_Phi;

    TH1F* h_pT_TOFmatch;

    TH1F* h_pT_electrons;
    TH1F* h_eta_electrons;
    TH1F* h_eta_electrons_w_PhiV_Cut;
    TH1F* h_phi_electrons;
    TH1F* h_phi_electrons_w_PhiV_Cut;
    
    TH2F* h_nSigmaElectron_P_tpc;
    TH2F* h_nSigmaPion_P_tpc;
    TH2F* h_nSigmaKaon_P_tpc;
    TH2F* h_nSigmaProton_P_tpc;
    
    TH2F* nSigmaE_P_Pion;
    TH2F* nSigmaE_P_Kaon;
    TH2F* nSigmaE_P_Proton;
    TH2F* nSigmaE_P_MergedPion;

    // TH2F* h_nSigmaElectron_eta_1_1p5;
    // TH2F* h_nSigmaElectron_eta_1_1p5_lowpT;
    TH2F* h_nSigmaElectron_P_wo_pTCut;
    TH2F* h_nSigmaPion_P_wo_pTCut;
    TH2F* h_nSigmaElectron_P_wo_pTCut_w_nSigmaPion;
    TH2F* h_nSigmaElectron_P_wo_pTCut_w_nSigmaOther;
    // TH2F* h_nSigmaElectron_P_wo_pTCut_lowpT;
    
    TH2F* h_nSigmaElectron_P; // with 1/beta cut

    TH2F* hMeeCountPhiV;
    TH1F* hMeeCount_wo_PhiV_Cut;
    TH1F* hMeeCount_w_PhiV_Cut;

    TH3F* hMeeCountPtCen;
    TH3F* hMeeCountPtCen_like1;
    TH3F* hMeeCountPtCen_like2;

    TH3F* hMeeMixPtCen;
    TH3F* hMeeMixPtCen_like1;
    TH3F* hMeeMixPtCen_like2;

    const float M_electron=0.000511;//GeV
    const float PI=TMath::Pi();

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
    cout << " get Bad runs list: [OK]" << endl;
}
inline void StPicoDstarMixedMaker::getBadTofCells(string inputFileName)
{
    ifstream fin(inputFileName.c_str());
    if(!fin)
    {
      cout <<"no Bad BTOF Cells list" << endl;
      return;
    }
    cout << "  " << inputFileName << flush;

    Int_t runId = 0 ;
    while( fin >> runId )
    {
      mBadTofCell.push_back(runId);
    }
    cout << " get Bad BTOF Cells list: [OK]" << endl;
}
inline bool StPicoDstarMixedMaker::isBadrun(Int_t runId)
{
    vector<Int_t>::iterator iter = std::find(mBadRun.begin(), mBadRun.end(), runId);
    return ( iter != mBadRun.end() ) ;
}

inline bool StPicoDstarMixedMaker::isGoodBTofCell(int const tofid)
{
  if(std::count(mBadTofCell.begin(),mBadTofCell.end(),tofid)>0) return false;
  else return true;
}

inline void StPicoDstarMixedMaker::setRecenterFile(string recenterfilename)
{
  mRecenterFile = recenterfilename;
}

inline void StPicoDstarMixedMaker::setShiftFile(string shiftfilename)
{
  mShiftFile = shiftfilename;
}

#endif
