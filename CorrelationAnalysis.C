//---------------------从OO.root中提取直方图，并进行分析----------------------
#include "someFunction.h"
void CorrelationAnalysis(TString inFilename = "roots/57_20260517_iTPCmodel44_DeltaPtEtaPhi_Qinv.root", Int_t number = 57) //
{
	// 从root文件中导入待拟合的直方图
	TFile *inFile = new TFile(inFilename);
	if (!inFile)
	{
		cout << "The input file is not found! Exiting..." << endl;
		return;
	}
	TH1F* h_Qinv__likemm = (TH1F *)inFile->Get("h_Qinv__likemm");//h_Qinv__likemm->Rebin(5);
	TH1F* h_Qinv__likepp = (TH1F *)inFile->Get("h_Qinv__likepp");//h_Qinv__likepp->Rebin(5);
	TH1F* h_Qinv__likemmMixed = (TH1F *)inFile->Get("h_Qinv__likemmMixed");h_Qinv__likemmMixed->Scale(1/100.0);//h_Qinv__likemmMixed->Rebin(5);
	TH1F* h_Qinv__likeppMixed = (TH1F *)inFile->Get("h_Qinv__likeppMixed");h_Qinv__likeppMixed->Scale(1/100.0);//h_Qinv__likeppMixed->Rebin(5);
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

	TH1F *h_Qinv_SamePerMix__likemm = (TH1F *)h_Qinv__likemm->Clone("h_Qinv_SamePerMix__likemm");
	h_Qinv_SamePerMix__likemm->Divide(h_Qinv__likemmMixed);
	TH1F *h_Qinv_SamePerMix__likepp = (TH1F *)h_Qinv__likepp->Clone("h_Qinv_SamePerMix__likepp");
	h_Qinv_SamePerMix__likepp->Divide(h_Qinv__likeppMixed);
	//cout<<h_Qinv__likemm->GetBinContent(0)<<endl;
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
	if (0)// invariant Q correlation analysis
	{
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