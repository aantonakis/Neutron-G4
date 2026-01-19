#include "G4RunManager.hh"
#include "G4RunManagerKernel.hh"
#include "G4UImanager.hh"
#include "QGSP_BERT_HP.hh"

#include "DetectorConstruction.hh"
#include "PrimaryGeneratorAction.hh"
#include "EventAction.hh"
#include "SteppingAction.hh"
#include "RunAction.hh"

int main(int argc, char** argv)
{
    auto runManager = new G4RunManager;

    // 1️⃣ Detector
    runManager->SetUserInitialization(new DetectorConstruction);

    // 2️⃣ Physics list — MUST COME BEFORE ACTIONS
    auto* physicsList = new QGSP_BERT_HP;
    runManager->SetUserInitialization(physicsList);

    // 3️⃣ User actions
    auto* runAction   = new RunAction;
    runManager->SetUserAction(runAction);

    auto* eventAction = new EventAction;
    runManager->SetUserAction(eventAction);

    runManager->SetUserAction(new PrimaryGeneratorAction);
    runManager->SetUserAction(new SteppingAction(eventAction));

    runManager->Initialize();

    auto ui = G4UImanager::GetUIpointer();
    ui->ApplyCommand("/run/beamOn 10000");

    // ---- IMPORTANT FIX ----
    G4RunManagerKernel::GetRunManagerKernel()->RunTermination();

    delete runManager;
    return 0;
}

