#include "PrimaryGeneratorAction.hh"

#include "G4ParticleGun.hh"
#include "G4Neutron.hh"
#include "G4SystemOfUnits.hh"

PrimaryGeneratorAction::PrimaryGeneratorAction()
{
    fGun = new G4ParticleGun(1);
    fGun->SetParticleDefinition(G4Neutron::Definition());
    fGun->SetParticleEnergy(1.0*MeV);
    fGun->SetParticleMomentumDirection({0,0,1});
    fGun->SetParticlePosition({0,0,-40*cm});
}

PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
    delete fGun;
}

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* event)
{
    fGun->GeneratePrimaryVertex(event);
}

