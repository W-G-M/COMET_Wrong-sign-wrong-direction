//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Sat Jun 12 20:59:42 2021 by ROOT version 6.22/08
// from TTree outTree/outTree
// found on file: oa_g4_em_00000000-0000_s3eouadfmh5p_SG4BH_000_histos.root
//////////////////////////////////////////////////////////

#ifndef Analysis_h
#define Analysis_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TSelector.h>
#include <TTreeReader.h>
#include <TTreeReaderValue.h>
#include <TTreeReaderArray.h>

// Headers needed by this particular selector
#include <vector>

#include <string>



class Analysis : public TSelector {
public :
   TTreeReader     fReader;  //!the tree reader
   TTree          *fChain = 0;   //!pointer to the analyzed TTree or TChain

   // Readers to access the data (delete the ones you do not need).
   TTreeReaderValue<Int_t> nG4Tracks = {fReader, "nG4Tracks"};
   TTreeReaderArray<int> G4Tracks_trackid = {fReader, "G4Tracks_trackid"};
   TTreeReaderArray<int> G4Tracks_parentid = {fReader, "G4Tracks_parentid"};
   TTreeReaderArray<int> G4Tracks_pid = {fReader, "G4Tracks_pid"};
   // TTreeReaderArray<float> G4Tracks_init_momX = {fReader, "G4Tracks_init_momX"};
   // TTreeReaderArray<float> G4Tracks_init_momY = {fReader, "G4Tracks_init_momY"};
   // TTreeReaderArray<float> G4Tracks_init_momZ = {fReader, "G4Tracks_init_momZ"};
   // TTreeReaderArray<float> G4Tracks_init_momE = {fReader, "G4Tracks_init_momE"};
   TTreeReaderArray<float> G4Tracks_init_posX = {fReader, "G4Tracks_init_posX"};
   TTreeReaderArray<float> G4Tracks_init_posY = {fReader, "G4Tracks_init_posY"};
   TTreeReaderArray<float> G4Tracks_init_posZ = {fReader, "G4Tracks_init_posZ"};
   // TTreeReaderArray<float> G4Tracks_init_posT = {fReader, "G4Tracks_init_posT"};
   // TTreeReaderArray<float> G4Tracks_final_posX = {fReader, "G4Tracks_final_posX"};
   // TTreeReaderArray<float> G4Tracks_final_posY = {fReader, "G4Tracks_final_posY"};
   // TTreeReaderArray<float> G4Tracks_final_posZ = {fReader, "G4Tracks_final_posZ"};
   // TTreeReaderArray<float> G4Tracks_final_posT = {fReader, "G4Tracks_final_posT"};
   // TTreeReaderArray<string> G4Tracks_partName = {fReader, "G4Tracks_partName"};
   // TTreeReaderArray<string> G4Tracks_processName = {fReader, "G4Tracks_processName"};
   TTreeReaderValue<Int_t> nG4TrackPoints = {fReader, "nG4TrackPoints"};
   TTreeReaderArray<int> G4TrackPoints_trackIdx = {fReader, "G4TrackPoints_trackIdx"};
   TTreeReaderArray<float> G4TrackPoints_posX = {fReader, "G4TrackPoints_posX"};
   TTreeReaderArray<float> G4TrackPoints_posY = {fReader, "G4TrackPoints_posY"};
   TTreeReaderArray<float> G4TrackPoints_posZ = {fReader, "G4TrackPoints_posZ"};
   TTreeReaderArray<float> G4TrackPoints_posT = {fReader, "G4TrackPoints_posT"};
   // TTreeReaderArray<float> G4TrackPoints_momX = {fReader, "G4TrackPoints_momX"};
   // TTreeReaderArray<float> G4TrackPoints_momY = {fReader, "G4TrackPoints_momY"};
   // TTreeReaderArray<float> G4TrackPoints_momZ = {fReader, "G4TrackPoints_momZ"};
   TTreeReaderValue<Int_t> nG4HitsCDC = {fReader, "nG4HitsCDC"};
   TTreeReaderArray<int> G4HitsCDC_trackid = {fReader, "G4HitsCDC_trackid"};
   TTreeReaderArray<int> G4HitsCDC_pid = {fReader, "G4HitsCDC_pid"};
   // TTreeReaderArray<float> G4HitsCDC_energyDep = {fReader, "G4HitsCDC_energyDep"};
   // TTreeReaderArray<float> G4HitsCDC_doca = {fReader, "G4HitsCDC_doca"};
   // TTreeReaderArray<float> G4HitsCDC_length = {fReader, "G4HitsCDC_length"};
   TTreeReaderArray<float> G4HitsCDC_posX = {fReader, "G4HitsCDC_posX"};
   TTreeReaderArray<float> G4HitsCDC_posY = {fReader, "G4HitsCDC_posY"};
   TTreeReaderArray<float> G4HitsCDC_posZ = {fReader, "G4HitsCDC_posZ"};
   TTreeReaderArray<float> G4HitsCDC_posT = {fReader, "G4HitsCDC_posT"};
   TTreeReaderArray<float> G4HitsCDC_localposX = {fReader, "G4HitsCDC_localposX"};
   TTreeReaderArray<float> G4HitsCDC_localposY = {fReader, "G4HitsCDC_localposY"};
   TTreeReaderArray<float> G4HitsCDC_localposZ = {fReader, "G4HitsCDC_localposZ"};
   TTreeReaderValue<Int_t> TurnNumber = {fReader, "TurnNumber"};
   // TTreeReaderArray<float> G4HitsCDC_momX = {fReader, "G4HitsCDC_momX"};
   // TTreeReaderArray<float> G4HitsCDC_momY = {fReader, "G4HitsCDC_momY"};
   // TTreeReaderArray<float> G4HitsCDC_momZ = {fReader, "G4HitsCDC_momZ"};
   // TTreeReaderArray<float> G4HitsCDC_momMag = {fReader, "G4HitsCDC_momMag"};
   // TTreeReaderArray<float> G4HitsCDC_pocaX = {fReader, "G4HitsCDC_pocaX"};
   // TTreeReaderArray<float> G4HitsCDC_pocaY = {fReader, "G4HitsCDC_pocaY"};
   // TTreeReaderArray<float> G4HitsCDC_pocaZ = {fReader, "G4HitsCDC_pocaZ"};
   // TTreeReaderArray<float> G4HitsCDC_pocaOnWireX = {fReader, "G4HitsCDC_pocaOnWireX"};
   // TTreeReaderArray<float> G4HitsCDC_pocaOnWireY = {fReader, "G4HitsCDC_pocaOnWireY"};
   // TTreeReaderArray<float> G4HitsCDC_pocaOnWireZ = {fReader, "G4HitsCDC_pocaOnWireZ"};
   // TTreeReaderValue<Int_t> nG4HitsCTH = {fReader, "nG4HitsCTH"};
   // TTreeReaderArray<int> G4HitsCTH_trackid = {fReader, "G4HitsCTH_trackid"};
   // TTreeReaderArray<int> G4HitsCTH_pid = {fReader, "G4HitsCTH_pid"};
   // TTreeReaderArray<float> G4HitsCTH_energyDep = {fReader, "G4HitsCTH_energyDep"};
   // TTreeReaderArray<float> G4HitsCTH_length = {fReader, "G4HitsCTH_length"};
   // TTreeReaderArray<float> G4HitsCTH_posX = {fReader, "G4HitsCTH_posX"};
   // TTreeReaderArray<float> G4HitsCTH_posY = {fReader, "G4HitsCTH_posY"};
   // TTreeReaderArray<float> G4HitsCTH_posZ = {fReader, "G4HitsCTH_posZ"};
   // TTreeReaderArray<float> G4HitsCTH_posT = {fReader, "G4HitsCTH_posT"};
   // TTreeReaderArray<int> G4HitsCTH_channel = {fReader, "G4HitsCTH_channel"};
   // TTreeReaderArray<int> G4HitsCTH_module = {fReader, "G4HitsCTH_module"};
   // TTreeReaderArray<int> G4HitsCTH_scint = {fReader, "G4HitsCTH_scint"};
   // TTreeReaderArray<int> G4HitsCTH_counter = {fReader, "G4HitsCTH_counter"};


   Analysis(TTree * /*tree*/ =0) { }
   virtual ~Analysis() { }
   virtual Int_t   Version() const { return 2; }
   virtual void    Begin(TTree *tree);
   virtual void    SlaveBegin(TTree *tree);
   virtual void    Init(TTree *tree);
   virtual Bool_t  Notify();
   virtual Bool_t  Process(Long64_t entry);
   virtual Int_t   GetEntry(Long64_t entry, Int_t getall = 0) { return fChain ? fChain->GetTree()->GetEntry(entry, getall) : 0; }
   virtual void    SetOption(const char *option) { fOption = option; }
   virtual void    SetObject(TObject *obj) { fObject = obj; }
   virtual void    SetInputList(TList *input) { fInput = input; }
   virtual TList  *GetOutputList() const { return fOutput; }
   virtual void    SlaveTerminate();
   virtual void    Terminate();

   ClassDef(Analysis,0);

};

#endif

#ifdef Analysis_cxx
void Analysis::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the reader is initialized.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   fReader.SetTree(tree);
}

Bool_t Analysis::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}


#endif // #ifdef Analysis_cxx
