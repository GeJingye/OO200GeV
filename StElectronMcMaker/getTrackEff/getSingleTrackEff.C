#include "iostream"
#include "TString.h"
#include "TPaveStats.h"
#include "TFile.h"
#include "TH1.h"
#include "TH2.h"
#include "TH3.h"
#include "TStyle.h"
#include "TCanvas.h"
#include "TPaveText.h"
#include "TLegend.h"
#include "TColor.h"
#include "TROOT.h"
#include "TError.h"

using namespace std;

TString outDir = "plots";
TFile* infile = 0;
TFile* outfile = 0;

const int nCentBins = 4;
const int nCuts     = 5; //0:Acc., 1:Acc.+nHitsFit, 2: Acc.+nHitsFit+nHitDedx, 3:Acc.+nHitsFit+nHitDedx+Dca
const int nSubCentBins = 9;

const int CentLow[nCentBins] = {0, 40, 10, 0}; // 9 Centrality Bins
const int CentHig[nCentBins] = {80, 80, 40, 10}; // 9 Centrality Bins

const int mCentBinLow[nCentBins] = {0, 0, 4, 7}; // 9 Centrality Bins
const int mCentBinHig[nCentBins] = {9, 4, 7, 9}; // 9 Centrality Bins

// const int nCentBins = 1;
// const int nCuts     = 5; //0:Acc., 1:Acc.+nHitsFit, 2: Acc.+nHitsFit+nHitDedx, 3:Acc.+nHitsFit+nHitDedx+Dca
// const int nSubCentBins = 9;

// const int CentLow[nCentBins] = {0}; // 9 Centrality Bins
// const int CentHig[nCentBins] = {80}; // 9 Centrality Bins

// const int mCentBinLow[nCentBins] = {0}; // 9 Centrality Bins
// const int mCentBinHig[nCentBins] = {9}; // 9 Centrality Bins

const char* CutName[nCuts] = {"|RcEta|<1", "...+nHitsFit#geq40", "...+nHitsDedx#geq30", "...+nHitsFitRatio#geq0.52", "...+Dca#leq1.0"};
vector<int> MarkerColor_Cens(nCentBins);
vector<int> MarkerColor_Cuts(nCuts);

const char* ValueType[2] = {"McValues", "RcValues"};
int i_type = 0; // 0: McValues, 1: RcValues

TH2F* H2D_PtDiff4Res_tem[nSubCentBins];
TH2F* H2D_PtDiff4Res[nCentBins];

TH3F* H3D_Mc_PtEtaPhi_EPlus_Cent_tem[nSubCentBins];	
TH3F* H3D_Rc_PtEtaPhi_EPlus_Cent_tem[nSubCentBins][nCuts];
TH3F* H3D_Mc_PtEtaPhi_EPlus_Cent[nCentBins];	
TH3F* H3D_Rc_PtEtaPhi_EPlus_Cent[nCentBins][nCuts];

TH3F* H3D_Mc_PtEtaPhi_EMinus_Cent_tem[nSubCentBins];	
TH3F* H3D_Rc_PtEtaPhi_EMinus_Cent_tem[nSubCentBins][nCuts];
TH3F* H3D_Mc_PtEtaPhi_EMinus_Cent[nCentBins];	
TH3F* H3D_Rc_PtEtaPhi_EMinus_Cent[nCentBins][nCuts];

TH3F* H3D_Mc_PtEtaPhi_Cent[nCentBins];	
TH3F* H3D_Rc_PtEtaPhi_Cent[nCentBins][nCuts];

TH1F* H1D_McPt_AllEtaPhi_EPlus_Cent[nCentBins];
TH1F* H1D_McEta_AllPhiPt_EPlus_Cent[nCentBins];
TH1F* H1D_McPhi_AllPtEta_EPlus_Cent[nCentBins];
TH1F* H1D_RcPt_AllEtaPhi_EPlus_Cent[nCentBins][nCuts];
TH1F* H1D_RcEta_AllPhiPt_EPlus_Cent[nCentBins][nCuts];
TH1F* H1D_RcPhi_AllPtEta_EPlus_Cent[nCentBins][nCuts];

TH1F* H1D_McPt_AllEtaPhi_EMinus_Cent[nCentBins];
TH1F* H1D_McEta_AllPhiPt_EMinus_Cent[nCentBins];
TH1F* H1D_McPhi_AllPtEta_EMinus_Cent[nCentBins];
TH1F* H1D_RcPt_AllEtaPhi_EMinus_Cent[nCentBins][nCuts];
TH1F* H1D_RcEta_AllPhiPt_EMinus_Cent[nCentBins][nCuts];
TH1F* H1D_RcPhi_AllPtEta_EMinus_Cent[nCentBins][nCuts];

TH1F* H1D_McPt_AllEtaPhi_Cent[nCentBins];
TH1F* H1D_McEta_AllPhiPt_Cent[nCentBins];
TH1F* H1D_McPhi_AllPtEta_Cent[nCentBins];
TH1F* H1D_RcPt_AllEtaPhi_Cent[nCentBins][nCuts];
TH1F* H1D_RcEta_AllPhiPt_Cent[nCentBins][nCuts];
TH1F* H1D_RcPhi_AllPtEta_Cent[nCentBins][nCuts];

TH1F* H1D_EffVsPt_AllEtaPhi_EPlus_Cent[nCentBins][nCuts];
TH1F* H1D_EffVsEta_AllPhiPt_EPlus_Cent[nCentBins][nCuts];
TH1F* H1D_EffVsPhi_AllPtEta_EPlus_Cent[nCentBins][nCuts];

TH1F* H1D_EffVsPt_AllEtaPhi_EMinus_Cent[nCentBins][nCuts];
TH1F* H1D_EffVsEta_AllPhiPt_EMinus_Cent[nCentBins][nCuts];
TH1F* H1D_EffVsPhi_AllPtEta_EMinus_Cent[nCentBins][nCuts];

TH1F* H1D_EffVsPt_AllEtaPhi_Cent[nCentBins][nCuts];
TH1F* H1D_EffVsEta_AllPhiPt_Cent[nCentBins][nCuts];
TH1F* H1D_EffVsPhi_AllPtEta_Cent[nCentBins][nCuts];

TH3F* H3D_Eff_EPlus[nCentBins];
TH3F* H3D_Eff_EMinus[nCentBins];

int getEff(int typeID);
void readFiles();
void drawEff();
void saveFiles();
TPaveText* CreateTPaveText(Double_t x1, Double_t y1, Double_t x2, Double_t y2, const char* option);
TLegend* CreateLegend(Double_t x1, Double_t y1, Double_t x2, Double_t y2);
void InitColor(std::vector<int>& color, int style);
TH3F* Cal3DEff(TH3F* AllTrks, TH3F* PassCutTrks);

//----------------------
int getSingleTrackEff()
{
	gStyle->SetOptStat(0000);
    gErrorIgnoreLevel = kWarning; // Ignore warnings and errors

	getEff(0); //McValues
	// getEff(1); //RcValues

	return 0;
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------------------------------
int getEff(int typeID)
{
	if(typeID<0 || typeID>1)
	{
		cout << "Error: typeID should be 0 or 1, but got " << typeID << endl;
		return -1;
	}
	i_type = typeID;

	cout << "----------------------------------------------" << endl;
	cout << "We calculate efficiency using the " << ValueType[i_type] << " now ..." << endl;
	cout << "----------------------------------------------" << endl;

	readFiles();	
	system(Form("mkdir -p %s", outDir.Data()));
	drawEff();
	saveFiles();

	cout << "----------------------------------------------" << endl;
	cout << "Finished all efficiency calculation using the " << ValueType[i_type] << "!" << endl;
	cout << "----------------------------------------------" << endl;
	
	infile = 0;
	outfile = 0;

	return 0;
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------------------------------
void readFiles()
{
	infile = TFile::Open("production/TrkEff.root");
	
	cout << "read in file: " << infile->GetName() << endl;

	for(Int_t icent=0; icent<nCentBins; icent++)
	{
		cout << "----------------------------------------------" << endl;
		cout << "read and merge histos in centrality: " << CentLow[icent] << "-" << CentHig[icent] << "%" << endl;

		for(Int_t isubcent=mCentBinLow[icent]; isubcent<mCentBinHig[icent]; isubcent++)
		{
			cout << "Cent9: " << isubcent << endl;

			H2D_PtDiff4Res_tem[isubcent] = (TH2F*) infile->Get( Form("h_PtDiff4Res_icent%d", isubcent) );

			H3D_Mc_PtEtaPhi_EPlus_Cent_tem[isubcent] = (TH3F*) infile->Get( Form("h3d_Mc_PtEtaPhi_Cent%d_eplus", isubcent) );
			H3D_Mc_PtEtaPhi_EMinus_Cent_tem[isubcent] = (TH3F*) infile->Get( Form("h3d_Mc_PtEtaPhi_Cent%d_eminus", isubcent) );
			
			for(int icut=0; icut<nCuts; icut++)
			{
				H3D_Rc_PtEtaPhi_EPlus_Cent_tem[isubcent][icut] = (TH3F*) infile->Get( Form("h3d_Rc_PtEtaPhi_Cent%d_icut%d_%s_eplus", isubcent, icut, ValueType[i_type]) );
				H3D_Rc_PtEtaPhi_EMinus_Cent_tem[isubcent][icut] = (TH3F*) infile->Get( Form("h3d_Rc_PtEtaPhi_Cent%d_icut%d_%s_eminus", isubcent, icut, ValueType[i_type]) );
			}

			if(isubcent==mCentBinLow[icent]) //clone the first one
			{
				H2D_PtDiff4Res[icent] = (TH2F *)H2D_PtDiff4Res_tem[isubcent]->Clone( Form("H2D_PtDiff4Res_Cent%d_%d", CentLow[icent],CentHig[icent]) );

				H3D_Mc_PtEtaPhi_EPlus_Cent[icent] = (TH3F *)H3D_Mc_PtEtaPhi_EPlus_Cent_tem[isubcent]->Clone(Form("H3D_Mc_PtEtaPhi_Cent%d_%d_eplus", CentLow[icent],CentHig[icent]) );
				H3D_Mc_PtEtaPhi_EMinus_Cent[icent] = (TH3F *)H3D_Mc_PtEtaPhi_EMinus_Cent_tem[isubcent]->Clone(Form("H3D_Mc_PtEtaPhi_Cent%d_%d_eminus", CentLow[icent],CentHig[icent]) );

				for(int icut=0; icut<nCuts; icut++)
				{
					H3D_Rc_PtEtaPhi_EPlus_Cent[icent][icut] = (TH3F*) H3D_Rc_PtEtaPhi_EPlus_Cent_tem[isubcent][icut]->Clone( Form("H3D_Rc_PtEtaPhi_Cent%d_%d_icut%d_eplus", CentLow[icent],CentHig[icent],icut) );
					H3D_Rc_PtEtaPhi_EMinus_Cent[icent][icut] = (TH3F*) H3D_Rc_PtEtaPhi_EMinus_Cent_tem[isubcent][icut]->Clone( Form("H3D_Rc_PtEtaPhi_Cent%d_%d_icut%d_eminus", CentLow[icent],CentHig[icent],icut) );
				}
			}
			else                            //add the others
			{
				H2D_PtDiff4Res[icent]->Add(H2D_PtDiff4Res_tem[isubcent]);

				H3D_Mc_PtEtaPhi_EPlus_Cent[icent]->Add(H3D_Mc_PtEtaPhi_EPlus_Cent_tem[isubcent], 1.);	
				H3D_Mc_PtEtaPhi_EMinus_Cent[icent]->Add(H3D_Mc_PtEtaPhi_EMinus_Cent_tem[isubcent], 1.);		

				for(int icut=0; icut<nCuts; icut++)
				{
					H3D_Rc_PtEtaPhi_EPlus_Cent[icent][icut]->Add(H3D_Rc_PtEtaPhi_EPlus_Cent_tem[isubcent][icut], 1.);
					H3D_Rc_PtEtaPhi_EMinus_Cent[icent][icut]->Add(H3D_Rc_PtEtaPhi_EMinus_Cent_tem[isubcent][icut], 1.);
				}
			}
			
			H2D_PtDiff4Res_tem[isubcent]->SetDirectory(0);
			delete H2D_PtDiff4Res_tem[isubcent];

			H3D_Mc_PtEtaPhi_EPlus_Cent_tem[isubcent]->SetDirectory(0);
			delete H3D_Mc_PtEtaPhi_EPlus_Cent_tem[isubcent];
			H3D_Mc_PtEtaPhi_EMinus_Cent_tem[isubcent]->SetDirectory(0);
			delete H3D_Mc_PtEtaPhi_EMinus_Cent_tem[isubcent];

			for(int icut=0; icut<nCuts; icut++)
			{
				H3D_Rc_PtEtaPhi_EPlus_Cent_tem[isubcent][icut]->SetDirectory(0);
				delete H3D_Rc_PtEtaPhi_EPlus_Cent_tem[isubcent][icut];
				H3D_Rc_PtEtaPhi_EMinus_Cent_tem[isubcent][icut]->SetDirectory(0);
				delete H3D_Rc_PtEtaPhi_EMinus_Cent_tem[isubcent][icut];
			}
		}//isubcent

		//merge e+ and e-
		H3D_Mc_PtEtaPhi_Cent[icent] = (TH3F *)H3D_Mc_PtEtaPhi_EPlus_Cent[icent]->Clone(Form("H3D_Mc_PtEtaPhi_Cent%d_%d", CentLow[icent],CentHig[icent]) );
		H3D_Mc_PtEtaPhi_Cent[icent]->Add(H3D_Mc_PtEtaPhi_EMinus_Cent[icent], 1.);

		for(int icut=0; icut<nCuts; icut++)
		{
			H3D_Rc_PtEtaPhi_Cent[icent][icut] = (TH3F *)H3D_Rc_PtEtaPhi_EPlus_Cent[icent][icut]->Clone( Form("H3D_Rc_PtEtaPhi_Cent%d_%d_icut%d", CentLow[icent],CentHig[icent],icut) );
			H3D_Rc_PtEtaPhi_Cent[icent][icut]->Add(H3D_Rc_PtEtaPhi_EMinus_Cent[icent][icut], 1.);
		}

	}//icent for analysis
	
	//calculate the efficiency
	for(Int_t icent=0; icent<nCentBins; icent++)
	{	
		// e+	
		H1D_McPt_AllEtaPhi_EPlus_Cent[icent] = (TH1F*)H3D_Mc_PtEtaPhi_EPlus_Cent[icent]->ProjectionX( Form("H1D_McPt_AllEtaPhi_icent%d_eplus", icent));
		H1D_McEta_AllPhiPt_EPlus_Cent[icent] = (TH1F*)H3D_Mc_PtEtaPhi_EPlus_Cent[icent]->ProjectionY( Form("H1D_McEta_AllPhiPt_icent%d_eplus", icent));
		H1D_McPhi_AllPtEta_EPlus_Cent[icent] = (TH1F*)H3D_Mc_PtEtaPhi_EPlus_Cent[icent]->ProjectionZ( Form("H1D_McPhi_AllPtEta_icent%d_eplus", icent));
		
		// e-
		H1D_McPt_AllEtaPhi_EMinus_Cent[icent] = (TH1F*)H3D_Mc_PtEtaPhi_EMinus_Cent[icent]->ProjectionX( Form("H1D_McPt_AllEtaPhi_icent%d_eminus", icent));
		H1D_McEta_AllPhiPt_EMinus_Cent[icent] = (TH1F*)H3D_Mc_PtEtaPhi_EMinus_Cent[icent]->ProjectionY( Form("H1D_McEta_AllPhiPt_icent%d_eminus", icent));
		H1D_McPhi_AllPtEta_EMinus_Cent[icent] = (TH1F*)H3D_Mc_PtEtaPhi_EMinus_Cent[icent]->ProjectionZ( Form("H1D_McPhi_AllPtEta_icent%d_eminus", icent));

		// e+ & e-
		H1D_McPt_AllEtaPhi_Cent[icent] = (TH1F*)H3D_Mc_PtEtaPhi_Cent[icent]->ProjectionX( Form("H1D_McPt_AllEtaPhi_icent%d", icent));
		H1D_McEta_AllPhiPt_Cent[icent] = (TH1F*)H3D_Mc_PtEtaPhi_Cent[icent]->ProjectionY( Form("H1D_McEta_AllPhiPt_icent%d", icent));
		H1D_McPhi_AllPtEta_Cent[icent] = (TH1F*)H3D_Mc_PtEtaPhi_Cent[icent]->ProjectionZ( Form("H1D_McPhi_AllPtEta_icent%d", icent));

		//calculate 1D efficiency
		for(int icut=0; icut<nCuts; icut++)
		{
			// e+
			H1D_RcPt_AllEtaPhi_EPlus_Cent[icent][icut]    = (TH1F*)H3D_Rc_PtEtaPhi_EPlus_Cent[icent][icut]->ProjectionX( Form("H1D_RcPt_AllEtaPhi_icent%d_icut%d_eplus", icent, icut) );
			H1D_EffVsPt_AllEtaPhi_EPlus_Cent[icent][icut] = (TH1F*)H1D_RcPt_AllEtaPhi_EPlus_Cent[icent][icut]->Clone( Form("H1D_EffVsPt_AllEtaPhi_icent%d_icut%d_eplus", icent, icut) );
			H1D_EffVsPt_AllEtaPhi_EPlus_Cent[icent][icut] -> Reset();
			H1D_EffVsPt_AllEtaPhi_EPlus_Cent[icent][icut] -> Divide( H1D_RcPt_AllEtaPhi_EPlus_Cent[icent][icut], H1D_McPt_AllEtaPhi_EPlus_Cent[icent], 1., 1., "B" );
		
			H1D_RcEta_AllPhiPt_EPlus_Cent[icent][icut]    = (TH1F*)H3D_Rc_PtEtaPhi_EPlus_Cent[icent][icut]->ProjectionY( Form("H1D_RcEta_AllPhiPt_icent%d_icut%d_eplus", icent, icut) );
			H1D_EffVsEta_AllPhiPt_EPlus_Cent[icent][icut] = (TH1F*)H1D_RcEta_AllPhiPt_EPlus_Cent[icent][icut]->Clone( Form("H1D_EffVsEta_AllPhiPt_icent%d_icut%d_eplus", icent, icut) );
			H1D_EffVsEta_AllPhiPt_EPlus_Cent[icent][icut] -> Reset();
			H1D_EffVsEta_AllPhiPt_EPlus_Cent[icent][icut] -> Divide( H1D_RcEta_AllPhiPt_EPlus_Cent[icent][icut], H1D_McEta_AllPhiPt_EPlus_Cent[icent], 1., 1., "B" );

			H1D_RcPhi_AllPtEta_EPlus_Cent[icent][icut]    = (TH1F*)H3D_Rc_PtEtaPhi_EPlus_Cent[icent][icut]->ProjectionZ( Form("H1D_RcPhi_AllPtEta_icent%d_icut%d_eplus", icent, icut) );
			H1D_EffVsPhi_AllPtEta_EPlus_Cent[icent][icut] = (TH1F*)H1D_RcPhi_AllPtEta_EPlus_Cent[icent][icut]->Clone( Form("H1D_EffVsPhi_AllPtEta_icent%d_icut%d_eplus", icent, icut) );
			H1D_EffVsPhi_AllPtEta_EPlus_Cent[icent][icut] -> Reset();
			H1D_EffVsPhi_AllPtEta_EPlus_Cent[icent][icut] -> Divide( H1D_RcPhi_AllPtEta_EPlus_Cent[icent][icut], H1D_McPhi_AllPtEta_EPlus_Cent[icent], 1., 1., "B" );

			// e-
			H1D_RcPt_AllEtaPhi_EMinus_Cent[icent][icut]    = (TH1F*)H3D_Rc_PtEtaPhi_EMinus_Cent[icent][icut]->ProjectionX( Form("H1D_RcPt_AllEtaPhi_icent%d_icut%d_eminus", icent, icut) );
			H1D_EffVsPt_AllEtaPhi_EMinus_Cent[icent][icut] = (TH1F*)H1D_RcPt_AllEtaPhi_EMinus_Cent[icent][icut]->Clone( Form("H1D_EffVsPt_AllEtaPhi_icent%d_icut%d_eminus", icent, icut) );
			H1D_EffVsPt_AllEtaPhi_EMinus_Cent[icent][icut] -> Reset();
			H1D_EffVsPt_AllEtaPhi_EMinus_Cent[icent][icut] -> Divide( H1D_RcPt_AllEtaPhi_EMinus_Cent[icent][icut], H1D_McPt_AllEtaPhi_EMinus_Cent[icent], 1., 1., "B" );
		
			H1D_RcEta_AllPhiPt_EMinus_Cent[icent][icut]    = (TH1F*)H3D_Rc_PtEtaPhi_EMinus_Cent[icent][icut]->ProjectionY( Form("H1D_RcEta_AllPhiPt_icent%d_icut%d_eminus", icent, icut) );
			H1D_EffVsEta_AllPhiPt_EMinus_Cent[icent][icut] = (TH1F*)H1D_RcEta_AllPhiPt_EMinus_Cent[icent][icut]->Clone( Form("H1D_EffVsEta_AllPhiPt_icent%d_icut%d_eminus", icent, icut) );
			H1D_EffVsEta_AllPhiPt_EMinus_Cent[icent][icut] -> Reset();
			H1D_EffVsEta_AllPhiPt_EMinus_Cent[icent][icut] -> Divide( H1D_RcEta_AllPhiPt_EMinus_Cent[icent][icut], H1D_McEta_AllPhiPt_EMinus_Cent[icent], 1., 1., "B" );

			H1D_RcPhi_AllPtEta_EMinus_Cent[icent][icut]    = (TH1F*)H3D_Rc_PtEtaPhi_EMinus_Cent[icent][icut]->ProjectionZ( Form("H1D_RcPhi_AllPtEta_icent%d_icut%d_eminus", icent, icut) );
			H1D_EffVsPhi_AllPtEta_EMinus_Cent[icent][icut] = (TH1F*)H1D_RcPhi_AllPtEta_EMinus_Cent[icent][icut]->Clone( Form("H1D_EffVsPhi_AllPtEta_icent%d_icut%d_eminus", icent, icut) );
			H1D_EffVsPhi_AllPtEta_EMinus_Cent[icent][icut] -> Reset();
			H1D_EffVsPhi_AllPtEta_EMinus_Cent[icent][icut] -> Divide( H1D_RcPhi_AllPtEta_EMinus_Cent[icent][icut], H1D_McPhi_AllPtEta_EMinus_Cent[icent], 1., 1., "B" );

			// e+ & e-
			H1D_RcPt_AllEtaPhi_Cent[icent][icut]    = (TH1F*)H3D_Rc_PtEtaPhi_Cent[icent][icut]->ProjectionX( Form("H1D_RcPt_AllEtaPhi_icent%d_icut%d", icent, icut) );
			H1D_EffVsPt_AllEtaPhi_Cent[icent][icut] = (TH1F*)H1D_RcPt_AllEtaPhi_Cent[icent][icut]->Clone( Form("H1D_EffVsPt_AllEtaPhi_icent%d_icut%d", icent, icut) );
			H1D_EffVsPt_AllEtaPhi_Cent[icent][icut] -> Reset();
			H1D_EffVsPt_AllEtaPhi_Cent[icent][icut] -> Divide( H1D_RcPt_AllEtaPhi_Cent[icent][icut], H1D_McPt_AllEtaPhi_Cent[icent], 1., 1., "B" );
		
			H1D_RcEta_AllPhiPt_Cent[icent][icut]    = (TH1F*)H3D_Rc_PtEtaPhi_Cent[icent][icut]->ProjectionY( Form("H1D_RcEta_AllPhiPt_icent%d_icut%d", icent, icut) );
			H1D_EffVsEta_AllPhiPt_Cent[icent][icut] = (TH1F*)H1D_RcEta_AllPhiPt_Cent[icent][icut]->Clone( Form("H1D_EffVsEta_AllPhiPt_icent%d_icut%d", icent, icut) );
			H1D_EffVsEta_AllPhiPt_Cent[icent][icut] -> Reset();
			H1D_EffVsEta_AllPhiPt_Cent[icent][icut] -> Divide( H1D_RcEta_AllPhiPt_Cent[icent][icut], H1D_McEta_AllPhiPt_Cent[icent], 1., 1., "B" );

			H1D_RcPhi_AllPtEta_Cent[icent][icut]    = (TH1F*)H3D_Rc_PtEtaPhi_Cent[icent][icut]->ProjectionZ( Form("H1D_RcPhi_AllPtEta_icent%d_icut%d", icent, icut) );
			H1D_EffVsPhi_AllPtEta_Cent[icent][icut] = (TH1F*)H1D_RcPhi_AllPtEta_Cent[icent][icut]->Clone( Form("H1D_EffVsPhi_AllPtEta_icent%d_icut%d", icent, icut) );
			H1D_EffVsPhi_AllPtEta_Cent[icent][icut] -> Reset();
			H1D_EffVsPhi_AllPtEta_Cent[icent][icut] -> Divide( H1D_RcPhi_AllPtEta_Cent[icent][icut], H1D_McPhi_AllPtEta_Cent[icent], 1., 1., "B" );
		}

		H3D_Mc_PtEtaPhi_EPlus_Cent[icent]->Rebin3D(2, 5, 2);
		H3D_Mc_PtEtaPhi_EMinus_Cent[icent]->Rebin3D(2, 5, 2);
		H3D_Rc_PtEtaPhi_EPlus_Cent[icent][4]->Rebin3D(2, 5, 2);
		H3D_Rc_PtEtaPhi_EMinus_Cent[icent][4]->Rebin3D(2, 5, 2);

		//calculate 3D efficiency
		H3D_Eff_EPlus[icent] = Cal3DEff(H3D_Mc_PtEtaPhi_EPlus_Cent[icent], H3D_Rc_PtEtaPhi_EPlus_Cent[icent][4]);
		H3D_Eff_EMinus[icent] = Cal3DEff(H3D_Mc_PtEtaPhi_EMinus_Cent[icent], H3D_Rc_PtEtaPhi_EMinus_Cent[icent][4]);
	}//icent
	cout << "----------------------------------------------" << endl;
	cout << "Finished reading files" << endl;
}
//==================================================================================================================
//==================================================================================================================
void drawEff()
{
	cout << "----------------------------------------------" << endl;
	cout << "Now we begin to draw plots ..." << endl;
	InitColor(MarkerColor_Cens, 55);
    InitColor(MarkerColor_Cuts, 55);

	system(Form("mkdir -p %s/%s", outDir.Data(), ValueType[i_type]));

	TCanvas *c1 = new TCanvas("c1", "c1", 800, 600);
	TLegend *leg = CreateLegend(0.40, 0.12, 0.85, 0.42);
	
	//----------------------------------------------------------------------------------------------------
	//eff vs pT 
	//----------------------------------------------------------------------------------------------------
	cout << "----------------------------------------------" << endl;
	c1->cd();
	leg->SetHeader("RFF O+O@200GeV");
	for(Int_t icent=0; icent<nCentBins; icent++)
	{
		cout << "compare tracking eff. vs. Pt between different cuts, for centrality: " << icent << endl;
		for(int icut=0; icut<nCuts; icut++)
		{
			cout << "draw plots: icent= " << icent << " icut: " << icut << endl;

			H1D_EffVsPt_AllEtaPhi_Cent[icent][icut]->SetTitle( Form("Centrality: %d-%d%%", CentLow[icent],CentHig[icent]) );
			H1D_EffVsPt_AllEtaPhi_Cent[icent][icut]->SetMarkerStyle(20);
			H1D_EffVsPt_AllEtaPhi_Cent[icent][icut]->SetMarkerSize(0.5);
			H1D_EffVsPt_AllEtaPhi_Cent[icent][icut]->SetMarkerColor(MarkerColor_Cuts.at(icut));
			H1D_EffVsPt_AllEtaPhi_Cent[icent][icut]->SetLineColor(MarkerColor_Cuts.at(icut));
			H1D_EffVsPt_AllEtaPhi_Cent[icent][icut]->GetXaxis()->SetRangeUser(0.0, 5.0);
			H1D_EffVsPt_AllEtaPhi_Cent[icent][icut]->GetYaxis()->SetTitle("TPC Efficiency");
			H1D_EffVsPt_AllEtaPhi_Cent[icent][icut]->GetYaxis()->SetRangeUser(0., 1.2);
			H1D_EffVsPt_AllEtaPhi_Cent[icent][icut]->GetYaxis()->SetTitleSize(0.055);
			H1D_EffVsPt_AllEtaPhi_Cent[icent][icut]->GetYaxis()->SetTitleOffset(0.80);
			H1D_EffVsPt_AllEtaPhi_Cent[icent][icut]->GetYaxis()->CenterTitle(kTRUE);
			if(icut==0) H1D_EffVsPt_AllEtaPhi_Cent[icent][icut]->Draw("pe");
			else H1D_EffVsPt_AllEtaPhi_Cent[icent][icut]->Draw("pesame");
			
			leg->AddEntry(H1D_EffVsPt_AllEtaPhi_Cent[icent][icut], CutName[icut], "lp");
		}
		
		leg->Draw("same");
		c1->SaveAs(Form("%s/%s/H1D_EffVsPt_AllEtaPhi_icent%d.png", outDir.Data(), ValueType[i_type], icent));
		leg->Clear();

	}
	c1->Clear();
	c1->cd();
	leg->SetHeader("RFF O+O@200GeV");
	for(Int_t icent=0; icent<nCentBins; icent++)
	{
		cout << "compare tracking eff. vs. Pt between e+ and e-, for centrality: " << icent << endl;

		H1D_EffVsPt_AllEtaPhi_EPlus_Cent[icent][4]->SetTitle( Form("Centrality: %d-%d%%", CentLow[icent],CentHig[icent]) );
		H1D_EffVsPt_AllEtaPhi_EPlus_Cent[icent][4]->SetMarkerStyle(20);
		H1D_EffVsPt_AllEtaPhi_EPlus_Cent[icent][4]->SetMarkerSize(0.5);
		H1D_EffVsPt_AllEtaPhi_EPlus_Cent[icent][4]->SetMarkerColor(MarkerColor_Cuts.at(0));
		H1D_EffVsPt_AllEtaPhi_EPlus_Cent[icent][4]->SetLineColor(MarkerColor_Cuts.at(0));
		H1D_EffVsPt_AllEtaPhi_EPlus_Cent[icent][4]->GetXaxis()->SetRangeUser(0.0, 5.0);
		H1D_EffVsPt_AllEtaPhi_EPlus_Cent[icent][4]->GetYaxis()->SetTitle("TPC Efficiency");
		H1D_EffVsPt_AllEtaPhi_EPlus_Cent[icent][4]->GetYaxis()->SetRangeUser(0., 1.2);
		H1D_EffVsPt_AllEtaPhi_EPlus_Cent[icent][4]->GetYaxis()->SetTitleSize(0.055);
		H1D_EffVsPt_AllEtaPhi_EPlus_Cent[icent][4]->GetYaxis()->SetTitleOffset(0.80);
		H1D_EffVsPt_AllEtaPhi_EPlus_Cent[icent][4]->GetYaxis()->CenterTitle(kTRUE);
		H1D_EffVsPt_AllEtaPhi_EPlus_Cent[icent][4]->Draw("pe");
		leg->AddEntry(H1D_EffVsPt_AllEtaPhi_EPlus_Cent[icent][4], "e^{+}", "lp");

		H1D_EffVsPt_AllEtaPhi_EMinus_Cent[icent][4]->SetMarkerStyle(20);
		H1D_EffVsPt_AllEtaPhi_EMinus_Cent[icent][4]->SetMarkerSize(0.5);
		H1D_EffVsPt_AllEtaPhi_EMinus_Cent[icent][4]->SetMarkerColor(MarkerColor_Cuts.at(2));
		H1D_EffVsPt_AllEtaPhi_EMinus_Cent[icent][4]->SetLineColor(MarkerColor_Cuts.at(2));
		H1D_EffVsPt_AllEtaPhi_EMinus_Cent[icent][4]->GetXaxis()->SetRangeUser(0.0, 5.0);
		H1D_EffVsPt_AllEtaPhi_EMinus_Cent[icent][4]->GetYaxis()->SetTitle("TPC Efficiency");
		H1D_EffVsPt_AllEtaPhi_EMinus_Cent[icent][4]->GetYaxis()->SetRangeUser(0., 1.2);
		H1D_EffVsPt_AllEtaPhi_EMinus_Cent[icent][4]->GetYaxis()->SetTitleSize(0.055);
		H1D_EffVsPt_AllEtaPhi_EMinus_Cent[icent][4]->GetYaxis()->SetTitleOffset(0.80);
		H1D_EffVsPt_AllEtaPhi_EMinus_Cent[icent][4]->GetYaxis()->CenterTitle(kTRUE);
		H1D_EffVsPt_AllEtaPhi_EMinus_Cent[icent][4]->Draw("pesame");
		leg->AddEntry(H1D_EffVsPt_AllEtaPhi_EMinus_Cent[icent][4], "e^{-}", "lp");

		H1D_EffVsPt_AllEtaPhi_Cent[icent][4]->SetMarkerStyle(20);
		H1D_EffVsPt_AllEtaPhi_Cent[icent][4]->SetMarkerSize(0.5);
		H1D_EffVsPt_AllEtaPhi_Cent[icent][4]->SetMarkerColor(MarkerColor_Cuts.at(4));
		H1D_EffVsPt_AllEtaPhi_Cent[icent][4]->SetLineColor(MarkerColor_Cuts.at(4));
		H1D_EffVsPt_AllEtaPhi_Cent[icent][4]->GetXaxis()->SetRangeUser(0.0, 5.0);
		H1D_EffVsPt_AllEtaPhi_Cent[icent][4]->GetYaxis()->SetTitle("TPC Efficiency");
		H1D_EffVsPt_AllEtaPhi_Cent[icent][4]->GetYaxis()->SetRangeUser(0., 1.2);
		H1D_EffVsPt_AllEtaPhi_Cent[icent][4]->GetYaxis()->SetTitleSize(0.055);
		H1D_EffVsPt_AllEtaPhi_Cent[icent][4]->GetYaxis()->SetTitleOffset(0.80);
		H1D_EffVsPt_AllEtaPhi_Cent[icent][4]->GetYaxis()->CenterTitle(kTRUE);
		H1D_EffVsPt_AllEtaPhi_Cent[icent][4]->Draw("pesame");
		leg->AddEntry(H1D_EffVsPt_AllEtaPhi_Cent[icent][4], "e^{+} + e^{-}", "lp");
		
		leg->Draw("same");
		c1->SaveAs(Form("%s/%s/H1D_EffVsPt_AllEtaPhi_icent%d_diffcharge.png", outDir.Data(), ValueType[i_type], icent));
		leg->Clear();
	}
	c1->Clear();
	c1->cd();
	leg->SetHeader("RFF O+O@200GeV");
	cout << "compare tracking eff. vs. Pt between centralities: " << endl;
	for(Int_t icent=0; icent<nCentBins; icent++)
	{
		cout << "draw plots: icent= " << icent << endl;
		H1D_EffVsPt_AllEtaPhi_Cent[icent][4]->SetMarkerStyle(20);
		H1D_EffVsPt_AllEtaPhi_Cent[icent][4]->SetMarkerSize(0.5);
		H1D_EffVsPt_AllEtaPhi_Cent[icent][4]->SetMarkerColor(MarkerColor_Cens.at(icent));
		H1D_EffVsPt_AllEtaPhi_Cent[icent][4]->SetLineColor(MarkerColor_Cens.at(icent));
		
		if(icent==0)
		{
			H1D_EffVsPt_AllEtaPhi_Cent[icent][4]->SetTitle("");
			H1D_EffVsPt_AllEtaPhi_Cent[icent][4]->GetXaxis()->SetRangeUser(0.0, 5.0);
			H1D_EffVsPt_AllEtaPhi_Cent[icent][4]->GetYaxis()->SetTitle("TPC Efficiency");
			H1D_EffVsPt_AllEtaPhi_Cent[icent][4]->GetYaxis()->SetRangeUser(0., 1.2);
			H1D_EffVsPt_AllEtaPhi_Cent[icent][4]->GetYaxis()->SetTitleSize(0.055);
			H1D_EffVsPt_AllEtaPhi_Cent[icent][4]->GetYaxis()->SetTitleOffset(0.80);
			H1D_EffVsPt_AllEtaPhi_Cent[icent][4]->GetYaxis()->CenterTitle(kTRUE);
			H1D_EffVsPt_AllEtaPhi_Cent[icent][4]->Draw("pe");
		}
		else H1D_EffVsPt_AllEtaPhi_Cent[icent][4]->Draw("pesame");

		leg->AddEntry(H1D_EffVsPt_AllEtaPhi_Cent[icent][4], Form("Centrality: %d-%d%%", CentLow[icent],CentHig[icent]), "lp");
	}//icent	
	leg->Draw("same");	
	c1->SaveAs(Form("%s/%s/H1D_EffVsPt_btwCents.png", outDir.Data(), ValueType[i_type]));
	leg->Clear();	
	c1->Clear();

	//----------------------------------------------------------------------------------------------------
	//eff vs Eta 
	//----------------------------------------------------------------------------------------------------
	cout << "----------------------------------------------" << endl;
	c1->cd();
	leg->SetHeader("RFF O+O@200GeV");
	for(Int_t icent=0; icent<nCentBins; icent++)
	{
		cout << "compare tracking eff. vs. Eta between different cuts, for centrality: " << icent << endl;
		for(int icut=0; icut<nCuts; icut++)
		{
			cout << "draw plots: icent= " << icent << " icut: " << icut << endl;

			H1D_EffVsEta_AllPhiPt_Cent[icent][icut]->SetTitle( Form("Centrality: %d-%d%%", CentLow[icent],CentHig[icent]) );
			H1D_EffVsEta_AllPhiPt_Cent[icent][icut]->SetMarkerStyle(20);
			H1D_EffVsEta_AllPhiPt_Cent[icent][icut]->SetMarkerSize(0.5);
			H1D_EffVsEta_AllPhiPt_Cent[icent][icut]->SetMarkerColor(MarkerColor_Cuts.at(icut));
			H1D_EffVsEta_AllPhiPt_Cent[icent][icut]->SetLineColor(MarkerColor_Cuts.at(icut));
			H1D_EffVsEta_AllPhiPt_Cent[icent][icut]->GetXaxis()->SetRangeUser(-1.2, 1.2);
			H1D_EffVsEta_AllPhiPt_Cent[icent][icut]->GetYaxis()->SetTitle("TPC Efficiency");
			H1D_EffVsEta_AllPhiPt_Cent[icent][icut]->GetYaxis()->SetRangeUser(0., 1.2);
			H1D_EffVsEta_AllPhiPt_Cent[icent][icut]->GetYaxis()->SetTitleSize(0.055);
			H1D_EffVsEta_AllPhiPt_Cent[icent][icut]->GetYaxis()->SetTitleOffset(0.80);
			H1D_EffVsEta_AllPhiPt_Cent[icent][icut]->GetYaxis()->CenterTitle(kTRUE);
			if(icut==0) H1D_EffVsEta_AllPhiPt_Cent[icent][icut]->Draw("pe");
			else H1D_EffVsEta_AllPhiPt_Cent[icent][icut]->Draw("pesame");
			
			leg->AddEntry(H1D_EffVsEta_AllPhiPt_Cent[icent][icut], CutName[icut], "lp");
		}
		
		leg->Draw("same");
		c1->SaveAs(Form("%s/%s/H1D_EffVsEta_AllPhiPt_icent%d.png", outDir.Data(), ValueType[i_type], icent));	
		leg->Clear();

	}//icent	
	c1->Clear();	
	c1->cd();
	leg->SetHeader("RFF O+O@200GeV");
	for(Int_t icent=0; icent<nCentBins; icent++)
	{
		cout << "compare tracking eff. vs. Eta between e+ and e-, for centrality: " << icent << endl;

		H1D_EffVsEta_AllPhiPt_EPlus_Cent[icent][4]->SetTitle( Form("Centrality: %d-%d%%", CentLow[icent],CentHig[icent]) );
		H1D_EffVsEta_AllPhiPt_EPlus_Cent[icent][4]->SetMarkerStyle(20);
		H1D_EffVsEta_AllPhiPt_EPlus_Cent[icent][4]->SetMarkerSize(0.5);
		H1D_EffVsEta_AllPhiPt_EPlus_Cent[icent][4]->SetMarkerColor(MarkerColor_Cuts.at(0));
		H1D_EffVsEta_AllPhiPt_EPlus_Cent[icent][4]->SetLineColor(MarkerColor_Cuts.at(0));
		H1D_EffVsEta_AllPhiPt_EPlus_Cent[icent][4]->GetXaxis()->SetRangeUser(-1.2, 1.2);
		H1D_EffVsEta_AllPhiPt_EPlus_Cent[icent][4]->GetYaxis()->SetTitle("TPC Efficiency");
		H1D_EffVsEta_AllPhiPt_EPlus_Cent[icent][4]->GetYaxis()->SetRangeUser(0., 1.2);
		H1D_EffVsEta_AllPhiPt_EPlus_Cent[icent][4]->GetYaxis()->SetTitleSize(0.055);
		H1D_EffVsEta_AllPhiPt_EPlus_Cent[icent][4]->GetYaxis()->SetTitleOffset(0.80);
		H1D_EffVsEta_AllPhiPt_EPlus_Cent[icent][4]->GetYaxis()->CenterTitle(kTRUE);
		H1D_EffVsEta_AllPhiPt_EPlus_Cent[icent][4]->Draw("pe");
		leg->AddEntry(H1D_EffVsEta_AllPhiPt_EPlus_Cent[icent][4], "e^{+}", "lp");

		H1D_EffVsEta_AllPhiPt_EMinus_Cent[icent][4]->SetMarkerStyle(20);
		H1D_EffVsEta_AllPhiPt_EMinus_Cent[icent][4]->SetMarkerSize(0.5);
		H1D_EffVsEta_AllPhiPt_EMinus_Cent[icent][4]->SetMarkerColor(MarkerColor_Cuts.at(2));
		H1D_EffVsEta_AllPhiPt_EMinus_Cent[icent][4]->SetLineColor(MarkerColor_Cuts.at(2));
		H1D_EffVsEta_AllPhiPt_EMinus_Cent[icent][4]->GetXaxis()->SetRangeUser(-1.2, 1.2);
		H1D_EffVsEta_AllPhiPt_EMinus_Cent[icent][4]->GetYaxis()->SetTitle("TPC Efficiency");
		H1D_EffVsEta_AllPhiPt_EMinus_Cent[icent][4]->GetYaxis()->SetRangeUser(0., 1.2);
		H1D_EffVsEta_AllPhiPt_EMinus_Cent[icent][4]->GetYaxis()->SetTitleSize(0.055);
		H1D_EffVsEta_AllPhiPt_EMinus_Cent[icent][4]->GetYaxis()->SetTitleOffset(0.80);
		H1D_EffVsEta_AllPhiPt_EMinus_Cent[icent][4]->GetYaxis()->CenterTitle(kTRUE);
		H1D_EffVsEta_AllPhiPt_EMinus_Cent[icent][4]->Draw("pesame");
		leg->AddEntry(H1D_EffVsEta_AllPhiPt_EMinus_Cent[icent][4], "e^{-}", "lp");

		H1D_EffVsEta_AllPhiPt_Cent[icent][4]->SetMarkerStyle(20);
		H1D_EffVsEta_AllPhiPt_Cent[icent][4]->SetMarkerSize(0.5);
		H1D_EffVsEta_AllPhiPt_Cent[icent][4]->SetMarkerColor(MarkerColor_Cuts.at(4));
		H1D_EffVsEta_AllPhiPt_Cent[icent][4]->SetLineColor(MarkerColor_Cuts.at(4));
		H1D_EffVsEta_AllPhiPt_Cent[icent][4]->GetXaxis()->SetRangeUser(-1.2, 1.2);
		H1D_EffVsEta_AllPhiPt_Cent[icent][4]->GetYaxis()->SetTitle("TPC Efficiency");
		H1D_EffVsEta_AllPhiPt_Cent[icent][4]->GetYaxis()->SetRangeUser(0., 1.2);
		H1D_EffVsEta_AllPhiPt_Cent[icent][4]->GetYaxis()->SetTitleSize(0.055);
		H1D_EffVsEta_AllPhiPt_Cent[icent][4]->GetYaxis()->SetTitleOffset(0.80);
		H1D_EffVsEta_AllPhiPt_Cent[icent][4]->GetYaxis()->CenterTitle(kTRUE);
		H1D_EffVsEta_AllPhiPt_Cent[icent][4]->Draw("pesame");
		leg->AddEntry(H1D_EffVsEta_AllPhiPt_Cent[icent][4], "e^{+} + e^{-}", "lp");
		
		leg->Draw("same");
		c1->SaveAs(Form("%s/%s/H1D_EffVsEta_AllPhiPt_icent%d_diffcharge.png", outDir.Data(), ValueType[i_type], icent));
		leg->Clear();
	}
	c1->Clear();
	c1->cd();
	leg->SetHeader("RFF O+O@200GeV");
	cout << "compare tracking eff. vs. Eta between centralities: " << endl;
	for(Int_t icent=0; icent<nCentBins; icent++)
	{
		cout << "draw plots: icent= " << icent << endl;
		H1D_EffVsEta_AllPhiPt_Cent[icent][4]->SetMarkerStyle(20);
		H1D_EffVsEta_AllPhiPt_Cent[icent][4]->SetMarkerSize(0.5);
		H1D_EffVsEta_AllPhiPt_Cent[icent][4]->SetMarkerColor(MarkerColor_Cens.at(icent));
		H1D_EffVsEta_AllPhiPt_Cent[icent][4]->SetLineColor(MarkerColor_Cens.at(icent));
		
		if(icent==0)
		{
			H1D_EffVsEta_AllPhiPt_Cent[icent][4]->SetTitle("");
			H1D_EffVsEta_AllPhiPt_Cent[icent][4]->GetXaxis()->SetRangeUser(-1.2, 1.2);
			H1D_EffVsEta_AllPhiPt_Cent[icent][4]->GetYaxis()->SetTitle("TPC Efficiency");
			H1D_EffVsEta_AllPhiPt_Cent[icent][4]->GetYaxis()->SetRangeUser(0., 1.2);
			H1D_EffVsEta_AllPhiPt_Cent[icent][4]->GetYaxis()->SetTitleSize(0.055);
			H1D_EffVsEta_AllPhiPt_Cent[icent][4]->GetYaxis()->SetTitleOffset(0.80);
			H1D_EffVsEta_AllPhiPt_Cent[icent][4]->GetYaxis()->CenterTitle(kTRUE);
			H1D_EffVsEta_AllPhiPt_Cent[icent][4]->Draw("pe");
		}
		else H1D_EffVsEta_AllPhiPt_Cent[icent][4]->Draw("pesame");

		leg->AddEntry(H1D_EffVsEta_AllPhiPt_Cent[icent][4], Form("Centrality: %d-%d%%", CentLow[icent],CentHig[icent]), "lp");
	}//icent	
	leg->Draw("same");
	c1->SaveAs(Form("%s/%s/H1D_EffVsEta_btwCents.png", outDir.Data(), ValueType[i_type]));
	leg->Clear();
	c1->Clear();

	//----------------------------------------------------------------------------------------------------
	//eff vs Phi 
	//----------------------------------------------------------------------------------------------------	
	cout << "----------------------------------------------" << endl;
	c1->cd();
	leg->SetHeader("RFF O+O@200GeV");
	for(Int_t icent=0; icent<nCentBins; icent++)
	{
		cout << "compare tracking eff. vs. Phi between centralities: " << endl;
		for(int icut=0; icut<nCuts; icut++)
		{
			cout << "draw plots: icent= " << icent << " icut: " << icut << endl;

			H1D_EffVsPhi_AllPtEta_Cent[icent][icut]->SetTitle( Form("Centrality: %d-%d%%", CentLow[icent],CentHig[icent]) );
			H1D_EffVsPhi_AllPtEta_Cent[icent][icut]->SetMarkerStyle(20);
			H1D_EffVsPhi_AllPtEta_Cent[icent][icut]->SetMarkerSize(0.5);
			H1D_EffVsPhi_AllPtEta_Cent[icent][icut]->SetMarkerColor(MarkerColor_Cuts.at(icut));
			H1D_EffVsPhi_AllPtEta_Cent[icent][icut]->SetLineColor(MarkerColor_Cuts.at(icut));
			H1D_EffVsPhi_AllPtEta_Cent[icent][icut]->GetXaxis()->SetRangeUser(-4.0, 4.0);
			H1D_EffVsPhi_AllPtEta_Cent[icent][icut]->GetYaxis()->SetTitle("TPC Efficiency");
			H1D_EffVsPhi_AllPtEta_Cent[icent][icut]->GetYaxis()->SetRangeUser(0., 1.2);
			H1D_EffVsPhi_AllPtEta_Cent[icent][icut]->GetYaxis()->SetTitleSize(0.055);
			H1D_EffVsPhi_AllPtEta_Cent[icent][icut]->GetYaxis()->SetTitleOffset(0.80);
			H1D_EffVsPhi_AllPtEta_Cent[icent][icut]->GetYaxis()->CenterTitle(kTRUE);
			if(icut==0) H1D_EffVsPhi_AllPtEta_Cent[icent][icut]->Draw("pe");
			else H1D_EffVsPhi_AllPtEta_Cent[icent][icut]->Draw("pesame");
			
			leg->AddEntry(H1D_EffVsPhi_AllPtEta_Cent[icent][icut], CutName[icut], "lp");
		}
		
		leg->Draw("same");
		c1->SaveAs(Form("%s/%s/H1D_EffVsPhi_AllPtEta_icent%d.png", outDir.Data(), ValueType[i_type], icent));
		leg->Clear();

	}//icent
	c1->Clear();
	c1->cd();
	leg->SetHeader("RFF O+O@200GeV");
	for(Int_t icent=0; icent<nCentBins; icent++)
	{
		cout << "compare tracking eff. vs. Phi between e+ and e-, for centrality: " << icent << endl;

		H1D_EffVsPhi_AllPtEta_EPlus_Cent[icent][4]->SetTitle( Form("Centrality: %d-%d%%", CentLow[icent],CentHig[icent]) );
		H1D_EffVsPhi_AllPtEta_EPlus_Cent[icent][4]->SetMarkerStyle(20);
		H1D_EffVsPhi_AllPtEta_EPlus_Cent[icent][4]->SetMarkerSize(0.5);
		H1D_EffVsPhi_AllPtEta_EPlus_Cent[icent][4]->SetMarkerColor(MarkerColor_Cuts.at(0));
		H1D_EffVsPhi_AllPtEta_EPlus_Cent[icent][4]->SetLineColor(MarkerColor_Cuts.at(0));
		H1D_EffVsPhi_AllPtEta_EPlus_Cent[icent][4]->GetXaxis()->SetRangeUser(-4.0, 4.0);
		H1D_EffVsPhi_AllPtEta_EPlus_Cent[icent][4]->GetYaxis()->SetTitle("TPC Efficiency");
		H1D_EffVsPhi_AllPtEta_EPlus_Cent[icent][4]->GetYaxis()->SetRangeUser(0., 1.2);
		H1D_EffVsPhi_AllPtEta_EPlus_Cent[icent][4]->GetYaxis()->SetTitleSize(0.055);
		H1D_EffVsPhi_AllPtEta_EPlus_Cent[icent][4]->GetYaxis()->SetTitleOffset(0.80);
		H1D_EffVsPhi_AllPtEta_EPlus_Cent[icent][4]->GetYaxis()->CenterTitle(kTRUE);
		H1D_EffVsPhi_AllPtEta_EPlus_Cent[icent][4]->Draw("pe");
		leg->AddEntry(H1D_EffVsPhi_AllPtEta_EPlus_Cent[icent][4], "e^{+}", "lp");

		H1D_EffVsPhi_AllPtEta_EMinus_Cent[icent][4]->SetMarkerStyle(20);
		H1D_EffVsPhi_AllPtEta_EMinus_Cent[icent][4]->SetMarkerSize(0.5);
		H1D_EffVsPhi_AllPtEta_EMinus_Cent[icent][4]->SetMarkerColor(MarkerColor_Cuts.at(2));
		H1D_EffVsPhi_AllPtEta_EMinus_Cent[icent][4]->SetLineColor(MarkerColor_Cuts.at(2));
		H1D_EffVsPhi_AllPtEta_EMinus_Cent[icent][4]->GetXaxis()->SetRangeUser(-4.0, 4.0);
		H1D_EffVsPhi_AllPtEta_EMinus_Cent[icent][4]->GetYaxis()->SetTitle("TPC Efficiency");
		H1D_EffVsPhi_AllPtEta_EMinus_Cent[icent][4]->GetYaxis()->SetRangeUser(0., 1.2);
		H1D_EffVsPhi_AllPtEta_EMinus_Cent[icent][4]->GetYaxis()->SetTitleSize(0.055);
		H1D_EffVsPhi_AllPtEta_EMinus_Cent[icent][4]->GetYaxis()->SetTitleOffset(0.80);
		H1D_EffVsPhi_AllPtEta_EMinus_Cent[icent][4]->GetYaxis()->CenterTitle(kTRUE);
		H1D_EffVsPhi_AllPtEta_EMinus_Cent[icent][4]->Draw("pesame");
		leg->AddEntry(H1D_EffVsPhi_AllPtEta_EMinus_Cent[icent][4], "e^{-}", "lp");

		H1D_EffVsPhi_AllPtEta_Cent[icent][4]->SetMarkerStyle(20);
		H1D_EffVsPhi_AllPtEta_Cent[icent][4]->SetMarkerSize(0.5);
		H1D_EffVsPhi_AllPtEta_Cent[icent][4]->SetMarkerColor(MarkerColor_Cuts.at(4));
		H1D_EffVsPhi_AllPtEta_Cent[icent][4]->SetLineColor(MarkerColor_Cuts.at(4));
		H1D_EffVsPhi_AllPtEta_Cent[icent][4]->GetXaxis()->SetRangeUser(-4.0, 4.0);
		H1D_EffVsPhi_AllPtEta_Cent[icent][4]->GetYaxis()->SetTitle("TPC Efficiency");
		H1D_EffVsPhi_AllPtEta_Cent[icent][4]->GetYaxis()->SetRangeUser(0., 1.2);
		H1D_EffVsPhi_AllPtEta_Cent[icent][4]->GetYaxis()->SetTitleSize(0.055);
		H1D_EffVsPhi_AllPtEta_Cent[icent][4]->GetYaxis()->SetTitleOffset(0.80);
		H1D_EffVsPhi_AllPtEta_Cent[icent][4]->GetYaxis()->CenterTitle(kTRUE);
		H1D_EffVsPhi_AllPtEta_Cent[icent][4]->Draw("pesame");
		leg->AddEntry(H1D_EffVsPhi_AllPtEta_Cent[icent][4], "e^{+} + e^{-}", "lp");
		
		leg->Draw("same");
		c1->SaveAs(Form("%s/%s/H1D_EffVsPhi_AllPtEta_icent%d_diffcharge.png", outDir.Data(), ValueType[i_type], icent));
		leg->Clear();
	}
	c1->Clear();
	c1->cd();
	leg->SetHeader("RFF O+O@200GeV");
	cout<<"compare tracking eff. vs. Phi between centralities: "<<endl;
	for(Int_t icent=0; icent<nCentBins; icent++)
	{
		cout << "draw plots: icent= " << icent << endl;
		H1D_EffVsPhi_AllPtEta_Cent[icent][4]->SetMarkerStyle(20);
		H1D_EffVsPhi_AllPtEta_Cent[icent][4]->SetMarkerSize(0.5);
		H1D_EffVsPhi_AllPtEta_Cent[icent][4]->SetMarkerColor(MarkerColor_Cens.at(icent));
		H1D_EffVsPhi_AllPtEta_Cent[icent][4]->SetLineColor(MarkerColor_Cens.at(icent));
		
		if(icent==0)
		{
			H1D_EffVsPhi_AllPtEta_Cent[icent][4]->SetTitle("");
			H1D_EffVsPhi_AllPtEta_Cent[icent][4]->GetXaxis()->SetRangeUser(-4.0, 4.0);
			H1D_EffVsPhi_AllPtEta_Cent[icent][4]->GetYaxis()->SetTitle("TPC Efficiency");
			H1D_EffVsPhi_AllPtEta_Cent[icent][4]->GetYaxis()->SetRangeUser(0., 1.2);
			H1D_EffVsPhi_AllPtEta_Cent[icent][4]->GetYaxis()->SetTitleSize(0.055);
			H1D_EffVsPhi_AllPtEta_Cent[icent][4]->GetYaxis()->SetTitleOffset(0.80);
			H1D_EffVsPhi_AllPtEta_Cent[icent][4]->GetYaxis()->CenterTitle(kTRUE);
			H1D_EffVsPhi_AllPtEta_Cent[icent][4]->Draw("pe");
		}
		else H1D_EffVsPhi_AllPtEta_Cent[icent][4]->Draw("pesame");

		leg->AddEntry(H1D_EffVsPhi_AllPtEta_Cent[icent][4], Form("Centrality: %d-%d%%", CentLow[icent],CentHig[icent]), "lp");
	}//icent	
	leg->Draw("same");
	c1->SaveAs(Form("%s/%s/H1D_EffVsPhi_btwCents.png", outDir.Data(), ValueType[i_type]));
	leg->Clear();
	c1->Clear();

	cout << "----------------------------------------------" << endl;
	cout << "Finished drawing plots" << endl;

	delete c1;
	delete leg;
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------------------------------
void saveFiles()
{
	cout << "----------------------------------------------" << endl;
	cout << "Saving files ... " << endl;

	outfile = new TFile(Form("%s/%s/TPCMcRcTracks_1D.root", outDir.Data(), ValueType[i_type]), "recreate");
	outfile->cd();
	for(int icent=0; icent<nCentBins; icent++)
	{
		H1D_McPt_AllEtaPhi_Cent[icent] ->Write();
		H1D_McEta_AllPhiPt_Cent[icent] ->Write();
		H1D_McPhi_AllPtEta_Cent[icent] ->Write();
		for(int icut=0; icut<nCuts; icut++)
		{
			H1D_RcPt_AllEtaPhi_Cent[icent][icut] ->Write();
			H1D_RcEta_AllPhiPt_Cent[icent][icut] ->Write();
			H1D_RcPhi_AllPtEta_Cent[icent][icut] ->Write();
		}
	}
	outfile->Close();
	delete outfile;

	outfile = new TFile(Form("%s/%s/TPCTrackEff_1D.root", outDir.Data(), ValueType[i_type]), "recreate");
	outfile->cd();
	for(int icent=0; icent<nCentBins; icent++)
	{
		for(int icut=0; icut<nCuts; icut++)
		{
			H1D_EffVsPt_AllEtaPhi_EPlus_Cent[icent][icut] ->Write();
			H1D_EffVsEta_AllPhiPt_EPlus_Cent[icent][icut] ->Write();
			H1D_EffVsPhi_AllPtEta_EPlus_Cent[icent][icut] ->Write();

			H1D_EffVsPt_AllEtaPhi_EMinus_Cent[icent][icut] ->Write();
			H1D_EffVsEta_AllPhiPt_EMinus_Cent[icent][icut] ->Write();
			H1D_EffVsPhi_AllPtEta_EMinus_Cent[icent][icut] ->Write();

			H1D_EffVsPt_AllEtaPhi_Cent[icent][icut] ->Write();
			H1D_EffVsEta_AllPhiPt_Cent[icent][icut] ->Write();
			H1D_EffVsPhi_AllPtEta_Cent[icent][icut] ->Write();
		}
	}
	outfile->Close();
	delete outfile;

	outfile = new TFile(Form("%s/%s/TPCTrackEff_3D.root", outDir.Data(), ValueType[i_type]), "recreate");
	outfile->cd();
	for(int icent=0; icent<nCentBins; icent++)
	{
		H3D_Eff_EPlus[icent] ->Write();
		H3D_Eff_EMinus[icent] ->Write();
	}
	outfile->Close();
	delete outfile;

	outfile = new TFile(Form("rootfiles/McpT_RcpT.root", outDir.Data(), ValueType[i_type]), "recreate");
	outfile->cd();
	for(int icent=0; icent<nCentBins; icent++)
	{
		H2D_PtDiff4Res[icent] ->Write();
	}
	outfile->Close();
	delete outfile;
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------------------------------
TPaveText* CreateTPaveText(Double_t x1, Double_t y1, Double_t x2, Double_t y2, const char* option)
{
    TPaveText *pt = new TPaveText(x1, y1, x2, y2, option);
    pt->SetTextSize(0.03);
    pt->SetTextFont(42);
    pt->SetTextColor(kBlack);
    pt->SetTextAlign(12);
    pt->SetFillColor(0);
    pt->SetFillColorAlpha(0, 0);
    pt->SetBorderSize(0);
    return pt;
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------------------------------
TLegend* CreateLegend(Double_t x1, Double_t y1, Double_t x2, Double_t y2)
{
    TLegend* leg = new TLegend(x1, y1, x2, y2);  
    leg->SetTextSize(0.03);  
    leg->SetTextFont(42);
    leg->SetTextColor(kBlack);
    leg->SetTextAlign(12);
    leg->SetFillStyle(0);
    leg->SetFillColorAlpha(0, 0);
    leg->SetBorderSize(0);
    return leg;
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------------------------------
void InitColor(std::vector<int>& color, int style)
{
    using namespace std;
    gStyle->SetPalette(style);
    // cout << "Number of colors = " << gStyle->GetNumberOfColors() << endl;
    // cout << "Color size = " << color.size() << endl;
    int N_size = color.size();
    if(N_size==0) return;
    for(int i=0; i<N_size; i++)
    {
        int idx = 1;
        if(N_size>1) idx = TColor::GetColorPalette(i * (gStyle->GetNumberOfColors() - 1) / (color.size() - 1));
        color.at(i) = idx;
        // cout << "Color[" << i << "] = " << color.at(i) << endl;
    }
    gStyle->SetPalette(57);   
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------------------------------
TH3F* Cal3DEff(TH3F* AllTrks, TH3F* PassCutTrks)
{
    int NBinsX = AllTrks->GetNbinsX();
    int NBinsY = AllTrks->GetNbinsY();
    int NBinsZ = AllTrks->GetNbinsZ();
    double Xmin = AllTrks->GetXaxis()->GetXmin();
    double Xmax = AllTrks->GetXaxis()->GetXmax();
    double Ymin = AllTrks->GetYaxis()->GetXmin();
    double Ymax = AllTrks->GetYaxis()->GetXmax();
    double Zmin = AllTrks->GetZaxis()->GetXmin();
    double Zmax = AllTrks->GetZaxis()->GetXmax();

    TH3F* eff = new TH3F(Form("eff_%s", AllTrks->GetName()+16), Form("eff_%s", AllTrks->GetName()+16), NBinsX, Xmin, Xmax, NBinsY, Ymin, Ymax, NBinsZ, Zmin, Zmax);
    // eff->Sumw2();
    
    for(int i=1; i<=NBinsX; i++)
    {
        for(int j=1; j<=NBinsY; j++)
        {
            for(int k=1; k<=NBinsZ; k++)
            {
                double Num, Den, ratio, ratioErr;

                Num = PassCutTrks->GetBinContent(i, j, k);
                Den = AllTrks->GetBinContent(i, j, k);

				// cout << "Bin (" << i << ", " << j << ", " << k << ") of " << AllTrks->GetName()+16 << ": Num = " << Num << ", Den = " << Den << endl;

                if(Den==0)
                {
                    ratio = 0;
                    ratioErr = 0;
                }
                else
                {
					if(Num==Den && Num<10)
					{
						cout << "Warning: Num==Den at bin (" << i << ", " << j << ", " << k << ") of " << endl;
						cout << "Num = " << Num << ", Den = " << Den << endl;
					}
                    ratio = Num/Den;
                    ratioErr = sqrt((Num+1)*(Num+2)/(Den+2)/(Den+3)-pow((Num+1)/(Den+2),2));
                }

				// if(ratio==1.0)
				// {
				// 	cout << "Warning: ratio=1.0 at bin (" << i << ", " << j << ", " << k << ") of " << AllTrks->GetName()+16 << ", ratio ==1 !" << endl;
				// 	getchar();
				// }

                eff->SetBinContent(i, j, k, ratio);
                eff->SetBinError(i, j, k, ratioErr);
            }
        }
    }

    cout << "Finished calculating efficiency of " << AllTrks->GetName()+16 << endl;

    return eff;
}