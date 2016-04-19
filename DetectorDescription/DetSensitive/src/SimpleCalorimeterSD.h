#ifndef DETSENSITIVE_SIMPLECALORIMETERSD_H
#define DETSENSITIVE_SIMPLECALORIMETERSD_H

// FCCSW
#include "BaseSD.h"

// DD4hep
#include "DDG4/Geant4Hits.h"
#include "DDSegmentation/Segmentation.h"

// Geant
#include "G4VSensitiveDetector.hh"
#include "G4THitsCollection.hh"

/** SimpleCalorimeterSD DetectorDescription/DetSensitive/src/SimpleCalorimeterSD.h SimpleCalorimeterSD.h
 *
 *  Simple sensitive detector for calorimeter (accumulates energy deposits).
 *  It is based on DD4hep::Simulation::Geant4GenericSD<Calorimeter> (but it is not identical).
 *  In particular, the position of the hit is set to middle step between
 *  G4Step::GetPreStepPoint() and G4Step::GetPostStepPoint()
 *  No timing information is saved (energy deposits aggregated in the cells)
 *
 *  @author    Anna Zaborowska
 */

namespace det {
class SimpleCalorimeterSD : public G4VSensitiveDetector, public BaseSD
{
  public:
  /** Constructor.
   *  @param aDetectorName Name of the detector
   *  @param aReadoutName Name of the readout (used to name the collection)
   *  @param aSeg Segmentation of the detector (used to retrieve the cell ID)
   */
  SimpleCalorimeterSD(std::string aDetectorName,
    std::string aReadoutName,
    DD4hep::Geometry::Segmentation aSeg);
  /// Destructor
  ~SimpleCalorimeterSD();
  /** Initialization.
   *  Creates the hit collection with the name passed in the constructor.
   *  The hit collection is registered in Geant.
   *  @param aHitsCollections Geant hits collection.
   */
  virtual void Initialize(G4HCofThisEvent* aHitsCollections) final;
  /** Process hit once the particle hit the sensitive volume.
   *  Checks if the energy deposit is larger than 0, calculates the position and cellID,
   *  saves that into the hit collection.
   *  If there is already entry in the same cell, the energy is accumulated.
   *  Otherwise new hit is created.
   *  @param aStep Step in which particle deposited the energy.
   */
  virtual bool ProcessHits(G4Step* aStep, G4TouchableHistory*) final;

private:
  /// Collection of calorimeter hits
  G4THitsCollection<DD4hep::Simulation::Geant4CalorimeterHit> *calorimeterCollection;
};
}

#endif /* DETSENSITIVE_SIMPLECALORIMETERSD_H */
