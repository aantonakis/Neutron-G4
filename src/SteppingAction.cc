#include "SteppingAction.hh"
#include "EventAction.hh"

#include "G4Step.hh"
#include "G4Neutron.hh"
#include "G4VProcess.hh"
#include "G4SystemOfUnits.hh"

#include <cmath>

SteppingAction::SteppingAction(EventAction* evt)
    : fEvent(evt) {}

void SteppingAction::UserSteppingAction(const G4Step* step)
{
    auto track = step->GetTrack();
    if (track->GetDefinition() != G4Neutron::Definition()) return;

    auto post = step->GetPostStepPoint();
    auto process = post->GetProcessDefinedStep();
    if (!process) return;

    if (process->GetProcessName() == "hadElastic") {

        auto pre = step->GetPreStepPoint();

        double ER = pre->GetKineticEnergy() - post->GetKineticEnergy();

        auto pin  = pre->GetMomentumDirection();
        auto pout = post->GetMomentumDirection();
        double theta = pin.angle(pout);

        // Lindhard ionization
        double ER_keV = ER / keV;
        double eps = 11.5 * pow(18., -7./3.) * ER_keV;
        double g = 3*pow(eps,0.15) + 0.7*pow(eps,0.6) + eps;
        double k = 0.133;
        double f = (k*g)/(1 + k*g);
        double Ne = (f * ER) / (26.4 * eV);

        fEvent->AddElasticScatter(ER, theta, Ne);
    }
}

