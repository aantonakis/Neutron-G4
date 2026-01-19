#include "EventAction.hh"
#include "RunAction.hh"
#include "G4RunManager.hh"

EventAction::EventAction()
{
    fNscatter = 0;
    fER = 0.;
    fTheta = 0.;
    fNe = 0.;
}

void EventAction::BeginOfEventAction(const G4Event*)
{
    fNscatter = 0;
    fER = 0.;
    fTheta = 0.;
    fNe = 0.;
}

void EventAction::AddElasticScatter(double ER, double theta, double Ne)
{
    fNscatter++;
    fER += ER;
    fTheta = theta;
    fNe += Ne;
}

void EventAction::EndOfEventAction(const G4Event*)
{

    auto runAction = const_cast<RunAction*>(
        static_cast<const RunAction*>(
            G4RunManager::GetRunManager()->GetUserRunAction()));


    runAction->FillTree(fNscatter, fER, fTheta, fNe);
}

