//---------------------OO.root----------------------
#include "../2021_OO200/someFunction.h"
#include <iomanip>
void Recalibrate(TString inFileName = "roots/3_20260823_dAu2021_TOF.root", Int_t number = 3)//27_20260201_iTPC_PureEPi_Cen8_reW_P24iy;20260202_iTPCall_corrE_etaPhiPt;
{
	// root
	TFile *inFile = new TFile(inFileName);
	if (!inFile) { cout << "The input file is not found! Exiting..." << endl; return; }
	//from PureE
	TH3F* h_Pt_Cen_nSigmaE  = (TH3F*)inFile->Get("h_Pt_Cen_nSigmaE");RebinXCen(h_Pt_Cen_nSigmaE);
	TH3F* h_Eta_Cen_nSigmaE = (TH3F*)inFile->Get("h_Eta_Cen_nSigmaE");RebinXCen(h_Eta_Cen_nSigmaE);
	TH3F* h_Phi_Cen_nSigmaE = (TH3F*)inFile->Get("h_Phi_Cen_nSigmaE");RebinXCen(h_Phi_Cen_nSigmaE);
	TH3F* h_Pt_Cen_nSigmaEcorr = (TH3F*)inFile->Get("h_Pt_Cen_nSigmaEcorr"); RebinXCen(h_Pt_Cen_nSigmaEcorr);
	TH3F* h_Eta_Cen_nSigmaEcorr = (TH3F*)inFile->Get("h_Eta_Cen_nSigmaEcorr"); RebinXCen(h_Eta_Cen_nSigmaEcorr);
	TH3F* h_Phi_Cen_nSigmaEcorr = (TH3F*)inFile->Get("h_Phi_Cen_nSigmaEcorr"); RebinXCen(h_Phi_Cen_nSigmaEcorr);
	if (!h_Pt_Cen_nSigmaE || !h_Eta_Cen_nSigmaE || !h_Phi_Cen_nSigmaE) { cout << "Some histograms are not found! Exiting..." << endl; return; }

	//Pt
	TH2F* h2_zmean_binCount_Pt;
	TH2F* h2_zmean_gausFit_Pt;
	TH1F* h1_xmean_cen__binCount_Pt;
	TH1F* h1_xmean_cen__gausFit_Pt;
	//Eta
	TH2F* h2_zmean_binCount_Eta;
	TH2F* h2_zmean_gausFit_Eta;
	TH1F* h1_xmean_cen__binCount_Eta;
	TH1F* h1_xmean_cen__gausFit_Eta;
	//Phi
	TH2F* h2_zmean_binCount_Phi;
	TH2F* h2_zmean_gausFit_Phi;
	TH1F* h1_xmean_cen__binCount_Phi;
	TH1F* h1_xmean_cen__gausFit_Phi;
	if (0) // 输出修正量数组
	{
		//Pt
		h2_zmean_binCount_Pt = (TH2F*)MeanOfH3D_binCount(h_Pt_Cen_nSigmaE, "h2_zmean_binCount_Pt");
		h2_zmean_gausFit_Pt = (TH2F*)MeanOfH3D_gausFit(h_Pt_Cen_nSigmaE, "h2_zmean_gausFit_Pt");
		h1_xmean_cen__binCount_Pt = (TH1F*)Meanof2DAlongX(h2_zmean_binCount_Pt, "h1_xmean_cen__binCount_Pt");
		h1_xmean_cen__gausFit_Pt = (TH1F*)Meanof2DAlongX(h2_zmean_gausFit_Pt, "h1_xmean_cen__gausFit_Pt");
		//Eta
		h2_zmean_binCount_Eta = (TH2F*)MeanOfH3D_binCount(h_Eta_Cen_nSigmaE, "h2_zmean_binCount_Eta");
		h2_zmean_gausFit_Eta = (TH2F*)MeanOfH3D_gausFit(h_Eta_Cen_nSigmaE, "h2_zmean_gausFit_Eta");
		h1_xmean_cen__binCount_Eta = (TH1F*)Meanof2DAlongX(h2_zmean_binCount_Eta, "h1_xmean_cen__binCount_Eta");
		h1_xmean_cen__gausFit_Eta = (TH1F*)Meanof2DAlongX(h2_zmean_gausFit_Eta, "h1_xmean_cen__gausFit_Eta");

		//Phi
		h2_zmean_binCount_Phi = (TH2F*)MeanOfH3D_binCount(h_Phi_Cen_nSigmaE, "h2_zmean_binCount_Phi");
		h2_zmean_gausFit_Phi = (TH2F*)MeanOfH3D_gausFit(h_Phi_Cen_nSigmaE, "h2_zmean_gausFit_Phi");
		h1_xmean_cen__binCount_Phi = (TH1F*)Meanof2DAlongX(h2_zmean_binCount_Phi, "h1_xmean_cen__binCount_Phi");
		h1_xmean_cen__gausFit_Phi = (TH1F*)Meanof2DAlongX(h2_zmean_gausFit_Phi, "h1_xmean_cen__gausFit_Phi");
		//2D-eta-cen
		std::cout << "mean[cen][eta]:" << endl;
		for (int iy = 1; iy <= h2_zmean_gausFit_Eta->GetNbinsY(); iy++)//h2_zmean_gausFit_Eta;
		{
			std::cout << "{";
			for (int ix = 1; ix <= h2_zmean_gausFit_Eta->GetNbinsX(); ix++)
			{
				float content = h2_zmean_gausFit_Eta->GetBinContent(ix, iy);
				std::cout << std::fixed << std::setprecision(3);
				std::cout << content << ',';
			}
			std::cout << "}," << endl;
		}
		//2D-phi-cen
		std::cout << "mean[cen][phi]:" << endl;
		for (int iy = 1; iy <= h2_zmean_gausFit_Phi->GetNbinsY(); iy++)//h2_zmean_gausFit_Phi;
		{
			std::cout << "{";
			for (int ix = 1; ix <= h2_zmean_gausFit_Phi->GetNbinsX(); ix++)
			{
				float content = h2_zmean_gausFit_Phi->GetBinContent(ix, iy);
				std::cout << std::fixed << std::setprecision(3);
				std::cout << content << ',';
			}
			std::cout << "}," << endl;
		}
		//1D-phi-cen
		std::cout << "phimean[cen]:" << endl;
		for (int ix = 1; ix <= h1_xmean_cen__gausFit_Phi->GetNbinsX(); ix++)
		{
			float content = h1_xmean_cen__gausFit_Phi->GetBinContent(ix);
			std::cout << content << "," << endl;
			if(ix== h1_xmean_cen__gausFit_Phi->GetNbinsX())cout<<endl;
		}
		}
	if (0) //nsigma_e vs pT
	{
		TCanvas *c_Pt = new TCanvas("c_Pt", "c_Pt", 1400, 600);
		c_Pt->Divide(4, 2);
		c_Pt->cd(1);
		gPad->SetLeftMargin(0.12);
		gPad->SetRightMargin(0.12);
		gStyle->SetOptStat(0);
		h2_zmean_binCount_Pt->SetMaximum(1);
		h2_zmean_binCount_Pt->SetMinimum(-1.0);
		h2_zmean_binCount_Pt->DrawClone("col z");

		c_Pt->cd(2);
		gPad->SetLeftMargin(0.12);
		gPad->SetRightMargin(0.12);
		gStyle->SetOptStat(0);
		h2_zmean_gausFit_Pt->SetMaximum(1);
		h2_zmean_gausFit_Pt->SetMinimum(-1.0);
		h2_zmean_gausFit_Pt->DrawClone("col z");

		c_Pt->cd(3);
		//gPad->SetLogz(1);
		gPad->SetLeftMargin(0.12);
		gPad->SetRightMargin(0.12);
		gStyle->SetOptStat(0);
		h1_xmean_cen__binCount_Pt->DrawClone("");

		c_Pt->cd(4);
		//gPad->SetLogz(1);
		gPad->SetLeftMargin(0.12);
		gPad->SetRightMargin(0.12);
		gStyle->SetOptStat(0);
		h1_xmean_cen__gausFit_Pt->DrawClone("");

		c_Pt->cd(5);
		gPad->SetLeftMargin(0.12);
		gPad->SetRightMargin(0.12);
		gStyle->SetOptStat(0);

		TLegend *leg_E = new TLegend(0.7, 0.6, 0.9, 0.9);
		leg_E->SetBorderSize(1);
		leg_E->SetFillColor(0);
		TH1F* proj1_bC = (TH1F*)h2_zmean_binCount_Pt->ProjectionX("_1", 1, 1);
		proj1_bC->SetLineColor(1);
		proj1_bC->DrawClone();
		leg_E->AddEntry(proj1_bC, "70~80%");
		for (int iy = 2; iy <= h2_zmean_binCount_Pt->GetYaxis()->GetNbins(); iy++)
		{
			TH1F* proj_bC = (TH1F*)h2_zmean_binCount_Pt->ProjectionX(Form("_%d", iy), iy, iy);
			proj_bC->SetLineColor(iy);
			leg_E->AddEntry(proj_bC, Form("%d~%d%", 80 - 10 * iy, 90 - 10 * iy), "l");
			proj_bC->DrawClone("same");
		}
		leg_E->DrawClone("same");

		c_Pt->cd(6);
		gPad->SetLeftMargin(0.12);
		gPad->SetRightMargin(0.12);
		gStyle->SetOptStat(0);

		TLegend *leg_Ecorr = new TLegend(0.7, 0.6, 0.9, 0.9);
		leg_Ecorr->SetBorderSize(1);
		leg_Ecorr->SetFillColor(0);
		TH1F* proj1_gF = (TH1F*)h2_zmean_gausFit_Pt->ProjectionX("_1", 1, 1);
		proj1_gF->SetLineColor(1);
		proj1_gF->DrawClone();
		leg_Ecorr->AddEntry(proj1_gF, "70~80%");
		for (int iy = 2; iy <= h2_zmean_gausFit_Pt->GetYaxis()->GetNbins(); iy++)
		{
			TH1F* proj_gF = (TH1F*)h2_zmean_gausFit_Pt->ProjectionX(Form("_%d", iy), iy, iy);
			proj_gF->SetLineColor(iy);
			leg_Ecorr->AddEntry(proj_gF, Form("%d~%d%", 80 - 10 * iy, 90 - 10 * iy), "l");
			proj_gF->DrawClone("same");
		}
		leg_Ecorr->DrawClone("same");

		c_Pt->SaveAs(Form("roots/%d_nsigmaMean_Pt.png", number));
	}
	if (0) //nsigma_e vs Eta
	{
		TCanvas *c_Eta = new TCanvas("c_Eta", "c_Eta", 1400, 600);
		c_Eta->Divide(4, 2);
		c_Eta->cd(1);
		gPad->SetLeftMargin(0.12);
		gPad->SetRightMargin(0.12);
		gStyle->SetOptStat(0);
		h2_zmean_binCount_Eta->SetMaximum(1);
		h2_zmean_binCount_Eta->SetMinimum(-1.0);
		h2_zmean_binCount_Eta->DrawClone("col z");

		c_Eta->cd(2);
		gPad->SetLeftMargin(0.12);
		gPad->SetRightMargin(0.12);
		gStyle->SetOptStat(0);
		h2_zmean_gausFit_Eta->SetMaximum(1);
		h2_zmean_gausFit_Eta->SetMinimum(-1.0);
		h2_zmean_gausFit_Eta->DrawClone("col z");

		c_Eta->cd(3);
		//gPad->SetLogz(1);
		gPad->SetLeftMargin(0.12);
		gPad->SetRightMargin(0.12);
		gStyle->SetOptStat(0);
		h1_xmean_cen__binCount_Eta->DrawClone("");

		c_Eta->cd(4);
		//gPad->SetLogz(1);
		gPad->SetLeftMargin(0.12);
		gPad->SetRightMargin(0.12);
		gStyle->SetOptStat(0);
		h1_xmean_cen__gausFit_Eta->DrawClone("");

		c_Eta->cd(5);
		gPad->SetLeftMargin(0.12);
		gPad->SetRightMargin(0.12);
		gStyle->SetOptStat(0);

		TLegend *leg_E = new TLegend(0.7, 0.6, 0.9, 0.9);
		leg_E->SetBorderSize(1);
		leg_E->SetFillColor(0);
		TH1F* proj1_bC = (TH1F*)h2_zmean_binCount_Eta->ProjectionX("_1", 1, 1);
		proj1_bC->SetLineColor(1);
		proj1_bC->DrawClone();
		leg_E->AddEntry(proj1_bC, "70~80%");
		for (int iy = 2; iy <= h2_zmean_binCount_Eta->GetYaxis()->GetNbins(); iy++)
		{
			TH1F* proj_bC = (TH1F*)h2_zmean_binCount_Eta->ProjectionX(Form("_%d", iy), iy, iy);
			proj_bC->SetLineColor(iy);
			leg_E->AddEntry(proj_bC, Form("%d~%d%", 80-10*iy,90-10*iy), "l");
			proj_bC->DrawClone("same");
		}
		leg_E->DrawClone("same");

		c_Eta->cd(6);
		gPad->SetLeftMargin(0.12);
		gPad->SetRightMargin(0.12);
		gStyle->SetOptStat(0);

		TLegend *leg_Ecorr = new TLegend(0.7, 0.6, 0.9, 0.9);
		leg_Ecorr->SetBorderSize(1);
		leg_Ecorr->SetFillColor(0);
		TH1F* proj1_gF = (TH1F*)h2_zmean_gausFit_Eta->ProjectionX("_1", 1, 1);
		proj1_gF->SetLineColor(1);
		proj1_gF->DrawClone();
		leg_Ecorr->AddEntry(proj1_gF, "70~80%");
		for (int iy = 2; iy <= h2_zmean_gausFit_Eta->GetYaxis()->GetNbins(); iy++)
		{
			TH1F* proj_gF = (TH1F*)h2_zmean_gausFit_Eta->ProjectionX(Form("_%d", iy), iy, iy);
			proj_gF->SetLineColor(iy);
			leg_Ecorr->AddEntry(proj_gF, Form("%d~%d%", 80 - 10 * iy, 90 - 10 * iy), "l");
			proj_gF->DrawClone("same");
		}
		leg_Ecorr->DrawClone("same");


		c_Eta->SaveAs(Form("roots/%d_nsigmaMean_Eta.png", number));
	}
	if (0) //nsigma_e vs Phi
	{
		TCanvas *c_Phi = new TCanvas("c_Phi", "c_Phi", 1400, 600);
		c_Phi->Divide(4, 2);
		c_Phi->cd(1);
		gPad->SetLeftMargin(0.12);
		gPad->SetRightMargin(0.12);
		gStyle->SetOptStat(0);
		h2_zmean_binCount_Phi->SetMaximum(1);
		h2_zmean_binCount_Phi->SetMinimum(-1.0);
		h2_zmean_binCount_Phi->DrawClone("col z");

		c_Phi->cd(2);
		gPad->SetLeftMargin(0.12);
		gPad->SetRightMargin(0.12);
		gStyle->SetOptStat(0);
		h2_zmean_gausFit_Phi->SetMaximum(1);
		h2_zmean_gausFit_Phi->SetMinimum(-1.0);
		h2_zmean_gausFit_Phi->DrawClone("col z");

		c_Phi->cd(3);
		//gPad->SetLogz(1);
		gPad->SetLeftMargin(0.12);
		gPad->SetRightMargin(0.12);
		gStyle->SetOptStat(0);
		h1_xmean_cen__binCount_Phi->DrawClone("");

		c_Phi->cd(4);
		//gPad->SetLogz(1);
		gPad->SetLeftMargin(0.12);
		gPad->SetRightMargin(0.12);
		gStyle->SetOptStat(0);
		h1_xmean_cen__gausFit_Phi->DrawClone("");

		c_Phi->cd(5);
		gPad->SetLeftMargin(0.12);
		gPad->SetRightMargin(0.12);
		gStyle->SetOptStat(0);

		TLegend *leg_E = new TLegend(0.7, 0.6, 0.9, 0.9);
		leg_E->SetBorderSize(1);
		leg_E->SetFillColor(0);
		TH1F* proj1_bC = (TH1F*)h2_zmean_binCount_Phi->ProjectionX("_1", 1, 1);
		proj1_bC->SetLineColor(1);
		proj1_bC->DrawClone();
		leg_E->AddEntry(proj1_bC, "70~80%");
		for (int iy = 2; iy <= h2_zmean_binCount_Phi->GetYaxis()->GetNbins(); iy++)
		{
			TH1F* proj_bC = (TH1F*)h2_zmean_binCount_Phi->ProjectionX(Form("_%d", iy), iy, iy);
			proj_bC->SetLineColor(iy);
			leg_E->AddEntry(proj_bC, Form("%d~%d%", 80 - 10 * iy, 90 - 10 * iy), "l");
			proj_bC->DrawClone("same");
		}
		leg_E->DrawClone("same");

		c_Phi->cd(6);
		gPad->SetLeftMargin(0.12);
		gPad->SetRightMargin(0.12);
		gStyle->SetOptStat(0);

		TLegend *leg_Ecorr = new TLegend(0.7, 0.6, 0.9, 0.9);
		leg_Ecorr->SetBorderSize(1);
		leg_Ecorr->SetFillColor(0);
		TH1F* proj1_gF = (TH1F*)h2_zmean_gausFit_Phi->ProjectionX("_1", 1, 1);
		proj1_gF->SetLineColor(1);
		proj1_gF->DrawClone();
		leg_Ecorr->AddEntry(proj1_gF, "70~80%");
		for (int iy = 2; iy <= h2_zmean_gausFit_Phi->GetYaxis()->GetNbins(); iy++)
		{
			TH1F* proj_gF = (TH1F*)h2_zmean_gausFit_Phi->ProjectionX(Form("_%d", iy), iy, iy);
			proj_gF->SetLineColor(iy);
			leg_Ecorr->AddEntry(proj_gF, Form("%d~%d%", 80 - 10 * iy, 90 - 10 * iy), "l");
			proj_gF->DrawClone("same");
		}
		leg_Ecorr->DrawClone("same");

		c_Phi->SaveAs(Form("roots/%d_nsigmaMean_Phi.png", number));
	}	

	//Pt
	TH2F* h2_zmean_Pt_Cen__E = (TH2F*)MeanOfH3D_gausFit(h_Pt_Cen_nSigmaE, "h2_zmean_Pt_Cen__E");
	TH2F* h2_zmean_Pt_Cen__Ecorr = (TH2F*)MeanOfH3D_gausFit(h_Pt_Cen_nSigmaEcorr, "h2_zmean_Pt_Cen__Ecorr");
	TH1F* h1_xmean_cen__Pt_E = (TH1F*)Meanof2DAlongX(h2_zmean_Pt_Cen__E, "h1_xmean_cen__Pt_E");
	TH1F* h1_xmean_cen__Pt_Ecorr = (TH1F*)Meanof2DAlongX(h2_zmean_Pt_Cen__Ecorr, "h1_xmean_cen__Pt_Ecorr");
	//Eta
	TH2F* h2_zmean_Eta_Cen__E = (TH2F*)MeanOfH3D_gausFit(h_Eta_Cen_nSigmaE, "h2_zmean_Eta_Cen__E");
	TH2F* h2_zmean_Eta_Cen__Ecorr = (TH2F*)MeanOfH3D_gausFit(h_Eta_Cen_nSigmaEcorr, "h2_zmean_Eta_Cen__Ecorr");
	TH1F* h1_xmean_cen__Eta_E = (TH1F*)Meanof2DAlongX(h2_zmean_Eta_Cen__E, "h1_xmean_cen__Eta_E");
	TH1F* h1_xmean_cen__Eta_Ecorr = (TH1F*)Meanof2DAlongX(h2_zmean_Eta_Cen__Ecorr, "h1_xmean_cen__Eta_Ecorr");
	//Phi
	TH2F* h2_zmean_Phi_Cen__E = (TH2F*)MeanOfH3D_gausFit(h_Phi_Cen_nSigmaE, "h2_zmean_Phi_Cen__E");
	TH2F* h2_zmean_Phi_Cen__Ecorr = (TH2F*)MeanOfH3D_gausFit(h_Phi_Cen_nSigmaEcorr, "h2_zmean_Phi_Cen__Ecorr");
	TH1F* h1_xmean_cen__Phi_E = (TH1F*)Meanof2DAlongX(h2_zmean_Phi_Cen__E, "h1_xmean_cen__Phi_E");
	TH1F* h1_xmean_cen__Phi_Ecorr = (TH1F*)Meanof2DAlongX(h2_zmean_Phi_Cen__Ecorr, "h1_xmean_cen__Phi_Ecorr");

	// E and Ecorr
	if (0)//nsigma_e vs pT
	{
		TCanvas *c_Pt = new TCanvas("c_Pt", "c_Pt", 1100, 500);
		c_Pt->Divide(2);
		c_Pt->cd(1);
		gPad->SetLeftMargin(0.12);
		gPad->SetRightMargin(0.12);
		gStyle->SetOptStat(0);
		TLegend *leg_Ecorr = new TLegend(0.7, 0.6, 0.9, 0.9);
		leg_Ecorr->SetBorderSize(1);
		leg_Ecorr->SetFillColor(0);
		TH1F* proj1_Ecorr = (TH1F*)h2_zmean_Pt_Cen__Ecorr->ProjectionX("_1", 1, 1);
		proj1_Ecorr->SetTitle("Without Correction;p_{T};average of n#sigma_{e}");
		proj1_Ecorr->SetMaximum(1.5);
		proj1_Ecorr->SetMinimum(-1.5);
		proj1_Ecorr->SetLineColor(1);
		proj1_Ecorr->DrawClone();
		leg_Ecorr->AddEntry(proj1_Ecorr, "70~80%");
		for (int iy = 2; iy <= h2_zmean_Pt_Cen__Ecorr->GetYaxis()->GetNbins(); iy++)
		{
			TH1F* proj_Ecorr = (TH1F*)h2_zmean_Pt_Cen__Ecorr->ProjectionX(Form("_%d", iy), iy, iy);
			proj_Ecorr->SetLineColor(iy);
			leg_Ecorr->AddEntry(proj_Ecorr, Form("%d~%d%", 80 - 10 * iy, 90 - 10 * iy), "l");
			proj_Ecorr->DrawClone("same");
		}
		leg_Ecorr->DrawClone("same");

		c_Pt->cd(2);
		gPad->SetLeftMargin(0.12);
		gPad->SetRightMargin(0.12);
		gStyle->SetOptStat(0);
		TLegend *leg_E = new TLegend(0.7, 0.6, 0.9, 0.9);
		leg_E->SetBorderSize(1);
		leg_E->SetFillColor(0);
		TH1F* proj1_E = (TH1F*)h2_zmean_Pt_Cen__E->ProjectionX("_1", 1, 1);
		proj1_E->SetTitle("With Correction;p_{T};average of n#sigma_{e}");
		proj1_E->SetLineColor(1);
		proj1_E->SetMaximum(1.5);
		proj1_E->SetMinimum(-1.5);
		proj1_E->DrawClone();
		leg_E->AddEntry(proj1_E, "70~80%");
		for (int iy = 2; iy <= h2_zmean_Pt_Cen__E->GetYaxis()->GetNbins(); iy++)
		{
			TH1F* proj_E = (TH1F*)h2_zmean_Pt_Cen__E->ProjectionX(Form("_%d", iy), iy, iy);
			proj_E->SetLineColor(iy);
			leg_E->AddEntry(proj_E, Form("%d~%d%", 80 - 10 * iy, 90 - 10 * iy), "l");
			proj_E->DrawClone("same");
		}
		leg_E->DrawClone("same");


		c_Pt->SaveAs(Form("roots/%d_nsigmaMean_Pt.png", number));
	}
	if (1)//nsigma_e vs Eta
	{
		//��ͼ
		TCanvas *c_Eta = new TCanvas("c_Eta", "c_Eta", 1100, 500);
		c_Eta->Divide(2);
		c_Eta->cd(1);
		gPad->SetLeftMargin(0.12);
		gPad->SetRightMargin(0.12);
		gStyle->SetOptStat(0);
		TLegend *leg_Ecorr = new TLegend(0.7, 0.6, 0.9, 0.9);
		leg_Ecorr->SetBorderSize(1);
		leg_Ecorr->SetFillColor(0);
		TH1F* proj1_Ecorr = (TH1F*)h2_zmean_Eta_Cen__Ecorr->ProjectionX("_1", 1, 1);
		proj1_Ecorr->SetTitle("Without Correction;#eta;average of n#sigma_{e}");
		proj1_Ecorr->SetMaximum(1.5);
		proj1_Ecorr->SetMinimum(-1.5);
		proj1_Ecorr->SetLineColor(1);
		proj1_Ecorr->DrawClone();
		leg_Ecorr->AddEntry(proj1_Ecorr, "70~80%");
		for (int iy = 2; iy <= h2_zmean_Eta_Cen__Ecorr->GetYaxis()->GetNbins(); iy++)
		{
			TH1F* proj_Ecorr = (TH1F*)h2_zmean_Eta_Cen__Ecorr->ProjectionX(Form("_%d", iy), iy, iy);
			proj_Ecorr->SetLineColor(iy);
			leg_Ecorr->AddEntry(proj_Ecorr, Form("%d~%d%", 80 - 10 * iy, 90 - 10 * iy), "l");
			proj_Ecorr->DrawClone("same");
		}
		leg_Ecorr->DrawClone("same");

		c_Eta->cd(2);
		gPad->SetLeftMargin(0.12);
		gPad->SetRightMargin(0.12);
		gStyle->SetOptStat(0);
		TLegend *leg_E = new TLegend(0.7, 0.6, 0.9, 0.9);
		leg_E->SetBorderSize(1);
		leg_E->SetFillColor(0);
		TH1F* proj1_E = (TH1F*)h2_zmean_Eta_Cen__E->ProjectionX("_1", 1, 1);
		proj1_E->SetTitle("With Correction;#eta;average of n#sigma_{e}");
		proj1_E->SetMaximum(1.5);
		proj1_E->SetMinimum(-1.5);
		proj1_E->SetLineColor(1);
		proj1_E->DrawClone();
		leg_E->AddEntry(proj1_E, "70~80%");
		for (int iy = 2; iy <= h2_zmean_Eta_Cen__E->GetYaxis()->GetNbins(); iy++)
		{
			TH1F* proj_E = (TH1F*)h2_zmean_Eta_Cen__E->ProjectionX(Form("_%d", iy), iy, iy);
			proj_E->SetLineColor(iy);
			leg_E->AddEntry(proj_E, Form("%d~%d%", 80 - 10 * iy, 90 - 10 * iy), "l");
			proj_E->DrawClone("same");
		}
		leg_E->DrawClone("same");

		c_Eta->SaveAs(Form("roots/%d_nsigmaMean_Eta.png", number));
	}
	if (1)//nsigma_e vs Phi
	{
		TCanvas *c_Phi = new TCanvas("c_Phi", "c_Phi", 1100, 500);
		c_Phi->Divide(2);
		c_Phi->cd(1);
		gPad->SetLeftMargin(0.12);
		gPad->SetRightMargin(0.12);
		gStyle->SetOptStat(0);
		TLegend *leg_Ecorr = new TLegend(0.7, 0.6, 0.9, 0.9);
		leg_Ecorr->SetBorderSize(1);
		leg_Ecorr->SetFillColor(0);
		TH1F* proj1_Ecorr = (TH1F*)h2_zmean_Phi_Cen__Ecorr->ProjectionX("_1", 1, 1);
		proj1_Ecorr->SetTitle("Without Correction;#phi;average of n#sigma_{e}");
		proj1_Ecorr->SetMaximum(1.5);
		proj1_Ecorr->SetMinimum(-1.5);
		proj1_Ecorr->SetLineColor(1);
		proj1_Ecorr->DrawClone();
		leg_Ecorr->AddEntry(proj1_Ecorr, "70~80%");
		for (int iy = 2; iy <= h2_zmean_Phi_Cen__Ecorr->GetYaxis()->GetNbins(); iy++)
		{
			TH1F* proj_Ecorr = (TH1F*)h2_zmean_Phi_Cen__Ecorr->ProjectionX(Form("_%d", iy), iy, iy);
			proj_Ecorr->SetLineColor(iy);
			leg_Ecorr->AddEntry(proj_Ecorr, Form("%d~%d%", 80 - 10 * iy, 90 - 10 * iy), "l");
			proj_Ecorr->DrawClone("same");
		}
		leg_Ecorr->DrawClone("same");

		c_Phi->cd(2);
		gPad->SetLeftMargin(0.12);
		gPad->SetRightMargin(0.12);
		gStyle->SetOptStat(0);
		TLegend *leg_E = new TLegend(0.7, 0.6, 0.9, 0.9);
		leg_E->SetBorderSize(1);
		leg_E->SetFillColor(0);
		TH1F* proj1_E = (TH1F*)h2_zmean_Phi_Cen__E->ProjectionX("_1", 1, 1);
		proj1_E->SetTitle("With Correction;#phi;average of n#sigma_{e}");
		proj1_E->SetMaximum(1.5);
		proj1_E->SetMinimum(-1.5);
		proj1_E->SetLineColor(1);
		proj1_E->DrawClone();
		leg_E->AddEntry(proj1_E, "70~80%");
		for (int iy = 2; iy <= h2_zmean_Phi_Cen__E->GetYaxis()->GetNbins(); iy++)
		{
			TH1F* proj_E = (TH1F*)h2_zmean_Phi_Cen__E->ProjectionX(Form("_%d", iy), iy, iy);
			proj_E->SetLineColor(iy);
			leg_E->AddEntry(proj_E, Form("%d~%d%", 80 - 10 * iy, 90 - 10 * iy), "l");
			proj_E->DrawClone("same");
		}
		leg_E->DrawClone("same");

		c_Phi->SaveAs(Form("roots/%d_nsigmaMean_Phi.png", number));
	}
	if (0)//Calibrate
	{
		TH2F* h_nSigmaE_Pt = (TH2F*)h_Pt_Cen_nSigmaE->Project3D("zx");
		TH2F* h_nSigmaE_Eta = (TH2F*)h_Eta_Cen_nSigmaE->Project3D("zx");
		TH2F* h_nSigmaE_Phi = (TH2F*)h_Phi_Cen_nSigmaE->Project3D("zx");
		TH2F* h_nSigmaEcorr_Pt = (TH2F*)h_Pt_Cen_nSigmaEcorr->Project3D("zx");
		TH2F* h_nSigmaEcorr_Eta = (TH2F*)h_Eta_Cen_nSigmaEcorr->Project3D("zx");
		TH2F* h_nSigmaEcorr_Phi = (TH2F*)h_Phi_Cen_nSigmaEcorr->Project3D("zx");

		//TH2F* h_nSigmaPion_Pt = (TH2F*)h_Pt_Cen_nSigmaPi->Project3D("zx");
		//TH2F* h_nSigmaPion_Eta = (TH2F*)h_Eta_Cen_nSigmaPi->Project3D("zx");
		//TH2F* h_nSigmaPion_Phi = (TH2F*)h_Phi_Cen_nSigmaPi->Project3D("zx");
		//TH2F* h_nSigmaPicorr_Pt = (TH2F*)h_Pt_Cen_nSigmaPicorr->Project3D("zx");
		//TH2F* h_nSigmaPicorr_Eta = (TH2F*)h_Eta_Cen_nSigmaPicorr->Project3D("zx");
		//TH2F* h_nSigmaPicorr_Phi = (TH2F*)h_Phi_Cen_nSigmaPicorr->Project3D("zx");

		h_nSigmaE_Pt->GetYaxis()->SetRangeUser(-5, 5);
		h_nSigmaE_Eta->GetYaxis()->SetRangeUser(-5, 5);
		h_nSigmaE_Phi->GetYaxis()->SetRangeUser(-5, 5);
		h_nSigmaEcorr_Pt->GetYaxis()->SetRangeUser(-5, 5);
		h_nSigmaEcorr_Eta->GetYaxis()->SetRangeUser(-5, 5);
		h_nSigmaEcorr_Phi->GetYaxis()->SetRangeUser(-5, 5);
		//h_nSigmaPion_Pt->GetYaxis()->SetRangeUser(-5, 5);
		//h_nSigmaPion_Eta->GetYaxis()->SetRangeUser(-5, 5);
		//h_nSigmaPion_Phi->GetYaxis()->SetRangeUser(-5, 5);
		//h_nSigmaPicorr_Pt->GetYaxis()->SetRangeUser(-5, 5);
		//h_nSigmaPicorr_Eta->GetYaxis()->SetRangeUser(-5, 5);
		//h_nSigmaPicorr_Phi->GetYaxis()->SetRangeUser(-5, 5);

		TCanvas *c_corrE = new TCanvas("c_corrE", "c_corrE", 1200, 700);
		c_corrE->Divide(3, 2);

		c_corrE->cd(1);
		gPad->SetLogz(1);
		gPad->SetLeftMargin(0.12);
		gPad->SetRightMargin(0.12);
		gStyle->SetOptStat(0);
		h_nSigmaE_Pt->Draw("col z");

		c_corrE->cd(2);
		gPad->SetLogz(1);
		gPad->SetLeftMargin(0.12);
		gPad->SetRightMargin(0.12);
		gStyle->SetOptStat(0);
		h_nSigmaE_Eta->Draw("col z");

		c_corrE->cd(3);
		gPad->SetLogz(1);
		gPad->SetLeftMargin(0.12);
		gPad->SetRightMargin(0.12);
		gStyle->SetOptStat(0);
		h_nSigmaE_Phi->Draw("col z");


		c_corrE->cd(4);
		gPad->SetLogz(1);
		gPad->SetLeftMargin(0.12);
		gPad->SetRightMargin(0.12);
		gStyle->SetOptStat(0);
		h_nSigmaEcorr_Pt->Draw("col z");

		c_corrE->cd(5);
		gPad->SetLogz(1);
		gPad->SetLeftMargin(0.12);
		gPad->SetRightMargin(0.12);
		gStyle->SetOptStat(0);
		h_nSigmaEcorr_Eta->Draw("col z");

		c_corrE->cd(6);
		gPad->SetLogz(1);
		gPad->SetLeftMargin(0.12);
		gPad->SetRightMargin(0.12);
		gStyle->SetOptStat(0);
		h_nSigmaEcorr_Phi->Draw("col z");

		//c_corrE->cd(7);
		//gPad->SetLogz(1);
		//gPad->SetLeftMargin(0.12);
		//gPad->SetRightMargin(0.12);
		//gStyle->SetOptStat(0);
		//h_nSigmaPion_Pt->Draw("col z");

		//c_corrE->cd(8);
		//gPad->SetLogz(1);
		//gPad->SetLeftMargin(0.12);
		//gPad->SetRightMargin(0.12);
		//gStyle->SetOptStat(0);
		//h_nSigmaPion_Eta->Draw("col z");

		//c_corrE->cd(9);
		//gPad->SetLogz(1);
		//gPad->SetLeftMargin(0.12);
		//gPad->SetRightMargin(0.12);
		//gStyle->SetOptStat(0);
		//h_nSigmaPion_Phi->Draw("col z");


		//c_corrE->cd(10);
		//gPad->SetLogz(1);
		//gPad->SetLeftMargin(0.12);
		//gPad->SetRightMargin(0.12);
		//gStyle->SetOptStat(0);
		//h_nSigmaPicorr_Pt->Draw("col z");

		//c_corrE->cd(11);
		//gPad->SetLogz(1);
		//gPad->SetLeftMargin(0.12);
		//gPad->SetRightMargin(0.12);
		//gStyle->SetOptStat(0);
		//h_nSigmaPicorr_Eta->Draw("col z");

		//c_corrE->cd(12);
		//gPad->SetLogz(1);
		//gPad->SetLeftMargin(0.12);
		//gPad->SetRightMargin(0.12);
		//gStyle->SetOptStat(0);
		//h_nSigmaPicorr_Phi->Draw("col z");

		c_corrE->SaveAs(Form("roots/%d_calibrate.png", number));
	}

}