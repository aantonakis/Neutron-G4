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

    // Defaults
    int nEvents = 1000;
    G4double energy = 1.0 * MeV;

    // CLI: ./neutron_argon <nEvents> <E> <unit>
    if (argc >= 2) nEvents = std::atoi(argv[1]);
    if (argc >= 4) {
        const std::string eStr = argv[2];
        const std::string uStr = argv[3];
        energy = std::stod(eStr) * G4UIcommand::ValueOf(uStr.c_str());
    }

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

    auto* genAction = new PrimaryGeneratorAction;
    genAction->SetEnergy(energy);
    //runManager->SetUserAction(new PrimaryGeneratorAction);
    runManager->SetUserAction(genAction);
    runManager->SetUserAction(new SteppingAction(eventAction));

    runManager->Initialize();

    auto ui = G4UImanager::GetUIpointer();
    //ui->ApplyCommand("/run/beamOn 10000");

    ui->ApplyCommand("/run/beamOn " + std::to_string(nEvents));

    // ---- IMPORTANT FIX ----
    G4RunManagerKernel::GetRunManagerKernel()->RunTermination();

    delete runManager;
    return 0;
}

