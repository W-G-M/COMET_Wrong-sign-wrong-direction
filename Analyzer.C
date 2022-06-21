IVAnalyzer class as basis.}, captionpos=t, label={AnalyzerR}]
#include "ICTHGeom.hxx"
#include "IG4Trajectory.hxx"
#include "IHandle.hxx"

void Analyzer(TString inFileName, TString outFileName){  
  gROOT->SetBatch(1);
  using namespace std;
  using namespace COMET;
  bool debug = false;
  TStopwatch* timer = new TStopwatch();
  timer->Start(); 
  TGeoManager::Import(inFileName);
  gGeoManager->PushPath();
  gGeoManager->GetTopNode()->cd();  
  //
  // Open input file and retrieve the COMETEvents TTree
  //
  TFile* inFile = TFile::Open(inFileName); 
  TTree* inTree = (TTree*)inFile->Get("COMETEvents");
  //================================
  //
  //
  //
  //
  //================================
  ICOMETEvent* event = nullptr; // declare event pointer
  inTree->SetBranchAddress("COMETEvent", &event); // set the branch address to the pointer 
  IHandle<IG4TrajectoryContainer> g4Tracks(nullptr);
  IHandle<IG4HitContainer> g4HitsCDC(nullptr);
  IHandle<IG4HitContainer> g4HitsCTH(nullptr);
  IHandle<IHitSelection>   mcHitsCDC(nullptr);
  //================================
  //
  // Initialize histograms
  //
  //================================
  TH1D* h_cutflow = new TH1D("h_cutflow","h_cutflow",50,0,50);
  h_cutflow->GetXaxis()->SetBinLabel(1,"Positron");  
  h_cutflow->LabelsDeflate("X");
  TH1D* pos = new TH1D("pos", "pos", 200, 0., 800.);
  //================================
  //
  //
  // EVENT LOOP
  //
  //
  //================================
  unsigned int nEntries = inTree->GetEntries();

  if (debug)
  {
    int maxEntries = 5000;
    if (nEntries > maxEntries) nEntries = maxEntries;
    cout << "DEBUG: set nEntries to " << nEntries << endl; 
  }
  cout << "Looping over tree with " << nEntries << endl; 
  for (size_t i=0; i<nEntries; ++i)
  {
    inTree->GetEntry(i);
    if (i%10000 == 0)
    {
      std::cout << "Event: "<< i << "/" << nEntries<< std::endl;
    }    
    h_cutflow->Fill("InitialEvents",1);
    g4Tracks = event->Get<IG4TrajectoryContainer>("truth/G4Trajectories");
    if (g4Tracks)
    {            
      bool hasPos = false;
      IHandle<IG4Trajectory> track_positron0(nullptr);     
      int count = 0;

      for (auto& trackMap: *g4Tracks)
      {
        TString thisTrack_partName;
        TString thisTrack_procName;
        IHandle<IG4Trajectory> thisTrack = g4Tracks->GetTrajectory(trackMap.second.GetTrackId());
        int thisTrack_id = thisTrack->GetTrackId();
        thisTrack_partName = TString(thisTrack->GetParticleName());
        thisTrack_procName = TString(thisTrack->GetProcessName());
        if(thisTrack_partName.Contains("e+"))
        {
          hasPos = true;
          track_positron0 = thisTrack;
        }                       
        if(hasPos)
        {                 
          cout << "\n" << endl;
          std :: cout <<"particla_name"<<": "<<thisTrack_partName<<", "<<"process name: "<<thisTrack_procName<<endl; 
          std :: string volumeNameB = track_positron0->GetTrajectoryPoints().back().GetVolumeName();
          std :: string volumeNameF = track_positron0->GetTrajectoryPoints().front().GetVolumeName();
          if(TString(volumeNameB).Contains("CTH") && TString(volumeNameF).Contains("CTH") && thisTrack_partName.Contains("e+"))          
          {            
            cout << "Back Volume: " <<volumeNameB << endl;
            cout << "Front Volume: " <<volumeNameF << endl;
            pos->Fill(track_positron0->GetInitialMomentum().E() / unit::MeV, 1);
            h_cutflow->Fill("Positron", 1);             
          }
        }
      }
    }
  }
  cout << "End of event loop" << endl; 
  TFile* outFile = new TFile(outFileName,"RECREATE");
  outFile->cd();
  h_cutflow->Write();
  pos->Write();
  outFile->Close();
  inFile->Close();
  timer->Stop();
  timer->Print();
}
