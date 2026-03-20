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
using namespace std;
using std::cout;
using std::endl;
using std::setw;
#endif

void  init(int mRoundIndex);
bool  passEvent(mcEvent* mEvent, int mRoundIndex );
bool  passTrack(mcEvent* mcEvt,  int mRoundIndex);
void  bookHistograms(int mRoundIndex);
void  writeHistograms(char *outFile, int mRoundIndex);
int findPartner( Int_t mcid, Short_t mcCharge, mcEvent *event);

int cent9_refmult6  	 = -1;
int cent16_refmult6		 = -1;
double reweight_refmult6 = 1.;
double wt_nEfrac  		 = 1.;
TFile* currentFile;
TString currentFileName;
//---------------------------------------------------------------------
//---------------------------------------------------------------------
int main(int argc, char** argv)
{
	if(argc!=1&&argc!=3) return -1;

	char *inFile="../ReadEmbedding/list/test.list";
	char outFile[1024];
	sprintf(outFile,"test");
		
	mRoundIndex = 0;

	if(argc==1)
	{
		cout << "Running in the test mode, using default input file: " << inFile << endl;
		cout << "Output will be saved to directory: " << outFile << "/" << endl;
	}
	else if(argc==3)
	{
		inFile = argv[1];
		sprintf(outFile,"%s",argv[2]);
		// mRoundIndex = atoi(argv[3]);
	}
	else
	{
		// cout << "wrong input, please run like ./readEmbdMini test.list testout roundindex" << endl;
		cout << "wrong input, please run like ./readEmbdMini test.list testout" << endl;
		return 0;
	}

	//+---------------------------------+
	//| open files and add to the chain |
	//+---------------------------------+
	TChain *chain = new TChain("mcT");
	Int_t ifile=0;
	char filename[1024];
	ifstream *inputStream = new ifstream;
	inputStream->open(inFile);

	if (!(inputStream)) 
	{
		printf("can not open list file\n");
		return 0;
	}

	cout << "Begin reading files....." << endl;
	for(;inputStream->good();)
	{
		inputStream->getline(filename,1024);
		if(inputStream->good()) 
		{
			filename[strcspn(filename, "\r\n")] = '\0';
			TFile *ftmp = TFile::Open(filename);
			if(!ftmp||!(ftmp->IsOpen())||!(ftmp->GetNkeys())) 
			{
				cout << "Something wrong with file " << ifile+1 << endl;
			} 
			else 
			{
				// cout << "read in " << ifile+1 << "th file: " << filename << endl;
				chain->Add(filename);
				ifile++;
			}
			delete ftmp;
		}
	}
	delete inputStream;
	cout << ifile << " files have been read!" << endl;

	//----------------------------------------------------------------
	//----------------------------------------------------------------
	init( mRoundIndex );
	bookHistograms(mRoundIndex);
	//----------------------------------------------------------------
	//----------------------------------------------------------------

	mcEvent *mcEvt = new mcEvent(chain);
	Int_t nEvents = chain->GetEntries();

	cout << "nEvents = " << nEvents << endl;

	//+-------------+
	//| loop mEvents |
	//+-------------+
	for(int i=0; i<nEvents; i++)
	{
		mcEvt->GetEntry(i);

		currentFile = chain->GetFile();
		currentFileName = currentFile->GetName();

		bool isEventPassed = passEvent(mcEvt, mRoundIndex);
		if(isEventPassed) passTrack(mcEvt, mRoundIndex);

		if( i%10000==0 )
		{
			cout << i << " entries done!" << endl;
		}

	} //end of mEvent loop

	writeHistograms(outFile, mRoundIndex); 
	delete chain;
	cout << "End of program" << endl;
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
	if (mEvent->muEvtId<0)    return kFALSE;
	//----------------------------------------------------------------------------------------------------

	double muVx = mEvent->muPriVertexX;
	double muVy = mEvent->muPriVertexY;
	double muVz = mEvent->muPriVertexZ;
	double muVr = sqrt( muVx*muVx + muVy*muVy );

	cent9_refmult6    = mEvent->Centrality_9_refmult6;
	cent16_refmult6   = mEvent->Centrality_16_refmult6;
	reweight_refmult6 = mEvent->reweight_refmult6;

	//----------------------------------------------------------------------------------------------------
	if( fabs(muVx)<1.e-5 && fabs(muVy)<1.e-5 && fabs(muVz)<1.e-5 ) 	return kFALSE;
	if( fabs(muVz)>30. ) 											return kFALSE;
	if( muVr>2.0 )       											return kFALSE;
	if( cent9_refmult6<0 || cent9_refmult6>9 ) 						return kFALSE;
	//----------------------------------------------------------------------------------------------------

	double nMcEoverNRefmult = 1.*mEvent->nMcE/(1.*mEvent->muRefMult);
	wt_nEfrac = (0.05/nMcEoverNRefmult);

	return kTRUE;
}
//----------------------------------------------------------------------------------------------------
bool passTrack(mcEvent* mEvent, int mRoundIndex)
{
	// int nTracks = mEvent->nMcTrks;
	int nTracks = mEvent->nMcE;
	if(nTracks<=0) return kFALSE;

	//loop over tracks: electrons
	for(int itrk=0; itrk<nTracks; itrk++)
	{
		int geantId = mEvent->geantId[itrk];
		if( geantId !=2 && geantId !=3 ) continue;//let electron-3 or positron-2 pass here
		int mcTrack_par_Geantid = mEvent->mcTrack_par_Geantid[itrk];
		if( mcTrack_par_Geantid != -999)
		{
			cout << "The parent GeantId is " << mcTrack_par_Geantid << " for track " << itrk << " in event " << mEvent->muEvtId << endl;
		}

		Short_t  mcCharge  = mEvent->mcCharge[itrk];
		Double_t mcPt      = mEvent->mcPt[itrk];
		Double_t mcEta     = mEvent->mcEta[itrk];
		Double_t mcPhi     = mEvent->mcPhi[itrk];
		// if( mcPhi < -PI+PI/12 ) mcPhi += 2*PI;cd pr	

		if( (geantId==2&&mcCharge<0) || (geantId==3&&mcCharge>0) )
		{
			cout << "The charge doesn't match geantId!" << endl;
			cout << "GeantId = " << geantId << endl;
			cout << "mcCharge = " << mcCharge << endl;
			continue;
		}
		
		// if( fabs(mcEta)>1.0) continue; //apply the acceptance cut |mcEta|<1.0
		if( fabs(mcEta)<=1.0)
		{
			if(  mcCharge>0 )
			{
				h3d_Mc_PtEtaPhi_perCent_eplus[cent9_refmult6] -> Fill(mcPt, mcEta, mcPhi, reweight_refmult6*wt_nEfrac);
			}
			else{
				h3d_Mc_PtEtaPhi_perCent_eminus[cent9_refmult6] -> Fill(mcPt, mcEta, mcPhi, reweight_refmult6*wt_nEfrac);
			}
		}

		int RCtrkID = findPartner(mEvent->mcTrackId[itrk], mcCharge, mEvent);

		if(RCtrkID==-999) continue;
		if(mEvent->rcQaTruth[RCtrkID]<50) continue; //require at least 50% of hits to come from the above MC track
		if(mEvent->rcflag[RCtrkID]<100) continue; //skip bad tracks
		if(mEvent->rcflag[RCtrkID]>1000) continue; //skip bad tracks
		if(mEvent->rcflag[RCtrkID]%100==11) continue; //skip bad tracks
		if(mEvent->rcflag[RCtrkID]%100==12) continue; //skip bad tracks

		Int_t    nHitsFit  = mEvent->rcNHitsFit[RCtrkID];
		Int_t    nHitsDedx = mEvent->rcNHitsDedx[RCtrkID];
		Int_t    nHitsMax  = mEvent->rcNHitsPoss[RCtrkID];
		Double_t nHitsFrac = 1.*nHitsFit/nHitsMax;
		Double_t Dca       = mEvent->rcDca[RCtrkID];
		Double_t rcPt      = mEvent->rcPt[RCtrkID];
		Double_t rcEta     = mEvent->rcEta[RCtrkID];
		Double_t rcPhi     = mEvent->rcPhi[RCtrkID];
		// if( rcPhi < -PI+PI/12 ) rcPhi += 2*PI;	

		if( fabs(rcEta)>1.0 || rcPt<=0.0) continue;
		if( mcCharge>0 )
		{
			h3d_Rc_PtEtaPhi_perCent_McValues_eplus[cent9_refmult6][0] -> Fill(mcPt, mcEta, mcPhi, reweight_refmult6*wt_nEfrac);
			h3d_Rc_PtEtaPhi_perCent_RcValues_eplus[cent9_refmult6][0] -> Fill(rcPt, rcEta, rcPhi, reweight_refmult6*wt_nEfrac);
		}
		else{
			h3d_Rc_PtEtaPhi_perCent_McValues_eminus[cent9_refmult6][0] -> Fill(mcPt, mcEta, mcPhi, reweight_refmult6*wt_nEfrac);
			h3d_Rc_PtEtaPhi_perCent_RcValues_eminus[cent9_refmult6][0] -> Fill(rcPt, rcEta, rcPhi, reweight_refmult6*wt_nEfrac);
		}

		if( nHitsFit<40 ) continue;
		if( mcCharge>0 )
		{
			h3d_Rc_PtEtaPhi_perCent_McValues_eplus[cent9_refmult6][1] -> Fill(mcPt, mcEta, mcPhi, reweight_refmult6*wt_nEfrac);
			h3d_Rc_PtEtaPhi_perCent_RcValues_eplus[cent9_refmult6][1] -> Fill(rcPt, rcEta, rcPhi, reweight_refmult6*wt_nEfrac);
		}
		else{
			h3d_Rc_PtEtaPhi_perCent_McValues_eminus[cent9_refmult6][1] -> Fill(mcPt, mcEta, mcPhi, reweight_refmult6*wt_nEfrac);
			h3d_Rc_PtEtaPhi_perCent_RcValues_eminus[cent9_refmult6][1] -> Fill(rcPt, rcEta, rcPhi, reweight_refmult6*wt_nEfrac);
		}

		if( nHitsDedx<30 ) continue;
		if( mcCharge>0 )
		{
			h3d_Rc_PtEtaPhi_perCent_McValues_eplus[cent9_refmult6][2] -> Fill(mcPt, mcEta, mcPhi, reweight_refmult6*wt_nEfrac);
			h3d_Rc_PtEtaPhi_perCent_RcValues_eplus[cent9_refmult6][2] -> Fill(rcPt, rcEta, rcPhi, reweight_refmult6*wt_nEfrac);
		}
		else{
			h3d_Rc_PtEtaPhi_perCent_McValues_eminus[cent9_refmult6][2] -> Fill(mcPt, mcEta, mcPhi, reweight_refmult6*wt_nEfrac);
			h3d_Rc_PtEtaPhi_perCent_RcValues_eminus[cent9_refmult6][2] -> Fill(rcPt, rcEta, rcPhi, reweight_refmult6*wt_nEfrac);
		}

		if( nHitsFrac<0.52 ) continue;
		if( mcCharge>0 )
		{
			h3d_Rc_PtEtaPhi_perCent_McValues_eplus[cent9_refmult6][3] -> Fill(mcPt, mcEta, mcPhi, reweight_refmult6*wt_nEfrac);
			h3d_Rc_PtEtaPhi_perCent_RcValues_eplus[cent9_refmult6][3] -> Fill(rcPt, rcEta, rcPhi, reweight_refmult6*wt_nEfrac);
		}
		else{
			h3d_Rc_PtEtaPhi_perCent_McValues_eminus[cent9_refmult6][3] -> Fill(mcPt, mcEta, mcPhi, reweight_refmult6*wt_nEfrac);
			h3d_Rc_PtEtaPhi_perCent_RcValues_eminus[cent9_refmult6][3] -> Fill(rcPt, rcEta, rcPhi, reweight_refmult6*wt_nEfrac);
		}

		if( Dca>1.0 ) continue;
		if( mcCharge>0 )
		{
			h3d_Rc_PtEtaPhi_perCent_McValues_eplus[cent9_refmult6][4] -> Fill(mcPt, mcEta, mcPhi, reweight_refmult6*wt_nEfrac);
			h3d_Rc_PtEtaPhi_perCent_RcValues_eplus[cent9_refmult6][4] -> Fill(rcPt, rcEta, rcPhi, reweight_refmult6*wt_nEfrac);
		}
		else{
			h3d_Rc_PtEtaPhi_perCent_McValues_eminus[cent9_refmult6][4] -> Fill(mcPt, mcEta, mcPhi, reweight_refmult6*wt_nEfrac);
			h3d_Rc_PtEtaPhi_perCent_RcValues_eminus[cent9_refmult6][4] -> Fill(rcPt, rcEta, rcPhi, reweight_refmult6*wt_nEfrac);
		}

		//fill histograms for the momentum smearing study, for mRoundIndex ==0
		h_PtDiff4Res[cent9_refmult6] ->Fill( mcPt, (rcPt-mcPt)/mcPt, reweight_refmult6*wt_nEfrac );

	}// track loop

	return kTRUE;
}
//=======================================================================================
void init(int mRoundIndex)
{
	mNBadRuns = sizeof(mBadRuns)/sizeof(int);
	return;
}
//=======================================================================================
void bookHistograms(int mRoundIndex)
{
	TH1::SetDefaultSumw2();

	for(int icent=0; icent<nCentBins; icent++)
	{
		h_PtDiff4Res[icent] = new TH2F( Form("h_PtDiff4Res_icent%d", icent),"(rcPt-mcPt)/mcPt vs mcPt; p_{T}^{mc} GeV/c; (rcPt-mcPt)/mcPt", 100,0,5, 400,-0.2,0.2);

		h3d_Mc_PtEtaPhi_perCent_eplus[icent] = new TH3F( Form("h3d_Mc_PtEtaPhi_Cent%d_eplus", icent), ";p_{T}(GeV/c);#eta;#phi", 100,0,5,110,-1.1,1.1,120,-PI,PI);	
		h3d_Mc_PtEtaPhi_perCent_eminus[icent] = new TH3F( Form("h3d_Mc_PtEtaPhi_Cent%d_eminus", icent), ";p_{T}(GeV/c);#eta;#phi", 100,0,5,110,-1.1,1.1,120,-PI,PI);

		for(int icut=0; icut<nCuts; icut++)
		{
			h3d_Rc_PtEtaPhi_perCent_McValues_eplus[icent][icut] = new TH3F(Form("h3d_Rc_PtEtaPhi_Cent%d_icut%d_McValues_eplus", icent, icut), ";p_{T}(GeV/c);#eta;#phi", 100,0,5,110,-1.1,1.1,120,-PI,PI);
			h3d_Rc_PtEtaPhi_perCent_RcValues_eplus[icent][icut] = new TH3F(Form("h3d_Rc_PtEtaPhi_Cent%d_icut%d_RcValues_eplus", icent, icut), ";p_{T}(GeV/c);#eta;#phi", 100,0,5,110,-1.1,1.1,120,-PI,PI);

			h3d_Rc_PtEtaPhi_perCent_McValues_eminus[icent][icut] = new TH3F(Form("h3d_Rc_PtEtaPhi_Cent%d_icut%d_McValues_eminus", icent, icut), ";p_{T}(GeV/c);#eta;#phi", 100,0,5,110,-1.1,1.1,120,-PI,PI);
			h3d_Rc_PtEtaPhi_perCent_RcValues_eminus[icent][icut] = new TH3F(Form("h3d_Rc_PtEtaPhi_Cent%d_icut%d_RcValues_eminus", icent, icut), ";p_{T}(GeV/c);#eta;#phi", 100,0,5,110,-1.1,1.1,120,-PI,PI);
		}
	}
}
//=======================================================================================
void writeHistograms(char *outFile, int mRoundIndex)
{	
	outfile = new TFile(Form("production/%s_round%d.histo.root", outFile, mRoundIndex),"recreate");	
	cout << "Outputfile: " << outfile->GetName() << endl;
	outfile->cd();
	for(int icent=0; icent<nCentBins; icent++)
	{
		h_PtDiff4Res[icent] -> Write();

		h3d_Mc_PtEtaPhi_perCent_eplus[icent] -> Write();
		h3d_Mc_PtEtaPhi_perCent_eminus[icent] -> Write();

		for(int icut=0; icut<nCuts; icut++)
		{
			h3d_Rc_PtEtaPhi_perCent_McValues_eplus[icent][icut] -> Write();
			h3d_Rc_PtEtaPhi_perCent_RcValues_eplus[icent][icut] -> Write();

			h3d_Rc_PtEtaPhi_perCent_McValues_eminus[icent][icut] -> Write();
			h3d_Rc_PtEtaPhi_perCent_RcValues_eminus[icent][icut] -> Write();
		}
	}
	outfile->Close();
}
//=======================================================================================
int findPartner( Int_t mcid, Short_t mcCharge, mcEvent *event)
{
    Int_t nrcTracks = event->nRcE;

    float maxTruth = -1;
    int iTrack = -1;
    for(int i=0;i<nrcTracks;i++)
	{
        if(event->rcidtruth[i] == mcid && event->rcCharge[i]==mcCharge)
		{
            if(event->rcQaTruth[i]>maxTruth)  //quality of this information (percentage of hits coming the above MC track)
            {
                iTrack = i;
                maxTruth = event->rcQaTruth[i];
            }
        }
    }

    if(iTrack != -1)
	{
		Double_t eta1 = event->rcEta[iTrack];
		if(!(fabs(eta1)<1.0)) return -999;
		return iTrack;
    }
    return -999;
}
