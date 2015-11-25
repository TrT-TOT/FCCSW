#include "GeantFullSimConfig.h"

// Gaudi
#include "GaudiKernel/DeclareFactoryEntries.h"

// Geant4
#include "FTFP_BERT.hh"
#include "G4VModularPhysicsList.hh"
#include "GeantGeneral/TrackingAction.h"

DECLARE_COMPONENT(GeantFullSimConfig)

GeantFullSimConfig::GeantFullSimConfig(const std::string& type, const std::string& name, const IInterface* parent) :
GaudiTool(type, name, parent) {
   declareInterface<IGeantConfigTool>(this);
}

GeantFullSimConfig::~GeantFullSimConfig() {}

StatusCode GeantFullSimConfig::initialize() {
   if(GaudiTool::initialize().isFailure()) {
      return StatusCode::FAILURE;
   }
   return StatusCode::SUCCESS;
}

G4VModularPhysicsList* GeantFullSimConfig::getPhysicsList() {
   // we pass the ownership to Geant4Simulation which will register it in G4RunManager and will be deleted in ~G4RunManager()
   G4VModularPhysicsList* physicsList = new FTFP_BERT;
   return physicsList;
}

G4UserTrackingAction* GeantFullSimConfig::getTrackingAction() {
   // G4VUserTrackingAction deleted in ~G4TrackingManager()
   G4UserTrackingAction* trackingAction = new TrackingAction;
   return trackingAction;
}

StatusCode GeantFullSimConfig::setOther() {
   return StatusCode::SUCCESS;
}
