#include "ICTHGeom.hxx"
#include "IG4Trajectory.hxx"
#include "IG4HitSegment.hxx"
#include "IHandle.hxx"

void Analyzer(TString inFileName, TString outFileName)
{
  using namespace std;
  using namespace COMET;

  bool debug = false;
  bool storeTracks  = true;
  bool storeTracksPoints = true;
  bool storeCDCHits = true;
  bool storeCTHHits = true;
  bool storeEmptyEvents = true;

  TStopwatch* timer = new TStopwatch();
  timer->Start();
 
  TGeoManager::Import(inFileName);
  gGeoManager->PushPath();
  gGeoManager->GetTopNode()->cd();

  COMET::IGeomIdManager& geomId = COMET::IOADatabase::Get().GeomId();

  int NumberOfWires = COMET::IGeomInfo::CDC().GetNumberOfWires();
  std::cout << "Number of Wires: " << NumberOfWires << std::endl;

  double* wirePosX = new double[NumberOfWires];
  double* wirePosY = new double[NumberOfWires];

  for (Int_t i=0; i< NumberOfWires; i++){
    TVector3 wirepos  = COMET::IGeomInfo::Get().CDC().GetWirePosition(i);
    TVector3 wirepos_local;
    if(!COMET::IGeomInfo::DetectorSolenoid().GetDetPositionInDSCoordinate(wirepos, wirepos_local)) continue;
    
    wirePosX[i] = wirepos_local.X();
    wirePosY[i] = wirepos_local.Y();
  }
  
  TGraph *grWires = new TGraph(NumberOfWires, wirePosX, wirePosY);
  grWires->SetName("CDCWires");
  grWires->SetMarkerStyle(20);
  grWires->SetMarkerSize(0.25);
  delete[] wirePosX;
  delete[] wirePosY;

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
  // Setup the variables that we will 
  // save in TTree
  //
  //
  //================================
  //
  //
  //
  int b_nG4Tracks;
  vector<int>    b_G4Tracks_trackid;
  vector<int>    b_G4Tracks_parentid;
  vector<int>    b_G4Tracks_pid;
  vector<float>  b_G4Tracks_init_momX;
  vector<float>  b_G4Tracks_init_momY;
  vector<float>  b_G4Tracks_init_momZ;
  vector<float>  b_G4Tracks_init_momE;
  vector<float>  b_G4Tracks_init_posX;
  vector<float>  b_G4Tracks_init_posY;
  vector<float>  b_G4Tracks_init_posZ;
  vector<float>  b_G4Tracks_init_posT;
  vector<float>  b_G4Tracks_final_posX;
  vector<float>  b_G4Tracks_final_posY;
  vector<float>  b_G4Tracks_final_posZ;
  vector<float>  b_G4Tracks_final_posT;
  vector<string> b_G4Tracks_partName;
  vector<string> b_G4Tracks_processName;
  //
  //
  //
  int b_nG4TrackPoints;
  vector<int>    b_G4TrackPoints_trackIdx;
  vector<float>  b_G4TrackPoints_posX;
  vector<float>  b_G4TrackPoints_posY;
  vector<float>  b_G4TrackPoints_posZ;
  vector<float>  b_G4TrackPoints_posT;
  vector<float>  b_G4TrackPoints_momX;
  vector<float>  b_G4TrackPoints_momY;
  vector<float>  b_G4TrackPoints_momZ;
  //
  //
  //
  int b_nG4HitsCDC;
  vector<int>   b_G4HitsCDC_trackid;
  vector<int>   b_G4HitsCDC_pid;
  vector<float> b_G4HitsCDC_energyDep;
  vector<float> b_G4HitsCDC_doca;
  vector<float> b_G4HitsCDC_length;
  vector<float> b_G4HitsCDC_posX;
  vector<float> b_G4HitsCDC_posY;
  vector<float> b_G4HitsCDC_posZ;
  vector<float> b_G4HitsCDC_posT;
  vector<float> b_G4HitsCDC_localposX;
  vector<float> b_G4HitsCDC_localposY;
  vector<float> b_G4HitsCDC_localposZ;
  vector<float> b_G4HitsCDC_momX;
  vector<float> b_G4HitsCDC_momY;
  vector<float> b_G4HitsCDC_momZ;
  vector<float> b_G4HitsCDC_momMag;
  vector<float> b_G4HitsCDC_pocaX;
  vector<float> b_G4HitsCDC_pocaY;
  vector<float> b_G4HitsCDC_pocaZ;
  vector<float> b_G4HitsCDC_pocaOnWireX;
  vector<float> b_G4HitsCDC_pocaOnWireY;
  vector<float> b_G4HitsCDC_pocaOnWireZ;
  int b_TurnNumber;
  //
  //
  //
  int b_nG4HitsCTH;
  vector<int>   b_G4HitsCTH_trackid;
  vector<int>   b_G4HitsCTH_pid;
  vector<float> b_G4HitsCTH_energyDep;
  vector<float> b_G4HitsCTH_length;
  vector<float> b_G4HitsCTH_posX;
  vector<float> b_G4HitsCTH_posY;
  vector<float> b_G4HitsCTH_posZ;
  vector<float> b_G4HitsCTH_posT;
  vector<int>   b_G4HitsCTH_channel;
  vector<int>   b_G4HitsCTH_module;
  vector<int>   b_G4HitsCTH_scint;
  vector<int>   b_G4HitsCTH_counter;

  //
  // Setup file and the output TTree
  //
  TFile* outFile = new TFile(outFileName,"RECREATE");
  outFile->cd();
  TTree* outTree = new TTree("outTree", "outTree");
  
  //
  // Setup the branches
  //
  outTree->Branch("nG4Tracks", &b_nG4Tracks,  "nG4Tracks/I");
  //
  //
  //
  outTree->Branch("G4Tracks_trackid",    "std::vector<int>",         &b_G4Tracks_trackid);
  outTree->Branch("G4Tracks_parentid",   "std::vector<int>",         &b_G4Tracks_parentid);
  outTree->Branch("G4Tracks_pid",        "std::vector<int>",         &b_G4Tracks_pid);
  outTree->Branch("G4Tracks_init_momX",  "std::vector<float>",       &b_G4Tracks_init_momX);
  outTree->Branch("G4Tracks_init_momY",  "std::vector<float>",       &b_G4Tracks_init_momY);
  outTree->Branch("G4Tracks_init_momZ",  "std::vector<float>",       &b_G4Tracks_init_momZ);
  outTree->Branch("G4Tracks_init_momE",  "std::vector<float>",       &b_G4Tracks_init_momE);
  outTree->Branch("G4Tracks_init_posX",  "std::vector<float>",       &b_G4Tracks_init_posX);
  outTree->Branch("G4Tracks_init_posY",  "std::vector<float>",       &b_G4Tracks_init_posY);
  outTree->Branch("G4Tracks_init_posZ",  "std::vector<float>",       &b_G4Tracks_init_posZ);
  outTree->Branch("G4Tracks_init_posT",  "std::vector<float>",       &b_G4Tracks_init_posT);
  outTree->Branch("G4Tracks_final_posX", "std::vector<float>",       &b_G4Tracks_final_posX);
  outTree->Branch("G4Tracks_final_posY", "std::vector<float>",       &b_G4Tracks_final_posY);
  outTree->Branch("G4Tracks_final_posZ", "std::vector<float>",       &b_G4Tracks_final_posZ);
  outTree->Branch("G4Tracks_final_posT", "std::vector<float>",       &b_G4Tracks_final_posT);
  outTree->Branch("G4Tracks_partName",   "std::vector<std::string>", &b_G4Tracks_partName);
  outTree->Branch("G4Tracks_processName","std::vector<std::string>", &b_G4Tracks_processName);
  //
  //
  //
  if (storeTracksPoints){
    outTree->Branch("nG4TrackPoints",            &b_nG4TrackPoints,        "nG4TrackPoints/I");
    outTree->Branch("G4TrackPoints_trackIdx",   "std::vector<int>",        &b_G4TrackPoints_trackIdx);
    outTree->Branch("G4TrackPoints_posX",       "std::vector<float>",      &b_G4TrackPoints_posX);
    outTree->Branch("G4TrackPoints_posY",       "std::vector<float>",      &b_G4TrackPoints_posY);
    outTree->Branch("G4TrackPoints_posZ",       "std::vector<float>",      &b_G4TrackPoints_posZ);
    outTree->Branch("G4TrackPoints_posT",       "std::vector<float>",      &b_G4TrackPoints_posT);
    outTree->Branch("G4TrackPoints_momX",       "std::vector<float>",      &b_G4TrackPoints_momX);
    outTree->Branch("G4TrackPoints_momY",       "std::vector<float>",      &b_G4TrackPoints_momY);
    outTree->Branch("G4TrackPoints_momZ",       "std::vector<float>",      &b_G4TrackPoints_momZ);
  }
  //
  //
  //
  outTree->Branch("nG4HitsCDC", &b_nG4HitsCDC,  "nG4HitsCDC/I");
  outTree->Branch("G4HitsCDC_trackid",     "std::vector<int>",   &b_G4HitsCDC_trackid);
  outTree->Branch("G4HitsCDC_pid",         "std::vector<int>",   &b_G4HitsCDC_pid);
  outTree->Branch("G4HitsCDC_energyDep",   "std::vector<float>", &b_G4HitsCDC_energyDep);
  outTree->Branch("G4HitsCDC_doca",        "std::vector<float>", &b_G4HitsCDC_doca);
  outTree->Branch("G4HitsCDC_length",      "std::vector<float>", &b_G4HitsCDC_length);
  outTree->Branch("G4HitsCDC_posX",        "std::vector<float>", &b_G4HitsCDC_posX);
  outTree->Branch("G4HitsCDC_posY",        "std::vector<float>", &b_G4HitsCDC_posY);
  outTree->Branch("G4HitsCDC_posZ",        "std::vector<float>", &b_G4HitsCDC_posZ);
  outTree->Branch("G4HitsCDC_posT",        "std::vector<float>", &b_G4HitsCDC_posT);
  outTree->Branch("G4HitsCDC_localposX",    "std::vector<float>", &b_G4HitsCDC_localposX);
  outTree->Branch("G4HitsCDC_localposY",    "std::vector<float>", &b_G4HitsCDC_localposY);
  outTree->Branch("G4HitsCDC_localposZ",    "std::vector<float>", &b_G4HitsCDC_localposZ);
  outTree->Branch("G4HitsCDC_momX",        "std::vector<float>", &b_G4HitsCDC_momX);
  outTree->Branch("G4HitsCDC_momY",        "std::vector<float>", &b_G4HitsCDC_momY);
  outTree->Branch("G4HitsCDC_momZ",        "std::vector<float>", &b_G4HitsCDC_momZ);
  outTree->Branch("G4HitsCDC_momMag",      "std::vector<float>", &b_G4HitsCDC_momMag);
  outTree->Branch("G4HitsCDC_pocaX",       "std::vector<float>", &b_G4HitsCDC_pocaX);
  outTree->Branch("G4HitsCDC_pocaY",       "std::vector<float>", &b_G4HitsCDC_pocaY);
  outTree->Branch("G4HitsCDC_pocaZ",       "std::vector<float>", &b_G4HitsCDC_pocaZ);
  outTree->Branch("G4HitsCDC_pocaOnWireX", "std::vector<float>", &b_G4HitsCDC_pocaOnWireX);
  outTree->Branch("G4HitsCDC_pocaOnWireY", "std::vector<float>", &b_G4HitsCDC_pocaOnWireY);
  outTree->Branch("G4HitsCDC_pocaOnWireZ", "std::vector<float>", &b_G4HitsCDC_pocaOnWireZ);
  outTree->Branch("TurnNumber", &b_TurnNumber,  "TurnNumber/I");
  //
  //
  //
  outTree->Branch("nG4HitsCTH", &b_nG4HitsCTH,  "nG4HitsCTH/I");
  outTree->Branch("G4HitsCTH_trackid",   "std::vector<int>",   &b_G4HitsCTH_trackid);
  outTree->Branch("G4HitsCTH_pid",       "std::vector<int>",   &b_G4HitsCTH_pid);
  outTree->Branch("G4HitsCTH_energyDep", "std::vector<float>", &b_G4HitsCTH_energyDep);
  outTree->Branch("G4HitsCTH_length",    "std::vector<float>", &b_G4HitsCTH_length);
  outTree->Branch("G4HitsCTH_posX",      "std::vector<float>", &b_G4HitsCTH_posX);
  outTree->Branch("G4HitsCTH_posY",      "std::vector<float>", &b_G4HitsCTH_posY);
  outTree->Branch("G4HitsCTH_posZ",      "std::vector<float>", &b_G4HitsCTH_posZ);
  outTree->Branch("G4HitsCTH_posT",      "std::vector<float>", &b_G4HitsCTH_posT);
  outTree->Branch("G4HitsCTH_channel",   "std::vector<int>",   &b_G4HitsCTH_channel);
  outTree->Branch("G4HitsCTH_module",    "std::vector<int>",   &b_G4HitsCTH_module);
  outTree->Branch("G4HitsCTH_scint",     "std::vector<int>",   &b_G4HitsCTH_scint);
  outTree->Branch("G4HitsCTH_counter",   "std::vector<int>",   &b_G4HitsCTH_counter);


  //================================
  //
  // Initialize histograms
  //
  //================================
  TH1D* h_cutflow = new TH1D("h_cutflow","h_cutflow",50,0,50);
  h_cutflow->GetXaxis()->SetBinLabel(1,"InitialEvents");
  h_cutflow->GetXaxis()->SetBinLabel(2,"PrimaryElectron");
  h_cutflow->GetXaxis()->SetBinLabel(3,"PrimaryPositron");
  h_cutflow->LabelsDeflate("X");

  TH1D* h_primary_electron_e   = new TH1D("h_primary_electron_e",   "h_primary_electron_e",   100, 50., 150.);
  TH1D* h_primary_electron_mom = new TH1D("h_primary_electron_mom", "h_primary_electron_mom", 100, 50., 150.);

  TH1D* h_primary_positron_e   = new TH1D("h_primary_positron_e",   "h_primary_positron_e",   100, 50., 150.);
  TH1D* h_primary_positron_mom = new TH1D("h_primary_positron_mom", "h_primary_positron_mom", 100, 50., 150.);
  //================================
  //
  //
  // EVENT LOOP
  //
  //
  //================================

  unsigned int nEntries = inTree->GetEntries();

  if (debug){
    int maxEntries = 5000;
    if (nEntries > maxEntries) nEntries = maxEntries;
    cout << "DEBUG: set nEntries to " << nEntries << endl; 
  }


  cout << "Looping over tree with " << nEntries << endl; 
  for (size_t i=0; i<nEntries; ++i)
  {
    inTree->GetEntry(i);
    if (i%1000 == 0){
      std::cout << "Event: "<< i << "/" << nEntries<< std::endl;
    }
    //================================
    //
    //
    //
    //
    //================================
    b_nG4Tracks = 0;
    b_G4Tracks_trackid.clear();
    b_G4Tracks_parentid.clear();
    b_G4Tracks_pid.clear();
    b_G4Tracks_init_momX.clear();
    b_G4Tracks_init_momY.clear();
    b_G4Tracks_init_momZ.clear();
    b_G4Tracks_init_momE.clear();
    b_G4Tracks_init_posX.clear();
    b_G4Tracks_init_posY.clear();
    b_G4Tracks_init_posZ.clear();
    b_G4Tracks_init_posT.clear();
    b_G4Tracks_final_posX.clear();
    b_G4Tracks_final_posY.clear();
    b_G4Tracks_final_posZ.clear();
    b_G4Tracks_final_posT.clear();
    b_G4Tracks_partName.clear();
    b_G4Tracks_processName.clear();    
    
    //
    b_nG4TrackPoints = 0;
    b_G4TrackPoints_trackIdx.clear();
    b_G4TrackPoints_posX.clear();
    b_G4TrackPoints_posY.clear();
    b_G4TrackPoints_posZ.clear();
    b_G4TrackPoints_posT.clear();
    b_G4TrackPoints_momX.clear();
    b_G4TrackPoints_momY.clear();
    b_G4TrackPoints_momZ.clear();
    //
    b_nG4HitsCDC = 0;
    b_G4HitsCDC_trackid.clear();
    b_G4HitsCDC_pid.clear();
    b_G4HitsCDC_energyDep.clear();
    b_G4HitsCDC_doca.clear();
    b_G4HitsCDC_length.clear();
    b_G4HitsCDC_posX.clear();
    b_G4HitsCDC_posY.clear();
    b_G4HitsCDC_posZ.clear();
    b_G4HitsCDC_posT.clear();
    b_G4HitsCDC_localposX.clear();
    b_G4HitsCDC_localposY.clear();
    b_G4HitsCDC_localposZ.clear();
    b_G4HitsCDC_momX.clear();
    b_G4HitsCDC_momY.clear();
    b_G4HitsCDC_momZ.clear();
    b_G4HitsCDC_momMag.clear();
    b_G4HitsCDC_pocaX.clear();
    b_G4HitsCDC_pocaY.clear();
    b_G4HitsCDC_pocaZ.clear();
    b_G4HitsCDC_pocaOnWireX.clear();
    b_G4HitsCDC_pocaOnWireY.clear();
    b_G4HitsCDC_pocaOnWireZ.clear();
    b_TurnNumber = 0;
    //
    b_nG4HitsCTH = 0;
    b_G4HitsCTH_trackid.clear();
    b_G4HitsCTH_pid.clear();
    b_G4HitsCTH_energyDep.clear();
    b_G4HitsCTH_length.clear();
    b_G4HitsCTH_posX.clear();
    b_G4HitsCTH_posY.clear();
    b_G4HitsCTH_posZ.clear();
    b_G4HitsCTH_posT.clear();
    b_G4HitsCTH_channel.clear();
    b_G4HitsCTH_module.clear();
    b_G4HitsCTH_scint.clear();
    b_G4HitsCTH_counter.clear();
    //================================
    //
    //
    //
    //
    //================================
    h_cutflow->Fill("InitialEvents",1);

    bool hasPositron_Primary = false;
    bool hasElectron_Primary = false;

    IHandle<IG4Trajectory> track_primary_electron(nullptr);
    IHandle<IG4Trajectory> track_primary_positron(nullptr);
    IHandle<IG4Trajectory> track_primary(nullptr);

    //
    // Find the primary electron or positron G4 trajectory
    //
    g4Tracks = event->Get<IG4TrajectoryContainer>("truth/G4Trajectories");
    if (g4Tracks){
      TLorentzVector tlv;
      int pointIndex = 0;// Start from 0 here

      for (auto& trackMap: *g4Tracks){
        //
        //
        //
        IHandle<IG4Trajectory> thisTrack = g4Tracks->GetTrajectory(trackMap.second.GetTrackId());
        //
        //
        //
        TString thisTrack_partName = TString(thisTrack->GetParticleName());
        if(!(thisTrack_partName.Contains("e-") || thisTrack_partName.Contains("e+"))) continue;
        //
        //
        //
        TString thisTrack_procName = TString(thisTrack->GetProcessName());
        if (thisTrack_partName.Contains("e-") && thisTrack_procName.Contains("primary")){ 
          hasElectron_Primary = true;
          track_primary = track_primary_electron = thisTrack;
        }
        if (thisTrack_partName.Contains("e+") && thisTrack_procName.Contains("primary")){ 
          hasPositron_Primary = true;
          track_primary = track_primary_positron = thisTrack;
        }

        //
        //
        //
        if(track_primary){

          b_G4Tracks_trackid.emplace_back(track_primary->GetTrackId());
          b_G4Tracks_parentid.emplace_back(track_primary->GetParentId());
          b_G4Tracks_pid.emplace_back(track_primary->GetPDGEncoding());

          tlv = track_primary->GetInitialMomentum();

          b_G4Tracks_init_momX.emplace_back(tlv.Px());
          b_G4Tracks_init_momY.emplace_back(tlv.Py());
          b_G4Tracks_init_momZ.emplace_back(tlv.Pz());
          b_G4Tracks_init_momE.emplace_back(tlv.E());

          tlv = track_primary->GetInitialPosition();

          b_G4Tracks_init_posX.emplace_back(tlv.X());
          b_G4Tracks_init_posY.emplace_back(tlv.Y());
          b_G4Tracks_init_posZ.emplace_back(tlv.Z());
          b_G4Tracks_init_posT.emplace_back(tlv.T());

          tlv = track_primary->GetFinalPosition();

          b_G4Tracks_final_posX.emplace_back(tlv.X());
          b_G4Tracks_final_posY.emplace_back(tlv.Y());
          b_G4Tracks_final_posZ.emplace_back(tlv.Z());
          b_G4Tracks_final_posT.emplace_back(tlv.T());


          b_G4Tracks_partName.emplace_back(track_primary->GetParticleName());
          b_G4Tracks_processName.emplace_back(track_primary->GetProcessName());

          b_nG4Tracks += 1;
          //
          //
          //
          if (storeTracksPoints){
            auto& track_points = track_primary->GetTrajectoryPoints();
            for (auto point: track_points)
            { 
              b_G4TrackPoints_trackIdx.emplace_back(b_nG4Tracks-1);// This maps this point to its track.
              const TLorentzVector pos = point.GetPosition();
              b_G4TrackPoints_posX.emplace_back(pos.X());
              b_G4TrackPoints_posY.emplace_back(pos.Y());
              b_G4TrackPoints_posZ.emplace_back(pos.Z());
              b_G4TrackPoints_posT.emplace_back(pos.Y());
              const TVector3 mom = point.GetMomentum();
              b_G4TrackPoints_momX.emplace_back(mom.X());
              b_G4TrackPoints_momY.emplace_back(mom.Y());
              b_G4TrackPoints_momZ.emplace_back(mom.Z());    
              b_nG4TrackPoints+=1;        
            }
          }
          break;
        }
      }
    }
    //
    // Plot some stuff and get the pointer to the primary track
    //
    if (hasElectron_Primary){ 
      track_primary = track_primary_electron;

      h_cutflow->Fill("PrimaryElectron",1);
      h_primary_electron_e->Fill(track_primary_electron->GetInitialMomentum().E() / unit::MeV, 1);
      h_primary_electron_mom->Fill(track_primary_electron->GetInitialMomentum().P() / unit::MeV,1);
    }
    if (hasPositron_Primary){ 
      track_primary = track_primary_positron;

      h_cutflow->Fill("PrimaryPositron",1);
      h_primary_positron_e->Fill(track_primary_positron->GetInitialMomentum().E() / unit::MeV, 1);
      h_primary_positron_mom->Fill(track_primary_positron->GetInitialMomentum().P() / unit::MeV,1);
    }
    
    if (track_primary == nullptr) continue;

    //================================
    //
    //
    // Loop Over
    //
    //
    //================================
    g4HitsCDC = event->Get<IG4HitContainer>("truth/g4Hits/CDC");

    if (g4HitsCDC){
      
      TVector3 hitPos, hitPos_LOCAL;
      TVector3 lastHitPos_LOCAL;
      

      for (IG4VHit* vhit: *g4HitsCDC) {
        auto hit = dynamic_cast<IG4HitGas*>(vhit); 
        //
        //
        //
        if(hit->GetTrajectory() != track_primary) continue;
        if(abs(hit->GetPDGEncoding()) != 11) continue;

        b_G4HitsCDC_trackid.emplace_back(hit->GetPrimaryId());

        b_G4HitsCDC_pid.emplace_back(hit->GetPDGEncoding());
        b_G4HitsCDC_energyDep.emplace_back(hit->GetEnergyDeposit());
        b_G4HitsCDC_doca.emplace_back(hit->GetDOCA());
        b_G4HitsCDC_length.emplace_back(hit->GetLength());

        b_G4HitsCDC_posX.emplace_back(hit->GetPosX());
        b_G4HitsCDC_posY.emplace_back(hit->GetPosY());
        b_G4HitsCDC_posZ.emplace_back(hit->GetPosZ());
        b_G4HitsCDC_posT.emplace_back(hit->GetPosT());
        
        TVector3 pos  = hit->GetPosition();
        TVector3 pos_local;
        COMET::IGeomInfo::DetectorSolenoid().GetDetPositionInDSCoordinate(pos, pos_local);
        b_G4HitsCDC_localposX.emplace_back(pos_local.X());
        b_G4HitsCDC_localposY.emplace_back(pos_local.Y());
        b_G4HitsCDC_localposZ.emplace_back(pos_local.Z());

        b_G4HitsCDC_momX.emplace_back(hit->GetMomX());
        b_G4HitsCDC_momY.emplace_back(hit->GetMomY());
        b_G4HitsCDC_momZ.emplace_back(hit->GetMomZ());
        b_G4HitsCDC_momMag.emplace_back(hit->GetMomentumMag());

        const TVector3 poca = hit->GetPOCA();
        b_G4HitsCDC_pocaX.emplace_back(poca.x());
        b_G4HitsCDC_pocaY.emplace_back(poca.y());
        b_G4HitsCDC_pocaZ.emplace_back(poca.z());

        const TVector3 pocawire = hit->GetPOCAOnWire();
        b_G4HitsCDC_pocaOnWireX.emplace_back(pocawire.x());
        b_G4HitsCDC_pocaOnWireY.emplace_back(pocawire.y());
        b_G4HitsCDC_pocaOnWireZ.emplace_back(pocawire.z());

        TGeoNode* volume = geomId.FindNode(pos);

        TString geoName = TString(volume->GetName());
        if (geoName.Contains("CDCSenseLayer")){
          hitPos_LOCAL = TVector3(pos_local.X(),pos_local.Y(),pos_local.Z());
          if (b_nG4HitsCDC>=1) {
            lastHitPos_LOCAL = TVector3(b_G4HitsCDC_localposX[b_nG4HitsCDC-1],b_G4HitsCDC_localposY[b_nG4HitsCDC-1],b_G4HitsCDC_localposZ[b_nG4HitsCDC-1]);
            if ((lastHitPos_LOCAL-hitPos_LOCAL).Mag()>100.) {
              b_TurnNumber++;
            }
          }
        }
        b_nG4HitsCDC += 1;
      }  
    }
    // if (g4HitsCDC){
    //   for(COMET::IG4HitContainer::const_iterator hitSeg = g4HitsCDC->begin(); hitSeg != g4HitsCDC->end(); ++hitSeg) {
    //     const COMET::IG4HitSegment* tmpSeg = dynamic_cast<COMET::IG4HitSegment*>(*hitSeg);    
    //     std::cout << tmpSeg << std::endl;    
    //     if (tmpSeg){
    //       // std::cout << "HERE" << std::endl;
    //     }
    //   }
    // }
    //================================
    //
    //
    //
    //
    //================================
    g4HitsCTH = event->Get<IG4HitContainer>("truth/g4Hits/CTH");

    if (g4HitsCTH){
      for (IG4VHit* vhit: *g4HitsCTH) {
        auto hit = dynamic_cast<IG4HitSegment*>(vhit);
        if(hit->GetTrajectory() != track_primary) continue;
        b_nG4HitsCTH += 1;

        b_G4HitsCTH_trackid.emplace_back(hit->GetPrimaryId());

        b_G4HitsCTH_posX.emplace_back(hit->GetPosX());
        b_G4HitsCTH_posY.emplace_back(hit->GetPosY());
        b_G4HitsCTH_posZ.emplace_back(hit->GetPosZ());
        b_G4HitsCTH_posT.emplace_back(hit->GetPosT());

        b_G4HitsCTH_trackid.emplace_back( hit->GetPrimaryId());
        b_G4HitsCTH_pid.emplace_back( hit->GetPDGEncoding());
        b_G4HitsCTH_energyDep.emplace_back( hit->GetEnergyDeposit());
        b_G4HitsCTH_length.emplace_back( hit->GetTrackLength());

        int chan = -1;
        IGeomInfo::CTH().GlobalPositionToChannel(TVector3(hit->GetPosX(), hit->GetPosY(), hit->GetPosZ()), chan);
        b_G4HitsCTH_channel.emplace_back(chan);

        IGeometryId id;
        IGeomInfo::CTH().GlobalPositionToGeomId(TVector3(hit->GetPosX(), hit->GetPosY(), hit->GetPosZ()), id);

        int module, scint, segId, lg;
        IGeomInfo::CTH().GetGeometryInfo(id, module, scint, segId, lg);
        b_G4HitsCTH_module.emplace_back(module);
        b_G4HitsCTH_scint.emplace_back(scint);
        
        int counter = IGeomInfo::CTH().GetCounterId(module, scint, segId, lg);
        b_G4HitsCTH_counter.emplace_back(counter);
      }
    }
    if (storeEmptyEvents){
      outTree->Fill();
    } else{
      if (b_nG4Tracks > 0 || b_nG4HitsCDC > 0 || b_nG4HitsCTH > 0) outTree->Fill();
    }
  }
  cout << "End of event loop" << endl; 
  //==========================================
  //
  //
  //
  //===========================================
  outFile->cd();
  //
  //
  //
  grWires->Write();
  //
  // Histos
  //
  h_cutflow->Write();
  h_primary_electron_e->Write();
  h_primary_electron_mom->Write();
  h_primary_positron_e->Write();
  h_primary_positron_mom->Write();
  //
  // TTree
  //
  outTree->Write();
  //
  outFile->Close();

  inFile->Close();
  timer->Stop();
  timer->Print();
}
