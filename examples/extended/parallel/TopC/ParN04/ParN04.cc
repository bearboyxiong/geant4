//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
/// \file parallel/ParN04/ParN04.cc
/// \brief Main program of the parallel/ParN04 example
//
//
// $Id: ParN04.cc 76296 2013-11-08 13:17:47Z gcosmo $
//
// 
// --------------------------------------------------------------
//      GEANT 4 - ParN04
// --------------------------------------------------------------


#include "G4RunManager.hh"
#include "G4UImanager.hh"

#include "ExN04DetectorConstruction.hh"
#include "QGSP_BERT.hh"
#include "ExN04PrimaryGeneratorAction.hh"
#include "ExN04RunAction.hh"
#include "ExN04EventAction.hh"
#include "ExN04StackingAction.hh"
#include "ExN04TrackingAction.hh"
#include "ExN04SteppingAction.hh"
#include "ExN04SteppingVerbose.hh"

#ifdef G4VIS_USE
#include "G4VisExecutive.hh"
#endif

#ifdef G4UI_USE
#include "G4UIExecutive.hh"
#endif

#include "ParTopC.icc"

int main(int argc,char** argv)
{
  // User Verbose output class
  //
  G4VSteppingVerbose* verbosity = new ExN04SteppingVerbose;
  G4VSteppingVerbose::SetInstance(verbosity);
  
  // Run manager
  //
  G4RunManager* runManager = new G4RunManager;

  // User Initialization classes (mandatory)
  //
  G4VUserDetectorConstruction* detector = new ExN04DetectorConstruction;
  runManager->SetUserInitialization(detector);
  //
  G4VUserPhysicsList* physics = new QGSP_BERT();
  runManager->SetUserInitialization(physics);
  
#ifdef G4VIS_USE
  // Visualization, if you choose to have it!
  //
  G4VisManager* visManager = new G4VisExecutive;
  visManager->Initialize();
#endif

  runManager->Initialize();

  // User Action classes
  //
  G4VUserPrimaryGeneratorAction* gen_action = new ExN04PrimaryGeneratorAction;
  runManager->SetUserAction(gen_action);
  //
  G4UserRunAction* run_action = new ExN04RunAction;
  runManager->SetUserAction(run_action);
  //
  G4UserEventAction* event_action = new ExN04EventAction;
  runManager->SetUserAction(event_action);
  //
  G4UserStackingAction* stacking_action = new ExN04StackingAction;
  runManager->SetUserAction(stacking_action);
  //
  G4UserTrackingAction* tracking_action = new ExN04TrackingAction;
  runManager->SetUserAction(tracking_action);
  //
  G4UserSteppingAction* stepping_action = new ExN04SteppingAction;
  runManager->SetUserAction(stepping_action);
  
  // Get the pointer to the User Interface manager   
  //
  G4UImanager* UImanager = G4UImanager::GetUIpointer();  

  if(argc==1)  // Define (G)UI terminal for interactive mode
  {
#ifdef G4UI_USE
    G4UIExecutive* ui = new G4UIExecutive(argc, argv);
    UImanager->ApplyCommand("/control/execute vis.mac");
    ui->SessionStart();
    delete ui;
#endif
  }
  else  // Batch mode
  {
    G4String command = "/control/execute ";
    G4String fileName = argv[1];
    UImanager->ApplyCommand(command+fileName);
  }

  // Free the store: user actions, physics_list and detector_description are
  //                 owned and deleted by the run manager, so they should not
  //                 be deleted in the main() program !

#ifdef G4VIS_USE
  delete visManager;
#endif
  delete runManager;
  delete verbosity;

  return 0;
}