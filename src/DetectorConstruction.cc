#include "DetectorConstruction.hh"

#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4Material.hh"
#include "G4NistManager.hh"
#include "G4SystemOfUnits.hh"

G4VPhysicalVolume* DetectorConstruction::Construct()
{
    auto nist = G4NistManager::Instance();

    // World
    auto worldMat = nist->FindOrBuildMaterial("G4_Galactic");
    auto solidWorld = new G4Box("World", 2*m, 2*m, 2*m);
    auto logicWorld = new G4LogicalVolume(solidWorld, worldMat, "World");
    auto physWorld =
        new G4PVPlacement(nullptr, {}, logicWorld, "World", nullptr, false, 0);

    // Argon gas (STP)
    G4double density = 1.784e-3 * g/cm3;
    auto argonGas = new G4Material(
        "ArgonGas", 18., 39.948*g/mole, density,
        kStateGas, 273.15*kelvin, atmosphere);

    auto solidGas = new G4Box("Gas", 50*cm, 50*cm, 50*cm);
    auto logicGas = new G4LogicalVolume(solidGas, argonGas, "Gas");

    new G4PVPlacement(nullptr, {}, logicGas, "Gas", logicWorld, false, 0);

    return physWorld;
}

