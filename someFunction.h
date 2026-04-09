// ���庯��
template <typename H1>
H1* SqrtHist(H1* h, const TString& newName = "h_new", Bool_t setError = kTRUE)
{
	if (h->GetSumw2N() == 0)	h->Sumw2("kTRUE");
	H1* h_new = static_cast<H1*>(h->Clone(newName));
	if (h_new->GetSumw2N() == 0)h_new->Sumw2("kTRUE");
	for (int ix = 1; ix <= h->GetNbinsX(); ++ix)
	{
		for (int iy = 1; iy <= h->GetNbinsY(); ++iy)
		{
			for (int iz = 1; iz <= h->GetNbinsZ(); ++iz)
			{
				Int_t bin = h->GetBin(ix, iy, iz);
				Float_t oldContent = h->GetBinContent(bin);
				if (oldContent <= 0)
				{
					h_new->SetBinContent(bin, 0);
					if (setError) h_new->SetBinError(bin, 0);
				}
				else
				{
					Float_t oldError = h->GetBinError(bin);
					Float_t newContent = TMath::Sqrt(oldContent);
					h_new->SetBinContent(bin, newContent);
					if (setError) h_new->SetBinError(bin, oldError / (2 * TMath::Sqrt(oldContent)));
				}
			}
		}
	}
	return h_new;
}
template <typename H3>
void RebinHist(H3* oldH, H3* newH)//Rebin
{
	if (oldH->GetSumw2N() == 0)oldH->Sumw2("kTRUE");
	if (newH->GetSumw2N() == 0)newH->Sumw2("kTRUE");
	// ��binǨ��
	for (int ix = 1; ix <= oldH->GetNbinsX(); ix++)
	{
		for (int iy = 1; iy <= oldH->GetNbinsY(); iy++)
		{
			for (int iz = 1; iz <= oldH->GetNbinsZ(); iz++)
			{
				Int_t gBinOld = oldH->GetBin(ix, iy, iz);

				Double_t oldContent = oldH->GetBinContent(gBinOld);
				Double_t oldError2 = oldH->GetSumw2()->At(gBinOld);// ���ƽ��

				Double_t oldBinx = oldH->GetXaxis()->GetBinCenter(ix);
				Double_t oldBiny = oldH->GetYaxis()->GetBinCenter(iy);
				Double_t oldBinz = oldH->GetZaxis()->GetBinCenter(iz);
				int gBinNew = newH->FindBin(oldBinx, oldBiny, oldBinz);// ��������λ

				Double_t newError2 = newH->GetSumw2()->At(gBinNew);
				newH->AddBinContent(gBinNew, oldContent);
				newH->GetSumw2()->AddAt(newError2 + oldError2, gBinNew);
			}
		}
	}

	// ͬ��ȫ��ͳ����
	newH->SetEntries(newH->GetEntries() + oldH->GetEntries());
}
template <typename H2>
void ResetBinContent(H2* h, Bool_t flag = kTRUE, Bool_t setError = kTRUE)
{
	int sum_minux = 0;
	if (h->GetSumw2N() == 0)h->Sumw2("kTRUE");
	for (int ix = 1; ix <= h->GetNbinsX(); ++ix)
	{
		for (int iy = 1; iy <= h->GetNbinsY(); ++iy)
		{
			for (int iz = 1; iz <= h->GetNbinsZ(); ++iz)
			{
				Int_t gBin = h->GetBin(ix, iy, iz);
				Float_t oldContent = h->GetBinContent(gBin);
				Float_t oldError = h->GetBinError(gBin);

				//NOTE: ��ֱ��ͼ����3��ά��ʱ�����ڲ����ڵ�ά�ȣ�GetNbinsZ()����1��GetBinWidth(1)����1���ʸĴ����1/2/3Dֱ��ͼͨ�á�
				Float_t binWidthx = h->GetXaxis()->GetBinWidth(ix);
				Float_t binWidthy = h->GetYaxis()->GetBinWidth(iy);
				Float_t binWidthz = h->GetZaxis()->GetBinWidth(iz);
				Float_t newContent, newError;
				if (flag)
				{
					newContent = oldContent / (binWidthx * binWidthy * binWidthz);//��ֵ����bin��
					newError = oldError / (binWidthx * binWidthy * binWidthz);
				}
				else
				{
					newContent = oldContent * (binWidthx * binWidthy * binWidthz);
					newError = oldError * (binWidthx * binWidthy * binWidthz);
				}

				h->SetBinContent(gBin, newContent);
				if (setError) h->SetBinError(gBin, newError);//error����
			}
		}
	}
	if (sum_minux > 0)cout << "sum_minux of " << h->GetName() << ": " << sum_minux << endl;
}
template <typename H4>
void SetMinusContentZero(H4* h, Bool_t setError = kTRUE)
{
	for (int ix = 1; ix <= h->GetNbinsX(); ++ix)
	{
		for (int iy = 1; iy <= h->GetNbinsY(); ++iy)
		{
			for (int iz = 1; iz <= h->GetNbinsZ(); ++iz)
			{
				Int_t gBin = h->GetBin(ix, iy, iz);
				Float_t oldContent = h->GetBinContent(gBin);
				if (oldContent <= 0)
				{
					h->SetBinContent(gBin, 0);
					if (setError) h->SetBinError(gBin, 0);
				}
			}
		}
	}
}
template <typename H5>
void SetHistXRangeZero(H5* h, Double_t x_low, Double_t x_up)
{
	//NOTE: x_low and x_up must be bordor of bin.
	x_low = x_low + 1e-3;
	x_up = x_up - 1e-3;
	int bin_low = h->GetXaxis()->FindBin(x_low);
	int bin_up = h->GetXaxis()->FindBin(x_up);
	for (int ix = 1; ix <= h->GetNbinsX(); ++ix)
	{
		for (int iy = 1; iy <= h->GetNbinsY(); ++iy)
		{
			for (int iz = 1; iz <= h->GetNbinsZ(); ++iz)
			{
				Int_t gBin = h->GetBin(ix, iy, iz);
				if (ix >= bin_low && ix <= bin_up)
				{
					h->SetBinContent(gBin, 0.0);
					h->SetBinError(gBin, 0.0);
				}
			}
		}
	}
}
template <typename H6>
std::tuple<Float_t, Float_t, Float_t, Float_t, Float_t> CalSignificance(H6* h_N, H6* h_B, Float_t x_low, Float_t x_up)
{

	if (h_N->GetDimension() > 1 || h_B->GetDimension() > 1) {
		cout << "ERROR! PLease input 1-D hisogram! Exiting...";
			return std::make_tuple(0, 0, 0, 0, 0);
	}
	Float_t N = 0;
	Float_t	B = 0;
	Float_t N_width, B_width;
	Float_t N_error2 = 0;
	Float_t	B_error2 = 0;
	Int_t bin_low = h_N->GetXaxis()->FindBin(x_low + 1e-3);
	Int_t bin_up  = h_N->GetXaxis()->FindBin(x_up  - 1e-3);
	for (int ix = bin_low; ix <= bin_up; ix++)
	{
		N += h_N->GetBinContent(ix) * h_N->GetBinWidth(ix);
		B += h_B->GetBinContent(ix) * h_B->GetBinWidth(ix);
		N_error2 += TMath::Power(h_N->GetBinError(ix) * h_N->GetBinWidth(ix),2);
		B_error2 += TMath::Power(h_B->GetBinError(ix) * h_B->GetBinWidth(ix),2);
	}
	Float_t signif = (N - B) / TMath::Sqrt(N_error2 + B_error2);//TMath::Sqrt(N_error2+B_error2);
	return std::make_tuple(signif, N, N_error2, B, B_error2);
}
template <typename H7>
H7* CalSignificance(H7* h_N, H7* h_B, TString newName = "h_Mee__Signif")
{
	if (h_N->GetDimension() > 1 || h_B->GetDimension() > 1) {
		cout << "ERROR! PLease input 1-D hisogram! Exiting...";
		return nullptr;
	}
	H7* h_Signif = (H7*)h_N->Clone(newName);
	Float_t N = 0;
	Float_t	B = 0;
	Float_t N_width, B_width;
	Float_t N_error2 = 0;
	Float_t	B_error2 = 0;
	for (int ix = 1; ix <= h_N->GetNbinsX(); ++ix)
	{
		N = h_N->GetBinContent(ix) * h_N->GetBinWidth(ix);
		B = h_B->GetBinContent(ix) * h_B->GetBinWidth(ix);
		N_error2 = TMath::Power(h_N->GetBinError(ix) * h_N->GetBinWidth(ix), 2);
		B_error2 = TMath::Power(h_B->GetBinError(ix) * h_B->GetBinWidth(ix), 2);
		h_Signif->SetBinContent(ix, (N - B) / TMath::Sqrt(N_error2 + B_error2));
		h_Signif->SetBinError(ix, 0.0);
	}
	return h_Signif;
}

//��3Dֱ��ͼ(bin�ɱ�)����x�����ȨͶӰ��1Dֱ��ͼ��Ȩ����(y,z)Χ�ɵ����
TH1F* myProject3D2x(TH3* h3, const TString& outName = "h_xproj")
{
	if (!h3) {
		::Error("myProject3D2x", "input TH3 is nullptr");
		return nullptr;
	}

	/* ---------- ����������Ϣ ---------- */
	TAxis* xAxis = h3->GetXaxis();
	TAxis* yAxis = h3->GetYaxis();
	TAxis* zAxis = h3->GetZaxis();

	const Int_t nx = xAxis->GetNbins();
	const Int_t ny = yAxis->GetNbins();
	const Int_t nz = zAxis->GetNbins();

	/* ---------- �½� 1-D ֱ��ͼ��x ���� h3 ��ȫһ�� ---------- */
	std::unique_ptr<double[]> xEdges(new double[nx + 1]);
	for (int ix = 0; ix <= nx; ++ix)
		xEdges[ix] = xAxis->GetBinLowEdge(ix + 1);

	TH1F* h1 = new TH1F(outName,
		TString::Format("%s projection along X with (y,z) area weighting", h3->GetTitle()),
		nx, xEdges.get());
	h1->SetXTitle(h3->GetXaxis()->GetTitle());
	h1->SetYTitle("dN/dx [x]^{-1}");

	/* ---------- ��ÿ�� x-bin ����Ȩ�ۼ� + ���� ---------- */
	for (int ix = 1; ix <= nx; ++ix) {
		double sumW = 0.0;   // ��Ȩ����
		double sumErr2 = 0.0;  // ��Ȩ���ƽ����

		for (int iy = 1; iy <= ny; ++iy) {
			double dy = yAxis->GetBinWidth(iy);
			for (int iz = 1; iz <= nz; ++iz) {
				double dz = zAxis->GetBinWidth(iz);
				double area = dy * dz;

				double c = h3->GetBinContent(ix, iy, iz);
				double e = h3->GetBinError(ix, iy, iz);

				// ����û��������趨��������ʹ�ã������� sqrt(c)
				if (e == 0.0 && c > 0) e = std::sqrt(c);

				sumW += c * area;
				sumErr2 += (e * area) * (e * area);
			}
		}
		h1->SetBinContent(ix, sumW);
		h1->SetBinError(ix, std::sqrt(sumErr2));
	}

	return h1;   // �����߸�������ڴ�
}

void Draw_Mee_Ptslice(TH3F* h_Mee_Pt_Cen__unlikeSame_Rebin,TH3F* h_Mee_Pt_Cen__LikeSame_Rebin,TH3F* h_Mee_Pt_Cen__unlikeMixed_Rebin,TH3F* h_Mee_Pt_Cen__rmLS_Rebin,TH3F* h_Mee_Pt_Cen__rmUM_Rebin,float y_low,float y_up)
{
	float ylow = y_low + 0.001;
	float yup = y_up - 0.001;
	int bin_low = h_Mee_Pt_Cen__unlikeSame_Rebin->GetYaxis()->FindBin(y_low);
	int bin_up = h_Mee_Pt_Cen__unlikeSame_Rebin->GetYaxis()->FindBin(y_up);
	TH1F *h_Mee_Pt020p1__unlikeSame_Rebin = (TH1F*)h_Mee_Pt_Cen__unlikeSame_Rebin->ProjectionX("_px", bin_low, bin_up, 0, -1);	  ResetBinContent(h_Mee_Pt020p1__unlikeSame_Rebin);
	TH1F *h_Mee_Pt020p1__LikeSame_Rebin = (TH1F*)h_Mee_Pt_Cen__LikeSame_Rebin->ProjectionX("_px", bin_low, bin_up, 0, -1);		  ResetBinContent(h_Mee_Pt020p1__LikeSame_Rebin);
	TH1F *h_Mee_Pt020p1__unlikeMixed_Rebin = (TH1F*)h_Mee_Pt_Cen__unlikeMixed_Rebin->ProjectionX("_px", bin_low, bin_up, 0, -1);  ResetBinContent(h_Mee_Pt020p1__unlikeMixed_Rebin);
	TH1F *h_Mee_Pt020p1__rmLS_Rebin = (TH1F*)h_Mee_Pt_Cen__rmLS_Rebin->ProjectionX("_px", bin_low, bin_up, 0, -1);				  ResetBinContent(h_Mee_Pt020p1__rmLS_Rebin);
	TH1F *h_Mee_Pt020p1__rmUM_Rebin = (TH1F*)h_Mee_Pt_Cen__rmUM_Rebin->ProjectionX("_px", bin_low, bin_up, 0, -1);				  ResetBinContent(h_Mee_Pt020p1__rmUM_Rebin);

	h_Mee_Pt020p1__unlikeSame_Rebin->SetTitle(Form("Raw signal of e^{+}e^{-} with p_{T} range of %.2f ~ %.2f GeV/c;M_{ee} (GeV/c^{2});dN/dM_{ee} (GeV/c^{2})^{-1}",y_low,y_up));
	h_Mee_Pt020p1__unlikeSame_Rebin->SetMaximum(1e8);
	h_Mee_Pt020p1__unlikeSame_Rebin->SetMinimum(1e0);
	h_Mee_Pt020p1__unlikeSame_Rebin->DrawClone("");
	h_Mee_Pt020p1__LikeSame_Rebin->DrawClone("same");
	//h_Mee_Pt020p1__unlikeMixed_Rebin->DrawClone("same");
	h_Mee_Pt020p1__rmLS_Rebin->DrawClone("same");
	//h_Mee_Pt020p1__rmUM_Rebin->DrawClone("same");

	TPaveText *pt = new TPaveText(0.2, 0.75, 0.4, 0.85, "NDC NB");
	pt->SetFillColorAlpha(0, 0);   // ͸����
	pt->SetFillStyle(0);
	pt->SetBorderSize(0);
	pt->SetTextFont(42);
	pt->SetTextSize(0.09);
	pt->SetTextAlign(12);
	pt->AddText(Form("%.1f<p_{T}^{ee}<%.1f",y_low,y_up));
	pt->DrawClone("same");
}

// Recalibrate.C
// ��Ȩƽ���� bin count
TH2F* MeanOfH3D_binCount(TH3F* h3, const char* outputName = "h2_zmean_binCount")
{
	int nBinsX = h3->GetNbinsX();
	int nBinsY = h3->GetNbinsY();
	int nBinsZ = h3->GetNbinsZ();

	// ������άֱ��ͼ�洢 z �ľ�ֵ�������� h3 ��ͬ�� x,y �ᶨ�壩
	TH2F *h2_zmean = new TH2F("outputName", "Mean nsigmaE value;X(Pt,eta,phi);Cen",
		nBinsX, h3->GetXaxis()->GetXmin(), h3->GetXaxis()->GetXmax(),
		nBinsY, h3->GetYaxis()->GetXmin(), h3->GetYaxis()->GetXmax());

	// ����ÿ�� (x,y) bin
	for (int ix = 1; ix <= nBinsX; ix++)
	{
		for (int iy = 1; iy <= nBinsY; iy++)
		{
			float sum_weights = 0.0;
			float sum_weighted_z = 0.0;

			// ���� z ������Ȩƽ��
			for (int iz = 1; iz <= nBinsZ; iz++)
			{
				float weight = h3->GetBinContent(ix, iy, iz);
				float z_center = h3->GetZaxis()->GetBinCenter(iz);
				sum_weights += weight;
				sum_weighted_z += weight * z_center;
			}

			// �����ֵ�����������������ȫ�գ���Ϊ 0 �� histo Ĭ��ֵ��
			float mean_z = (sum_weights > 0) ? sum_weighted_z / sum_weights : 0.0;
			h2_zmean->SetBinContent(ix, iy, mean_z);

			// ��ѡ�����㲢��������׼��� of the mean��
			if (sum_weights > 0)
			{
				float sum_sq_diff = 0.0;
				for (int iz = 1; iz <= nBinsZ; iz++)
				{
					float weight = h3->GetBinContent(ix, iy, iz);
					if (weight == 0) continue;
					float z_center = h3->GetZaxis()->GetBinCenter(iz);
					sum_sq_diff += weight * pow(z_center - mean_z, 2);
				}
				float std_dev = sqrt(sum_sq_diff / sum_weights);
				float error = std_dev / sqrt(sum_weights); // ��׼���
				h2_zmean->SetBinError(ix, iy, error);
			}
		}
	}
	return h2_zmean;
}
// ��˹��Ϸ� gaus fit
TH2F* MeanOfH3D_gausFit(TH3F *h3, const char* outputName = "h2_zmean_gausFit") {

	int nBinsX = h3->GetNbinsX();
	int nBinsY = h3->GetNbinsY();

	// �����洢����Ķ�άֱ��ͼ
	TH2F *h2_mean = new TH2F(Form("%s_mean", outputName), "Z Mean from Gaussian Fit;X(Pt,eta,phi);Cen",
		nBinsX, h3->GetXaxis()->GetXmin(), h3->GetXaxis()->GetXmax(),
		nBinsY, h3->GetYaxis()->GetXmin(), h3->GetYaxis()->GetXmax());

	// �����˹������gaus = [0]*exp(-0.5*((x-[1])/[2])^2)
	// [0]=Amplitude, [1]=Mean, [2]=Sigma
	TF1 *gaus = new TF1("gaus_fit", "gaus", h3->GetZaxis()->GetXmin(), h3->GetZaxis()->GetXmax());

	int fitSuccess = 0;
	int fitFailed = 0;
	// ����ÿ�� (x,y) bin
	for (int ix = 1; ix <= nBinsX; ix++) {
		for (int iy = 1; iy <= nBinsY; iy++) {

			// ��ȡ�� (x,y) λ�õ� z ��ͶӰ�����Զ������µ� TH1F��
			TH1F* h1_z = (TH1F*)h3->ProjectionZ("_temp", ix, ix, iy, iy);

			// ����ͳ����̫�ٵ� bin���ɸ�����Ҫ������ֵ��
			if (h1_z->GetEntries() < 10) {
				h2_mean->SetBinContent(ix, iy, 0);
				h2_mean->SetBinError(ix, iy, 0);
				delete h1_z;
				fitFailed++;
				continue;
			}

			// ���ó�ʼ�����²⣨�������ȶ��ԣ�
			float mean_est = h1_z->GetMean();
			float sigma_est = h1_z->GetRMS();
			float amp_est = h1_z->GetMaximum();
			gaus->SetParameters(amp_est, mean_est, sigma_est);

			// ���Ʋ�����Χ����ѡ����ֹ��Ϸ�ɢ��
			gaus->SetParLimits(1, h3->GetZaxis()->GetXmin(), h3->GetZaxis()->GetXmax()); // Mean
			gaus->SetParLimits(2, 0, (h3->GetZaxis()->GetXmax() - h3->GetZaxis()->GetXmin())); // Sigma > 0

			// ִ�����
			// "Q"=Quiet, "N"=No drawing, "M"=Improve fit with Minuit
			int fitStatus = h1_z->Fit(gaus, "Q N L", "", h3->GetZaxis()->GetXmin(), h3->GetZaxis()->GetXmax());
			// ����������
			bool isGoodFit = (fitStatus == 0) && (gaus->GetParError(1) > 0)
				&& (gaus->GetParameter(2) > 0); // sigma����Ϊ��

			if (isGoodFit) {
				float mean = gaus->GetParameter(1);
				float meanErr = gaus->GetParError(1);
				h2_mean->SetBinContent(ix, iy, mean);
				h2_mean->SetBinError(ix, iy, meanErr);
				fitSuccess++;
			}
			else {
				// ���ʧ��ʱ���˵��򵥾�ֵ���㣬����Ϊ 0
				h2_mean->SetBinContent(ix, iy, h1_z->GetMean());
				h2_mean->SetBinError(ix, iy, h1_z->GetMeanError());
				fitFailed++;
			}

			delete h1_z; // �����ͷ��ڴ棬��ֹ�ڴ�й©
		}
	}
	delete gaus;

	printf(" fitting done: %d succeeded, %d failed (or low stats)\n", fitSuccess, fitFailed);
	return h2_mean;
}
//����ÿ��bin��ƽ��ֵ
TH1F* Meanof2DAlongX(const TH2F* h2, const char* outputName = "h1_xmean")
{
	int nybins = h2->GetNbinsY();
	const TAxis* yaxis = h2->GetYaxis();

	// ���� 1F ֱ��ͼ��X ���Ӧԭ Y ��
	TH1F* h1_xmean = new TH1F(outputName, ";Cen8;Mean along x",
		nybins, yaxis->GetXmin(), yaxis->GetXmax());

	for (int iy = 1; iy <= nybins; iy++) {
		double sum = 0;
		int nxbins = h2->GetNbinsX();
		int count = 0;

		for (int ix = 1; ix <= nxbins; ix++) {
			double content = h2->GetBinContent(ix, iy);
			// ������������Ƿ������� bin
			if (content == 0) continue;
			sum += content;
			count++;
		}

		double mean = (count > 0) ? sum / count : 0;
		h1_xmean->SetBinContent(iy, mean);
	}
	return h1_xmean;
}
//��ֱ��ͼ��contentȡ��ֵ
void NegateBinContents(TH1F* hist) {
	// ���ֱ��ͼָ���Ƿ���Ч
	if (!hist) {
		std::cerr << "Error: Null histogram pointer!" << std::endl;
		return;
	}

	// ��ȡֱ��ͼ��bin����
	int nBinsX = hist->GetNbinsX();

	// ��������bin������underflow bin 0 �� overflow bin nBinsX+1��
	for (int i = 0; i <= nBinsX + 1; ++i) {
		double content = hist->GetBinContent(i);
		double error = hist->GetBinError(i);

		// contentȡ��
		hist->SetBinContent(i, -content);
		// ������ֵ������ǶԳƵģ�ȡ����ֵ��
		hist->SetBinError(i, std::abs(error));
	}
}