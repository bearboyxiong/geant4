// This code implementation is the intellectual property of
// the RD44 GEANT4 collaboration.
//
// By copying, distributing or modifying the Program (or any work
// based on the Program) you indicate your acceptance of this statement,
// and all its terms.
//
// $Id: G4ParticlePropertyMessenger.cc,v 2.4 1998/07/13 17:18:47 urbi Exp $
// GEANT4 tag $Name: geant4-00 $
//
//
//---------------------------------------------------------------
//
//  G4ParticlePropertyMessenger.cc
//
//  Description:
//    This is a messenger class to interface to exchange information
//    between ParticleDefinition and UI.
//
//  History:
//    13 June 1997, H. Kurashige   : The 1st version created.
//    10 Nov. 1997  H. Kurashige   : fixed bugs 
//    08 jan. 1998  H. Kurashige   : new UIcommnds 
//    19 June 1998  H. Kurashige   : adds UnitCategory 
//---------------------------------------------------------------

#include "G4ParticlePropertyMessenger.hh"
#include "G4UImanager.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithoutParameter.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcmdWithABool.hh"
#include "G4ParticleDefinition.hh"
#include "G4DecayTableMessenger.hh"
#include "G4ParticlePropertyMessenger.hh"
#include "G4ParticleTable.hh"
#include "G4ios.hh"                 // Include from 'system'
#include <iomanip.h>                  // Include from 'system'

G4ParticlePropertyMessenger::G4ParticlePropertyMessenger(G4ParticleTable* pTable)
                        :theParticleTable(pTable),
			 currentParticle(NULL),
			 fDecayTableMessenger(NULL)
{
  if ( theParticleTable == NULL) theParticleTable = G4ParticleTable::GetParticleTable();
  //Commnad   /particle/property/
  thisDirectory = new G4UIdirectory("/particle/property/");
  thisDirectory->SetGuidance("Paricle Table control commands.");

  //Commnad   /particle/property/dump
  dumpCmd = new G4UIcmdWithoutParameter("/particle/property/dump",this);
  dumpCmd->SetGuidance("dump particle properties.");

  //Command   /particle/property/stable
  stableCmd = new G4UIcmdWithABool("/particle/property/stable",this);
  stableCmd->SetGuidance("Set stable flag.");
  stableCmd->SetGuidance("  false: Unstable   true: Stable");
  stableCmd->SetParameterName("stable",false);
  stableCmd->AvailableForStates(PreInit,Idle,GeomClosed);

  //particle/property/lifetime
  lifetimeCmd = new G4UIcmdWithADoubleAndUnit("/particle/property/lifetime",this);
  lifetimeCmd->SetGuidance("Set life time.");
  lifetimeCmd->SetGuidance("Unit of the time can be :");
  lifetimeCmd->SetGuidance(" s, ms, ns (default)");
  lifetimeCmd->SetParameterName("life",false);
  lifetimeCmd->SetDefaultValue(0.0);
  lifetimeCmd->SetRange("life >0.0");
  //lifetimeCmd->SetUnitCategory("Time");
  //lifetimeCmd->SetUnitCandidates("s ms ns");
  lifetimeCmd->SetDefaultUnit("ns");
  lifetimeCmd->AvailableForStates(PreInit,Idle,GeomClosed);

  // -- particle/property/Verbose ---
  verboseCmd = new G4UIcmdWithAnInteger("/particle/property/verbose",this);
  verboseCmd->SetGuidance("Set Verbose level of particle property.");
  verboseCmd->SetGuidance(" 0 : Silent (default)");
  verboseCmd->SetGuidance(" 1 : Display warning messages");
  verboseCmd->SetGuidance(" 2 : Display more");
  verboseCmd->SetParameterName("verbose_level",true);
  verboseCmd->SetDefaultValue(0);
  verboseCmd->SetRange("verbose_level >=0");

  //UI messenger for Decay Table
  fDecayTableMessenger = new G4DecayTableMessenger(theParticleTable);
  
}

G4ParticlePropertyMessenger::~G4ParticlePropertyMessenger()
{
  if (fDecayTableMessenger !=NULL) delete  fDecayTableMessenger;
  fDecayTableMessenger = NULL;

  delete stableCmd; 
  delete verboseCmd;
  delete lifetimeCmd;
  delete dumpCmd;
  delete thisDirectory;
} 

void G4ParticlePropertyMessenger::SetNewValue(G4UIcommand * command,G4String newValue)
{
  if (SetCurrentParticle()==NULL) {
      G4cout << "Particle is not selected yet !! Command ignored." << endl;
      return;
  }

  if( command == dumpCmd ){
    //Commnad   /particle/property/dump
    currentParticle->DumpTable();

  } else if (command == lifetimeCmd ) {
    //Commnad   /particle/property/lifetime
    currentParticle->SetPDGLifeTime(lifetimeCmd->GetNewDoubleValue(newValue)); 

  } else if (command == stableCmd ) {
    //Commnad   /particle/property/stable
    if (currentParticle->GetPDGLifeTime()<0.0) {
      G4cout << "Life time is negative! Command ignored." << endl; 
    } else if (currentParticle->GetPDGMass()<=0.0) {
      G4cout << "Zero Mass! Command ignored." << endl; 
    } else {
      currentParticle->SetPDGStable(stableCmd->GetNewBoolValue(newValue));
    }
  
  } else if( command==verboseCmd ) {
    //Commnad   /particle/property/Verbose
    currentParticle->SetVerboseLevel(verboseCmd->GetNewIntValue(newValue)); 
  }
}

G4ParticleDefinition* G4ParticlePropertyMessenger::SetCurrentParticle()
{
  // set currentParticle pointer
  
  // get particle name by asking G4ParticleMessenger via UImanager

  G4String particleName = G4UImanager::GetUIpointer()->GetCurrentStringValue("/particle/select");
	
  if (currentParticle != NULL ){
    // check whether selection is changed 
    if (currentParticle->GetParticleName() != particleName) {
      currentParticle = theParticleTable->FindParticle(particleName);
    }
  } else {
    currentParticle = theParticleTable->FindParticle(particleName);
  }
  return currentParticle;
}

G4String G4ParticlePropertyMessenger::GetCurrentValue(G4UIcommand * command)
{
  G4String returnValue('\0');

  if (SetCurrentParticle()==NULL) {
    // no particle is selected. return null 
    return returnValue;
  }

  if( command == stableCmd ){
    //Commnad   /particle/property/stable
    returnValue = stableCmd->ConvertToString( currentParticle->GetPDGStable());

  } else if( command == lifetimeCmd ){
    //Commnad   /particle/property/lifetime
    returnValue = lifetimeCmd->ConvertToString(  currentParticle->GetPDGLifeTime() , "ns" );
    
  } else if( command==verboseCmd ){
   //Commnad   /particle/property/Verbose
     returnValue= verboseCmd->ConvertToString(currentParticle ->GetVerboseLevel());

  } 
  
  return returnValue;
}







