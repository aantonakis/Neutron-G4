#pragma once
#include "G4UserRunAction.hh"

class TFile;
class TTree;

class RunAction : public G4UserRunAction {
public:
    RunAction();
    ~RunAction() override;

    void FillTree(int nscat, double ER, double theta, double Ne, int Elastic);

private:
    TFile* fFile;
    TTree* fTree;

    int    t_nscatter;
    double t_ER;
    double t_theta;
    double t_Ne;
    int    t_Elastic;
};

