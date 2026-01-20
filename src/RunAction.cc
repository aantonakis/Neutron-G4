#include "RunAction.hh"

#include "TFile.h"
#include "TTree.h"

RunAction::RunAction()
{
    fFile = new TFile("neutron_argon.root", "RECREATE");
    fTree = new TTree("events", "Neutron-Argon Scattering");

    fTree->Branch("nscatter", &t_nscatter, "nscatter/I");
    fTree->Branch("ER",       &t_ER,       "ER/D");      // eV
    fTree->Branch("theta",    &t_theta,    "theta/D");   // rad
    fTree->Branch("Ne",       &t_Ne,        "Ne/D");
    fTree->Branch("Elastic",  &t_Elastic,   "Elastic/I");
}

RunAction::~RunAction()
{
    fFile->Write();
    fFile->Close();
}

void RunAction::FillTree(int nscat, double ER, double theta, double Ne, int Elastic)
{
    t_nscatter = nscat;
    t_ER       = ER;
    t_theta    = theta;
    t_Ne       = Ne;
    t_Elastic  = Elastic;
    fTree->Fill();
}

