#define Mpion 0.13957
#define Mkaon 0.493677
#define Mproton 0.93827231
#define Melectron 0.00051099907
#define PI 3.1415927

#include <iostream>
#include <iomanip>
#include <fstream>
#include <cstdlib>
#include "sys/types.h"
#include "dirent.h"
#include <ctime>

#include "math.h"
#include "string.h"
#include <iomanip>

#ifndef __CINT__  
#include "TROOT.h"
#include "TFile.h"
#include "TChain.h"
#include "TMath.h"
#include "TH1.h"
#include "TH2.h"
#include "TH3.h"
#include "TF1.h"
#include "TStyle.h"
#include "TCanvas.h"
#include "TProfile.h"
#include "TVector2.h"
#include "TTree.h"
#include "TNtuple.h"
#include "TRandom.h"
#include "TRandom3.h"
#include "TUnixSystem.h"
#include "TVector3.h"

#include "mcEvent.h"
#include "heads.h"
#include "mBadRuns_Run18_27GeV.h"
#include "cuts.h"
using namespace std;
using std::cout;
using std::endl;
using std::setw;
#endif
//---------------------------------------------------------------------
//---------------------------------------------------------------------
const TString subCentNames[3] = {"Cent0_10", "Cent10_40", "Cent40_80"};
TFile *infile_ab[3];//0:0-10%, 1:10-40%, 2:40-80%
TH1F* H_ABf_Parms[3];
//---------------------------------------------------------------------
//---------------------------------------------------------------------
void  init(int mRoundIndex);
bool  passEvent(mcEvent* mEvent, int mRoundIndex );
bool  passTrack(mcEvent* mcEvt,  int mRoundIndex);
void  bookHistograms( char *outFile,  int mRoundIndex );
void  writeHistograms();
double getSigmaPt2Pt( double mcpt, int    icent9);
double getPtEtaWeight(double rcpt, double rceta);
int cent9  = -1;
int cent16 = -1;
double refmultcorr = 0.;
double reweight    = 0.;
double wt_nEfrac   = 0.;
//---------------------------------------------------------------------
//---------------------------------------------------------------------

//---------------------------------------------------------------------
//---------------------------------------------------------------------

int main(int argc, char** argv)
{
	if(argc!=1&&argc!=4) return -1;

	char *inFile="test.list";
	char outFile[1024];
	sprintf(outFile,"test");

	if(argc==4)
	{
		inFile = argv[1];
		sprintf(outFile,"%s",argv[2]);
		mRoundIndex = atoi(argv[3]);
	}
	else
	{
		cout<<"wrong input, please run like ./readEmbdMini test.list testout roundindex"<<endl;
		return 0;
	}

	//+---------------------------------+
	//| open files and add to the chain |
	//+---------------------------------+
	TChain *chain = new TChain("mcT");
	Int_t ifile=0;
	char filename[512];
	ifstream *inputStream = new ifstream;
	inputStream->open(inFile);

	if (!(inputStream)) 
	{
		printf("can not open list file\n");
		return 0;
	}

	for(;inputStream->good();)
	{
		inputStream->getline(filename,512);
		if(inputStream->good()) 
		{
			TFile *ftmp = new TFile(filename);
			if(!ftmp||!(ftmp->IsOpen())||!(ftmp->GetNkeys())) 
			{
				cout<<"something wrong"<<endl;
			} 
			else 
			{
				cout<<"read in "<<ifile<<"th file: "<<filename<<endl;
				chain->Add(filename);
				ifile++;
			}
			delete ftmp;
		}
	}
	delete inputStream;

	//----------------------------------------------------------------
	//----------------------------------------------------------------
	init( mRoundIndex );
	bookHistograms(outFile, mRoundIndex);
	//----------------------------------------------------------------
	//----------------------------------------------------------------

	mcEvent *mcEvt = new mcEvent(chain);
	Int_t nEvents = chain->GetEntries();

	cout<<" nEvents = "<<nEvents<<endl;

	//+-------------+
	//| loop mEvents |
	//+-------------+
	for(int i=0; i<nEvents; i++)
	{
		if( i%2000==0 )
		{
			cout << "begin " << i << "th entry...." << endl;
		}

		mcEvt->GetEntry(i);

		bool isEventPassed = passEvent(mcEvt, mRoundIndex);
		if(isEventPassed) passTrack(mcEvt, mRoundIndex);

	} //end of mEvent loop

	writeHistograms(); 
	delete chain;
	cout<<"end of program"<<endl;
	return 0;
}
//_____________________________________________________________________
bool passEvent(mcEvent* mEvent, int mRoundIndex)
{
	//---------------------------------------------------------------------------------
	//---------------------------------------------------------------------------------
	Int_t runid         = (int) mEvent->runId;
	for( int irun = 0; irun<mNBadRuns; irun++ ){ if( runid == mBadRuns[irun] ) return kFALSE; }
	//---------------------------------------------------------------------------------
	//---------------------------------------------------------------------------------
	
	//----------------------------------------------------------------------------------------------------
	if (!mEvent)              return kFALSE;
	if (mEvent->mcEvtId<0)    return kFALSE;
	//----------------------------------------------------------------------------------------------------

	double mcVz = mEvent->mcVertexZ;
	double mcVx = mEvent->mcVertexX;
	double mcVy = mEvent->mcVertexY;
	//double mcVr = sqrt( mcVx*mcVx + mcVy*mcVy );

	double rcVz = mEvent->rcVertexZ;
	double rcVx = mEvent->rcVertexX;
	double rcVy = mEvent->rcVertexY;
	double rcVr = sqrt(rcVx*rcVx + rcVy*rcVy);

	cent9       = mEvent->cent9;
	cent16      = mEvent->cent16;

	refmultcorr = mEvent->rcRefMultCorr;
	reweight    = mEvent->reWeight;

	hMc_Vz     ->Fill( mcVz );
	hRc_Vz     ->Fill( rcVz );
	hMc_VxVy   ->Fill( mcVx, mcVy );
	hRc_VxVy   ->Fill( rcVx, rcVy );
	hVz_McVsRc ->Fill( rcVz, mcVz );

	//----------------------------------------------------------------------------------------------------
	//if( fabs(rcVz)>70. ) return kFALSE;
	if( fabs(rcVz)>35. ) return kFALSE;
	if( rcVr>2.0 )       return kFALSE;
	if( cent9<0 )        return kFALSE;
	//----------------------------------------------------------------------------------------------------
	hcent9      ->Fill(cent9,       reweight);
	hcent16     ->Fill(cent16,      reweight);
	hRefMultCorr->Fill(refmultcorr, reweight);

	double nMcEoverNRefmult = 1.*mEvent->nMcE/(1.*mEvent->rcRefMult);

	h_nEfracVsCent9  ->Fill(cent9,  nMcEoverNRefmult);
	h_nEfracVsCent16 ->Fill(cent16, nMcEoverNRefmult);
	h_nEvsNRefmult   ->Fill(1.*mEvent->rcRefMult, mEvent->nMcE);

	wt_nEfrac = (0.05/nMcEoverNRefmult);

	h_nEfracVsCent9_aftwt  ->Fill(cent9,  nMcEoverNRefmult*wt_nEfrac);
	h_nEfracVsCent16_aftwt ->Fill(cent16, nMcEoverNRefmult*wt_nEfrac);
	h_nEvsNRefmult_aftwt   ->Fill(1.*mEvent->rcRefMult, mEvent->nMcE*wt_nEfrac);

	return kTRUE;
}

//----------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------
bool passTrack(mcEvent* mEvent, int mRoundIndex)
{
	int nTracks = mEvent->nMcE;
	if(nTracks<=0) return kFALSE;

	//loop over tracks: electrons
	for(int itrk=0; itrk<nTracks; itrk++)
	{
		//----------------------------------------------------------------------------------------------------
		//if( mEvent->mcTrkKey[itrk]<0 )                               continue;
		if( mEvent->geantId[itrk] !=2 && mEvent->geantId[itrk] !=3 ) continue;//let electron-3 or positron-2 pass here
		//----------------------------------------------------------------------------------------------------
		Int_t    mComnHits = mEvent->rcNHitsCommon[itrk];
		Int_t    nHitsFit  = mEvent->rcNHitsFit[itrk];
		Int_t    nHitsDedx = mEvent->rcNHitsDedx[itrk];
		Int_t    nHitsMax  = mEvent->rcNHitsPoss[itrk];
		Double_t nHitsFrac = 1.*nHitsFit/nHitsMax;
		Double_t Dca       = mEvent->rcDca[itrk];

		Double_t mcPt      = mEvent->mcPt[itrk];
		Double_t mcEta     = mEvent->mcEta[itrk];
		Double_t mcPhi     = mEvent->mcPhi[itrk];
		if( mcPhi < -PI+PI/12 ) mcPhi += 2*PI;

		Double_t rcPt      = mEvent->rcPtFirst[itrk];//wrong name, but correct value
		Double_t rcEta     = mEvent->rcEtaFirst[itrk];
		Double_t rcPhi     = mEvent->rcPhiFirst[itrk];
		if( rcPhi < -PI+PI/12 ) rcPhi += 2*PI;

		//----------------------------------------------------------------------------------------------------
		hMc_ComnHitsPtEta      ->Fill( mComnHits, mcPt,     mcEta, reweight*wt_nEfrac);
		hRc_ComnHitsPtEta      ->Fill( mComnHits, rcPt,     rcEta, reweight*wt_nEfrac);
		hRc_ComnHitsnHitsFitPt ->Fill( mComnHits, nHitsFit, rcPt , reweight*wt_nEfrac);
		//----------------------------------------------------------------------------------------------------

		//----------------------------------------------------------------------------------------------------
		//----------------------------------------------------------------------------------------------------
		//apply the acceptance cut |mcEta|<1.0
		if( fabs(mcEta)>1.0)      continue;
		//----------------------------------------------------------------------------------------------------
		//----------------------------------------------------------------------------------------------------

		//----------------------------------------------------------------------------------------------------
		h3d_Mc_PtEtaPhi_perCent[cent9] -> Fill(mcPt, mcEta, mcPhi, reweight*wt_nEfrac); //for single e Eff., as denominator
		//----------------------------------------------------------------------------------------------------

		//----------------------------------------------------------------------------------------------------
		if( fabs(rcEta)>1.0 || rcPt<=0.0)      continue;
		//h3d_Rc_PtEtaPhi_perCent[cent9][0] -> Fill(rcPt, rcEta, rcPhi);
		h3d_Rc_PtEtaPhi_perCent[cent9][0] -> Fill(mcPt, mcEta, mcPhi, reweight*wt_nEfrac);
		//----------------------------------------------------------------------------------------------------

		//----------------------------------------------------------------------------------------------------
		//fill QA histograms for comparing to Real Data QA
		//----------------------------------------------------------------------------------------------------
		h3d_nHitsFit_vsPtEta       ->Fill(nHitsFit,  rcPt, rcEta, reweight*wt_nEfrac);
		h3d_nHitsDedx_vsPtEta      ->Fill(nHitsDedx, rcPt, rcEta, reweight*wt_nEfrac);
		h3d_nHitsFrac_vsPtEta      ->Fill(nHitsFrac, rcPt, rcEta, reweight*wt_nEfrac);
		h3d_Dca_vsPtEta            ->Fill(Dca,       rcPt, rcEta, reweight*wt_nEfrac);
		h3d_Phi_vsPtEta            ->Fill(rcPhi,     rcPt, rcEta, reweight*wt_nEfrac);

		if(mRoundIndex>=1)
		{
			//Fill QA for tracking syst. errors
			h3d_nHitsFit_vsPtEta_Cen[cent9]  ->Fill(nHitsFit,  rcPt, rcEta, reweight*wt_nEfrac);
			h3d_nHitsDedx_vsPtEta_Cen[cent9] ->Fill(nHitsDedx, rcPt, rcEta, reweight*wt_nEfrac);
			h3d_nHitsFrac_vsPtEta_Cen[cent9] ->Fill(nHitsFrac, rcPt, rcEta, reweight*wt_nEfrac);
			h3d_Dca_vsPtEta_Cen[cent9]       ->Fill(Dca,       rcPt, rcEta, reweight*wt_nEfrac);
		}
		//----------------------------------------------------------------------------------------------------

		//----------------------------------------------------------------------------------------------------
		if(mRoundIndex>=1)
		{
			double wt_PtEta = getPtEtaWeight(rcPt,rcEta);
			//----------------------------------------------------------------------------------------------------
			h3d_nHitsFit_vsPtEta_wted  ->Fill(nHitsFit,  rcPt, rcEta, wt_PtEta*reweight*wt_nEfrac);
			h3d_nHitsDedx_vsPtEta_wted ->Fill(nHitsDedx, rcPt, rcEta, wt_PtEta*reweight*wt_nEfrac);
			h3d_nHitsFrac_vsPtEta_wted ->Fill(nHitsFrac, rcPt, rcEta, wt_PtEta*reweight*wt_nEfrac);
			h3d_Dca_vsPtEta_wted       ->Fill(Dca,       rcPt, rcEta, wt_PtEta*reweight*wt_nEfrac);
			h3d_Phi_vsPtEta_wted       ->Fill(rcPhi,     rcPt, rcEta, wt_PtEta*reweight*wt_nEfrac);
			//----------------------------------------------------------------------------------------------------
		}
		//----------------------------------------------------------------------------------------------------

		//----------------------------------------------------------------------------------------------------
		if( nHitsFit<20 )                      continue;
		//h3d_Rc_PtEtaPhi_perCent[cent9][1] -> Fill(rcPt, rcEta, rcPhi);
		h3d_Rc_PtEtaPhi_perCent[cent9][1] -> Fill(mcPt, mcEta, mcPhi, reweight*wt_nEfrac);
		//----------------------------------------------------------------------------------------------------

		//----------------------------------------------------------------------------------------------------
		if( nHitsDedx<15 )                     continue;
		//h3d_Rc_PtEtaPhi_perCent[cent9][2] -> Fill(rcPt, rcEta, rcPhi);
		h3d_Rc_PtEtaPhi_perCent[cent9][2] -> Fill(mcPt, mcEta, mcPhi, reweight*wt_nEfrac);
		//----------------------------------------------------------------------------------------------------

		//----------------------------------------------------------------------------------------------------
		if( nHitsFrac<0.52 || nHitsFrac>1.05 ) continue;
		//h3d_Rc_PtEtaPhi_perCent[cent9][3] -> Fill(rcPt, rcEta, rcPhi);
		h3d_Rc_PtEtaPhi_perCent[cent9][3] -> Fill(mcPt, mcEta, mcPhi, reweight*wt_nEfrac);
		//----------------------------------------------------------------------------------------------------

		//----------------------------------------------------------------------------------------------------
		if( Dca>1.0 )                          continue;
		//h3d_Rc_PtEtaPhi_perCent[cent9][4] -> Fill(rcPt, rcEta, rcPhi);
		h3d_Rc_PtEtaPhi_perCent[cent9][4] -> Fill(mcPt, mcEta, mcPhi, reweight*wt_nEfrac);
		//----------------------------------------------------------------------------------------------------

		//----------------------------------------------------------------------------------------------------
		//fill histograms for the momentum smearing study, for mRoundIndex ==0
		h_PtDiff4Res[cent9] ->Fill( mcPt, (rcPt-mcPt)/mcPt, reweight*wt_nEfrac ); // raw

		if(mRoundIndex>=1)
		{
			double sigmaPt2Pt = getSigmaPt2Pt(mcPt, cent9);
			h_PtDiff4Res_aftnorm[cent9] ->Fill( mcPt, ((rcPt-mcPt)/mcPt)*(0.01/sigmaPt2Pt), reweight*wt_nEfrac ); // Normalize
		}
		//----------------------------------------------------------------------------------------------------
	}// track loop

	return kTRUE;
}
//=======================================================================================
void init( int mRoundIndex)
{
	mNBadRuns = sizeof(mBadRuns)/sizeof(int);

	if(mRoundIndex>=1)
	{
		//--------------------------------------------------------------------------------------------------------------------
		//--------------------------------------------------------------------------------------------------------------------
		//read in the input files for abf
		for(int isubCent=0; isubCent<3; isubCent++)
		{
			infile_ab[isubCent]   = new TFile("./inputfiles/forVz35cm/out_ab_DCB_parms_"+subCentNames[isubCent]+"_Vz35cm_round0.root", "read");
			cout<<"readin: "<<infile_ab[isubCent]->GetName()<<endl;

			H_ABf_Parms[isubCent] = (TH1F*)infile_ab[isubCent]->Get("H_ABf_Parms_"+subCentNames[isubCent]);
		}
		//--------------------------------------------------------------------------------------------------------------------
		//--------------------------------------------------------------------------------------------------------------------

		//--------------------------------------------------------------------------------------------------------------------
		//--------------------------------------------------------------------------------------------------------------------
		//read the input weights for embd to data
		infile_PtEtaWT = new TFile("./inputfiles/outhist_Data2Embd_Ratio_SingleE_forQa_Vz70cm.root", "read");
		cout<<"readin: "<<infile_PtEtaWT->GetName()<<endl;

		H_Pt_WT  = (TH1F*)infile_PtEtaWT->Get("H1D_Pt_WT");
		H_Eta_WT = (TH1F*)infile_PtEtaWT->Get("H1D_Eta_WT");
		//--------------------------------------------------------------------------------------------------------------------
		//--------------------------------------------------------------------------------------------------------------------
	}
	return;
}
//=======================================================================================
void bookHistograms(char *outFile, int mRoundIndex)
{
	//--------------------------------------------------------------------------------------------------------------------
	//--------------------------------------------------------------------------------------------------------------------
	char buf[1024];
	sprintf(buf,"%s_round%d.histo.root", outFile, mRoundIndex);
	outfile = new TFile(buf,"recreate");
	//--------------------------------------------------------------------------------------------------------------------
	//--------------------------------------------------------------------------------------------------------------------
	
	outfile->cd();

	TH1::SetDefaultSumw2();
	h_EvtCounter = new TH1F("h_EvtCounter", " ", 21, -0.5, 20.5);

	hMc_Vz       = new TH1F("hMc_Vz",         "hMc_Vz;Vz^{Mc}(cm)",                 400,-200,200);
	hRc_Vz       = new TH1F("hRc_Vz",         "hRc_Vz;Vz^{Mc}(cm)",                 400,-200,200);
	hVz_McVsRc   = new TH2F("hVz_McVsRc",     "hVz_McVsRc; Vz^{Rc}; Vz^{Mc}",       400,-200,200, 400,-200,200);
	hMc_VxVy     = new TH2F("hMc_VxVy",       "hMc_VxVy;Vx^{Mc} (cm);Vy^{Mc} (cm)", 40,-2,2, 40,-2,2);
	hRc_VxVy     = new TH2F("hRc_VxVy",       "hRc_VxVy;Vx^{Mc} (cm);Vy^{Mc} (cm)", 40,-2,2, 40,-2,2);
	hRefMultCorr = new TH1F("hRefMultCorr",   "hRefMultCorr; RefMultCorr;",         500,0,500);
	hcent9       = new TH1F("hcent9",         "hcent9       ;CentIndex;nEvents;",   11, -1, 10);
	hcent16      = new TH1F("hcent16",        "hcent16      ;CentIndex;nEvents;",   21, -1, 20);

	h_nEfracVsCent9  = new TH2F("h_nEfracVsCent9",  "nMcE/NRefmult VsCent9; Cent9; nMcE/nRefmult", 11, -1, 10, 50, 0., 1.);
	h_nEfracVsCent16 = new TH2F("h_nEfracVsCent16", "nMcE/NRefmult VsCent16;Cent16;nMcE/nRefmult", 21, -1, 20, 50, 0., 1.);
	h_nEvsNRefmult   = new TH2F("h_nEvsNRefmult",   "h_nEvsNRefmult;          nRefmult;nMcE",      500, 0, 500,25,  0., 25);

	h_nEfracVsCent9_aftwt  = new TH2F("h_nEfracVsCent9_aftwt",  "nMcE/NRefmult VsCent9; Cent9; nMcE/nRefmult", 11, -1, 10, 50, 0., 1.);
	h_nEfracVsCent16_aftwt = new TH2F("h_nEfracVsCent16_aftwt", "nMcE/NRefmult VsCent16;Cent16;nMcE/nRefmult", 21, -1, 20, 50, 0., 1.);
	h_nEvsNRefmult_aftwt   = new TH2F("h_nEvsNRefmult_aftwt",   "h_nEvsNRefmult_aftwt;          nRefmult;nMcE",500, 0, 500,25, 0., 25);

	hMc_ComnHitsPtEta       = new TH3F("hMc_ComnHitsPtEta",      "hMc_ComnHitsPtEta;nComnHits;mcPt;mcEta",         50,0,50, 100,0,5, 100,-2,2);
	hRc_ComnHitsPtEta       = new TH3F("hRc_ComnHitsPtEta",      "hRc_ComnHitsPtEta;nComnHits;rcPt;rcEta",         50,0,50, 100,0,5, 100,-2,2);
	hRc_ComnHitsnHitsFitPt  = new TH3F("hRc_ComnHitsnHitsFitPt", "hRc_ComnHitsnHitsFitPt;nComnHits;nHitsFit;rcPt", 50,0,50, 50,0,50, 100, 0,5);
	//----------------------------------------------------------------------------------------------------------------------------------------------
	//----------------------------------------------------------------------------------------------------------------------------------------------
	//for compare to Real Data QA 
	h3d_nHitsFit_vsPtEta       = new TH3F("h3d_nHitsFit_vsPtEta",       "h3d_nHitsFit_vsPtEta;      nHitsFit; p_{T}(GeV/c);#eta", 50,0,50, 100,0,5,100,-1,1);
	h3d_nHitsDedx_vsPtEta      = new TH3F("h3d_nHitsDedx_vsPtEta",      "h3d_nHitsDedx_vsPtEta;     nHitsDedx;p_{T}(GeV/c);#eta", 50,0,50, 100,0,5,100,-1,1);
	h3d_nHitsFrac_vsPtEta      = new TH3F("h3d_nHitsFrac_vsPtEta",      "h3d_nHitsFrac_vsPtEta;     nHitsFrac;p_{T}(GeV/c);#eta", 100,0.4,1.1,100,0,5,100,-1,1);
	h3d_Dca_vsPtEta            = new TH3F("h3d_Dca_vsPtEta",            "h3d_Dca_vsPtEta;           Dca;      p_{T}(GeV/c);#eta", 50,0,3,  100,0,5,100,-1,1);
	h3d_Phi_vsPtEta            = new TH3F("h3d_Phi_vsPtEta",            "h3d_Phi_vsPtEta;           Phi;      p_{T}(GeV/c);#eta", 120,-PI+PI/12,PI+PI/12,100,0,5,100,-1,1);

	//QA for eff. systematic uncertainties, have to be after weight Embd to Data Eta and Pt
	if(mRoundIndex>=1)
	{
		for(int ic=0; ic<nCentBins; ic++)
		{
			h3d_nHitsFit_vsPtEta_Cen[ic] = new TH3F(Form("h3d_nHitsFit_vsPtEta_ic%d",ic), Form("h3d_nHitsFit_vsPtEta_ic%d;nHitsFit;p_{T}(GeV/c);#eta",ic), 50,0,50, 40,0,4,20,-1,1);
			h3d_nHitsDedx_vsPtEta_Cen[ic] = new TH3F(Form("h3d_nHitsDedx_vsPtEta_ic%d",ic), Form("h3d_nHitsDedx_vsPtEta_ic%d;nHitsDedx;p_{T}(GeV/c);#eta",ic), 50,0,50, 40,0,4,20,-1,1);
			h3d_nHitsFrac_vsPtEta_Cen[ic] = new TH3F(Form("h3d_nHitsFrac_vsPtEta_ic%d",ic), Form("h3d_nHitsFrac_vsPtEta_ic%d;nHitsFrac;p_{T}(GeV/c);#eta",ic), 50,0.4,1.1, 40,0,4,20,-1,1);
			h3d_Dca_vsPtEta_Cen[ic] = new TH3F(Form("h3d_Dca_vsPtEta_ic%d",ic), Form("h3d_Dca_vsPtEta_ic%d;Dca;p_{T}(GeV/c);#eta",ic), 50,0,3.0, 40,0,4,20,-1,1);
		}//ic
	}
	//----------------------------------------------------------------------------------------------------------------------------------------------
	if(mRoundIndex>=1)//after weight Embd to Data
	{
		h3d_nHitsFit_vsPtEta_wted  = new TH3F("h3d_nHitsFit_vsPtEta_wted",  "h3d_nHitsFit_vsPtEta_wted; nHitsFit; p_{T}(GeV/c);#eta", 50,0,50, 100,0,5,100,-1,1);
		h3d_nHitsDedx_vsPtEta_wted = new TH3F("h3d_nHitsDedx_vsPtEta_wted", "h3d_nHitsDedx_vsPtEta_wted;nHitsDedx;p_{T}(GeV/c);#eta", 50,0,50, 100,0,5,100,-1,1);
		h3d_nHitsFrac_vsPtEta_wted = new TH3F("h3d_nHitsFrac_vsPtEta_wted", "h3d_nHitsFrac_vsPtEta_wted;nHitsFrac;p_{T}(GeV/c);#eta", 100,0.4,1.1,100,0,5,100,-1,1);
		h3d_Dca_vsPtEta_wted       = new TH3F("h3d_Dca_vsPtEta_wted",       "h3d_Dca_vsPtEta_wted;      Dca;      p_{T}(GeV/c);#eta", 50,0,3,  100,0,5,100,-1,1);
		h3d_Phi_vsPtEta_wted       = new TH3F("h3d_Phi_vsPtEta_wted",       "h3d_Phi_vsPtEta_wted;      Phi;      p_{T}(GeV/c);#eta", 120,-PI+PI/12,PI+PI/12,100,0,5,100,-1,1);
	}
	//----------------------------------------------------------------------------------------------------------------------------------------------

	//for single e Eff. calculation
	for(int icent=0; icent<nCentBins; icent++)
	{
		h3d_Mc_PtEtaPhi_perCent[icent]           = new TH3F( Form("h3d_Mc_PtEtaPhi_Cent%d", icent),            ";p_{T}(GeV/c);#eta;#phi", 100,0,5,100,-1,1,120,-PI+PI/12,PI+PI/12);

		//const int nCuts     = 5; //0:Acc., 1:Acc.+nHitsFit, 2: Acc.+nHitsFit+nHitDedx, 3:Acc.+nHitsFit+nHitDedx+Dca
		for(int icut=0; icut<nCuts; icut++)
		{
			h3d_Rc_PtEtaPhi_perCent[icent][icut] = new TH3F(Form("h3d_Rc_PtEtaPhi_Cent%d_icut%d", icent, icut), ";p_{T}(GeV/c);#eta;#phi", 100,0,5,100,-1,1,120,-PI+PI/12,PI+PI/12);
		}//icut

		h_PtDiff4Res[icent]         = new TH2F( Form("h_PtDiff4Res_icent%d",        icent),"(rcPt-mcPt)/mcPt vs mcPt; p_{T}^{mc} GeV/c; (rcPt-mcPt)/mcPt", 100,0,5, 500,-0.2,0.2);
		if(mRoundIndex>=1)
		{
			h_PtDiff4Res_aftnorm[icent] = new TH2F( Form("h_PtDiff4Res_aftnorm_icent%d",icent),"(rcPt-mcPt)/mcPt vs mcPt; p_{T}^{mc} GeV/c; (rcPt-mcPt)/mcPt", 100,0,5, 500,-0.2,0.2);
		}
	}//icent
}
//=======================================================================================
void writeHistograms()
{
	cout<<"outputfile: "<<outfile->GetName()<<endl;
	outfile->Write();
	outfile->Close();
}
//=======================================================================================
//-------------------------------------------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------------------------------
double getSigmaPt2Pt(double mcPt, int mcent9)
{
	int temSubCent = -1;

	if(      mcent9>=0&&mcent9<=3 ) temSubCent = 2 ;
	else if( mcent9>=4&&mcent9<=6 ) temSubCent = 1 ;
	else if( mcent9>=7&&mcent9<=8 ) temSubCent = 0 ;
	else                            temSubCent = -1;

	double a = H_ABf_Parms[temSubCent]->GetBinContent(1);
	double b = H_ABf_Parms[temSubCent]->GetBinContent(2);

	double sigmapt2pt = sqrt(a*a*mcPt*mcPt + b*b);

	return sigmapt2pt;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------------------------------
double getPtEtaWeight(double rcpt, double rceta)
{
	double ptweight  = 0.0;
	double etaweight = 0.0;
	ptweight  = H_Pt_WT ->GetBinContent( H_Pt_WT->FindBin(rcpt)   );
	etaweight = H_Eta_WT->GetBinContent( H_Eta_WT->FindBin(rceta) );

	double ptetaWeight = ptweight*etaweight;

	if( ptetaWeight>0.0 ) return ptetaWeight;
	else                  return 0.0;
}
