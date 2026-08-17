void temp()
{
    // Define input files with labels
    const Int_t nFiles = 3;
    TString fileNames[nFiles] = {
        "roots/63_20260708_OO_iTPC11_PureE.root",
        "roots/61_20260703_OO_iTPC1p2_PureE.root",
        "roots/62_20260704_OO_iTPC1p2p3_PureE.root"
    };
    TString labels[nFiles] = {
        "iTPC1",
        "iTPC2",
        "iTPC3"
    };

    // Histogram names to draw
    TString hNames[2] = {"h_pT__positrons", "h_eta__positrons"};
    TString hTitles[2] = {";p_{T} (GeV/c);Counts", ";#eta;Counts"};

    // Open files and retrieve histograms
    TFile* files[nFiles];
    TH1* hists[nFiles][2];

    for (int i = 0; i < nFiles; i++)
    {
        files[i] = new TFile(fileNames[i]);
        if (!files[i] || files[i]->IsZombie())
        {
            std::cerr << "Error: Cannot open " << fileNames[i] << std::endl;
            return;
        }
        for (int j = 0; j < 2; j++)
        {
            hists[i][j] = (TH1*)files[i]->Get(hNames[j]);
            if (!hists[i][j])
            {
                std::cerr << "Error: Cannot find " << hNames[j] << " in " << fileNames[i] << std::endl;
                return;
            }
            hists[i][j]->SetDirectory(nullptr);
        }
    }

    // Styling
    Color_t colors[nFiles] = {kGreen, kRed, kBlue};
    Style_t markers[nFiles] = {kOpenSquare, kOpenSquare, kOpenSquare};

    for (int i = 0; i < nFiles; i++)
    {
        for (int j = 0; j < 2; j++)
        {
            hists[i][j]->SetLineColor(colors[i]);
            hists[i][j]->SetMarkerColor(colors[i]);
            hists[i][j]->SetMarkerStyle(markers[i]);
            hists[i][j]->SetMarkerSize(0.3);
        }
    }

    // Draw on 2x3 canvas
    TCanvas* c = new TCanvas("c", "Electron pT and Eta Comparison", 1000, 1200);
    c->Divide(2, 3);

    for (int i = 0; i < nFiles; i++)
    {
        // Top row: pT
        c->cd(2*i + 1);
        gPad->SetLogy(1);
        gStyle->SetOptStat(0);
        hists[i][0]->GetXaxis()->SetRangeUser(0,2);
        hists[i][0]->DrawClone("PE");

        // Bottom row: Eta
        c->cd(2*i + 2);
        gPad->SetLogy(1);
        gStyle->SetOptStat(0);
        hists[i][1]->SetMaximum(1e5);
        hists[i][1]->SetMinimum(10);
        hists[i][1]->DrawClone("PE");
    }

    c->SaveAs("roots/59_61_62_positron_pT_Eta.png");
}
