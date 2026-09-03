//---------------------从OO.root中提取直方图，并进行设置更改和元素添加---------------------
#include "someFunction.h"
void ElectronDistribution(TString inFilename = "roots/group4/21_20260903_iTPC44_etagap0p2_RmPair_noRm.root") //
{
	// Extract number from filename for output naming
	TString basename = inFilename;
	Int_t lastSlash = inFilename.Last('/');
	if (lastSlash != kNPOS) basename = inFilename(lastSlash + 1, inFilename.Length());
	Int_t firstUnderscore = basename.First('_');
	Int_t num = 0;	TString outPrefix;
	if (firstUnderscore != kNPOS) {
		TString numStr = basename(0, firstUnderscore);
		num = numStr.Atoi();
		if (num > 0) outPrefix.Form("%d_", num);
	}
	// 从root文件中导入待拟合的直方图
	TFile *inFile = new TFile(inFilename);
	if (!inFile){
		cout << "The input file is not found! Exiting..." << endl;
		return;
	}
	TH1F *h_pT__e = (TH1F *)inFile->Get("h_pT__electrons");if(!h_pT__e) {cout<<"h_pT__electrons not found! Exiting..."<<endl; return;}
	TH1F *h_eta__e = (TH1F *)inFile->Get("h_eta__electrons");if(!h_eta__e) {cout<<"h_eta__electrons not found! Exiting..."<<endl; return;}
	TH1F *h_phi__e = (TH1F *)inFile->Get("h_phi__electrons");if(!h_phi__e) {cout<<"h_phi__electrons not found! Exiting..."<<endl; return;}
	TH1F *h_pT__p = (TH1F *)inFile->Get("h_pT__positrons");if(!h_pT__p) {cout<<"h_pT__positrons not found! Exiting..."<<endl; return;}
	TH1F *h_eta__p = (TH1F *)inFile->Get("h_eta__positrons");if(!h_eta__p) {cout<<"h_eta__positrons not found! Exiting..."<<endl; return;}
	TH1F *h_phi__p = (TH1F *)inFile->Get("h_phi__positrons");if(!h_phi__p) {cout<<"h_phi__positrons not found! Exiting..."<<endl; return;}
	TH1F *h_pT__e_PhiV_Cut = (TH1F *)inFile->Get("h_pT__electrons_w_PhiV_Cut");if(!h_pT__e_PhiV_Cut) {cout<<"h_pT__electrons not found! Exiting..."<<endl; return;}
	TH1F *h_eta__e_PhiV_Cut = (TH1F *)inFile->Get("h_eta__electrons_w_PhiV_Cut");if(!h_eta__e_PhiV_Cut) {cout<<"h_eta__electrons not found! Exiting..."<<endl; return;}
	TH1F *h_phi__e_PhiV_Cut = (TH1F *)inFile->Get("h_phi__electrons_w_PhiV_Cut");if(!h_phi__e_PhiV_Cut) {cout<<"h_phi__electrons not found! Exiting..."<<endl; return;}
	TH1F *h_pT__p_PhiV_Cut = (TH1F *)inFile->Get("h_pT__positrons_w_PhiV_Cut");if(!h_pT__p_PhiV_Cut) {cout<<"h_pT__positrons not found! Exiting..."<<endl; return;}
	TH1F *h_eta__p_PhiV_Cut = (TH1F *)inFile->Get("h_eta__positrons_w_PhiV_Cut");if(!h_eta__p_PhiV_Cut) {cout<<"h_eta__positrons not found! Exiting..."<<endl; return;}
	TH1F *h_phi__p_PhiV_Cut = (TH1F *)inFile->Get("h_phi__positrons_w_PhiV_Cut");if(!h_phi__p_PhiV_Cut) {cout<<"h_phi__positrons not found! Exiting..."<<endl; return;}
	
	TH1F* h_DeltaPt__unlikeSame = (TH1F *)inFile->Get("h_DeltaPt__unlikeSame");
	TH1F* h_DeltaEta__unlikeSame = (TH1F *)inFile->Get("h_DeltaEta__unlikeSame");
	TH1F* h_DeltaPhi__unlikeSame = (TH1F *)inFile->Get("h_DeltaPhi__unlikeSame");
	TH1F* h_DeltaPt__likepp = (TH1F *)inFile->Get("h_DeltaPt__likepp");
	TH1F* h_DeltaEta__likepp = (TH1F *)inFile->Get("h_DeltaEta__likepp");
	TH1F* h_DeltaPhi__likepp = (TH1F *)inFile->Get("h_DeltaPhi__likepp");
	TH1F* h_DeltaPt__likemm = (TH1F *)inFile->Get("h_DeltaPt__likemm");
	TH1F* h_DeltaEta__likemm = (TH1F *)inFile->Get("h_DeltaEta__likemm");
	TH1F* h_DeltaPhi__likemm = (TH1F *)inFile->Get("h_DeltaPhi__likemm");
	TH1F* h_DeltaPt__unlikeMixed = (TH1F *)inFile->Get("h_DeltaPt__unlikeMixed");
	TH1F* h_DeltaEta__unlikeMixed = (TH1F *)inFile->Get("h_DeltaEta__unlikeMixed");
	TH1F* h_DeltaPhi__unlikeMixed = (TH1F *)inFile->Get("h_DeltaPhi__unlikeMixed");
	TH1F* h_SumEta__unlikeSame = (TH1F *)inFile->Get("h_SumEta__unlikeSame");

	// DeltaPhi cut related histograms
	TH1F* h_Phi__unlike_deltaPhi_0p2 = (TH1F *)inFile->Get("h_Phi__unlike_deltaPhi_0p2");
	TH1F* h_Phi__unlike_deltaPhi_3p0 = (TH1F *)inFile->Get("h_Phi__unlike_deltaPhi_3p0");
	TH1F* h_Mee__unlike_deltaPhi_3p0 = (TH1F *)inFile->Get("h_Mee__unlike_deltaPhi_3p0");
	TH1F* h_Mee__likemm_deltaPhi_0p2 = (TH1F *)inFile->Get("h_Mee__likemm_deltaPhi_0p2");
	TH1F* h_Mee__likepp_deltaPhi_0p2 = (TH1F *)inFile->Get("h_Mee__likepp_deltaPhi_0p2");
	TH1F* h_Phi__likemm_deltaPhi_0p2 = (TH1F *)inFile->Get("h_Phi__likemm_deltaPhi_0p2");
	TH1F* h_Eta__likemm_deltaPhi_0p2 = (TH1F *)inFile->Get("h_Eta__likemm_deltaPhi_0p2");
	TH1F* h_Pt__likemm_deltaPhi_0p2 = (TH1F *)inFile->Get("h_Pt__likemm_deltaPhi_0p2");
	TH1F* h_Phi__likepp_deltaPhi_0p2 = (TH1F *)inFile->Get("h_Phi__likepp_deltaPhi_0p2");
	TH1F* h_Eta__likepp_deltaPhi_0p2 = (TH1F *)inFile->Get("h_Eta__likepp_deltaPhi_0p2");
	TH1F* h_Pt__likepp_deltaPhi_0p2 = (TH1F *)inFile->Get("h_Pt__likepp_deltaPhi_0p2");
	TH1F* h_DeltaEta__likemm_deltaPhi_0p2 = (TH1F *)inFile->Get("h_DeltaEta__likemm_deltaPhi_0p2");
	TH1F* h_DeltaPt__likemm_deltaPhi_0p2 = (TH1F *)inFile->Get("h_DeltaPt__likemm_deltaPhi_0p2");
	TH1F* h_DeltaEta__likepp_deltaPhi_0p2 = (TH1F *)inFile->Get("h_DeltaEta__likepp_deltaPhi_0p2");
	TH1F* h_DeltaPt__likepp_deltaPhi_0p2 = (TH1F *)inFile->Get("h_DeltaPt__likepp_deltaPhi_0p2");

    if (0) // ep number
	{
		TH2F * h_e_p__Number_Same = (TH2F *)inFile->Get("h_e_p__Number_Same");if(!h_e_p__Number_Same) {cout<<"h_e_p__Number_Same not found! Exiting..."<<endl; return;}
		//cout<<"Same Event:"<<endl;printHist2DSci(h_e_p__Number_Same);
		TH2F * h_e_p__Number_Mix = (TH2F *)inFile->Get("h_e_p__Number_Mix");if(!h_e_p__Number_Mix) {cout<<"h_e_p__Number_Mix not found! Exiting..."<<endl; return;}
		//cout<<"Mixed Event:"<<endl;printHist2DSci(h_e_p__Number_Mix);


		TCanvas *c_sum = new TCanvas("c_sum", "c_sum", 1000, 500);
		c_sum->Divide(2, 1);
		c_sum->cd(1);
		gPad->SetLogz(1);
		gPad->SetLeftMargin(0.12);
		gPad->SetRightMargin(0.12);
		//gStyle->SetOptStat(0);
		h_e_p__Number_Same->Draw("col z");

		c_sum->cd(2);
		gPad->SetLogz(1);
		gPad->SetLeftMargin(0.12);
		gPad->SetRightMargin(0.12);
		//gStyle->SetOptStat(0);
		h_e_p__Number_Mix->GetXaxis()->SetRangeUser(0.0, 30.0);
		h_e_p__Number_Mix->GetYaxis()->SetRangeUser(0.0, 30.0);
		h_e_p__Number_Mix->Draw("col z");

		c_sum->SaveAs(Form("roots/group4/%sepNumber.png", outPrefix.Data()));
	}
    if (0) // e+ e- pt/eta/phi distribution phiV cut check
	{
        //设置格式
		h_pT__p_PhiV_Cut->SetLineColor(kRed);
		h_eta__p_PhiV_Cut->SetLineColor(kRed);
		h_phi__p_PhiV_Cut->SetLineColor(kRed);
		h_pT__e_PhiV_Cut->SetLineColor(kRed);
		h_eta__e_PhiV_Cut->SetLineColor(kRed);
		h_phi__e_PhiV_Cut->SetLineColor(kRed);
        //画图
		TCanvas *c_PhiV = new TCanvas("c_PhiV", "c_PhiV", 1200, 800);
		c_PhiV->Divide(3, 2);
		c_PhiV->cd(1);
		gPad->SetLogy(0);
		TLegend *leg1 = new TLegend(0.65, 0.75, 0.88, 0.88);
		leg1->AddEntry(h_pT__e_PhiV_Cut, "w #phi_{V} cut", "l");
		leg1->AddEntry(h_pT__e, "wo #phi_{V} cut", "l");
		leg1->SetBorderSize(0);
		leg1->SetFillStyle(0);
		// h_pT__e_PhiV_Cut->GetXaxis()->SetRangeUser(0., 0.5);
		h_pT__e_PhiV_Cut->DrawClone();
		h_pT__e->DrawClone("same");
		leg1->DrawClone("same");

		c_PhiV->cd(2);
		gPad->SetLogy(0);
		// h_eta__e_PhiV_Cut->GetXaxis()->SetRangeUser(-2.0, 2.0);
		h_eta__e_PhiV_Cut->DrawClone();
		h_eta__e->DrawClone("same");

		c_PhiV->cd(3);
		gPad->SetLogy(0);
		// h_phi__e_PhiV_Cut->GetXaxis()->SetRangeUser(-3.2, 3.2);
		h_phi__e_PhiV_Cut->DrawClone();
		h_phi__e->DrawClone("same");

		c_PhiV->cd(4);
		gPad->SetLogy(0);
		// h_pT__e_PhiV_Cut->GetXaxis()->SetRangeUser(0., 0.5);
		h_pT__p_PhiV_Cut->DrawClone();
		h_pT__p->DrawClone("same");
		leg1->DrawClone("same");

		c_PhiV->cd(5);
		gPad->SetLogy(0);
		// h_eta__p_PhiV_Cut->GetXaxis()->SetRangeUser(-2.0, 2.0);
		h_eta__p_PhiV_Cut->DrawClone();
		h_eta__p->DrawClone("same");

		c_PhiV->cd(6);
		gPad->SetLogy(0);
		// h_phi__p_PhiV_Cut->GetXaxis()->SetRangeUser(-3.2, 3.2);
		h_phi__p_PhiV_Cut->DrawClone();
		h_phi__p->DrawClone("same");

		c_PhiV->SaveAs(Form("roots/group4/%sep_pt_eta_phi_PhiVcut.png", outPrefix.Data()));
	}	
	if (0) // e+ e- pt/eta/phi ratio check
	{
        //计算ratio e-/e+
        TH1F *h_pT_ratio_EperP = (TH1F *)h_pT__e_PhiV_Cut->Clone("h_pT_ratio_EperP");
        TH1F *h_eta_ratio_EperP = (TH1F *)h_eta__e_PhiV_Cut->Clone("h_eta_ratio_EperP");
        TH1F *h_phi_ratio_EperP = (TH1F *)h_phi__e_PhiV_Cut->Clone("h_phi_ratio_EperP");
        h_pT_ratio_EperP->SetTitle("e^{-}/e^{+} p_{T} ratio;p_{T} (GeV/c);Ratio");
        h_pT_ratio_EperP->Divide(h_pT__p_PhiV_Cut);
        h_eta_ratio_EperP->SetTitle("e^{-}/e^{+} #eta ratio;#eta;Ratio");
        h_eta_ratio_EperP->Divide(h_eta__p_PhiV_Cut);
        h_phi_ratio_EperP->SetTitle("e^{-}/e^{+} #phi ratio;#phi;Ratio");
        h_phi_ratio_EperP->Divide(h_phi__p_PhiV_Cut);
        //设置格式
		h_pT__p_PhiV_Cut->SetLineColor(kRed);
		h_eta__p_PhiV_Cut->SetLineColor(kRed);
		h_phi__p_PhiV_Cut->SetLineColor(kRed);
		h_pT__e_PhiV_Cut->SetLineColor(kBlue);
		h_eta__e_PhiV_Cut->SetLineColor(kBlue);
		h_phi__e_PhiV_Cut->SetLineColor(kBlue);
        //画图
		TCanvas *c_eTrack = new TCanvas("c_eTrack", "c_eTrack", 1200, 800);
		c_eTrack->Divide(3, 2);
		c_eTrack->cd(1);
		gPad->SetLogy(0);
		TLegend *leg1 = new TLegend(0.65, 0.75, 0.88, 0.88);
		leg1->AddEntry(h_pT__e_PhiV_Cut, "e^{-} (electrons)", "l");
		leg1->AddEntry(h_pT__p_PhiV_Cut, "e^{+} (positrons)", "l");
		leg1->SetBorderSize(0);
		leg1->SetFillStyle(0);
		// h_pT__e_PhiV_Cut->GetXaxis()->SetRangeUser(0., 0.5);
		h_pT__e_PhiV_Cut->Draw();
		h_pT__p_PhiV_Cut->Draw("same");
		leg1->Draw("same");

		c_eTrack->cd(2);
		gPad->SetLogy(0);
		// h_eta__e_PhiV_Cut->GetXaxis()->SetRangeUser(-2.0, 2.0);
		h_eta__e_PhiV_Cut->Draw();
		h_eta__p_PhiV_Cut->Draw("same");

		c_eTrack->cd(3);
		gPad->SetLogy(0);
		// h_phi__e_PhiV_Cut->GetXaxis()->SetRangeUser(-3.2, 3.2);
		h_phi__e_PhiV_Cut->Draw();
		h_phi__p_PhiV_Cut->Draw("same");

		c_eTrack->cd(4);
		gPad->SetLogy(0);
		gStyle->SetOptStat(0);
		h_pT_ratio_EperP->SetMaximum(2);
		h_pT_ratio_EperP->SetMinimum(0.5);
		h_pT_ratio_EperP->Draw();

		c_eTrack->cd(5);
		gPad->SetLogy(0);
		gStyle->SetOptStat(0);
		h_eta_ratio_EperP->SetMaximum(2);
		h_eta_ratio_EperP->SetMinimum(0.5);
		h_eta_ratio_EperP->Draw();

		c_eTrack->cd(6);
		gPad->SetLogy(0);
		gStyle->SetOptStat(0);
		h_phi_ratio_EperP->SetMaximum(2);
		h_phi_ratio_EperP->SetMinimum(0.5);
		h_phi_ratio_EperP->Draw();

		c_eTrack->SaveAs(Form("roots/group4/%sep_TrackQA.png", outPrefix.Data()));
		// delete c_eTrack;
	}
	
	if (0) // Delta Pt, Delta Eta, Delta Phi, Sum Eta correlation analysis (only e^{-}e^{-} for like-sign, e^{+}e^{+} omitted)
	{
		//画图
		TCanvas *c1 = new TCanvas("c1", "c1", 1000, 1300);
		c1->Divide(3, 4);

		c1->cd(1);
		gPad->SetLogz(1);
		gStyle->SetOptStat(0);
		h_DeltaPt__likemm->GetXaxis()->SetRangeUser(-0.1,0.1);
		h_DeltaPt__likemm->DrawClone("col z");

		c1->cd(2);
		gPad->SetLogz(1);
		gStyle->SetOptStat(0);
		h_DeltaPt__unlikeSame->GetXaxis()->SetRangeUser(-0.1,0.1);
		h_DeltaPt__unlikeSame->DrawClone("col z");

		c1->cd(3);
		gPad->SetLogz(1);
		gStyle->SetOptStat(0);
		h_DeltaPt__unlikeMixed->GetXaxis()->SetRangeUser(-0.1,0.1);
		h_DeltaPt__unlikeMixed->DrawClone("col z");

		c1->cd(4);
		gPad->SetLogz(1);
		gStyle->SetOptStat(0);
		h_DeltaEta__likemm->DrawClone("col z");

		c1->cd(5);
		gPad->SetLogz(1);
		gStyle->SetOptStat(0);
		h_DeltaEta__unlikeSame->DrawClone("col z");

		c1->cd(6);
		gPad->SetLogz(1);
		gStyle->SetOptStat(0);
		h_DeltaEta__unlikeMixed->DrawClone("col z");

		c1->cd(7);
		gPad->SetLogz(1);
		gStyle->SetOptStat(0);
		h_DeltaPhi__likemm->DrawClone("col z");

		c1->cd(8);
		gPad->SetLogz(1);
		gStyle->SetOptStat(0);
		h_DeltaPhi__unlikeSame->DrawClone("col z");

		c1->cd(9);
		gPad->SetLogz(1);
		gStyle->SetOptStat(0);
		h_DeltaPhi__unlikeMixed->DrawClone("col z");

		c1->cd(10);
		gPad->SetLogz(1);
		gStyle->SetOptStat(0);
		//h_SumEta__unlikeSame->DrawClone();

		c1->SaveAs(Form("roots/group4/%sDelta_Pt_Eta_Phi.png", outPrefix.Data()));
	}
	if (0) // deltaPhi < 0.2
	{
		TCanvas *c_deltaPhi_0p2 = new TCanvas("c_deltaPhi_0p2", "c_deltaPhi_0p2", 1200, 800);
		c_deltaPhi_0p2->Divide(3, 2);

		c_deltaPhi_0p2->cd(1);
		gPad->SetLogy(0);
		gStyle->SetOptStat(0);
		h_Pt__likepp_deltaPhi_0p2->SetTitle("p_{T} of e^{+}e^{+} (#Delta#phi<0.2);p_{T} (GeV/c);counts");
		h_Pt__likepp_deltaPhi_0p2->DrawClone();

		c_deltaPhi_0p2->cd(2);
		gPad->SetLogy(0);
		gStyle->SetOptStat(0);
		h_Eta__likepp_deltaPhi_0p2->SetTitle("#eta of e^{+}e^{+} (#Delta#phi<0.2);#eta;counts");
		h_Eta__likepp_deltaPhi_0p2->DrawClone();

		c_deltaPhi_0p2->cd(3);
		gPad->SetLogy(0);
		gStyle->SetOptStat(0);
		h_Phi__likepp_deltaPhi_0p2->SetTitle("#phi of e^{+}e^{+} (#Delta#phi<0.2);#phi;counts");
		h_Phi__likepp_deltaPhi_0p2->DrawClone();

		c_deltaPhi_0p2->cd(4);
		gPad->SetLogy(0);
		gStyle->SetOptStat(0);
		h_DeltaPt__likepp_deltaPhi_0p2->SetTitle("#Delta p_{T} of e^{+}e^{+} (#Delta#phi<0.2);#Delta p_{T} (GeV/c);counts");
		h_DeltaPt__likepp_deltaPhi_0p2->DrawClone();

		c_deltaPhi_0p2->cd(5);
		gPad->SetLogy(0);
		gStyle->SetOptStat(0);
		h_DeltaEta__likepp_deltaPhi_0p2->SetTitle("#Delta#eta of e^{+}e^{+} (#Delta#phi<0.2);#Delta#eta;counts");
		h_DeltaEta__likepp_deltaPhi_0p2->DrawClone();

		c_deltaPhi_0p2->cd(6);
		gPad->SetLogy(0);
		gStyle->SetOptStat(0);
		h_Mee__likepp_deltaPhi_0p2->GetXaxis()->SetRangeUser(0,0.3);
		h_Mee__likepp_deltaPhi_0p2->SetTitle("M_{ee} of e^{+}e^{+} (#Delta#phi<0.2);M_{ee} (GeV/c^{2});counts");
		h_Mee__likepp_deltaPhi_0p2->DrawClone();

		c_deltaPhi_0p2->SaveAs(Form("roots/group4/%sdeltaPhi_0p2.png", outPrefix.Data()));
	}
	if (0) // unlike-sign deltaPhi > 3.0
	{
		TCanvas *c_unlike_deltaPhi_3p0 = new TCanvas("c_unlike_deltaPhi_3p0", "c_unlike_deltaPhi_3p0", 800, 400);
		c_unlike_deltaPhi_3p0->Divide(2, 1);

		c_unlike_deltaPhi_3p0->cd(1);
		gPad->SetLogy(0);
		gStyle->SetOptStat(0);
		h_Phi__unlike_deltaPhi_3p0->SetTitle("#phi of e^{+}e^{-} (#Delta#phi>3.0);#phi;counts");
		h_Phi__unlike_deltaPhi_3p0->DrawClone();

		c_unlike_deltaPhi_3p0->cd(2);
		gPad->SetLogy(0);
		gStyle->SetOptStat(0);
		h_Mee__unlike_deltaPhi_3p0->GetXaxis()->SetRangeUser(0,0.3);
		h_Mee__unlike_deltaPhi_3p0->SetTitle("M_{ee} of e^{+}e^{-} (#Delta#phi>3.0);M_{ee} (GeV/c^{2});counts");
		h_Mee__unlike_deltaPhi_3p0->DrawClone();

		c_unlike_deltaPhi_3p0->SaveAs(Form("roots/group4/%sunlike_deltaPhi_3p0.png", outPrefix.Data()));
	}

	if (0) // DeltaR distribution with ratio (Same/Mixed)
	{
		TH1F* h_DeltaR__unlikeSame = (TH1F *)inFile->Get("h_DeltaR__unlikeSame");h_DeltaR__unlikeSame->RebinX(10);
		TH1F* h_DeltaR__likemm = (TH1F *)inFile->Get("h_DeltaR__likemm");h_DeltaR__likemm->RebinX(10);
		TH1F* h_DeltaR__unlikeMixed = (TH1F *)inFile->Get("h_DeltaR__unlikeMixed");h_DeltaR__unlikeMixed->RebinX(10);h_DeltaR__unlikeMixed->Scale(1/100.0);
		TH1F* h_DeltaR__likemmMixed = (TH1F *)inFile->Get("h_DeltaR__likemmMixed");h_DeltaR__likemmMixed->RebinX(10);h_DeltaR__likemmMixed->Scale(1/100.0);

		// Compute ratios: Same / Mixed
		TH1F* h_DeltaR_Ratio__likemm = (TH1F *)h_DeltaR__likemm->Clone("h_DeltaR_Ratio__likemm");
		h_DeltaR_Ratio__likemm->Divide(h_DeltaR__likemmMixed);
		h_DeltaR_Ratio__likemm->SetTitle("#Delta R ratio of e^{-}e^{-} same/mixed;#Delta R;Ratio");

		TH1F* h_DeltaR_Ratio__unlike = (TH1F *)h_DeltaR__unlikeSame->Clone("h_DeltaR_Ratio__unlike");
		h_DeltaR_Ratio__unlike->Divide(h_DeltaR__unlikeMixed);
		h_DeltaR_Ratio__unlike->SetTitle("#Delta R ratio of e^{+}e^{-} same/mixed;#Delta R;Ratio");

		TCanvas *c_DeltaR = new TCanvas("c_DeltaR", "c_DeltaR", 1200, 800);
		c_DeltaR->Divide(3, 2);

		// Row 1: e^{-}e^{-} (likemm)
		c_DeltaR->cd(1);
		gPad->SetLogy(0);
		gStyle->SetOptStat(0);
		h_DeltaR__likemm->SetTitle("#Delta R of e^{-}e^{-} Same Event;#Delta R;counts");
		h_DeltaR__likemm->DrawClone();

		c_DeltaR->cd(2);
		gPad->SetLogy(0);
		gStyle->SetOptStat(0);
		h_DeltaR__likemmMixed->SetTitle("#Delta R of e^{-}e^{-} Mixed Event;#Delta R;counts");
		h_DeltaR__likemmMixed->DrawClone();

		c_DeltaR->cd(3);
		gPad->SetLogy(0);
		gStyle->SetOptStat(0);
		h_DeltaR_Ratio__likemm->SetMaximum(5);
		h_DeltaR_Ratio__likemm->SetMinimum(0);
		h_DeltaR_Ratio__likemm->DrawClone("PE");

		// Row 2: e^{+}e^{-} (unlike)
		c_DeltaR->cd(4);
		gPad->SetLogy(0);
		gStyle->SetOptStat(0);
		h_DeltaR__unlikeSame->SetTitle("#Delta R of e^{+}e^{-} Same Event;#Delta R;counts");
		h_DeltaR__unlikeSame->DrawClone();

		c_DeltaR->cd(5);
		gPad->SetLogy(0);
		gStyle->SetOptStat(0);
		h_DeltaR__unlikeMixed->SetTitle("#Delta R of e^{+}e^{-} Mixed Event;#Delta R;counts");
		h_DeltaR__unlikeMixed->DrawClone();

		c_DeltaR->cd(6);
		gPad->SetLogy(0);
		gStyle->SetOptStat(0);
		h_DeltaR_Ratio__unlike->SetMaximum(5);
		h_DeltaR_Ratio__unlike->SetMinimum(0);
		h_DeltaR_Ratio__unlike->DrawClone("PE");

		c_DeltaR->SaveAs(Form("roots/group4/%sDeltaR.png", outPrefix.Data()));
	}
	if (0) // invariant Q correlation analysis
	{
		TH1F* h_Qinv__likemm = (TH1F *)inFile->Get("h_Qinv__likemm");//h_Qinv__likemm->Rebin(5);
		TH1F* h_Qinv__likepp = (TH1F *)inFile->Get("h_Qinv__likepp");//h_Qinv__likepp->Rebin(5);
		TH1F* h_Qinv__likemmMixed = (TH1F *)inFile->Get("h_Qinv__likemmMixed");h_Qinv__likemmMixed->Scale(1/100.0);//h_Qinv__likemmMixed->Rebin(5);
		TH1F* h_Qinv__likeppMixed = (TH1F *)inFile->Get("h_Qinv__likeppMixed");h_Qinv__likeppMixed->Scale(1/100.0);//h_Qinv__likeppMixed->Rebin(5);
	
		TH1F *h_Qinv_SamePerMix__likemm = (TH1F *)h_Qinv__likemm->Clone("h_Qinv_SamePerMix__likemm");
		TH1F *h_Qinv_SamePerMix__likepp = (TH1F *)h_Qinv__likepp->Clone("h_Qinv_SamePerMix__likepp");
		h_Qinv_SamePerMix__likemm->Divide(h_Qinv__likemmMixed);
		h_Qinv_SamePerMix__likepp->Divide(h_Qinv__likeppMixed);

		h_Qinv_SamePerMix__likepp->SetTitle("Invariant Q of e^{+}e^{+} ratio of same-event over mixed-event;invariant Q (GeV/c);Ratio");
		h_Qinv_SamePerMix__likemm->SetTitle("Invariant Q of e^{-}e^{-} ratio of same-event over mixed-event;invariant Q (GeV/c);Ratio");
		TCanvas *c2 = new TCanvas("c2", "c2", 1800, 600);
		c2->Divide(3, 1);

		c2->cd(1);
		gPad->SetLogy(0);
		gStyle->SetOptStat(0);
		h_Qinv__likemm->GetXaxis()->SetRangeUser(0,0.3);
		h_Qinv__likemm->DrawClone("PE");

		c2->cd(2);
		gPad->SetLogy(0);
		gStyle->SetOptStat(0);
		h_Qinv__likemmMixed->GetXaxis()->SetRangeUser(0,0.3);
		h_Qinv__likemmMixed->DrawClone("PE");

		c2->cd(3);
		gPad->SetLogy(0);
		gStyle->SetOptStat(0);
		h_Qinv_SamePerMix__likemm->GetXaxis()->SetRangeUser(0,0.3);
		h_Qinv_SamePerMix__likemm->SetMaximum(8);
		h_Qinv_SamePerMix__likemm->SetMinimum(0);
		//h_Qinv_SamePerMix__likemm->GetYaxis()->SetNdivisions(505);
		h_Qinv_SamePerMix__likemm->DrawClone("PE");

		c2->SaveAs(Form("roots/group4/%sInvariant_Q.png", outPrefix.Data()));
	}	
	
	if (10) // Mee distribution
	{
		// TH1F* h_Mee__unlike_deltaPhi_3p0 = (TH1F *)inFile->Get("h_Mee__unlike_deltaPhi_3p0");
		// TH1F* h_Mee__likemm_deltaPhi_0p2 = (TH1F *)inFile->Get("h_Mee__likemm_deltaPhi_0p2");
		// TH1F* h_Mee__likepp_deltaPhi_0p2 = (TH1F *)inFile->Get("h_Mee__likepp_deltaPhi_0p2");
		TH1F* h_Mee__unlikeSame 		    = (TH1F *)inFile->Get("h_Mee__unlikeSame");
		TH1F* h_Mee__unlikeSame__w_PhiV_Cut = (TH1F *)inFile->Get("h_Mee__unlikeSame__w_PhiV_Cut");
					TH3F* h_Mee_Pt_Cen__likemm = (TH3F *)inFile->Get("h_Mee_Pt_Cen__likemm");
		TH1F* h_Mee__likemm = (TH1F *)h_Mee_Pt_Cen__likemm->ProjectionX("h_Mee__likemm", 1, -1, 1, -1);if (h_Mee__likemm->GetSumw2N() == 0) h_Mee__likemm->Sumw2("kTRUE");
					TH3F* h_Mee_Pt_Cen__likepp = (TH3F *)inFile->Get("h_Mee_Pt_Cen__likepp");
		TH1F* h_Mee__likepp = (TH1F *)h_Mee_Pt_Cen__likepp->ProjectionX("h_Mee__likepp", 1, -1, 1, -1);if (h_Mee__likepp->GetSumw2N() == 0) h_Mee__likepp->Sumw2("kTRUE");

		// Load event count for normalization
		TH1F* h_passEvtcut = (TH1F *)inFile->Get("h_passEvtcut");
		Float_t Nevents = 1;
		if (h_passEvtcut) {
			Int_t lastBin = h_passEvtcut->GetNbinsX();
			Nevents = h_passEvtcut->GetBinContent(lastBin);
			cout << "Nevents (last bin of h_passEvtcut, bin " << lastBin << ") = " << Nevents << endl;
		} else {
			cout << "h_passEvtcut not found, scaling disabled." << endl;
		}
		const Float_t kScale = 1.0 / Nevents;
		const Float_t kLegendFactor = 2.0e8; // 200M for legend display

		// h_Mee__unlike_deltaPhi_3p0->SetLineColor(kRed);h_Mee__unlike_deltaPhi_3p0->SetMarkerStyle(20);
		// h_Mee__likemm_deltaPhi_0p2->SetLineColor(kRed);h_Mee__likemm_deltaPhi_0p2->SetMarkerStyle(20);
		// h_Mee__likepp_deltaPhi_0p2->SetLineColor(kRed);h_Mee__likepp_deltaPhi_0p2->SetMarkerStyle(20);
		h_Mee__unlikeSame->SetLineColor(kRed);h_Mee__unlikeSame->SetMarkerStyle(20);
		h_Mee__unlikeSame__w_PhiV_Cut->SetLineColor(kRed);h_Mee__unlikeSame__w_PhiV_Cut->SetMarkerStyle(20);
		h_Mee__likemm->SetLineColor(kRed);h_Mee__likemm->SetMarkerStyle(20);
		h_Mee__likepp->SetLineColor(kRed);h_Mee__likepp->SetMarkerStyle(20);

		//计算背景h_Mee__likemm+h_Mee__likepp
		TH1F* h_Mee_Background = (TH1F *)h_Mee__likemm->Clone("h_Mee_Background");if (h_Mee_Background->GetSumw2N() == 0) h_Mee_Background->Sumw2("kTRUE");h_Mee_Background->Add(h_Mee__likepp);
		// h_Mee_Background->Add(h_Mee__likemm_deltaPhi_0p2,-1);h_Mee_Background->Add(h_Mee__likepp_deltaPhi_0p2,-1);
		h_Mee_Background->SetTitle("Background;;");
		//计算信号=h_Mee__unlikeSame - h_Mee_Background
		TH1F* h_Mee_Signal = (TH1F *)h_Mee__unlikeSame__w_PhiV_Cut->Clone("h_Mee_Signal");h_Mee_Signal->Sumw2("kTRUE");h_Mee_Signal->Add(h_Mee_Background, -1);
		h_Mee_Signal->SetTitle("Signal(US-LS);;");
		// Compute PSAC correction using mixed-event like-sign (following CalMee.C)
		TH3F* h_Mee_Pt_Cen__likeppMixed = (TH3F *)inFile->Get("h_Mee_Pt_Cen__likeppMixed");
		TH3F* h_Mee_Pt_Cen__likemmMixed = (TH3F *)inFile->Get("h_Mee_Pt_Cen__likemmMixed");
		TH3F* h_Mee_Pt_Cen__unlikeMixed = (TH3F *)inFile->Get("h_Mee_Pt_Cen__unlikeMixed");
		
		TH1F* h_Mee_Background_Corrected = nullptr;
		TH1F* h_Mee_Signal_Corrected = nullptr;
		TH1F* h_Mee__CorrLS = nullptr;
		cout<<"1";
		if (h_Mee_Pt_Cen__likeppMixed && h_Mee_Pt_Cen__likemmMixed && h_Mee_Pt_Cen__unlikeMixed) {
			cout << "Mixed-event 3D histograms found. Computing PSAC correction..." << endl;
			// Project mixed-event 3D histograms to 1D (full pT & centrality)
			TH1F* h_Mee__likemmMixed = (TH1F*)h_Mee_Pt_Cen__likemmMixed->ProjectionX("h_Mee__likemmMixed", 1, -1, 1, -1);
			TH1F* h_Mee__likeppMixed = (TH1F*)h_Mee_Pt_Cen__likeppMixed->ProjectionX("h_Mee__likeppMixed", 1, -1, 1, -1);
			TH1F* h_Mee__unlikeMixed = (TH1F*)h_Mee_Pt_Cen__unlikeMixed->ProjectionX("h_Mee__unlikeMixed", 1, -1, 1, -1);
			if (h_Mee__likemmMixed->GetSumw2N() == 0) h_Mee__likemmMixed->Sumw2("kTRUE");
			if (h_Mee__likeppMixed->GetSumw2N() == 0) h_Mee__likeppMixed->Sumw2("kTRUE");
			if (h_Mee__unlikeMixed->GetSumw2N() == 0) h_Mee__unlikeMixed->Sumw2("kTRUE");
			
			// LikeMixed = 2 * sqrt(likemmMixed * likeppMixed)  [Geometric Average]
			TH1F* h_Mee__temp_mix = (TH1F*)h_Mee__likemmMixed->Clone("h_Mee__temp_mix");
			h_Mee__temp_mix->Multiply(h_Mee__likeppMixed);
			TH1F* h_Mee__LikeMixed = SqrtHist(h_Mee__temp_mix, "h_Mee__LikeMixed");
			h_Mee__LikeMixed->Scale(2.0);
			
			// PSAC correction factor: CorrLS = unlikeMixed / LikeMixed
			h_Mee__CorrLS = (TH1F*)h_Mee__unlikeMixed->Clone("h_Mee__CorrLS");
			h_Mee__CorrLS->Divide(h_Mee__LikeMixed);
			
			// Apply correction: Background_Corrected = Background * CorrLS
			h_Mee_Background_Corrected = (TH1F*)h_Mee_Background->Clone("h_Mee_Background_Corrected");if (h_Mee_Background_Corrected->GetSumw2N() == 0) h_Mee_Background_Corrected->Sumw2("kTRUE");
			h_Mee_Background_Corrected->Multiply(h_Mee__CorrLS);
			
			// Corrected signal = US - Background_Corrected
			h_Mee_Signal_Corrected = (TH1F*)h_Mee__unlikeSame__w_PhiV_Cut->Clone("h_Mee_Signal_Corrected");if (h_Mee_Signal_Corrected->GetSumw2N() == 0) h_Mee_Signal_Corrected->Sumw2("kTRUE");
			h_Mee_Signal_Corrected->Add(h_Mee_Background_Corrected, -1);
			h_Mee_Signal_Corrected->SetTitle("Signal(US-LS*PSAC);;");
			h_Mee_Signal_Corrected->SetLineColor(kMagenta);
			h_Mee_Signal_Corrected->SetMarkerStyle(23);
			h_Mee_Signal_Corrected->SetMarkerColor(kMagenta);
			//h_Mee_Signal_Corrected->SetMarkerSize(0.1);
			
			cout << "PSAC correction applied." << endl;
		} else {
			cout << "Mixed-event 3D histograms not found, skipping PSAC correction." << endl;
		}

		// Print bin 23 info
		// Int_t bin23 = 23;
		// cout << "--- Bin 23 Debug ---" << endl;
		// cout << "h_Mee__unlikeSame__w_PhiV_Cut: content = " << h_Mee__unlikeSame__w_PhiV_Cut->GetBinContent(bin23)
		// 	 << ", error = " << h_Mee__unlikeSame__w_PhiV_Cut->GetBinError(bin23) << endl;
		// cout << "h_Mee_Background:            content = " << h_Mee_Background->GetBinContent(bin23)
		// 	 << ", error = " << h_Mee_Background->GetBinError(bin23) << endl;
		// cout << "h_Mee__CorrLS:                content = " << h_Mee__CorrLS->GetBinContent(bin23)
		// 	 << ", error = " << h_Mee__CorrLS->GetBinError(bin23) << endl;
		// cout << "h_Mee_Background_Corrected:            content = " << h_Mee_Background_Corrected->GetBinContent(bin23)
		// 	 << ", error = " << h_Mee_Background_Corrected->GetBinError(bin23) << endl;
		// cout << "h_Mee_Signal:                content = " << h_Mee_Signal->GetBinContent(bin23)
		// 	 << ", error = " << h_Mee_Signal->GetBinError(bin23) << endl;
		// cout << "h_Mee_Signal_Corrected:      content = " << h_Mee_Signal_Corrected->GetBinContent(bin23)
		// 	 << ", error = " << h_Mee_Signal_Corrected->GetBinError(bin23) << endl;

		// Scale histograms to per-event
		h_Mee__unlikeSame__w_PhiV_Cut->Scale(kScale);
		h_Mee_Background->Scale(kScale);
		h_Mee_Signal->Scale(kScale);
		if (h_Mee_Signal_Corrected) h_Mee_Signal_Corrected->Scale(kScale);

		// Update titles to reflect per-event normalization
		h_Mee__unlikeSame__w_PhiV_Cut->SetTitle(";M_{ee} (GeV/c^{2});counts / N_{ev}");
		h_Mee_Background->SetTitle("Background;;");
		h_Mee_Signal->SetTitle("Signal(US-LS);;");
		if (h_Mee_Signal_Corrected) h_Mee_Signal_Corrected->SetTitle("Signal(US-LS*PSAC);;");

		//计算h_Ratio=h_Mee__likemm/h_Mee__likepp
		TH1F* h_Mee_Ratio = (TH1F *)h_Mee__likemm->Clone("h_Mee_Ratio");if (h_Mee_Ratio->GetSumw2N() == 0) h_Mee_Ratio->Sumw2("kTRUE");h_Mee_Ratio->Divide(h_Mee__likepp);
		Float_t ymax = 0.45e-3; // 100000 / Nevents
		Float_t ymin = -0.2e-3; // -40000 / Nevents

		// Calculate integral of h_Mee_Signal from 0 to 0.15
		Int_t binLow = h_Mee_Signal->GetXaxis()->FindBin(0.0 + 1e-6);
		Int_t binUp  = h_Mee_Signal->GetXaxis()->FindBin(0.15 - 1e-6);
		Double_t integral = h_Mee_Signal->Integral(binLow, binUp);
		Double_t integralErr = 0;
		for (Int_t i = binLow; i <= binUp; i++) {
			integralErr += h_Mee_Signal->GetBinError(i) * h_Mee_Signal->GetBinError(i);
		}
		integralErr = TMath::Sqrt(integralErr);
		cout << "--- Integral of h_Mee_Signal [0, 0.15] ---" << endl;
		cout << "Integral = " << integral << " +/- " << integralErr << endl;

		// //计算修正信号h_Mee_Signal2=h_Mee_Signal-h_Mee__unlike_deltaPhi_3p0
		// TH1F* h_Mee_Signal2 = (TH1F *)h_Mee_Signal->Clone("h_Mee_Signal2");if (h_Mee_Signal2->GetSumw2N() == 0) h_Mee_Signal2->Sumw2("kTRUE");h_Mee_Signal2->Add(h_Mee__unlike_deltaPhi_3p0, -1);
		// h_Mee_Signal2->SetTitle("Signal with deltaPhi cut correction;;");
		// TCanvas *c_Mee = new TCanvas("c_Mee", "c_Mee", 900, 800);
		// c_Mee->Divide(3, 3);
		// c_Mee->cd(1);
		// gPad->SetLogy(0);
		// gStyle->SetOptStat(0);
		// h_Mee__unlike_deltaPhi_3p0->SetMaximum(ymax);
		// h_Mee__unlike_deltaPhi_3p0->GetXaxis()->SetRangeUser(0,0.3);
		// h_Mee__unlike_deltaPhi_3p0->DrawClone("PE");
		// //h_Mee__likemm_deltaPhi_0p2->DrawClone("PE same");
		// //h_Mee__likepp_deltaPhi_0p2->DrawClone("PE same");

		// c_Mee->cd(2);
		// gPad->SetLogy(0);
		// gStyle->SetOptStat(0);
		// h_Mee__unlikeSame__w_PhiV_Cut->SetMaximum(ymax);
		// h_Mee__unlikeSame__w_PhiV_Cut->GetXaxis()->SetRangeUser(0,0.3);
		// h_Mee__unlikeSame__w_PhiV_Cut->DrawClone("PE");

		// c_Mee->cd(3);
		// gPad->SetLogy(0);
		// gStyle->SetOptStat(0);
		// h_Mee__unlikeSame->SetMaximum(ymax);
		// h_Mee__unlikeSame->GetXaxis()->SetRangeUser(0,0.3);
		// h_Mee__unlikeSame->DrawClone("PE");

		// c_Mee->cd(4);
		// gPad->SetLogy(0);
		// gStyle->SetOptStat(0);
		// h_Mee__likemm_deltaPhi_0p2->SetMaximum(ymax);
		// h_Mee__likemm_deltaPhi_0p2->GetXaxis()->SetRangeUser(0,0.3);
		// h_Mee__likemm_deltaPhi_0p2->DrawClone("PE");

		// c_Mee->cd(5);
		// gPad->SetLogy(0);
		// gStyle->SetOptStat(0);
		// h_Mee__likemm->SetMaximum(ymax);
		// h_Mee__likemm->GetXaxis()->SetRangeUser(0,0.3);
		// h_Mee__likemm->DrawClone("PE");

		// c_Mee->cd(6);
		// gPad->SetLogy(0);
		// gStyle->SetOptStat(0);
		// h_Mee_Background->SetMaximum(ymax);
		// h_Mee_Background->GetXaxis()->SetRangeUser(0,0.3);
		// h_Mee_Background->DrawClone("PE");

		// c_Mee->cd(7);
		// gPad->SetLogy(0);
		// gStyle->SetOptStat(0);
		// h_Mee__likepp_deltaPhi_0p2->SetMaximum(ymax);
		// h_Mee__likepp_deltaPhi_0p2->GetXaxis()->SetRangeUser(0,0.3);
		// h_Mee__likepp_deltaPhi_0p2->DrawClone("PE");

		// c_Mee->cd(8);
		// gPad->SetLogy(0);
		// gStyle->SetOptStat(0);
		// h_Mee__likepp->SetMaximum(ymax);
		// h_Mee__likepp->GetXaxis()->SetRangeUser(0,0.3);
		// h_Mee__likepp->DrawClone("PE");

		// c_Mee->cd(9);
		// gPad->SetLogy(0);
		// gStyle->SetOptStat(0);
		// h_Mee_Signal->SetMaximum(ymax);
		// h_Mee_Signal->GetXaxis()->SetRangeUser(0,0.3);
		// h_Mee_Signal->DrawClone("PE");
		// TLine *line = new TLine(0, 0, 0.3, 0);
		// line->SetLineColor(kRed);
		// line->DrawClone("same");
		
		// c_Mee->SaveAs(Form("roots/group4/%sMee_Distribution.png", outPrefix.Data()));


		// TCanvas *c_Mee2 = new TCanvas("c_Mee2", "c_Mee2", 800, 400);
		// c_Mee2->Divide(2);
		// c_Mee2->cd(1);
		// gPad->SetLogy(0);
		// gStyle->SetOptStat(0);
		// h_Mee_Signal2->SetMaximum(ymax);
		// h_Mee_Signal2->GetXaxis()->SetRangeUser(0,0.3);
		// h_Mee_Signal2->DrawClone("PE");
		// line->DrawClone("same");

		// c_Mee2->cd(2);
		// gPad->SetLogy(0);
		// gStyle->SetOptStat(0);
		// h_Mee_Ratio->GetXaxis()->SetRangeUser(0,0.3);
		// h_Mee_Ratio->DrawClone("PE");
		// TLine *line1 = new TLine(0, 1, 0.3, 1);
		// line1->SetLineColor(kRed);
		// line1->DrawClone("same");

		// Restore markers + line color + error bars
		h_Mee__unlikeSame__w_PhiV_Cut->SetLineColor(kBlack);
		h_Mee__unlikeSame__w_PhiV_Cut->SetMarkerStyle(20);
		h_Mee__unlikeSame__w_PhiV_Cut->SetMarkerColor(kBlack);
		//h_Mee__unlikeSame__w_PhiV_Cut->SetMarkerSize(0.1);

		h_Mee_Background->SetLineColor(kRed);
		h_Mee_Background->SetMarkerStyle(21);
		h_Mee_Background->SetMarkerColor(kRed);
		//h_Mee_Background->SetMarkerSize(0.1);

		h_Mee_Signal->SetLineColor(kBlue);
		h_Mee_Signal->SetMarkerStyle(22);
		h_Mee_Signal->SetMarkerColor(kBlue);
		//h_Mee_Signal->SetMarkerSize(0.1);

		// Canvas with 2 rows, 2 columns
		TCanvas *c_Mee3 = new TCanvas("c_Mee3", "c_Mee3", 1000, 900);
		c_Mee3->Divide(2, 2);

		// ---- Top-left: Mee distributions ----
		c_Mee3->cd(1);
		gPad->SetLogy(0);
		gStyle->SetOptStat(0);
		h_Mee__unlikeSame__w_PhiV_Cut->GetXaxis()->SetRangeUser(0, 0.3);
		h_Mee__unlikeSame__w_PhiV_Cut->SetMaximum(ymax);
		h_Mee__unlikeSame__w_PhiV_Cut->SetMinimum(ymin);
		h_Mee__unlikeSame__w_PhiV_Cut->SetTitle("M_{ee} distributions;M_{ee} (GeV/c^{2});counts / N_{ev}");
		h_Mee__unlikeSame__w_PhiV_Cut->DrawClone("PE");
		h_Mee_Background->DrawClone("PE same");
		h_Mee_Signal->DrawClone("PE same");
		if (h_Mee_Signal_Corrected) h_Mee_Signal_Corrected->DrawClone("PE same");

		TLegend *leg_Mee3 = new TLegend(0.55, 0.55, 0.85, 0.85);
		leg_Mee3->AddEntry(h_Mee__unlikeSame__w_PhiV_Cut, "Unlike Same with #phi_{V} cut", "lpe");
		leg_Mee3->AddEntry(h_Mee_Background, "Background", "lpe");
		leg_Mee3->AddEntry(h_Mee_Signal, "Signal(US-LS)", "lpe");
		if (h_Mee_Signal_Corrected) leg_Mee3->AddEntry(h_Mee_Signal_Corrected, "Signal(US-LS*PSAC)", "lpe");
		TPaveText *ptScale = new TPaveText(0.55, 0.48, 0.85, 0.55, "NDC NB");
		ptScale->SetFillStyle(0);
		ptScale->SetBorderSize(0);
		ptScale->SetTextFont(42);
		ptScale->SetTextSize(0.025);
		ptScale->AddText(Form("#times 1/%.1e", kLegendFactor));
		ptScale->DrawClone("same");

		// Draw integral value on canvas
		TPaveText *ptIntegral = new TPaveText(0.55, 0.10, 0.85, 0.27, "NDC NB");
		ptIntegral->SetFillStyle(0);
		ptIntegral->SetBorderSize(0);
		ptIntegral->SetTextFont(42);
		ptIntegral->SetTextSize(0.025);
		ptIntegral->AddText(Form("Integral [0,0.15] = %.4e #pm %.4e", integral, integralErr));
		ptIntegral->DrawClone("same");

		leg_Mee3->SetTextSize(0.03);
		leg_Mee3->SetBorderSize(0);
		leg_Mee3->SetFillStyle(0);
		leg_Mee3->DrawClone("same");

		TLine *line_Mee3 = new TLine(0, 0, 0.3, 0);
		line_Mee3->SetLineColor(kGreen);
		line_Mee3->DrawClone("same");

		// ---- Top-right: PSAC vs Mee ----
		c_Mee3->cd(2);
		gPad->SetLogy(0);
		gStyle->SetOptStat(0);
		if (h_Mee__CorrLS) {
			h_Mee__CorrLS->SetTitle("PSAC correction;M_{ee} (GeV/c^{2});#LT UM #GT / #LT LM #GT");
			h_Mee__CorrLS->SetMaximum(1.05);
			h_Mee__CorrLS->SetMinimum(0.95);
			h_Mee__CorrLS->GetXaxis()->SetRangeUser(0, 0.3);
			h_Mee__CorrLS->SetMarkerStyle(20);
			h_Mee__CorrLS->SetMarkerSize(0.6);
			h_Mee__CorrLS->SetLineColor(kMagenta);
			h_Mee__CorrLS->SetMarkerColor(kMagenta);
			h_Mee__CorrLS->DrawClone("PE");
			TLine *line_PSAC = new TLine(0, 1, 0.3, 1);
			line_PSAC->SetLineColor(kRed);
			line_PSAC->SetLineStyle(2);
			line_PSAC->DrawClone("same");
		}

		// ---- Bottom-left: signal significance (# sigma below zero) ----
		c_Mee3->cd(3);
		gPad->SetLogy(0);
		gStyle->SetOptStat(0);

		// Clone original signal significance
		TH1F *h_Signal_Sigma = (TH1F *)h_Mee_Signal->Clone("h_Signal_Sigma");
		h_Signal_Sigma->SetTitle("Signal significance;M_{ee} (GeV/c^{2});#sigma below zero");
		Int_t nb = h_Signal_Sigma->GetNbinsX();
		for (Int_t i = 1; i <= nb; i++) {
			Double_t val = h_Mee_Signal->GetBinContent(i);
			Double_t err = h_Mee_Signal->GetBinError(i);
			if (val < 0) {
				//cout << "Bin " << i << ": val = " << val << ", err = " << err << endl;
				h_Signal_Sigma->SetBinContent(i, TMath::Abs(val) / err);
			} else {
				h_Signal_Sigma->SetBinContent(i, 0);
			}
			h_Signal_Sigma->SetBinError(i, 0);
		}
		h_Signal_Sigma->GetXaxis()->SetRangeUser(0, 0.3);
		h_Signal_Sigma->SetMaximum(100);
		h_Signal_Sigma->SetMinimum(0);
		h_Signal_Sigma->SetMarkerStyle(20);
		h_Signal_Sigma->SetMarkerSize(0.6);
		h_Signal_Sigma->SetLineColor(kBlue);
		h_Signal_Sigma->SetMarkerColor(kBlue);
		h_Signal_Sigma->DrawClone("PE");

		// ---- Bottom-right: corrected signal significance (# sigma below zero) ----
		c_Mee3->cd(4);
		gPad->SetLogy(0);
		gStyle->SetOptStat(0);
		if (h_Mee_Signal_Corrected) {
			TH1F *h_Corrected_Sigma = (TH1F *)h_Mee_Signal_Corrected->Clone("h_Corrected_Sigma");
			h_Corrected_Sigma->SetTitle("Corrected signal significance;M_{ee} (GeV/c^{2});#sigma below zero");
			for (Int_t i = 1; i <= nb; i++) {
				Double_t val = h_Mee_Signal_Corrected->GetBinContent(i);
				Double_t err = h_Mee_Signal_Corrected->GetBinError(i);
				if (val < 0 && err > 0) {
					//cout << "Bin " << i << ": val = " << val << ", err = " << err << endl;
					h_Corrected_Sigma->SetBinContent(i, TMath::Abs(val) / err);
				} else {
					h_Corrected_Sigma->SetBinContent(i, 0);
				}
				h_Corrected_Sigma->SetBinError(i, 0);
			}
			h_Corrected_Sigma->GetXaxis()->SetRangeUser(0, 0.3);
			h_Corrected_Sigma->SetMaximum(100);
			h_Corrected_Sigma->SetMinimum(0);
			h_Corrected_Sigma->SetMarkerStyle(23);
			h_Corrected_Sigma->SetMarkerSize(0.6);
			h_Corrected_Sigma->SetLineColor(kMagenta);
			h_Corrected_Sigma->SetMarkerColor(kMagenta);
			h_Corrected_Sigma->DrawClone("PE");
		} else {
			TPaveText *pt_empty = new TPaveText(0.3, 0.45, 0.7, 0.55, "NDC");
			pt_empty->AddText("No PSAC correction");
			pt_empty->SetFillStyle(0);
			pt_empty->SetBorderSize(0);
			pt_empty->DrawClone();
		}

		c_Mee3->SaveAs(Form("roots/group4/%sMee_Signal_Overlay.png", outPrefix.Data()));
	}
	}
