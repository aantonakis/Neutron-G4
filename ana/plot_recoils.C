// loop_events.C
// Usage:
//   root -l -q 'loop_events.C("neutron_argon.root")'

#include "TFile.h"
#include "TTree.h"
#include <iostream>

void plot_recoils(const char* infile)
{
    // Open file
    TFile* f = TFile::Open(infile, "READ");
    if (!f || f->IsZombie()) {
        std::cerr << "ERROR: cannot open file\n";
        return;
    }

    // Get tree
    TTree* t = nullptr;
    f->GetObject("events", t);
    if (!t) {
        std::cerr << "ERROR: cannot find TTree 'events'\n";
        return;
    }

    TFile* fout = new TFile("output.root", "RECREATE");


    TH1D* h_E = new TH1D("h_E", "h_E", 100, 0, 100);
    TH1D* h_Ne = new TH1D("h_Ne", "h_Ne", 100, 0, 1000);
    TH1D* h_El = new TH1D("h_Elastic", "h_Elastic", 2, -0.5, 1.5);    
    TH1D* h_Scatter = new TH1D("h_Scatter", "h_Scatter", 11, -0.5, 10.5);    
    TH1D* h_Theta = new TH1D("h_Theta", "h_Theta", 180, 0, 180);


    // Variables to read branches into
    int    nscatter;
    double ER;       // stored in MeV (Geant4 internal units)
    double theta;    // rad
    double Ne;
    int    Elastic; // whether the interaction is elastic

    // Connect branches
    t->SetBranchAddress("nscatter", &nscatter);
    t->SetBranchAddress("ER",       &ER);
    t->SetBranchAddress("theta",    &theta);
    t->SetBranchAddress("Ne",       &Ne);
    t->SetBranchAddress("Elastic",  &Elastic);

    const Long64_t nEntries = t->GetEntries();

    std::cout << "nEntries " << nEntries << std::endl;

    for (Long64_t i = 0; i < nEntries; ++i) {
        t->GetEntry(i);

        // Example: print first few events
        if (i < 5) {
            std::cout
              << "Event " << i
              << "  nscatter=" << nscatter
              << "  ER=" << ER*1000.0 << " keV"
              << "  theta=" << theta*180.0/3.1415926535 << " deg"
              << "  Ne=" << Ne
              << std::endl;

        }
        if (nscatter > 0) {
          h_E->Fill(ER*1000);
          h_Ne->Fill(Ne);
          h_El->Fill(Elastic);
          h_Scatter->Fill(nscatter);
          h_Theta->Fill(theta*180.0/3.1415926535);
        } 
    
    }
    h_E->SetDirectory(0);
    h_El->SetDirectory(0);
    h_Ne->SetDirectory(0);
    h_Scatter->SetDirectory(0);
    h_Theta->SetDirectory(0);

    fout->cd();
    h_E->Write();
    h_Ne->Write();
    h_El->Write();
    h_Scatter->Write();
    h_Theta->Write();
    fout->Close();

    f->Close();
}

