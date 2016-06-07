// G4 Process: Gheisha High Energy Collision model.
// This includes the high energy cascading model, the two-body-resonance model
// and the low energy two-body model. Not included are the low energy stuff like
// nuclear reactions, nuclear fission without any cascading and all processes for
// particles at rest.  
// First work done by J.L.Chuma and F.W.Jones, TRIUMF, June 96.  
// H. Fesefeldt, RWTH-Aachen, 23-October-1996
// Last modified: 29-July-1998 
 
#include "G4HEKaonZeroLongInelastic.hh"

G4VParticleChange *  G4HEKaonZeroLongInelastic::
ApplyYourself( const G4Track &aTrack, G4Nucleus &targetNucleus )
  {     
    G4HEKaonZeroInelastic theKaonZeroInelastic;
    G4HEAntiKaonZeroInelastic theAntiKaonZeroInelastic;
    theKaonZeroInelastic.SetVerboseLevel(verboseLevel);
    theAntiKaonZeroInelastic.SetVerboseLevel(verboseLevel);
    
    if(G4UniformRand() < 0.50)
      {
         return theKaonZeroInelastic.ApplyYourself(aTrack, targetNucleus);
      }
    else
      {
         return theAntiKaonZeroInelastic.ApplyYourself(aTrack, targetNucleus);
      }
  } 
        







