#pragma once
#include "G4UserEventAction.hh"

class EventAction : public G4UserEventAction {
public:
    EventAction();
    ~EventAction() override = default;

    void BeginOfEventAction(const G4Event*) override;
    void EndOfEventAction(const G4Event*) override;

    // Called by SteppingAction
    void AddElasticScatter(double ER, double theta, double Ne);

private:
    int    fNscatter;
    double fER;     // total recoil energy
    double fTheta;  // last scatter angle
    double fNe;     // total ionization electrons
};

