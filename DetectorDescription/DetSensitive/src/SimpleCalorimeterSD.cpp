#include "SimpleCalorimeterSD.h"

// DD4hep
#include "DDG4/Geant4Mapping.h"
#include "DDG4/Geant4VolumeManager.h"

// CLHEP
#include "CLHEP/Vector/ThreeVector.h"

namespace det {
SimpleCalorimeterSD::SimpleCalorimeterSD(std::string aDetectorName,
  std::string aReadoutName,
  DD4hep::Geometry::Segmentation aSeg)
  :G4VSensitiveDetector(aDetectorName), BaseSD(aSeg) {
  // name of the collection of hits is determined byt the readout name (from XML)
  collectionName.insert(aReadoutName);
  std::cout<<" Adding a collection with the name: "<<aReadoutName<<std::endl;
}

SimpleCalorimeterSD::~SimpleCalorimeterSD(){;}

void SimpleCalorimeterSD::Initialize(G4HCofThisEvent* aHitsCollections)
{
  // create a collection of hits and add it to G4HCofThisEvent
  // get id for collection
  static int HCID = -1;
  calorimeterCollection = new G4THitsCollection
    <DD4hep::Simulation::Geant4CalorimeterHit>(SensitiveDetectorName,collectionName[0]);
  if(HCID<0)
    HCID = GetCollectionID(0);
  aHitsCollections->AddHitsCollection(HCID,calorimeterCollection);
}

bool SimpleCalorimeterSD::ProcessHits(G4Step* aStep, G4TouchableHistory*)
{
  // check if energy was deposited
  G4double edep = aStep->GetTotalEnergyDeposit();
  if(edep==0.)
    return false;

  // as in DD4hep::Simulation::Geant4GenericSD<Calorimeter>
  CLHEP::Hep3Vector prePos = aStep->GetPreStepPoint()->GetPosition();
  CLHEP::Hep3Vector postPos = aStep->GetPostStepPoint()->GetPosition();
  CLHEP::Hep3Vector midPos = 0.5*(postPos + prePos);
  DD4hep::Simulation::Position pos(midPos.x(), midPos.y(), midPos.z());
  // check the cell ID
  uint64_t id = getCellID(aStep);
  DD4hep::Simulation::Geant4CalorimeterHit* hit, *hitMatch = nullptr;
  // Check if there is already some energy deposit in that cell
  for(int i=0; i<calorimeterCollection->entries(); i++) {
    hit = dynamic_cast<DD4hep::Simulation::Geant4CalorimeterHit*>
      (calorimeterCollection->GetHit(i));
    if(hit->cellID == id) {
      hitMatch = hit;
      hitMatch->energyDeposit += edep;
      return true;
    }
  }
  // if not, create a new hit
  if ( !hitMatch )  {
    hitMatch = new DD4hep::Simulation::Geant4CalorimeterHit(pos);
    hitMatch->cellID  = id;
    hitMatch->energyDeposit = edep;
    calorimeterCollection->insert(hitMatch);
    return true;
  }
  return false;
}
}
