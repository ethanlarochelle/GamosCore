

#include "GamosCore/GamosApplication/include/GmRunManager.hh"
#include "GamosCore/GamosBase/Base/include/GmAnalysisMgr.hh"

#include "G4UImanager.hh"
#include "G4UIterminal.hh"
#include "G4UItcsh.hh"

#ifdef G4VIS_USE
//#include "GamosCore/GamosBase/include/GmVisManager.hh"
#include "G4VisExecutive.hh"
#endif

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

int main(int argc,char** argv) {

  GmRunManager* runManager = new GmRunManager();

#ifdef G4VIS_USE
  // Visualization, if you choose to have it!
  //  G4VisManager* visManager = new GmVisManager;
  G4VisManager* visManager = new G4VisExecutive;
  visManager->Initialise();
#endif

  //Initialise G4 kernel
  //  runManager->Initialise();

      
  //get the pointer to the User Interface manager 
  G4UImanager * UI = G4UImanager::GetUIpointer();  

  if(argc==1)
  // Define (G)UI terminal for interactive mode  
  { 
    // G4UIterminal is a (dumb) terminal.
    G4UIsession * session = 0;
#ifdef G4UI_USE_TCSH
      session = new G4UIterminal(new G4UItcsh);      
#else
      session = new G4UIterminal();
#endif    

      //    UI->ApplyCommand("/control/execute vis.mac");    
    session->SessionStart();
    delete session;
  }
  else
  // Batch mode
  { 
    G4String command = "/control/execute ";
    G4String fileName = argv[1];
    UI->ApplyCommand(command+fileName);
  }


#ifdef G4VIS_USE
  delete visManager;
#endif
  delete runManager;

  GmAnalysisMgr::DeleteInstances();

  return 0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

