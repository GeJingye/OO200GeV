//---------------------从OO.root中提取直方图，并进行设置更改和元素添加----------------------
#include "someFunction.h"
// 16_20260116_TOFElectron_newPhiVcut_P24ia;17_20260116_TOFElectron_newPhiVcut_P24iy;23_20260118_iTPC_withLowP_newPhiVcut;24_20260117_iTPC_rmLowEta0p1_newPhiVcut;25_20260119_iTPC_rmLowP_newPhiVcut;
void ElectronDistribution(TString inFilename = "roots/59_20260526_OO_iTPC11_PureE_mb.root", Int_t number = 59) //
{
	// 从root文件中导入待拟合的直方图
	TFile *inFile = new TFile(inFilename);
	if (!inFile){
		cout << "The input file is not found! Exiting..." << endl;
		return;
	}
	TH2F * h_e_p__Number_Same = (TH2F *)inFile->Get("h_e_p__Number_Same");if(!h_e_p__Number_Same) {cout<<"h_e_p__Number_Same not found! Exiting..."<<endl; return;}
    //cout<<"Same Event:"<<endl;printHist2DSci(h_e_p__Number_Same);
	TH2F * h_e_p__Number_Mix = (TH2F *)inFile->Get("h_e_p__Number_Mix");if(!h_e_p__Number_Mix) {cout<<"h_e_p__Number_Mix not found! Exiting..."<<endl; return;}
	//cout<<"Mixed Event:"<<endl;printHist2DSci(h_e_p__Number_Mix);
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

	TH1F* h_Qinv__likemm = (TH1F *)inFile->Get("h_Qinv__likemm");//h_Qinv__likemm->Rebin(5);
	TH1F* h_Qinv__likepp = (TH1F *)inFile->Get("h_Qinv__likepp");//h_Qinv__likepp->Rebin(5);
	TH1F* h_Qinv__likemmMixed = (TH1F *)inFile->Get("h_Qinv__likemmMixed");h_Qinv__likemmMixed->Scale(1/100.0);//h_Qinv__likemmMixed->Rebin(5);
	TH1F* h_Qinv__likeppMixed = (TH1F *)inFile->Get("h_Qinv__likeppMixed");h_Qinv__likeppMixed->Scale(1/100.0);//h_Qinv__likeppMixed->Rebin(5);
	
    if (0) // ep number
	{
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

		c_sum->SaveAs(Form("roots/%d_epNumber.png", number));
	}
    if (1) // e+ e- pt/eta/phi distribution phiV cut check
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
		gPad->SetLogy(1);
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
		gPad->SetLogy(1);
		// h_eta__e_PhiV_Cut->GetXaxis()->SetRangeUser(-2.0, 2.0);
		h_eta__e_PhiV_Cut->DrawClone();
		h_eta__e->DrawClone("same");

		c_PhiV->cd(3);
		gPad->SetLogy(1);
		// h_phi__e_PhiV_Cut->GetXaxis()->SetRangeUser(-3.2, 3.2);
		h_phi__e_PhiV_Cut->DrawClone();
		h_phi__e->DrawClone("same");

		c_PhiV->cd(4);
		gPad->SetLogy(1);
		// h_pT__e_PhiV_Cut->GetXaxis()->SetRangeUser(0., 0.5);
		h_pT__p_PhiV_Cut->DrawClone();
		h_pT__p->DrawClone("same");
		leg1->DrawClone("same");

		c_PhiV->cd(5);
		gPad->SetLogy(1);
		// h_eta__p_PhiV_Cut->GetXaxis()->SetRangeUser(-2.0, 2.0);
		h_eta__p_PhiV_Cut->DrawClone();
		h_eta__p->DrawClone("same");

		c_PhiV->cd(6);
		gPad->SetLogy(1);
		// h_phi__p_PhiV_Cut->GetXaxis()->SetRangeUser(-3.2, 3.2);
		h_phi__p_PhiV_Cut->DrawClone();
		h_phi__p->DrawClone("same");

		c_PhiV->SaveAs(Form("roots/%d_ep_pt_eta_phi_PhiVcut.png", number));
	}	
	if (1) // e+ e- pt/eta/phi distribution track QA
	{
        //计算ratio e-/e+
        TH1F *h_pT_ratio_EperP = (TH1F *)h_pT__e_PhiV_Cut->Clone("h_pT_ratio_EperP");
        TH1F *h_eta_ratio_EperP = (TH1F *)h_eta__e_PhiV_Cut->Clone("h_eta_ratio_EperP");
        TH1F *h_phi_ratio_EperP = (TH1F *)h_phi__e_PhiV_Cut->Clone("h_phi_ratio_EperP");
        h_pT_ratio_EperP->SetTitle("e^{-}/e^{+} p_{T} ratio;p_{T} [GeV/c];Ratio");
        h_pT_ratio_EperP->Divide(h_pT__p_PhiV_Cut);
        h_eta_ratio_EperP->SetTitle("e^{-}/e^{+} #eta ratio;#eta [GeV/c];Ratio");
        h_eta_ratio_EperP->Divide(h_eta__p_PhiV_Cut);
        h_phi_ratio_EperP->SetTitle("e^{-}/e^{+} #phi ratio;#phi [GeV/c];Ratio");
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
		gPad->SetLogy(1);
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
		gPad->SetLogy(1);
		// h_eta__e_PhiV_Cut->GetXaxis()->SetRangeUser(-2.0, 2.0);
		h_eta__e_PhiV_Cut->Draw();
		h_eta__p_PhiV_Cut->Draw("same");

		c_eTrack->cd(3);
		gPad->SetLogy(1);
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

		c_eTrack->SaveAs(Form("roots/%d_ep_TrackQA.png", number));
		// delete c_eTrack;
	}
	if (1)// Delta Pt, Delta Eta, Delta Phi correlation analysis
	{
		//画图
		TCanvas *c1 = new TCanvas("c1", "c1", 1200, 800);
		c1->Divide(4, 3);

		c1->cd(1);
		gPad->SetLogz(1);
		gStyle->SetOptStat(0);
		h_DeltaPt__likemm->SetTitle("#Delta p_{T} of e^{-}e^{-};");
		h_DeltaPt__likemm->GetXaxis()->SetRangeUser(-0.1,0.1);
		h_DeltaPt__likemm->DrawClone("col z");

		c1->cd(2);
		gPad->SetLogz(1);
		gStyle->SetOptStat(0);
		h_DeltaPt__likepp->SetTitle("#Delta p_{T} of e^{+}e^{+};");
		h_DeltaPt__likepp->GetXaxis()->SetRangeUser(-0.1,0.1);
		h_DeltaPt__likepp->DrawClone("col z");

		c1->cd(3);
		gPad->SetLogz(1);
		gStyle->SetOptStat(0);
		h_DeltaPt__unlikeSame->SetTitle("#Delta p_{T} of e^{-}e^{+};");
		h_DeltaPt__unlikeSame->GetXaxis()->SetRangeUser(-0.1,0.1);
		h_DeltaPt__unlikeSame->DrawClone("col z");

		c1->cd(4);
		gPad->SetLogz(1);
		gStyle->SetOptStat(0);
		h_DeltaPt__unlikeMixed->SetTitle("#Delta p_{T} of e^{-}e^{+} in Mixed Events;");
		h_DeltaPt__unlikeMixed->GetXaxis()->SetRangeUser(-0.1,0.1);
		h_DeltaPt__unlikeMixed->DrawClone("col z");

		c1->cd(5);
		gPad->SetLogz(1);
		gStyle->SetOptStat(0);
		h_DeltaEta__likemm->SetTitle("#Delta #eta of e^{-}e^{-};");
		h_DeltaEta__likemm->DrawClone("col z");

		c1->cd(6);
		gPad->SetLogz(1);
		gStyle->SetOptStat(0);
		h_DeltaEta__likepp->SetTitle("#Delta #eta of e^{+}e^{+};");
		h_DeltaEta__likepp->DrawClone("col z");

		c1->cd(7);
		gPad->SetLogz(1);
		gStyle->SetOptStat(0);
		h_DeltaEta__unlikeSame->SetTitle("#Delta #eta of e^{-}e^{+};");
		h_DeltaEta__unlikeSame->DrawClone("col z");

		c1->cd(8);
		gPad->SetLogz(1);
		gStyle->SetOptStat(0);
		h_DeltaEta__unlikeMixed->SetTitle("#Delta #eta of e^{-}e^{+} in Mixed Events;");
		h_DeltaEta__unlikeMixed->DrawClone("col z");

		c1->cd(9);
		gPad->SetLogz(1);
		gStyle->SetOptStat(0);
		h_DeltaPhi__likemm->SetTitle("#Delta #phi of e^{-}e^{-};");
		h_DeltaPhi__likemm->DrawClone("col z");

		c1->cd(10);
		gPad->SetLogz(1);
		gStyle->SetOptStat(0);
		h_DeltaPhi__likepp->SetTitle("#Delta #phi of e^{+}e^{+};");
		h_DeltaPhi__likepp->DrawClone("col z");

		c1->cd(11);
		gPad->SetLogz(1);
		gStyle->SetOptStat(0);
		h_DeltaPhi__unlikeSame->SetTitle("#Delta #phi of e^{-}e^{+};");
		h_DeltaPhi__unlikeSame->DrawClone("col z");

		c1->cd(12);
		gPad->SetLogz(1);
		gStyle->SetOptStat(0);
		h_DeltaPhi__unlikeMixed->SetTitle("#Delta #phi of e^{-}e^{+} in Mixed Events;");
		h_DeltaPhi__unlikeMixed->DrawClone("col z");

		c1->SaveAs(Form("roots/%d_Delta_Pt_Eta_Phi.png", number));
	}
	if (1)// invariant Q correlation analysis
	{
		TH1F *h_Qinv_SamePerMix__likemm = (TH1F *)h_Qinv__likemm->Clone("h_Qinv_SamePerMix__likemm");
		TH1F *h_Qinv_SamePerMix__likepp = (TH1F *)h_Qinv__likepp->Clone("h_Qinv_SamePerMix__likepp");
		h_Qinv_SamePerMix__likemm->Divide(h_Qinv__likemmMixed);
		h_Qinv_SamePerMix__likepp->Divide(h_Qinv__likeppMixed);

		h_Qinv_SamePerMix__likepp->SetTitle("Invariant Q of e^{+}e^{+} ratio of same-event over mixed-event;invariant Q (GeV/c);Ratio");
		h_Qinv_SamePerMix__likemm->SetTitle("Invariant Q of e^{-}e^{-} ratio of same-event over mixed-event;invariant Q (GeV/c);Ratio");
		TCanvas *c2 = new TCanvas("c2", "c2", 1200, 600);
		c2->Divide(3, 2);

		c2->cd(1);
		gPad->SetLogy(1);
		gStyle->SetOptStat(0);
		h_Qinv__likemm->GetXaxis()->SetRangeUser(0,0.3);
		h_Qinv__likemm->DrawClone("PE");

		c2->cd(2);
		gPad->SetLogy(1);
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

		c2->cd(4);
		gPad->SetLogy(1);
		gStyle->SetOptStat(0);
		h_Qinv__likepp->GetXaxis()->SetRangeUser(0,0.3);
		h_Qinv__likepp->DrawClone("PE");

		c2->cd(5);
		gPad->SetLogy(1);
		gStyle->SetOptStat(0);
		h_Qinv__likeppMixed->GetXaxis()->SetRangeUser(0,0.3);
		h_Qinv__likeppMixed->DrawClone("PE");

		c2->cd(6);
		gPad->SetLogy(0);
		gStyle->SetOptStat(0);
		h_Qinv_SamePerMix__likepp->GetXaxis()->SetRangeUser(0,0.3);
		h_Qinv_SamePerMix__likepp->SetMaximum(8);
		h_Qinv_SamePerMix__likepp->SetMinimum(0);
		//h_Qinv_SamePerMix__likepp->GetYaxis()->SetNdivisions(505);
		h_Qinv_SamePerMix__likepp->DrawClone("PE");

		c2->SaveAs(Form("roots/%d_Invariant_Q.png", number));
	}	

}