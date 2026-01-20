#include "SteppingAction.hh"
#include "EventAction.hh"

#include "G4Step.hh"
#include "G4Track.hh"
#include "G4Neutron.hh"
#include "G4VProcess.hh"
#include "G4SystemOfUnits.hh"
#include "G4ParticleDefinition.hh"

#include <cmath>

SteppingAction::SteppingAction(EventAction* evt)
    : fEvent(evt) {}

void SteppingAction::UserSteppingAction(const G4Step* step)
{
    auto track = step->GetTrack();
    if (track->GetDefinition() != G4Neutron::Definition()) return;

    auto pre = step->GetPreStepPoint();
    auto post = step->GetPostStepPoint();
    auto process = post->GetProcessDefinedStep();
    if (!process) return;

    // Full 3-momenta of the neutron (lab frame)
    const G4ThreeVector p_in  = pre->GetMomentum();   // MeV/c units internally
    const G4ThreeVector p_out = post->GetMomentum();

    if (process->GetProcessName() == "hadElastic") {


        double ER = pre->GetKineticEnergy() - post->GetKineticEnergy();
        // Recoil nucleus momentum from momentum conservation
        const G4ThreeVector p_recoil = p_in - p_out;
        const G4ThreeVector n_in_hat = p_in.unit();

        // Nuclear recoil angle for elastic scattering
        G4double theta = p_recoil.angle(n_in_hat);

        //auto pin  = pre->GetMomentumDirection();
        //auto pout = post->GetMomentumDirection();
        //double theta = pin.angle(pout);

        // Lindhard ionization
        double ER_keV = ER / keV;
        double eps = 11.5 * pow(18., -7./3.) * ER_keV;
        double g = 3*pow(eps,0.15) + 0.7*pow(eps,0.6) + eps;
        double k = 0.133;
        double f = (k*g)/(1 + k*g);
        double Ne = (f * ER) / (26.4 * eV);

        fEvent->AddElasticScatter(ER, theta, Ne, 1);
    }
    else if (process->GetProcessName() == "neutronInelastic") {

        // Beam reference direction options
        const G4ThreeVector n_in_hat = p_in.unit();  // incoming direction for THIS interaction
        //const G4ThreeVector n0_hat   = track->GetVertexMomentumDirection().unit(); // original beam

        // Sum momenta of secondaries created in this step
        G4ThreeVector p_secondaries(0,0,0);

        // Optionally: try to identify a nuclear recoil/residual nucleus explicitly
        bool foundNucleus = false;
        G4ThreeVector p_nucleus(0,0,0);

        const auto* secondaries = step->GetSecondaryInCurrentStep();
        if (secondaries) {
          for (const auto* s : *secondaries) {
            if (!s) continue;

            const G4ThreeVector ps = s->GetMomentum();
            p_secondaries += ps;

            // Identify nucleus-like secondaries (often the residual)
            // ParticleType "nucleus" is a common marker for ions/fragments.
            const auto* def = s->GetDefinition();
            if (def && def->GetParticleType() == "nucleus") {
                // If there are multiple nuclear fragments, you may want to keep the
                // heaviest/most energetic; here we just sum all nucleus momenta.
                p_nucleus += ps;
                foundNucleus = true;
            }
          }
        }

        // Missing momentum: what is not carried by tracked outgoing particles
        // (initial target is at rest, so no +p_target term)
        const G4ThreeVector p_missing = p_in - (p_out + p_secondaries);

        // Choose recoil momentum estimate:
        // 1) If nucleus fragments are tracked, use their summed momentum as "recoil".
        // 2) Otherwise use missing momentum (untracked residual recoil).
        G4ThreeVector p_recoil = foundNucleus ? p_nucleus : p_missing;

        // Compute recoil angle(s)
        G4double theta = p_recoil.angle(n_in_hat);

        // (Optional) sanity diagnostics:
        if (foundNucleus)  
          G4cout << "inelastic (nucleus): |p_missing| = " << p_nucleus.mag()/(keV) << " keV/c\n";
        G4cout << "inelastic (missing): |p_missing| = " << p_missing.mag()/(keV) << " keV/c\n";
        // G4cout << "thetaA_local = " << thetaA_local/deg << " deg\n";

        // Choose a mass for the recoiling residual nucleus.
        // If you truly mean "argon atom", use ~Ar-40 nucleus mass.
        // Geant4 has ion masses; simplest is approximate:
        const G4double M = 39.948 * 931.494 * MeV;  // ~37211.6 MeV/c^2

        const G4double p = p_recoil.mag(); // MeV/c in Geant4 internal units

        // Relativistic kinetic energy:
        const G4double ER = std::sqrt(p*p + M*M) - M;  // MeV

        // Lindhard ionization
        double ER_keV = ER / keV;
        double eps = 11.5 * pow(18., -7./3.) * ER_keV;
        double g = 3*pow(eps,0.15) + 0.7*pow(eps,0.6) + eps;
        double k = 0.133;
        double f = (k*g)/(1 + k*g);
        double Ne = (f * ER) / (26.4 * eV);

        fEvent->AddElasticScatter(ER, theta, Ne, 0);
        
    }

}

