//---------------------从OO.root中提取直方图，并进行设置更改和元素添加----------------------
#include "someFunction.h"
void CalPt(TString inFileName = "roots/61_20260603_OO_iTPC1p2p3p4_PureE_mb.root", Int_t number = 61){
	// 定义新的bin边界
	//vector<Double_t> Mee__newEdges = {0, 0.005, 0.01, 0.015, 0.02, 0.025, 0.03, 0.035, 0.04, 0.045, 0.05, 0.055, 0.06, 0.065, 0.07, 0.075, 0.08, 0.085, 0.09, 0.095, 0.1, 0.105, 0.11, 0.115, 0.12, 0.125, 0.13, 0.135, 0.14, 0.145, 0.15, 0.155, 0.16, 0.165, 0.17, 0.175, 0.18, 0.185, 0.19, 0.195, 0.2, 0.205, 0.21, 0.215, 0.22, 0.225, 0.23, 0.235, 0.24, 0.245, 0.25, 0.255, 0.26, 0.265, 0.27, 0.275, 0.28, 0.285, 0.29, 0.295, 0.3, 0.305, 0.31, 0.315, 0.32, 0.325, 0.33, 0.335, 0.34, 0.345, 0.35, 0.355, 0.36, 0.365, 0.37, 0.375, 0.38, 0.385, 0.39, 0.395, 0.4, 0.405, 0.41, 0.415, 0.42, 0.425, 0.43, 0.435, 0.44, 0.445, 0.45, 0.455, 0.46, 0.465, 0.47, 0.475, 0.48, 0.485, 0.49, 0.495, 0.5, 0.505, 0.51, 0.515, 0.52, 0.525, 0.53, 0.535, 0.54, 0.545, 0.55, 0.555, 0.56, 0.565, 0.57, 0.575, 0.58, 0.585, 0.59, 0.595, 0.6, 0.605, 0.61, 0.615, 0.62, 0.625, 0.63, 0.635, 0.64, 0.645, 0.65, 0.655, 0.66, 0.665, 0.67, 0.675, 0.68, 0.685, 0.69, 0.695, 0.7, 0.705, 0.71, 0.715, 0.72, 0.725, 0.73, 0.735, 0.74, 0.745, 0.75, 0.755, 0.76, 0.765, 0.77, 0.775, 0.78, 0.785, 0.79, 0.795, 0.8, 0.805, 0.81, 0.815, 0.82, 0.825, 0.83, 0.835, 0.84, 0.845, 0.85, 0.855, 0.86, 0.865, 0.87, 0.875, 0.88, 0.885, 0.89, 0.895, 0.9, 0.905, 0.91, 0.915, 0.92, 0.925, 0.93, 0.935, 0.94, 0.945, 0.95, 0.955, 0.96, 0.965, 0.97, 0.975, 0.98, 0.985, 0.99, 0.995, 1, 1.005, 1.01, 1.015, 1.02, 1.025, 1.03, 1.035, 1.04, 1.045, 1.05, 1.055, 1.06, 1.065, 1.07, 1.075, 1.08, 1.085, 1.09, 1.095, 1.1, 1.105, 1.11, 1.115, 1.12, 1.125, 1.13, 1.135, 1.14, 1.145, 1.15, 1.155, 1.16, 1.165, 1.17, 1.175, 1.18, 1.185, 1.19, 1.195, 1.2, 1.205, 1.21, 1.215, 1.22, 1.225, 1.23, 1.235, 1.24, 1.245, 1.25, 1.255, 1.26, 1.265, 1.27, 1.275, 1.28, 1.285, 1.29, 1.295, 1.3, 1.305, 1.31, 1.315, 1.32, 1.325, 1.33, 1.335, 1.34, 1.345, 1.35, 1.355, 1.36, 1.365, 1.37, 1.375, 1.38, 1.385, 1.39, 1.395, 1.4, 1.405, 1.41, 1.415, 1.42, 1.425, 1.43, 1.435, 1.44, 1.445, 1.45, 1.455, 1.46, 1.465, 1.47, 1.475, 1.48, 1.485, 1.49, 1.495, 1.5, 1.505, 1.51, 1.515, 1.52, 1.525, 1.53, 1.535, 1.54, 1.545, 1.55, 1.555, 1.56, 1.565, 1.57, 1.575, 1.58, 1.585, 1.59, 1.595, 1.6, 1.605, 1.61, 1.615, 1.62, 1.625, 1.63, 1.635, 1.64, 1.645, 1.65, 1.655, 1.66, 1.665, 1.67, 1.675, 1.68, 1.685, 1.69, 1.695, 1.7, 1.705, 1.71, 1.715, 1.72, 1.725, 1.73, 1.735, 1.74, 1.745, 1.75, 1.755, 1.76, 1.765, 1.77, 1.775, 1.78, 1.785, 1.79, 1.795, 1.8, 1.805, 1.81, 1.815, 1.82, 1.825, 1.83, 1.835, 1.84, 1.845, 1.85, 1.855, 1.86, 1.865, 1.87, 1.875, 1.88, 1.885, 1.89, 1.895, 1.9, 1.905, 1.91, 1.915, 1.92, 1.925, 1.93, 1.935, 1.94, 1.945, 1.95, 1.955, 1.96, 1.965, 1.97, 1.975, 1.98, 1.985, 1.99, 1.995, 2, 2.005, 2.01, 2.015, 2.02, 2.025, 2.03, 2.035, 2.04, 2.045, 2.05, 2.055, 2.06, 2.065, 2.07, 2.075, 2.08, 2.085, 2.09, 2.095, 2.1, 2.105, 2.11, 2.115, 2.12, 2.125, 2.13, 2.135, 2.14, 2.145, 2.15, 2.155, 2.16, 2.165, 2.17, 2.175, 2.18, 2.185, 2.19, 2.195, 2.2, 2.205, 2.21, 2.215, 2.22, 2.225, 2.23, 2.235, 2.24, 2.245, 2.25, 2.255, 2.26, 2.265, 2.27, 2.275, 2.28, 2.285, 2.29, 2.295, 2.3, 2.305, 2.31, 2.315, 2.32, 2.325, 2.33, 2.335, 2.34, 2.345, 2.35, 2.355, 2.36, 2.365, 2.37, 2.375, 2.38, 2.385, 2.39, 2.395, 2.4, 2.405, 2.41, 2.415, 2.42, 2.425, 2.43, 2.435, 2.44, 2.445, 2.45, 2.455, 2.46, 2.465, 2.47, 2.475, 2.48, 2.485, 2.49, 2.495, 2.5, 2.505, 2.51, 2.515, 2.52, 2.525, 2.53, 2.535, 2.54, 2.545, 2.55, 2.555, 2.56, 2.565, 2.57, 2.575, 2.58, 2.585, 2.59, 2.595, 2.6, 2.605, 2.61, 2.615, 2.62, 2.625, 2.63, 2.635, 2.64, 2.645, 2.65, 2.655, 2.66, 2.665, 2.67, 2.675, 2.68, 2.685, 2.69, 2.695, 2.7, 2.705, 2.71, 2.715, 2.72, 2.725, 2.73, 2.735, 2.74, 2.745, 2.75, 2.755, 2.76, 2.765, 2.77, 2.775, 2.78, 2.785, 2.79, 2.795, 2.8, 2.805, 2.81, 2.815, 2.82, 2.825, 2.83, 2.835, 2.84, 2.845, 2.85, 2.855, 2.86, 2.865, 2.87, 2.875, 2.88, 2.885, 2.89, 2.895, 2.9, 2.905, 2.91, 2.915, 2.92, 2.925, 2.93, 2.935, 2.94, 2.945, 2.95, 2.955, 2.96, 2.965, 2.97, 2.975, 2.98, 2.985, 2.99, 2.995, 3, 3.005, 3.01, 3.015, 3.02, 3.025, 3.03, 3.035, 3.04, 3.045, 3.05, 3.055, 3.06, 3.065, 3.07, 3.075, 3.08, 3.085, 3.09, 3.095, 3.1, 3.105, 3.11, 3.115, 3.12, 3.125, 3.13, 3.135, 3.14, 3.145, 3.15, 3.155, 3.16, 3.165, 3.17, 3.175, 3.18, 3.185, 3.19, 3.195, 3.2, 3.205, 3.21, 3.215, 3.22, 3.225, 3.23, 3.235, 3.24, 3.245, 3.25, 3.255, 3.26, 3.265, 3.27, 3.275, 3.28, 3.285, 3.29, 3.295, 3.3, 3.305, 3.31, 3.315, 3.32, 3.325, 3.33, 3.335, 3.34, 3.345, 3.35, 3.355, 3.36, 3.365, 3.37, 3.375, 3.38, 3.385, 3.39, 3.395, 3.4, 3.405, 3.41, 3.415, 3.42, 3.425, 3.43, 3.435, 3.44, 3.445, 3.45, 3.455, 3.46, 3.465, 3.47, 3.475, 3.48, 3.485, 3.49, 3.495, 3.5, 3.505, 3.51, 3.515, 3.52, 3.525, 3.53, 3.535, 3.54, 3.545, 3.55, 3.555, 3.56, 3.565, 3.57, 3.575, 3.58, 3.585, 3.59, 3.595, 3.6, 3.605, 3.61, 3.615, 3.62, 3.625, 3.63, 3.635, 3.64, 3.645, 3.65, 3.655, 3.66, 3.665, 3.67, 3.675, 3.68, 3.685, 3.69, 3.695, 3.7, 3.705, 3.71, 3.715, 3.72, 3.725, 3.73, 3.735, 3.74, 3.745, 3.75, 3.755, 3.76, 3.765, 3.77, 3.775, 3.78, 3.785, 3.79, 3.795, 3.8, 3.805, 3.81, 3.815, 3.82, 3.825, 3.83, 3.835, 3.84, 3.845, 3.85, 3.855, 3.86, 3.865, 3.87, 3.875, 3.88, 3.885, 3.89, 3.895, 3.9, 3.905, 3.91, 3.915, 3.92, 3.925, 3.93, 3.935, 3.94, 3.945, 3.95, 3.955, 3.96, 3.965, 3.97, 3.975, 3.98, 3.985, 3.99, 3.995, 4.000};
	vector<Double_t> Mee__newEdges = {0.00,0.02,0.04,0.06,0.08,0.10,0.12,0.16,0.20,0.30,0.40,0.50,0.60,0.68,0.72,0.76,0.82,0.86,0.96,1.0,1.02,1.04,1.06,1.1,1.2,1.6,2.0,2.6,3.0, 3.04,3.08,3.10,3.12,3.14,3.18,3.22,3.26,3.3,4.0};//size()=38
	Int_t Mee_bins = Mee__newEdges.size() - 1;

	//vector<Double_t> Pt__newEdges =  {0,0.05,0.1,0.15,0.20,0.25,0.3,0.35,0.4,0.45,0.5,0.55,0.6,0.65,0.7,0.75,0.8,0.85,0.9,0.95,1,1.05,1.1,1.15,1.2,1.25,1.3,1.35,1.4,1.45,1.5,1.55,1.6,1.65,1.7,1.75,1.8,1.85,1.9,1.95,2,2.05,2.1,2.15,2.2,2.25,2.3,2.35,2.4,2.45,2.5,2.55,2.6,2.65,2.7,2.75,2.8,2.85,2.9,2.95,3,3.05,3.1,3.15,3.2,3.25,3.3,3.35,3.4,3.45,3.5,3.55,3.6,3.65,3.7,3.75,3.8,3.85,3.9,3.95,4,4.05,4.1,4.15,4.2,4.25,4.3,4.35,4.4,4.45,4.5,4.55,4.6,4.65,4.7,4.75,4.8,4.85,4.9,4.95,5};
	vector<Double_t> Pt__newEdges = {0,0.01,0.02,0.03,0.04,0.05,0.06,0.08,0.1,0.12,0.16,0.20,0.30, 0.6, 1.0,1.2, 1.6, 2.0,3.0, 5.0};
	//vector<Double_t> Pt__newEdges = {0,0.5,1.0,2.0,3.0,4.0,5.0};
	Int_t Pt_bins = Pt__newEdges.size() - 1;
	//                                 80   75   70   65   60   55   50   45   40   35   30    25    20    15    10    5     0   
	//vector<Double_t> Cen__newEdges = {0.0, 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0, 10.0, 11.0, 12.0, 13.0, 14.0, 15.0, 16.0 };
	vector<Double_t> Cen__newEdges = {0,2,4,8,16};
	Int_t Cen_bins = Cen__newEdges.size() - 1;
	// 从root文件中导入待拟合的直方图
	TFile *inFile = new TFile(inFileName);
	//TFile *outFile = new TFile(Form("roots/%d_output.root", number), "RECREATE");
	if (!inFile){cout << "The input file is not found! Exiting..." << endl; return;}
	// 导入直方图
	TH3F* h_Mee_Pt_Cen__unlikeSame  = (TH3F*)inFile->Get("h_Mee_Pt_Cen__unlikeSame");//hMeePtTPCTrks;h_Mee_Pt_Cen__w_PhiV_cut;h_Mee_Pt_Cen__unlikeSame
	TH3F* h_Mee_Pt_Cen__likemm 	    = (TH3F*)inFile->Get("h_Mee_Pt_Cen__likemm");//hMeePtTPCTrks_like1;h_Mee_Pt_Cen__likemm
	TH3F* h_Mee_Pt_Cen__likepp 	    = (TH3F*)inFile->Get("h_Mee_Pt_Cen__likepp");//hMeePtTPCTrks_like2;h_Mee_Pt_Cen__likepp
	TH3F* h_Mee_Pt_Cen__likemmMixed = (TH3F*)inFile->Get("h_Mee_Pt_Cen__likemmMixed");//hMeeMixlike1_Pt_Cen;h_Mee_Pt_Cen__likemmMixed
	TH3F* h_Mee_Pt_Cen__likeppMixed = (TH3F*)inFile->Get("h_Mee_Pt_Cen__likeppMixed");//hMeeMixlike2_Pt_Cen;h_Mee_Pt_Cen__likeppMixed
	TH3F* h_Mee_Pt_Cen__unlikeMixed = (TH3F*)inFile->Get("h_Mee_Pt_Cen__unlikeMixed");//hMeeMix_Pt_Cen;h_Mee_Pt_Cen__unlikeMixed

	// 创建直方图
	TH3F* h_Mee_Pt_Cen__likemm_Rebin	  = new TH3F("h_Mee_Pt_Cen__likemm_Rebin", ";M_{ee};p_{T};Cen", Mee_bins, Mee__newEdges.data(), Pt_bins, Pt__newEdges.data(), Cen_bins, Cen__newEdges.data());
	TH3F* h_Mee_Pt_Cen__likepp_Rebin	  = new TH3F("h_Mee_Pt_Cen__likepp_Rebin", ";M_{ee};p_{T};Cen", Mee_bins, Mee__newEdges.data(), Pt_bins, Pt__newEdges.data(), Cen_bins, Cen__newEdges.data());
	TH3F* h_Mee_Pt_Cen__unlikeSame_Rebin  = new TH3F("h_Mee_Pt_Cen__unlikeSame_Rebin", ";M_{ee};p_{T};Cen", Mee_bins, Mee__newEdges.data(), Pt_bins, Pt__newEdges.data(), Cen_bins, Cen__newEdges.data());
	TH3F* h_Mee_Pt_Cen__likemmMixed_Rebin = new TH3F("h_Mee_Pt_Cen__likemmMixed_Rebin", ";M_{ee};p_{T};Cen", Mee_bins, Mee__newEdges.data(), Pt_bins, Pt__newEdges.data(), Cen_bins, Cen__newEdges.data());
	TH3F* h_Mee_Pt_Cen__likeppMixed_Rebin = new TH3F("h_Mee_Pt_Cen__likeppMixed_Rebin", ";M_{ee};p_{T};Cen", Mee_bins, Mee__newEdges.data(), Pt_bins, Pt__newEdges.data(), Cen_bins, Cen__newEdges.data());
	TH3F* h_Mee_Pt_Cen__unlikeMixed_Rebin = new TH3F("h_Mee_Pt_Cen__unlikeMixed_Rebin", ";M_{ee};p_{T};Cen", Mee_bins, Mee__newEdges.data(), Pt_bins, Pt__newEdges.data(), Cen_bins, Cen__newEdges.data());

	TH3F* h_Mee_Pt_Cen__temp_Rebin = new TH3F("h_Mee_Pt_Cen__temp_Rebin", ";M_{ee};p_{T};Cen", Mee_bins, Mee__newEdges.data(), Pt_bins, Pt__newEdges.data(), Cen_bins, Cen__newEdges.data());
	TH3F* h_Mee_Pt_Cen__temp2_Rebin = new TH3F("h_Mee_Pt_Cen__temp2_Rebin", ";M_{ee};p_{T};Cen", Mee_bins, Mee__newEdges.data(), Pt_bins, Pt__newEdges.data(), Cen_bins, Cen__newEdges.data());
	TH3F* h_Mee_Pt_Cen__LikeMixed_Rebin = new TH3F("h_Mee_Pt_Cen__LikeMixed_Rebin", ";M_{ee};p_{T};Cen", Mee_bins, Mee__newEdges.data(), Pt_bins, Pt__newEdges.data(), Cen_bins, Cen__newEdges.data());
	TH3F* h_Mee_Pt_Cen__LikeSame_Rebin = new TH3F("h_Mee_Pt_Cen__LikeSame_Rebin", ";M_{ee};p_{T};Cen", Mee_bins, Mee__newEdges.data(), Pt_bins, Pt__newEdges.data(), Cen_bins, Cen__newEdges.data());
	TH3F* h_Mee_Pt_Cen__LikeSame_PSACcorr_Rebin = new TH3F("h_Mee_Pt_Cen__LikeSame_PSACcorr_Rebin", ";M_{ee};p_{T};Cen", Mee_bins, Mee__newEdges.data(), Pt_bins, Pt__newEdges.data(), Cen_bins, Cen__newEdges.data());
	TH3F* h_Mee_Pt_Cen__CorrLS_Rebin = new TH3F("h_Mee_Pt_Cen__CorrLS_Rebin", ";M_{ee};p_{T};Cen", Mee_bins, Mee__newEdges.data(), Pt_bins, Pt__newEdges.data(), Cen_bins, Cen__newEdges.data());
	TH3F* h_Mee_Pt_Cen__rmLS_Rebin = new TH3F("h_Mee_Pt_Cen__rmLS_Rebin", ";M_{ee};p_{T};Cen", Mee_bins, Mee__newEdges.data(), Pt_bins, Pt__newEdges.data(), Cen_bins, Cen__newEdges.data());
	TH3F* h_Mee_Pt_Cen__rmLS_PSACcorr_Rebin = new TH3F("h_Mee_Pt_Cen__rmLS_PSACcorr_Rebin", ";M_{ee};p_{T};Cen", Mee_bins, Mee__newEdges.data(), Pt_bins, Pt__newEdges.data(), Cen_bins, Cen__newEdges.data());
	TH3F* h_Mee_Pt_Cen__rmUM_Rebin = new TH3F("h_Mee_Pt_Cen__rmUM_Rebin", ";M_{ee};p_{T};Cen", Mee_bins, Mee__newEdges.data(), Pt_bins, Pt__newEdges.data(), Cen_bins, Cen__newEdges.data());

	TH1F* h_Pt__temp_Rebin = new TH1F("h_Pt__temp_Rebin", ";p_{T}", Pt_bins, Pt__newEdges.data());
	TH1F* h_Pt__temp2_Rebin = new TH1F("h_Pt__temp2_Rebin", ";p_{T}", Pt_bins, Pt__newEdges.data());
	TH1F* h_Pt__temp3_Rebin = new TH1F("h_Pt__temp3_Rebin", ";p_{T}", Pt_bins, Pt__newEdges.data());
	TH1F* h_Pt__LikeMixed_Rebin = new TH1F("h_Pt__LikeMixed_Rebin", ";p_{T}", Pt_bins, Pt__newEdges.data());
	TH1F* h_Pt__LikeSame_Rebin = new TH1F("h_Pt__LikeSame_Rebin", ";p_{T}", Pt_bins, Pt__newEdges.data());
	TH1F* h_Pt__LikeSame_PSACcorr_Rebin = new TH1F("h_Pt__LikeSame_PSACcorr_Rebin", ";p_{T}", Pt_bins, Pt__newEdges.data());
	TH1F* h_Pt__rmLS_Rebin = new TH1F("h_Pt__rmLS_Rebin", ";p_{T}", Pt_bins, Pt__newEdges.data());
	TH1F* h_Pt__rmLS_PSACcorr_Rebin = new TH1F("h_Pt__rmLS_PSACcorr_Rebin", ";p_{T}", Pt_bins, Pt__newEdges.data());
	TH1F* h_Pt__rmUM_Rebin = new TH1F("h_Pt__rmUM_Rebin", ";p_{T}", Pt_bins, Pt__newEdges.data());
	TH1F* h_Pt__CorrLS_Rebin = new TH1F("h_Pt__CorrLS_Rebin", ";p_{T}", Pt_bins, Pt__newEdges.data());
	
	// Rebin
	RebinHist(h_Mee_Pt_Cen__likemm, h_Mee_Pt_Cen__likemm_Rebin);
	RebinHist(h_Mee_Pt_Cen__likepp, h_Mee_Pt_Cen__likepp_Rebin);
	RebinHist(h_Mee_Pt_Cen__unlikeSame, h_Mee_Pt_Cen__unlikeSame_Rebin);
	RebinHist(h_Mee_Pt_Cen__likemmMixed, h_Mee_Pt_Cen__likemmMixed_Rebin);
	RebinHist(h_Mee_Pt_Cen__likeppMixed, h_Mee_Pt_Cen__likeppMixed_Rebin);
	RebinHist(h_Mee_Pt_Cen__unlikeMixed, h_Mee_Pt_Cen__unlikeMixed_Rebin);
	Float_t low_Mee = 0.0, up_Mee = 5.0, low_Cen = 0.0, up_Cen = 16.0;//model sum/11/12/13/14/22/23/24/33/34/44
	Int_t bin__low_Mee = h_Mee_Pt_Cen__likepp_Rebin->GetXaxis()->FindBin(low_Mee + 1e-3);
	Int_t bin__up_Mee = h_Mee_Pt_Cen__likepp_Rebin->GetXaxis()->FindBin(up_Mee - 1e-3);
	Int_t bin__low_Cen = h_Mee_Pt_Cen__likepp_Rebin->GetZaxis()->FindBin(low_Cen + 1e-3);
	Int_t bin__up_Cen = h_Mee_Pt_Cen__likepp_Rebin->GetZaxis()->FindBin(up_Cen + 1e-3);
	//cout << bin__low_Mee<<" "<<bin__up_Mee<<" "<<bin__low_Cen<<" "<<bin__up_Cen<<endl;
	TH1F *h_Pt__likemm_Rebin	  = (TH1F*)h_Mee_Pt_Cen__likemm_Rebin->ProjectionY("h_Pt__likemm_Rebin", bin__low_Mee, bin__up_Mee, bin__low_Cen, bin__up_Cen);
	TH1F *h_Pt__likepp_Rebin	  = (TH1F*)h_Mee_Pt_Cen__likepp_Rebin->ProjectionY("h_Pt__likepp_Rebin", bin__low_Mee, bin__up_Mee, bin__low_Cen, bin__up_Cen);
	TH1F *h_Pt__unlikeSame_Rebin = (TH1F*)h_Mee_Pt_Cen__unlikeSame_Rebin->ProjectionY("h_Pt__unlikeSame_Rebin", bin__low_Mee, bin__up_Mee, bin__low_Cen, bin__up_Cen);
	TH1F *h_Pt__likemmMixed_Rebin = (TH1F*)h_Mee_Pt_Cen__likemmMixed_Rebin->ProjectionY("h_Pt__likemmMixed_Rebin", bin__low_Mee, bin__up_Mee, bin__low_Cen, bin__up_Cen);
	TH1F *h_Pt__likeppMixed_Rebin = (TH1F*)h_Mee_Pt_Cen__likeppMixed_Rebin->ProjectionY("h_Pt__likeppMixed_Rebin", bin__low_Mee, bin__up_Mee, bin__low_Cen, bin__up_Cen);
	TH1F *h_Pt__unlikeMixed_Rebin = (TH1F*)h_Mee_Pt_Cen__unlikeMixed_Rebin->ProjectionY("h_Pt__unlikeMixed_Rebin", bin__low_Mee, bin__up_Mee, bin__low_Cen, bin__up_Cen);

	// 3-D Like-SameEvent背景
	h_Mee_Pt_Cen__temp_Rebin->Multiply(h_Mee_Pt_Cen__likemm_Rebin, h_Mee_Pt_Cen__likepp_Rebin, 1.0, 1.0);//GA
	h_Mee_Pt_Cen__LikeSame_Rebin = SqrtHist(h_Mee_Pt_Cen__temp_Rebin, "h_Mee_Pt_Cen__LikeSame_Rebin");
	h_Mee_Pt_Cen__LikeSame_Rebin->Scale(2.0);
	h_Mee_Pt_Cen__LikeSame_Rebin->Add(h_Mee_Pt_Cen__likemm_Rebin, h_Mee_Pt_Cen__likepp_Rebin, 1.0, 1.0);//AA
		// 使用LM计算LS背景修正因子
	h_Mee_Pt_Cen__temp2_Rebin->Multiply(h_Mee_Pt_Cen__likemmMixed_Rebin, h_Mee_Pt_Cen__likeppMixed_Rebin, 1.0, 1.0);//GAcorr
	h_Mee_Pt_Cen__LikeMixed_Rebin = SqrtHist(h_Mee_Pt_Cen__temp2_Rebin,"h_Mee_Pt_Cen__LikeMixed_Rebin");
	h_Mee_Pt_Cen__LikeMixed_Rebin->Scale(2.0);
	h_Mee_Pt_Cen__CorrLS_Rebin->Divide(h_Mee_Pt_Cen__unlikeMixed_Rebin, h_Mee_Pt_Cen__LikeMixed_Rebin, 1.0, 1.0);
	h_Mee_Pt_Cen__LikeSame_Rebin->Multiply(h_Mee_Pt_Cen__CorrLS_Rebin);
	// 1-D Like-SameEvent背景
	h_Pt__temp_Rebin->Multiply(h_Pt__likemm_Rebin, h_Pt__likepp_Rebin, 1.0, 1.0);
	h_Pt__LikeSame_Rebin = SqrtHist(h_Pt__temp_Rebin, "h_Pt__LikeSame_Rebin");
	h_Pt__LikeSame_Rebin->Scale(2.0);
		// 使用LM计算LS背景修正因子
	h_Pt__temp2_Rebin->Multiply(h_Pt__likemmMixed_Rebin, h_Pt__likeppMixed_Rebin, 1.0, 1.0);//GAcorr
	h_Pt__LikeMixed_Rebin = SqrtHist(h_Pt__temp2_Rebin, "h_Pt__LikeMixed_Rebin");
	h_Pt__LikeMixed_Rebin->Scale(2.0);
	h_Pt__CorrLS_Rebin->Divide(h_Pt__unlikeMixed_Rebin, h_Pt__LikeMixed_Rebin, 1.0, 1.0);
	h_Pt__LikeSame_PSACcorr_Rebin = (TH1F*)h_Pt__LikeSame_Rebin->Clone("h_Pt__LikeSame_PSACcorr_Rebin");
	h_Pt__LikeSame_PSACcorr_Rebin->Multiply(h_Pt__CorrLS_Rebin);

	// 3-D Unlike-MixedEvent背景
	Float_t NR_low_M = 0.5, NR_up_M = 2, NR_low_Mee = 0, NR_up_Mee = 2;//model ps_11
	Float_t scale = ComputeMixEventScale(h_Mee_Pt_Cen__likepp_Rebin, h_Mee_Pt_Cen__likemm_Rebin,h_Mee_Pt_Cen__likeppMixed_Rebin, h_Mee_Pt_Cen__likemmMixed_Rebin,h_Mee_Pt_Cen__unlikeMixed_Rebin, NR_low_M, NR_up_M, NR_low_Mee, NR_up_Mee, Cen__newEdges.front(), Cen__newEdges.back());
	cout << "scale: " << scale << endl;
	// 3-D Unlike-MixedEvent背景
	h_Mee_Pt_Cen__unlikeMixed_Rebin->Scale(scale);
	// 1-D Unlike-MixedEvent背景
	h_Pt__unlikeMixed_Rebin->Scale(scale);
	
	// 去除背景，还原信号
	h_Mee_Pt_Cen__rmLS_Rebin->Add(h_Mee_Pt_Cen__unlikeSame_Rebin, h_Mee_Pt_Cen__LikeSame_Rebin, 1.0, -1.0);
	h_Mee_Pt_Cen__rmLS_PSACcorr_Rebin->Add(h_Mee_Pt_Cen__unlikeSame_Rebin, h_Mee_Pt_Cen__LikeSame_Rebin, 1.0, -1.0);
	h_Mee_Pt_Cen__rmUM_Rebin->Add(h_Mee_Pt_Cen__unlikeSame_Rebin, h_Mee_Pt_Cen__unlikeMixed_Rebin, 1.0, -1.0);
	h_Pt__rmLS_Rebin->Add(h_Pt__unlikeSame_Rebin, h_Pt__LikeSame_Rebin, 1.0, -1.0);
	h_Pt__rmLS_PSACcorr_Rebin->Add(h_Pt__unlikeSame_Rebin, h_Pt__LikeSame_PSACcorr_Rebin, 1.0, -1.0);
	h_Pt__rmUM_Rebin->Add(h_Pt__unlikeSame_Rebin, h_Pt__unlikeMixed_Rebin, 1.0, -1.0);

	//Reset 直方图
	ResetBinContent(h_Pt__rmLS_Rebin);
	ResetBinContent(h_Pt__rmLS_PSACcorr_Rebin);
	ResetBinContent(h_Pt__rmUM_Rebin);
	ResetBinContent(h_Pt__unlikeSame_Rebin);
	ResetBinContent(h_Pt__LikeSame_Rebin);
	ResetBinContent(h_Pt__LikeSame_PSACcorr_Rebin);
	ResetBinContent(h_Pt__unlikeMixed_Rebin);
	ResetBinContent(h_Pt__LikeMixed_Rebin);
	
	if (1)// 信号1/2/3，背景1/2/3，信号-背景1/2/3
	{
		//设置直方图格式
		h_Pt__unlikeSame_Rebin->SetLineColor(1);		h_Pt__unlikeSame_Rebin->SetMarkerStyle(kOpenCircle);	h_Pt__unlikeSame_Rebin->SetMarkerColor(1); h_Pt__unlikeSame_Rebin->SetMarkerSize(0.5);
		h_Pt__LikeSame_PSACcorr_Rebin->SetLineColor(2);h_Pt__LikeSame_PSACcorr_Rebin->SetMarkerStyle(kOpenSquare);h_Pt__LikeSame_PSACcorr_Rebin->SetMarkerColor(2); h_Pt__LikeSame_PSACcorr_Rebin->SetMarkerSize(0.5);
		h_Pt__unlikeMixed_Rebin->SetLineColor(2);		h_Pt__unlikeMixed_Rebin->SetMarkerStyle(kOpenSquare);	h_Pt__unlikeMixed_Rebin->SetMarkerColor(2); h_Pt__unlikeMixed_Rebin->SetMarkerSize(0.5);
		h_Pt__rmLS_Rebin->SetLineColor(6);				h_Pt__rmLS_Rebin->SetMarkerStyle(kOpenCross);			h_Pt__rmLS_Rebin->SetMarkerColor(6); h_Pt__rmLS_Rebin->SetMarkerSize(0.5);
		h_Pt__rmLS_PSACcorr_Rebin->SetLineColor(9);	h_Pt__rmLS_PSACcorr_Rebin->SetMarkerStyle(kOpenCross);	h_Pt__rmLS_PSACcorr_Rebin->SetMarkerColor(9); h_Pt__rmLS_PSACcorr_Rebin->SetMarkerSize(0.5);
		h_Pt__rmUM_Rebin->SetLineColor(6);				h_Pt__rmUM_Rebin->SetMarkerStyle(kOpenCross);			h_Pt__rmUM_Rebin->SetMarkerColor(6); h_Pt__rmUM_Rebin->SetMarkerSize(0.5);
		
		h_Pt__rmLS_Rebin->SetTitle(";p_{T} (GeV/c);dN/dp_{T} (GeV/c)^{-1}");
		h_Pt__rmUM_Rebin->SetTitle(";p_{T} (GeV/c);dN/dp_{T} (GeV/c)^{-1}");
		h_Pt__unlikeSame_Rebin->SetTitle(";p_{T} (GeV/c);dN/dp_{T} (GeV/c)^{-1}");
		h_Pt__LikeSame_PSACcorr_Rebin->SetTitle(";p_{T} (GeV/c);dN/dp_{T} (GeV/c)^{-1}");
		h_Pt__CorrLS_Rebin->SetTitle("PSAC vs p_{T};p_{T} (GeV/c);PSAC");

		//画图
		TCanvas *c2 = new TCanvas("c2", "c2", 1400, 1200);
		c2->Divide(3, 3);
		c2->cd(1);//背景+信号
		gPad->SetLogy(1);
		gPad->SetLeftMargin(0.12);
		gPad->SetRightMargin(0.12);
		gStyle->SetOptStat(0);
		h_Pt__unlikeSame_Rebin->SetMaximum(1e8);
		h_Pt__unlikeSame_Rebin->SetMinimum(1e1);
		h_Pt__unlikeSame_Rebin->DrawClone("PE");
		h_Pt__rmLS_Rebin->DrawClone("same PE");
		h_Pt__rmLS_PSACcorr_Rebin->DrawClone("same PE");
		h_Pt__LikeSame_PSACcorr_Rebin->DrawClone("same PE");
		auto legend_LS = new TLegend(0.45, 0.65, 0.55, 0.90);
		legend_LS->SetFillColor(0); legend_LS->SetBorderSize(0);
		legend_LS->AddEntry(h_Pt__unlikeSame_Rebin, "\t Unlike-Sign Same-Event", "lp");
		legend_LS->AddEntry(h_Pt__LikeSame_PSACcorr_Rebin, "\t Like-Sign Same-Event", "lp");
		legend_LS->AddEntry(h_Pt__rmLS_Rebin, "\t US - LS", "lp");
		legend_LS->AddEntry(h_Pt__rmLS_PSACcorr_Rebin, "\t US - LS(corrected)", "lp");
		legend_LS->SetMargin(0.45); //legend_BR->SetTextAlign(12);
		gStyle->SetLegendTextSize(0.04);
		legend_LS->Draw("same");

		c2->cd(2);//背景+信号
		gPad->SetLogy(1);
		gPad->SetLeftMargin(0.12);
		gPad->SetRightMargin(0.12);
		gStyle->SetOptStat(0);
		h_Pt__unlikeSame_Rebin->SetMaximum(1e8);
		h_Pt__unlikeSame_Rebin->SetMinimum(1e1);
		h_Pt__unlikeSame_Rebin->DrawClone("PE");
		h_Pt__unlikeMixed_Rebin->DrawClone("same PE");
		h_Pt__rmUM_Rebin->DrawClone("same PE");
		auto legend_UM = new TLegend(0.45, 0.65, 0.55, 0.90);
		legend_UM->SetFillColor(0); legend_UM->SetBorderSize(0);
		legend_UM->AddEntry(h_Pt__unlikeSame_Rebin, "\t Unlike-Sign Same-Event", "lp");
		legend_UM->AddEntry(h_Pt__unlikeMixed_Rebin, "\t Unlike-Sign Mix-Event", "lp");
		legend_UM->AddEntry(h_Pt__rmUM_Rebin, "\t US - UM", "lp");
		legend_UM->SetMargin(0.45); //legend_BR->SetTextAlign(12);
		gStyle->SetLegendTextSize(0.04);
		legend_UM->Draw("same");

		c2->cd(3);//背景+信号
		gPad->SetLogy(1);
		gPad->SetLeftMargin(0.12);
		gPad->SetRightMargin(0.12);
		gStyle->SetOptStat(0);
		h_Pt__unlikeSame_Rebin->SetMaximum(1e8);
		h_Pt__unlikeSame_Rebin->SetMinimum(1e1);
		h_Pt__unlikeSame_Rebin->DrawClone("PE");


		c2->cd(4);//负信号_rmLS
		gPad->SetLogy(1);
		gPad->SetLeftMargin(0.12);
		gPad->SetRightMargin(0.12);
		gStyle->SetOptStat(0);
		TH1F* h_Pt__rmLS_Rebin_test = (TH1F*)h_Pt__rmLS_Rebin->Clone("h_Pt__rmLS_Rebin_test");
		TH1F* h_Pt__rmLS_PSACcorr_Rebin_test = (TH1F*)h_Pt__rmLS_PSACcorr_Rebin->Clone("h_Pt__rmLS_PSACcorr_Rebin_test");
		NegateBinContents(h_Pt__rmLS_Rebin_test);
		NegateBinContents(h_Pt__rmLS_PSACcorr_Rebin_test);
		h_Pt__rmLS_Rebin_test->SetMaximum(1e8);
		h_Pt__rmLS_Rebin_test->SetMinimum(10);
		h_Pt__rmLS_Rebin_test->DrawClone("PE");
		h_Pt__rmLS_PSACcorr_Rebin_test->DrawClone("same PE");

		c2->cd(5);//负信号_rmUM
		gPad->SetLogy(1);
		gPad->SetLeftMargin(0.12);
		gPad->SetRightMargin(0.12);
		gStyle->SetOptStat(0);
		TH1F* h_Pt__rmUM_Rebin_test = (TH1F*)h_Pt__rmUM_Rebin->Clone("h_Pt__rmUM_Rebin_test");
		NegateBinContents(h_Pt__rmUM_Rebin_test);
		h_Pt__rmUM_Rebin_test->SetMaximum(1e8);
		h_Pt__rmUM_Rebin_test->SetMinimum(10);
		h_Pt__rmUM_Rebin_test->DrawClone("PE");

		c2->cd(6);
		gPad->SetLogy(1);
		gPad->SetLeftMargin(0.12);
		gPad->SetRightMargin(0.12);
		gStyle->SetOptStat(0);


		c2->cd(7);//PSAC 2-D check
		gPad->SetLeftMargin(0.12);
		gPad->SetRightMargin(0.12);
		gStyle->SetOptStat(0);
		TH2F* h_Mee_Pt__CorrLS_Rebin = (TH2F*)h_Mee_Pt_Cen__CorrLS_Rebin->Project3D("yx");
		h_Mee_Pt__CorrLS_Rebin->SetTitle("PSAC distribution;Mee (GeV/c^{2});p_{T} (GeV/c)");
		h_Mee_Pt__CorrLS_Rebin->Scale(1.0 / (Cen__newEdges.size() - 1));
		h_Mee_Pt__CorrLS_Rebin->SetMaximum(1.2);
		h_Mee_Pt__CorrLS_Rebin->SetMinimum(0.8);
		h_Mee_Pt__CorrLS_Rebin->DrawClone("col z");

		c2->cd(8);//PSAC 1-D check
		gPad->SetLeftMargin(0.12);
		gPad->SetRightMargin(0.12);
		gStyle->SetOptStat(0);
		h_Pt__CorrLS_Rebin->SetLineColor(kBlack);  h_Pt__CorrLS_Rebin->SetMarkerStyle(kOpenCircle); h_Pt__CorrLS_Rebin->SetMarkerColor(kBlack);
		h_Pt__CorrLS_Rebin->SetMaximum(1.02);
		h_Pt__CorrLS_Rebin->SetMinimum(0.96);
		h_Pt__CorrLS_Rebin->DrawClone();

		c2->SaveAs(Form("roots/%d_Pt.png", number));
	}
	if (1)// c_pt different pt bin的US,LS,UM,US-LS,US-UM的分布
	{
		h_Mee_Pt_Cen__unlikeSame_Rebin->SetLineColor(1);		h_Mee_Pt_Cen__unlikeSame_Rebin->SetMarkerStyle(kOpenCircle);	h_Mee_Pt_Cen__unlikeSame_Rebin->SetMarkerColor(1); //h_Mee_Pt_Cen__unlikeSame_Rebin->SetMarkerSize(0.1);
		h_Mee_Pt_Cen__LikeSame_Rebin->SetLineColor(2);			h_Mee_Pt_Cen__LikeSame_Rebin->SetMarkerStyle(kOpenSquare);		h_Mee_Pt_Cen__LikeSame_Rebin->SetMarkerColor(2); //h_Mee_Pt_Cen__LikeSame_Rebin->SetMarkerSize(0.1);
		h_Mee_Pt_Cen__rmLS_Rebin->SetLineColor(6);				h_Mee_Pt_Cen__rmLS_Rebin->SetMarkerStyle(kOpenCross);			h_Mee_Pt_Cen__rmLS_Rebin->SetMarkerColor(6); //h_Mee_Pt_Cen__rmLS_Rebin->SetMarkerSize(0.1);		
		//画图
		Float_t Cen_bin_low = 0.0, Cen_bin_up = 4;
		TCanvas *c_Mee = new TCanvas("c_Mee", "c_Mee", 1400, 700);
		c_Mee->Divide(4, 2);
		c_Mee->cd(1);
		gPad->SetLogy(1);
		gStyle->SetOptStat(0);
		Draw_Pt_Meeslice(h_Mee_Pt_Cen__unlikeSame_Rebin, h_Mee_Pt_Cen__LikeSame_Rebin, h_Mee_Pt_Cen__unlikeMixed_Rebin, h_Mee_Pt_Cen__rmLS_Rebin, h_Mee_Pt_Cen__rmUM_Rebin,0.0,0.4,Cen_bin_low,Cen_bin_up);
		c_Mee->cd(2);
		gPad->SetLogy(1);
		gStyle->SetOptStat(0);
		Draw_Pt_Meeslice(h_Mee_Pt_Cen__unlikeSame_Rebin, h_Mee_Pt_Cen__LikeSame_Rebin, h_Mee_Pt_Cen__unlikeMixed_Rebin, h_Mee_Pt_Cen__rmLS_Rebin, h_Mee_Pt_Cen__rmUM_Rebin,0.4,0.76,Cen_bin_low,Cen_bin_up);
		c_Mee->cd(3);
		gPad->SetLogy(1);
		gStyle->SetOptStat(0);
		Draw_Pt_Meeslice(h_Mee_Pt_Cen__unlikeSame_Rebin, h_Mee_Pt_Cen__LikeSame_Rebin, h_Mee_Pt_Cen__unlikeMixed_Rebin, h_Mee_Pt_Cen__rmLS_Rebin, h_Mee_Pt_Cen__rmUM_Rebin, 0.76, 1.2,Cen_bin_low,Cen_bin_up);
		c_Mee->cd(4);
		gPad->SetLogy(1);
		gStyle->SetOptStat(0);
		Draw_Pt_Meeslice(h_Mee_Pt_Cen__unlikeSame_Rebin, h_Mee_Pt_Cen__LikeSame_Rebin, h_Mee_Pt_Cen__unlikeMixed_Rebin, h_Mee_Pt_Cen__rmLS_Rebin, h_Mee_Pt_Cen__rmUM_Rebin, 1.2, 2.6,Cen_bin_low,Cen_bin_up);
		c_Mee->cd(5);
		gPad->SetLogy(1);
		gStyle->SetOptStat(0);
		Draw_Pt_Meeslice(h_Mee_Pt_Cen__unlikeSame_Rebin, h_Mee_Pt_Cen__LikeSame_Rebin, h_Mee_Pt_Cen__unlikeMixed_Rebin, h_Mee_Pt_Cen__rmLS_Rebin, h_Mee_Pt_Cen__rmUM_Rebin,0.0,0.4,Cen_bin_low,Cen_bin_up,"-");
		c_Mee->cd(6);
		gPad->SetLogy(1);
		gStyle->SetOptStat(0);
		Draw_Pt_Meeslice(h_Mee_Pt_Cen__unlikeSame_Rebin, h_Mee_Pt_Cen__LikeSame_Rebin, h_Mee_Pt_Cen__unlikeMixed_Rebin, h_Mee_Pt_Cen__rmLS_Rebin, h_Mee_Pt_Cen__rmUM_Rebin,0.4,0.76,Cen_bin_low,Cen_bin_up,"-");
		c_Mee->cd(7);
		gPad->SetLogy(1);
		gStyle->SetOptStat(0);
		Draw_Pt_Meeslice(h_Mee_Pt_Cen__unlikeSame_Rebin, h_Mee_Pt_Cen__LikeSame_Rebin, h_Mee_Pt_Cen__unlikeMixed_Rebin, h_Mee_Pt_Cen__rmLS_Rebin, h_Mee_Pt_Cen__rmUM_Rebin, 0.76, 1.2,Cen_bin_low,Cen_bin_up,"-");
		c_Mee->cd(8);
		gPad->SetLogy(1);
		gStyle->SetOptStat(0);
		Draw_Pt_Meeslice(h_Mee_Pt_Cen__unlikeSame_Rebin, h_Mee_Pt_Cen__LikeSame_Rebin, h_Mee_Pt_Cen__unlikeMixed_Rebin, h_Mee_Pt_Cen__rmLS_Rebin, h_Mee_Pt_Cen__rmUM_Rebin, 1.2, 2.6,Cen_bin_low,Cen_bin_up,"-");
		c_Mee->SaveAs(Form("roots/%d_RawYield_MeeBins_Cen_60_80.png", number));
		//delete c_Mee;
	}

	//outFile->Write();    // 将当前目录中的所有对象写入文件
	//outFile->Close();
}

