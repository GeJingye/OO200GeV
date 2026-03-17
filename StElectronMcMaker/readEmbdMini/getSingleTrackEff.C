#include "head.h"
#include "TPDF.h"
#include "TString.h"
#include "TPaveStats.h"
#include "../../../../../common/myFunction.h"

//TString outDir = "plots/outEfffiles_Vz35cm";
TString outDir = "plots/outEfffiles_Vz35cm_wNMcEFarcWT";
TPDF *mypdf;
//----------------------
TString centTitle ="";

void readFiles();
void drawEff(TPDF* mypdf);
void saveFiles();

const int flag_save_png = 1; //0 no save png; 1: save png, for AnaNote
//----------------------
const TString partType = "Positron";
//const TString partType = "Electron";
//----------------------
void getSingleTrackEff()
{
	readFiles();
	
	system(Form("mkdir -p %s", outDir.Data()));
	gStyle->SetOptStat(0);
	//----------------------
	//----------------------
	//mypdf = new TPDF(Form("%s/Run18_27GeV_AuAu_"+partType+"_TrackingEff_mcPt_Summarry.pdf", outDir.Data()),111);
	//mypdf = new TPDF(Form("%s/Run18_27GeV_AuAu_"+partType+"_TrackingEff_mcPt_Vz35cm_Summarry.pdf", outDir.Data()),111);
	mypdf = new TPDF(Form("%s/Run18_27GeV_AuAu_"+partType+"_TrackingEff_mcPt_Vz35cm_wNEfracWT_Summarry.pdf", outDir.Data()),111);
	mypdf->Off();
	//----------------------
	//----------------------
	drawEff(mypdf);
	//----------------------
	//----------------------
	mypdf->On();
	mypdf->Close();
	//----------------------
	if(flag_save_png==1) return;
	//----------------------
	saveFiles();
}

//==================================================================================================================
void readFiles()
{
	//infile = new TFile( "./inputfiles/embd/outEmbdQa_"+partType+"_Run18AuAu_27GeV_round1_round1.histo.root", "read" );
	//infile = new TFile( "./inputfiles/embd/outEmbdQa_"+partType+"_Run18AuAu_27GeV_round1_round1_mcPt.histo.root", "read" );
	
	//infile = new TFile( "./inputfiles/embd/fromVz35cm/outEmbdQa_"+partType+"_Run18AuAu_27GeV_mcPt_Vz35cm_round1.histo.root", "read" );
	//outEmbdQa_Electron_Run18AuAu_27GeV_mcPt_Vz35cm_wNEfracWT_round1
	infile = new TFile( "./inputfiles/embd/fromVz35cm_wnEfracWT/outEmbdQa_"+partType+"_Run18AuAu_27GeV_mcPt_Vz35cm_wNEfracWT_round1.histo.root", "read" );
	
	cout<<"readin file: "<<infile->GetName()<<endl;

	for(Int_t icent=0; icent<nCentBins; icent++)
	{
		cout<<"----------------------------------------------"<<endl;
		cout<<"read and merge histos in Cent: "<<CentLow[icent]<<"-"<<CentHig[icent]<<endl;

		for(Int_t isubcent=mCentBinLow[icent]; isubcent<=mCentBinHig[icent]; isubcent++)
		{
			cout<<"isubcent: "<<isubcent<<endl;
			//----------------------------------------------------------------------------------------------------------------------------------------------
			//----------------------------------------------------------------------------------------------------------------------------------------------
			H3D_Mc_PtEtaPhi_Cent_tem[isubcent]           = (TH3D*) infile->Get( Form("h3d_Mc_PtEtaPhi_Cent%d", isubcent) );
			//H3D_Mc_PtEtaPhi_Cent_tem[isubcent]->Sumw2();

			for(int icut=0; icut<nCuts; icut++)
			{
				H3D_Rc_PtEtaPhi_Cent_tem[isubcent][icut] = (TH3D*) infile->Get( Form("h3d_Rc_PtEtaPhi_Cent%d_icut%d", isubcent, icut) );
				//H3D_Rc_PtEtaPhi_Cent_tem[isubcent][icut] -> Sumw2();
			}
			//----------------------------------------------------------------------------------------------------------------------------------------------
			//----------------------------------------------------------------------------------------------------------------------------------------------
			
			//----------------------------------------------------------------------------------------------------------------------------------------------
			//----------------------------------------------------------------------------------------------------------------------------------------------
			if(isubcent==mCentBinLow[icent]) //clone the first one
			{
				H3D_Mc_PtEtaPhi_Cent[icent]           = (TH3D *)H3D_Mc_PtEtaPhi_Cent_tem[isubcent]      ->Clone(Form("H3D_Mc_PtEtaPhi_Cent%d_%d",         CentLow[icent],CentHig[icent])     );
				for(int icut=0; icut<nCuts; icut++)
				{
					H3D_Rc_PtEtaPhi_Cent[icent][icut] = (TH3D*) H3D_Rc_PtEtaPhi_Cent_tem[isubcent][icut]->Clone( Form("H3D_Rc_PtEtaPhi_Cent%d_%d_icut%d", CentLow[icent],CentHig[icent],icut) );
				}
			}
			else                             //add the others
			{
				H3D_Mc_PtEtaPhi_Cent[icent]           ->Add(H3D_Mc_PtEtaPhi_Cent_tem[isubcent]);
				
				for(int icut=0; icut<nCuts; icut++)
				{
					H3D_Rc_PtEtaPhi_Cent[icent][icut] ->Add(H3D_Rc_PtEtaPhi_Cent_tem[isubcent][icut]);
				}
			}
			//----------------------------------------------------------------------------------------------------------------------------------------------
			//----------------------------------------------------------------------------------------------------------------------------------------------
			
			//----------------------------------------------------------------------------------------------------------------------------------------------
			H3D_Mc_PtEtaPhi_Cent_tem[isubcent]           -> Delete();

			for(int icut=0; icut<nCuts; icut++)
			{
				H3D_Rc_PtEtaPhi_Cent_tem[isubcent][icut] ->Delete();
			}
			//----------------------------------------------------------------------------------------------------------------------------------------------
		}//isubcent

		//----------------------------------------------------------------------------------
		//----------------------------------------------------------------------------------
	}//icent for analysis

	
	//calculate the efficiency
	for(Int_t icent=0; icent<nCentBins; icent++)
	{
		//H3D_Mc_PtEtaPhi_Cent[icent]->Sumw2();

		H1D_McPt_AllEtaPhi_Cent[icent] = (TH1D*)H3D_Mc_PtEtaPhi_Cent[icent]->ProjectionX( Form("H1D_McPt_AllEtaPhi_icent%d", icent));
		H1D_McEta_AllPhiPt_Cent[icent] = (TH1D*)H3D_Mc_PtEtaPhi_Cent[icent]->ProjectionY( Form("H1D_McEta_AllPhiPt_icent%d", icent));
		H1D_McPhi_AllPtEta_Cent[icent] = (TH1D*)H3D_Mc_PtEtaPhi_Cent[icent]->ProjectionZ( Form("H1D_McPhi_AllPtEta_icent%d", icent));

		//calculate efficiency with all eta and phi integrated
		for(int icut=0; icut<nCuts; icut++)
		{
			//H3D_Rc_PtEtaPhi_Cent[icent][icut]->Sumw2();
			H1D_RcPt_AllEtaPhi_Cent[icent][icut]    = (TH1D*)H3D_Rc_PtEtaPhi_Cent[icent][icut]   ->ProjectionX( Form("H1D_RcPt_AllEtaPhi_icent%d_icut%d",    icent, icut) );
			H1D_EffVsPt_AllEtaPhi_Cent[icent][icut] = (TH1D*)H1D_RcPt_AllEtaPhi_Cent[icent][icut]->Clone(       Form("H1D_EffVsPt_AllEtaPhi_icent%d_icut%d", icent, icut) );
			H1D_EffVsPt_AllEtaPhi_Cent[icent][icut] -> Reset();
			H1D_EffVsPt_AllEtaPhi_Cent[icent][icut] -> Divide( H1D_RcPt_AllEtaPhi_Cent[icent][icut], H1D_McPt_AllEtaPhi_Cent[icent], 1., 1., "B" );
		
			H1D_RcEta_AllPhiPt_Cent[icent][icut]    = (TH1D*)H3D_Rc_PtEtaPhi_Cent[icent][icut]   ->ProjectionY( Form("H1D_RcEta_AllPhiPt_icent%d_icut%d",    icent, icut) );
			H1D_EffVsEta_AllPhiPt_Cent[icent][icut] = (TH1D*)H1D_RcEta_AllPhiPt_Cent[icent][icut]->Clone(       Form("H1D_EffVsEta_AllPhiPt_icent%d_icut%d", icent, icut) );
			H1D_EffVsEta_AllPhiPt_Cent[icent][icut] -> Reset();
			H1D_EffVsEta_AllPhiPt_Cent[icent][icut] -> Divide( H1D_RcEta_AllPhiPt_Cent[icent][icut], H1D_McEta_AllPhiPt_Cent[icent], 1., 1., "B" );

			H1D_RcPhi_AllPtEta_Cent[icent][icut]    = (TH1D*)H3D_Rc_PtEtaPhi_Cent[icent][icut]   ->ProjectionZ( Form("H1D_RcPhi_AllPtEta_icent%d_icut%d",    icent, icut) );
			H1D_EffVsPhi_AllPtEta_Cent[icent][icut] = (TH1D*)H1D_RcPhi_AllPtEta_Cent[icent][icut]->Clone(       Form("H1D_EffVsPhi_AllPtEta_icent%d_icut%d", icent, icut) );
			H1D_EffVsPhi_AllPtEta_Cent[icent][icut] -> Reset();
			H1D_EffVsPhi_AllPtEta_Cent[icent][icut] -> Divide( H1D_RcPhi_AllPtEta_Cent[icent][icut], H1D_McPhi_AllPtEta_Cent[icent], 1., 1., "B" );
		}


		if(flag_save_png==1) continue;



		for(int ieta=0; ieta<nEtaBins; ieta++)
		{
			double ieta_low = -1.0+(2.0)/nEtaBins*ieta;
			double ieta_hig = -1.0+(2.0)/nEtaBins*(ieta+1);
			
			int ieta_lowbin = H3D_Mc_PtEtaPhi_Cent[icent]->GetYaxis()->FindBin(ieta_low);
			int ieta_higbin = H3D_Mc_PtEtaPhi_Cent[icent]->GetYaxis()->FindBin(ieta_hig);
			cout<<"------------------------"<<endl;
			//cout<<"ieta_low: "<<ieta_low<<" ieta_hig: "<<ieta_hig<<endl;
			cout<<"------------------------"<<endl;
			for(int iphi=0; iphi<nPhiBins; iphi++)
			{
				double iphi_low = -PI+PI/12+(2.0*PI)/nPhiBins*iphi;
				double iphi_hig = -PI+PI/12+(2.0*PI)/nPhiBins*(iphi+1);
				//cout<<"iphi_low: "<<iphi_low<<" iphi_hig: "<<iphi_hig<<endl;
				int iphi_lowbin = H3D_Mc_PtEtaPhi_Cent[icent]->GetZaxis()->FindBin(iphi_low);
				int iphi_higbin = H3D_Mc_PtEtaPhi_Cent[icent]->GetZaxis()->FindBin(iphi_hig);

				//projection pt axis
				//TH1D* H1D_McPt_perEtaPhiBin_Cent[nCentBins][nEtaBins][nPhiBins];
				//TH1D* H1D_RcPt_perEtaPhiBin_Cent[nCentBins][nCuts][nEtaBins][nPhiBins];
				H1D_McPt_perEtaPhiBin_Cent[icent][ieta][iphi] 
					= (TH1D*)H3D_Mc_PtEtaPhi_Cent[icent]->ProjectionX( Form("H1D_McPt_icent%d_ieta%d_iphi%d", icent, ieta, iphi), ieta_lowbin, ieta_higbin, iphi_lowbin, iphi_higbin);
				
				H1D_McPt_perEtaPhiBin_Cent[icent][ieta][iphi]->Rebin(2);
				
				for(int icut=0; icut<nCuts; icut++)
				{
					H1D_RcPt_perEtaPhiBin_Cent[icent][icut][ieta][iphi] 
						= (TH1D*)H3D_Rc_PtEtaPhi_Cent[icent][icut]->ProjectionX( Form("H1D_RcPt_icent%d_icut%d_ieta%d_iphi%d", icent, icut, ieta, iphi), ieta_lowbin, ieta_higbin, iphi_lowbin, iphi_higbin);
					
					H1D_RcPt_perEtaPhiBin_Cent[icent][icut][ieta][iphi]->Rebin(2);
				
					//take ratio: Rc/Mc for efficiency
					H1D_EffVsPt_perEtaPhiBin_Cent[icent][icut][ieta][iphi] = (TH1D*)H1D_RcPt_perEtaPhiBin_Cent[icent][icut][ieta][iphi]->Clone(Form("H1D_EffVsPt_icent%d_icut%d_ieta%d_iphi%d", icent, icut, ieta, iphi));
					H1D_EffVsPt_perEtaPhiBin_Cent[icent][icut][ieta][iphi] -> Reset();
					//hPhoEPlus4TofEffvsEta[icent] ->Divide(hNum_PhoEPlusvsEta[icent], hDen_PhoEPlusvsEta[icent], 1, 1, "B");
					
					H1D_EffVsPt_perEtaPhiBin_Cent[icent][icut][ieta][iphi] -> Divide( H1D_RcPt_perEtaPhiBin_Cent[icent][icut][ieta][iphi],H1D_McPt_perEtaPhiBin_Cent[icent][ieta][iphi], 1., 1., "B");
				}//icut
			}//iphi
		}//ieta
	}//icent
}

//==================================================================================================================
//==================================================================================================================
void drawEff(TPDF* mypdf)
{
	TCanvas *c1 = new TCanvas("c1", "c1",0,0,800,600);
	//c1->SetCanvasSize(1250,750);
	
	TLegend *leg = new TLegend(0.40, 0.15, 0.85, 0.55);
	leg->SetFillColor(0);
	leg->SetBorderSize(0);
	leg->SetTextFont(50);

	const int Markers[nCuts] = {20,21, 22, 23, 34};
	const int Colors[nCuts]  = {12, 4,  3,  2, 1 };
	
	c1->cd();
	updatePDF(c1,mypdf);
	c1->cd();
	for(Int_t icent=0; icent<nCentBins; icent++)
	{
		cout<<"compare tracking eff vs Pt between different cuts, for centrality: "<<icent<<endl;
		for(int icut=0; icut<nCuts; icut++)
		{
			cout<<"draw plots: icent= "<<icent<<" icut: "<<icut<<endl;

			H1D_EffVsPt_AllEtaPhi_Cent[icent][icut]->SetTitle( Form("Centrality: %d-%d%%", CentLow[icent],CentHig[icent]) );
			H1D_EffVsPt_AllEtaPhi_Cent[icent][icut]->SetMarkerStyle(Markers[icut]);
			H1D_EffVsPt_AllEtaPhi_Cent[icent][icut]->SetMarkerColor(Colors[icut]);
			H1D_EffVsPt_AllEtaPhi_Cent[icent][icut]->GetXaxis()->SetRangeUser(0., 4.5);
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

		if(flag_save_png==1) c1->SaveAs(Form("plots4notes/H1D_EffVsPt_AllEtaPhi_icent%d.png",icent));

		updatePDF(c1,mypdf);
		
		leg->Clear();

	}//icent

	updatePDF(c1,mypdf);
	
	c1->Clear();
	c1->cd();

	cout<<"compare tracking eff. vs. Pt between centralities: "<<endl;
	//compare the final tracking efficiency between different centralities
	//for(Int_t icent=0; icent<nCentBins; icent++)
	for(Int_t icent=0; icent<nCentBins-2; icent++)
	{
		cout<<"draw plots: icent= "<<icent<<endl;
		H1D_EffVsPt_AllEtaPhi_Cent[icent][4]->SetMarkerStyle(20);
		H1D_EffVsPt_AllEtaPhi_Cent[icent][4]->SetMarkerSize(0.5);
		H1D_EffVsPt_AllEtaPhi_Cent[icent][4]->SetMarkerColor(1+icent);
		
		if(icent==0)
		{
			H1D_EffVsPt_AllEtaPhi_Cent[icent][4]->SetTitle("");
			H1D_EffVsPt_AllEtaPhi_Cent[icent][4]->GetXaxis()->SetRangeUser(0., 4.5);
			H1D_EffVsPt_AllEtaPhi_Cent[icent][4]->GetYaxis()->SetTitle("TPC Efficiency");
			H1D_EffVsPt_AllEtaPhi_Cent[icent][4]->GetYaxis()->SetRangeUser(0.4, 1.0);
			H1D_EffVsPt_AllEtaPhi_Cent[icent][4]->GetYaxis()->SetTitleSize(0.055);
			H1D_EffVsPt_AllEtaPhi_Cent[icent][4]->GetYaxis()->SetTitleOffset(0.80);
			H1D_EffVsPt_AllEtaPhi_Cent[icent][4]->GetYaxis()->CenterTitle(kTRUE);
			H1D_EffVsPt_AllEtaPhi_Cent[icent][4]->Draw("pe");
		}
		else H1D_EffVsPt_AllEtaPhi_Cent[icent][4]->Draw("pesame");

		cout<<"content: "<<H1D_EffVsPt_AllEtaPhi_Cent[icent][4]->GetBinContent(10)<<endl;
		cout<<"error:   "<<H1D_EffVsPt_AllEtaPhi_Cent[icent][4]->GetBinError(10)<<endl;

		leg->AddEntry(H1D_EffVsPt_AllEtaPhi_Cent[icent][4], Form("Centrality: %d-%d%%", CentLow[icent],CentHig[icent]), "lp");
	}//icent
	
	leg->Draw("same");
	
	if(flag_save_png==1) c1->SaveAs("plots4notes/H1D_EffVsPt_btwCents.png");
	
	c1->cd();
	updatePDF(c1,mypdf);
	leg->Clear();
	c1->Clear();
	//----------------------------------------------------------------------------------------------------
	//eff vs Eta 
	//----------------------------------------------------------------------------------------------------
	c1->cd();
	for(Int_t icent=0; icent<nCentBins; icent++)
	{
		cout<<"compare tracking eff vsPhi between different cuts, for centrality: "<<icent<<endl;
		for(int icut=0; icut<nCuts; icut++)
		{
			cout<<"draw plots: icent= "<<icent<<" icut: "<<icut<<endl;

			H1D_EffVsEta_AllPhiPt_Cent[icent][icut]->SetTitle( Form("Centrality: %d-%d%%", CentLow[icent],CentHig[icent]) );
			H1D_EffVsEta_AllPhiPt_Cent[icent][icut]->SetMarkerStyle(Markers[icut]);
			H1D_EffVsEta_AllPhiPt_Cent[icent][icut]->SetMarkerColor(Colors[icut]);
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

		if(flag_save_png==1) c1->SaveAs(Form("plots4notes/H1D_EffVsEta_AllPhiPt_icent%d.png",icent));

		updatePDF(c1,mypdf);
		
		leg->Clear();

	}//icent

	updatePDF(c1,mypdf);
	
	c1->Clear();
	c1->cd();


	//----------------------------------------------------------------------------------------------------
	cout<<"compare tracking eff. vs. Eta between centralities: "<<endl;
	//----------------------------------------------------------------------------------------------------
	//compare the final tracking efficiency between different centralities
	for(Int_t icent=0; icent<nCentBins; icent++)
	{
		cout<<"draw plots: icent= "<<icent<<endl;
		H1D_EffVsEta_AllPhiPt_Cent[icent][4]->SetMarkerStyle(20);
		H1D_EffVsEta_AllPhiPt_Cent[icent][4]->SetMarkerColor(1+icent);
		
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

		cout<<"content: "<<H1D_EffVsEta_AllPhiPt_Cent[icent][4]->GetBinContent(10)<<endl;
		cout<<"error:   "<<H1D_EffVsEta_AllPhiPt_Cent[icent][4]->GetBinError(10)<<endl;

		leg->AddEntry(H1D_EffVsEta_AllPhiPt_Cent[icent][4], Form("Centrality: %d-%d%%", CentLow[icent],CentHig[icent]), "lp");
	}//icent
	
	leg->Draw("same");


	if(flag_save_png==1) c1->SaveAs("plots4notes/H1D_EffVsEta_btwCents.png");

	updatePDF(c1,mypdf);
	c1->Clear();
	leg->Clear();
	updatePDF(c1,mypdf);
	//----------------------------------------------------------------------------------------------------
	//----------------------------------------------------------------------------------------------------

	//----------------------------------------------------------------------------------------------------
	//eff vs Phi 
	//----------------------------------------------------------------------------------------------------
	leg->SetX1NDC(0.50);
	leg->SetX2NDC(0.89);
	leg->SetY1NDC(0.12);
	leg->SetY2NDC(0.30);
	
	c1->cd();
	for(Int_t icent=0; icent<nCentBins; icent++)
	{
		cout<<"compare tracking eff vsPhi between different cuts, for centrality: "<<icent<<endl;
		for(int icut=0; icut<nCuts; icut++)
		{
			cout<<"draw plots: icent= "<<icent<<" icut: "<<icut<<endl;

			H1D_EffVsPhi_AllPtEta_Cent[icent][icut]->SetTitle( Form("Centrality: %d-%d%%", CentLow[icent],CentHig[icent]) );
			H1D_EffVsPhi_AllPtEta_Cent[icent][icut]->SetMarkerStyle(Markers[icut]);
			H1D_EffVsPhi_AllPtEta_Cent[icent][icut]->SetMarkerColor(Colors[icut]);
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

		if(flag_save_png==1) c1->SaveAs(Form("plots4notes/H1D_EffVsPhi_AllPtEta_icent%d.png",icent));

		updatePDF(c1,mypdf);
		
		leg->Clear();

	}//icent

	updatePDF(c1,mypdf);
	
	c1->Clear();
	c1->cd();

	//----------------------------------------------------------------------------------------------------
	cout<<"compare tracking eff. vs. Phi between centralities: "<<endl;
	//----------------------------------------------------------------------------------------------------
	
	//compare the final tracking efficiency between different centralities
	for(Int_t icent=0; icent<nCentBins; icent++)
	{
		cout<<"draw plots: icent= "<<icent<<endl;
		H1D_EffVsPhi_AllPtEta_Cent[icent][4]->SetMarkerStyle(20);
		H1D_EffVsPhi_AllPtEta_Cent[icent][4]->SetMarkerColor(1+icent);
		
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

		cout<<"content: "<<H1D_EffVsPhi_AllPtEta_Cent[icent][4]->GetBinContent(10)<<endl;
		cout<<"error:   "<<H1D_EffVsPhi_AllPtEta_Cent[icent][4]->GetBinError(10)<<endl;

		leg->AddEntry(H1D_EffVsPhi_AllPtEta_Cent[icent][4], Form("Centrality: %d-%d%%", CentLow[icent],CentHig[icent]), "lp");
	}//icent
	
	leg->Draw("same");

	if(flag_save_png==1) c1->SaveAs("plots4notes/H1D_EffVsPhi_btwCents.png");

	updatePDF(c1,mypdf);
	leg->Clear();
	c1->Clear();


	if(flag_save_png==1) return;

	c1->cd();
	//----------------------------------------------------------------------------------------------------
	//----------------------------------------------------------------------------------------------------
	updatePDF(c1,mypdf);
	leg->Clear();
	c1->Clear();
	updatePDF(c1,mypdf);

	c1->cd();
	drawLatex(0.20, 0.70, "Scan electron and positron ",  10, 0.05, 1);
	drawLatex(0.20, 0.65, "TPC Tracking Efficiency",      10, 0.05, 1);
	drawLatex(0.20, 0.60, "in Different Eta and Phi Bins",10, 0.05, 1);
	updatePDF(c1,mypdf);

	Int_t nColumns = 5;
	Int_t nRows    = 3;
	Int_t nPads    = nColumns*nRows;
	
	c1->Clear();
	
	c1->Divide(nColumns,nRows);
	
	for(Int_t i=0; i<nColumns*nRows; i++)
	{
		c1->cd(i+1);
		setPad(0.1,0.02,0.08,0.1);
	}

	leg->Clear();
	leg->SetX1NDC(0.50);
	leg->SetY1NDC(0.25);
	leg->SetX2NDC(0.80);
	leg->SetY2NDC(0.45);
	leg->SetTextSize(0.08);
	leg->SetNColumns(1);

	for(Int_t icent=0; icent<nCentBins; icent++)
	{
		cout<<"-------------------------------------------------------------------------------------------------------"<<endl;

		for(Int_t iEta=0; iEta<nEtaBins; iEta++)
		{
			double iEta_low = -1.0+(2.0)/nEtaBins*iEta;
			double iEta_hig = -1.0+(2.0)/nEtaBins*(iEta+1);
			
			for(Int_t iPhi=0; iPhi<nPhiBins; iPhi++)
			{
				double iPhi_low = -PI+PI/12+(2.0*PI)/nPhiBins*iPhi;
				double iPhi_hig = -PI+PI/12+(2.0*PI)/nPhiBins*(iPhi+1);
				
				Int_t idx = iEta*nPhiBins+iPhi;
				c1->cd(idx%nPads+1);

				//H1D_EffVsPt_perEtaPhiBin_Cent[icent][icut][ieta][iphi]
				H1D_EffVsPt_perEtaPhiBin_Cent[icent][4][iEta][iPhi]  ->SetTitle(Form("%d-%d%%,%.2f<#eta<%.2f,%.2f<#phi<%.2f", CentLow[icent],CentHig[icent],iEta_low,iEta_hig,iPhi_low,iPhi_hig));
				H1D_EffVsPt_perEtaPhiBin_Cent[icent][4][iEta][iPhi]  ->GetXaxis()->SetRangeUser(0.,4.5);
				H1D_EffVsPt_perEtaPhiBin_Cent[icent][4][iEta][iPhi]  ->GetYaxis()->SetRangeUser(0.,1.1);
				H1D_EffVsPt_perEtaPhiBin_Cent[icent][4][iEta][iPhi]  ->GetYaxis()->SetTitleSize(0.055);
				H1D_EffVsPt_perEtaPhiBin_Cent[icent][4][iEta][iPhi]  ->GetYaxis()->SetTitleOffset(0.87);
				H1D_EffVsPt_perEtaPhiBin_Cent[icent][4][iEta][iPhi]  ->GetYaxis()->SetTitle("TPC Tracking Efficiency");
				H1D_EffVsPt_perEtaPhiBin_Cent[icent][4][iEta][iPhi]  ->GetYaxis()->CenterTitle(kTRUE);
				H1D_EffVsPt_perEtaPhiBin_Cent[icent][4][iEta][iPhi]  ->Draw("pe");

				leg->Clear();
				leg->AddEntry( H1D_EffVsPt_perEtaPhiBin_Cent[icent][4][iEta][iPhi],  "Electron", "p" );
				//leg->AddEntry( hPiMinusEff3D[icent][iEta][iPhi], "#pi^{-}", "p" );
				leg->Draw("same");

				//H1D_EffVsPt_perEtaPhiBin_Cent[icent][4][iEta][iPhi]  ->Draw("pesame");
				//hPiMinusEff3D[icent][iEta][iPhi] ->Draw("pesame");

				//drawLatex(0.15, 0.85, Form("%4.2f <#eta< %4.2f",  iEta_low, iEta_hig),  42, 0.07, 1);
				//drawLatex(0.15, 0.78, Form("%4.3f <#phi< %4.3f",  iPhi_low, iPhi_hig),  42, 0.07, 1);
				//drawLatex(0.30, 0.95, Form("Centrality: %d-%d%%", CentLow[icent],CentHig[icent]), 42, 0.06, 1);

				if( idx%nPads == nPads-1 ) updatePDF(c1,mypdf);
			}//iPhi
		}//iEta
		if((nEtaBins*nPhiBins)%nPads!=0) updatePDF(c1,mypdf);
	}//icent
	
	updatePDF(c1,mypdf);

	delete c1;
	delete leg;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------------------------------------------------------------
void saveFiles()
{
	//TFile * outf = new TFile(outDir+"/outhist_"+partType+"_TpcTrackingEff_Run18_27GeV_AuAu.root", "recreate");
	TFile * outf = new TFile(outDir+"/outhist_"+partType+"_TpcTrackingEff_Run18_27GeV_AuAu_Vz35cm.root", "recreate");
	//TFile * outf = new TFile(outDir+"/outhist_"+partType+"_TpcTrackingEff_Run18_27GeV_AuAu_wNEfracWT_Vz35cm.root", "recreate");
	cout<<" tracking efficiency files are saved into: "<<outf->GetName()<<endl;
	
	outf->cd();
	for(Int_t icent=0; icent<nCentBins; icent++)
	{
		for(Int_t iEta=0; iEta<nEtaBins; iEta++)
		{
			double iEta_low = -1.0+(2.0)/nEtaBins*iEta;
			double iEta_hig = -1.0+(2.0)/nEtaBins*(iEta+1);

			for(Int_t iPhi=0; iPhi<nPhiBins; iPhi++)
			{
				double iPhi_low = -PI+PI/12+(2.0*PI)/nPhiBins*iPhi;
				double iPhi_hig = -PI+PI/12+(2.0*PI)/nPhiBins*(iPhi+1);
				
				cout<<"write down tracking eff. for: "<<endl;
				cout<< Form("%d-%d%%,%.2f<#eta<%.2f,%.2f<#phi<%.2f", CentLow[icent],CentHig[icent],iEta_low,iEta_hig,iPhi_low,iPhi_hig)  <<endl;

				H1D_EffVsPt_perEtaPhiBin_Cent[icent][4][iEta][iPhi]  ->Write();
			}//iPhi
		}//iEta
	}//icent
	
	outf->Close();

	//	TFile * outf2 = new TFile(outDir+"/outhist_"+partType+"_TpcTrackingEff4comp_Run18_27GeV_AuAu_wNEfracWT_Vz35cm.root", "recreate");
	TFile * outf2 = new TFile(outDir+"/outhist_"+partType+"_TpcTrackingEff4comp_Run18_27GeV_AuAu_Vz35cm.root", "recreate");
	cout<<" tracking efficiency files are saved into: "<<outf2->GetName()<<"for comp w/wo nEfracWT"<<endl;
	
	outf2->cd();

	for(Int_t icent=0; icent<nCentBins-2; icent++)
	{
		cout<<"draw plots: icent= "<<icent<<endl;
		H1D_EffVsPt_AllEtaPhi_Cent[icent][4]->Write();
	}//icent
	
	outf2->Close();
}


