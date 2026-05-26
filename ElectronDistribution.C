//---------------------从OO.root中提取直方图，并进行设置更改和元素添加----------------------
#include "someFunction.h"
// 16_20260116_TOFElectron_newPhiVcut_P24ia;17_20260116_TOFElectron_newPhiVcut_P24iy;23_20260118_iTPC_withLowP_newPhiVcut;24_20260117_iTPC_rmLowEta0p1_newPhiVcut;25_20260119_iTPC_rmLowP_newPhiVcut;
void ElectronDistribution(TString inFilename = "roots/25_20260119_iTPC_rmLowP_newPhiVcut.root", Int_t number = 25) //
{
	// 从root文件中导入待拟合的直方图
	TFile *inFile = new TFile(inFilename);
	if (!inFile)
	{
		cout << "The input file is not found! Exiting..." << endl;
		return;
	}
    TH3F *h_pT_Eta_Phi__EIDcut_3_lowP_e = (TH3F *)inFile->Get("h_pT_Eta_Phi__EIDcut_3_lowP_e");if(!h_pT_Eta_Phi__EIDcut_3_lowP_e) {cout<<"h_pT_Eta_Phi__EIDcut_3_lowP_e not found! Exiting..."<<endl; return;}
	TH3F *h_pT_Eta_Phi__EIDcut_3_lowP_p = (TH3F *)inFile->Get("h_pT_Eta_Phi__EIDcut_3_lowP_p");if(!h_pT_Eta_Phi__EIDcut_3_lowP_p) {cout<<"h_pT_Eta_Phi__EIDcut_3_lowP_p not found! Exiting..."<<endl; return;}
    // sum
	TH2F * h_e_p__Number_Same = (TH2F *)inFile->Get("h_e_p__Number_Same");if(!h_e_p__Number_Same) {cout<<"h_e_p__Number_Same not found! Exiting..."<<endl; return;}
    //cout<<"Same Event:"<<endl;printHist2DSci(h_e_p__Number_Same);
	TH2F * h_e_p__Number_Mix = (TH2F *)inFile->Get("h_e_p__Number_Mix");if(!h_e_p__Number_Mix) {cout<<"h_e_p__Number_Mix not found! Exiting..."<<endl; return;}
	//cout<<"Mixed Event:"<<endl;printHist2DSci(h_e_p__Number_Mix);
	TH1F *h_pT__electrons = (TH1F *)inFile->Get("h_pT__electrons");if(!h_pT__electrons) {cout<<"h_pT__electrons not found! Exiting..."<<endl; return;}
	TH1F *h_eta__electrons = (TH1F *)inFile->Get("h_eta__electrons");if(!h_eta__electrons) {cout<<"h_eta__electrons not found! Exiting..."<<endl; return;}
	TH1F *h_phi__electrons = (TH1F *)inFile->Get("h_phi__electrons");if(!h_phi__electrons) {cout<<"h_phi__electrons not found! Exiting..."<<endl; return;}
	TH1F *h_pT__positrons = (TH1F *)inFile->Get("h_pT__positrons");if(!h_pT__positrons) {cout<<"h_pT__positrons not found! Exiting..."<<endl; return;}
	TH1F *h_eta__positrons = (TH1F *)inFile->Get("h_eta__positrons");if(!h_eta__positrons) {cout<<"h_eta__positrons not found! Exiting..."<<endl; return;}
	TH1F *h_phi__positrons = (TH1F *)inFile->Get("h_phi__positrons");if(!h_phi__positrons) {cout<<"h_phi__positrons not found! Exiting..."<<endl; return;}

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
	
    if (0) // e+ e- track QA
	{
        //投影到1维直方图
        TH1F *h_pT__3_lowP_e = (TH1F *)h_pT_Eta_Phi__EIDcut_3_lowP_e->Project3D("x");
        TH1F *h_eta__3_lowP_e = (TH1F *)h_pT_Eta_Phi__EIDcut_3_lowP_e->Project3D("y");
        TH1F *h_phi__3_lowP_e = (TH1F *)h_pT_Eta_Phi__EIDcut_3_lowP_e->Project3D("z");
        TH1F *h_pT__3_lowP_p = (TH1F *)h_pT_Eta_Phi__EIDcut_3_lowP_p->Project3D("x");
        TH1F *h_eta__3_lowP_p = (TH1F *)h_pT_Eta_Phi__EIDcut_3_lowP_p->Project3D("y");
        TH1F *h_phi__3_lowP_p = (TH1F *)h_pT_Eta_Phi__EIDcut_3_lowP_p->Project3D("z");
        TH1F *h_pT_ratio_EperP_3 = (TH1F *)h_pT__3_lowP_e->Clone("h_pT_ratio_EperP_3");
        //计算ratio e-/e+ in group4
        h_pT_ratio_EperP_3->SetTitle("e^{-}/e^{+} p_{T} ratio;p_{T} [GeV/c];Ratio");
        h_pT_ratio_EperP_3->Divide(h_pT__3_lowP_p);
        TH1F *h_eta_ratio_EperP_3 = (TH1F *)h_eta__3_lowP_e->Clone("h_eta_ratio_EperP_3");
        h_eta_ratio_EperP_3->SetTitle("e^{-}/e^{+} #eta ratio;#eta [GeV/c];Ratio");
        h_eta_ratio_EperP_3->Divide(h_eta__3_lowP_p);
        TH1F *h_phi_ratio_EperP_3 = (TH1F *)h_phi__3_lowP_e->Clone("h_phi_ratio_EperP_3");
        h_phi_ratio_EperP_3->SetTitle("e^{-}/e^{+} #phi ratio;#phi [GeV/c];Ratio");
        h_phi_ratio_EperP_3->Divide(h_phi__3_lowP_p);
        //计算ratio e-/e+ in group1
        TH1F *h_pT_ratio_EperP = (TH1F *)h_pT__electrons->Clone("h_pT_ratio_EperP");
        TH1F *h_eta_ratio_EperP = (TH1F *)h_eta__electrons->Clone("h_eta_ratio_EperP");
        TH1F *h_phi_ratio_EperP = (TH1F *)h_phi__electrons->Clone("h_phi_ratio_EperP");
        h_pT_ratio_EperP->SetTitle("e^{-}/e^{+} p_{T} ratio;p_{T} [GeV/c];Ratio");
        h_pT_ratio_EperP->Divide(h_pT__positrons);
        h_eta_ratio_EperP->SetTitle("e^{-}/e^{+} #eta ratio;#eta [GeV/c];Ratio");
        h_eta_ratio_EperP->Divide(h_eta__positrons);
        h_phi_ratio_EperP->SetTitle("e^{-}/e^{+} #phi ratio;#phi [GeV/c];Ratio");
        h_phi_ratio_EperP->Divide(h_phi__positrons);
        //设置格式
		h_pT__positrons->SetLineColor(kRed);
		h_eta__positrons->SetLineColor(kRed);
		h_phi__positrons->SetLineColor(kRed);
		h_pT__3_lowP_p->SetLineColor(kRed);
		h_eta__3_lowP_p->SetLineColor(kRed);
		h_phi__3_lowP_p->SetLineColor(kRed);
        //画图
		TCanvas *c_eTrack = new TCanvas("c_eTrack", "c_eTrack", 600, 900);
		c_eTrack->Divide(3, 4);
		c_eTrack->cd(1);
		gPad->SetLogy(1);
		TLegend *leg1 = new TLegend(0.65, 0.75, 0.88, 0.88);
		leg1->AddEntry(h_pT__electrons, "e^{-} (electrons)", "l");
		leg1->AddEntry(h_pT__positrons, "e^{+} (positrons)", "l");
		leg1->SetBorderSize(0);
		leg1->SetFillStyle(0);
		// h_pT__electrons->GetXaxis()->SetRangeUser(0., 0.5);
		h_pT__electrons->Draw();
		h_pT__positrons->Draw("same");
		leg1->Draw("same");

		c_eTrack->cd(2);
		gPad->SetLogy(1);
		// h_eta__electrons->GetXaxis()->SetRangeUser(-2.0, 2.0);
		h_eta__electrons->Draw();
		h_eta__positrons->Draw("same");

		c_eTrack->cd(3);
		gPad->SetLogy(1);
		// h_phi__electrons->GetXaxis()->SetRangeUser(-3.2, 3.2);
		h_phi__electrons->Draw();
		h_phi__positrons->Draw("same");

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

		c_eTrack->cd(7);
		gPad->SetLogy(1);
		TLegend *leg2 = new TLegend(0.65, 0.75, 0.88, 0.88);
		leg2->AddEntry(h_pT__3_lowP_e, "e^{-} (electrons)", "l");
		leg2->AddEntry(h_pT__3_lowP_p, "e^{+} (positrons)", "l");
		leg2->SetBorderSize(0);
		leg2->SetFillStyle(0);
		// h_pT__3_lowP_e->GetXaxis()->SetRangeUser(0., 0.5);
		h_pT__3_lowP_e->Draw();
		h_pT__3_lowP_p->Draw("same");
		leg2->Draw("same");

		c_eTrack->cd(8);
		gPad->SetLogy(1);
		// h_eta__3_lowP_e->GetXaxis()->SetRangeUser(-2.0, 2.0);
		h_eta__3_lowP_e->Draw();
		h_eta__3_lowP_p->Draw("same");

		c_eTrack->cd(9);
		gPad->SetLogy(1);
		// h_phi__3_lowP_e->GetXaxis()->SetRangeUser(-3.2, 3.2);
		h_phi__3_lowP_e->Draw();
		h_phi__3_lowP_p->Draw("same");

		c_eTrack->cd(10);
		gPad->SetLogy(0);
		gStyle->SetOptStat(0);
		h_pT_ratio_EperP_3->SetMaximum(2);
		h_pT_ratio_EperP_3->SetMinimum(0.5);
		h_pT_ratio_EperP_3->Draw();

		c_eTrack->cd(11);
		gPad->SetLogy(0);
		gStyle->SetOptStat(0);
		h_eta_ratio_EperP_3->SetMaximum(2);
		h_eta_ratio_EperP_3->SetMinimum(0.5);
		h_eta_ratio_EperP_3->Draw();

		c_eTrack->cd(12);
		gPad->SetLogy(0);
		gStyle->SetOptStat(0);
		h_phi_ratio_EperP_3->SetMaximum(2);
		h_phi_ratio_EperP_3->SetMinimum(0.5);
		h_phi_ratio_EperP_3->Draw();

		c_eTrack->SaveAs(Form("roots/%d_ep_TrackQA.png", number));
		// delete c_eTrack;
	}
	

}