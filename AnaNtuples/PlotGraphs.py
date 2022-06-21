import sys
import os
import glob
import ROOT
import enum
import math
import numpy
import array
import argparse

ROOT.gROOT.SetBatch()
ROOT.gROOT.LoadMacro("rootlogon.C")
ROOT.gROOT.SetStyle("MyStyle")

def main():

  inputFileName = "outputGraph_electron.root"
  outDir="./graphs_electron"
  MakeDirIfDontExist(outDir)
  PlotGraphs(inputFileName, outDir)

  inputFileName = "outputGraph_positron.root"
  MakeDirIfDontExist(outDir)
  outDir="./graphs_positron"
  
  PlotGraphs(inputFileName, outDir)

def PlotGraphs(inputFileName, outDir):


  inFile = ROOT.TFile(inputFileName,"OPEN")
  g_cdcWires = inFile.Get("CDCWires")

  keys = inFile.GetListOfKeys()

  graphNames = []
  for k in keys:
    keyName = k.GetName()
    if "Graph2D_HitsCDC_LOCAL_XY" in keyName:
      graphNames.append(keyName) 
    if "Graph2D_HitsCDC_LOCAL_ZY" in keyName:
      graphNames.append(keyName) 

  for name in graphNames:
    g_cdc   = inFile.Get(name)
    g_title = g_cdc.GetName()

    # iEvt = g_title.split("_")[-1]
    iEvt = g_title.split("_")[-2]

    xc     = None
    yc     = None
    radius = None
    h_param = None
    if "Graph2D_HitsCDC_LOCAL_XY" in name:
      h_param = inFile.Get("h_CircleParam_XY_"+iEvt)
      if h_param != None:
        xc     = h_param.GetBinContent(1)
        yc     = h_param.GetBinContent(2)
        radius = h_param.GetBinContent(3)


    isRPhi = False
    isRhoZ = False
    isRPhiLocal = False
    isRhoZLocal = False
    
    if "XY" in name: isRhoZ=True
    if "YZ" in name: isRPhi=True
    if "LOCAL_XY" in name: 
      isRPhiLocal=True
      isRhoZ=False
    if "LOCAL_ZY" in name: 
      isRhoZLocal=True
      isRhoZ=False

    MakeGraph(g_cdc,g_cdcWires,isRPhiLocal,isRPhi,isRhoZLocal,isRhoZ,[xc,yc,radius],outDir,g_title)

def MakeGraph(graph,g_cdcWires,isRPhiLocal,isRPhi,isRhoZLocal,isRhoZ,paramCirc,outDir,pdftitle):


  #
  # Make TCanvas
  #
  canv = ROOT.TCanvas("canv","canv", 600, 600)
  canv.SetFillStyle( 4000 )
  canv.SetLeftMargin(0.15)
  canv.SetRightMargin(0.08)
  canv.SetTopMargin(0.05)
  canv.SetBottomMargin(0.15)

  graph.SetMarkerColor(ROOT.kRed+1)
  graph.SetMarkerStyle(20)
  graph.SetMarkerSize(0.25)

  if isRPhi or isRPhiLocal:
    #
    # X-axis
    #
    graph.GetXaxis().SetLimits(-900.,+900.);    
    #
    # Y-axis
    #
    graph.GetHistogram().SetMinimum(-900.);
    graph.GetHistogram().SetMaximum(+900.);   
  if isRhoZ or isRhoZLocal:
    #
    # X-axis
    #
    graph.GetXaxis().SetLimits(-1200.,+1200.);    
    #
    # Y-axis
    #
    graph.GetHistogram().SetMinimum(-900.);
    graph.GetHistogram().SetMaximum(+900.);           
  
  if isRPhi:
    graph.GetXaxis().SetTitle("Y [mm]")
    graph.GetYaxis().SetTitle("Z [mm]")
  if isRPhiLocal:
    graph.GetXaxis().SetTitle("(Local) X [mm]")
    graph.GetYaxis().SetTitle("(Local) Y [mm]")
  if isRhoZ:
    graph.GetXaxis().SetTitle("X [mm]")
    graph.GetYaxis().SetTitle("Z [mm]")
  if isRhoZLocal:
    graph.GetXaxis().SetTitle("(Local) Z [mm]")
    graph.GetYaxis().SetTitle("(Local) Y [mm]")

  if isRPhi or isRPhiLocal:
    g_cdcWiresC = g_cdcWires.Clone()
    g_cdcWiresC.GetXaxis().SetLimits(-900.,+900.);    
    g_cdcWiresC.GetHistogram().SetMinimum(-900.);
    g_cdcWiresC.GetHistogram().SetMaximum(+900.); 
    g_cdcWiresC.SetMarkerStyle(20);
    g_cdcWiresC.SetMarkerSize(0.12);
    if isRPhi:
      g_cdcWiresC.GetXaxis().SetTitle("Y [mm]")
      g_cdcWiresC.GetYaxis().SetTitle("Z [mm]")
    if isRPhiLocal:
      g_cdcWiresC.GetXaxis().SetTitle("X [mm]")
      g_cdcWiresC.GetYaxis().SetTitle("Y [mm]")
    g_cdcWiresC.Draw("AP")
    graph.Draw("Psame")
  else: 
    graph.Draw("AP")

  if isRPhi or isRPhiLocal:
    #https://gitlab.in2p3.fr/comet/ICEDUST_packages/-/blob/master/SimG4/geometry_macros/CylindricalDriftChamber.macro#L50
    ewiret_o = ROOT.TEllipse(0,0,846,846)
    ewiret_o.SetFillStyle(0)
    ewiret_o.SetLineColor(ROOT.kBlack)
    #https://gitlab.in2p3.fr/comet/ICEDUST_packages/-/blob/master/SimG4/geometry_macros/CylindricalDriftChamber.macro#L51
    ewiret_i = ROOT.TEllipse(0,0,488.8,488.8)
    ewiret_i.SetFillStyle(0)
    ewiret_i.SetLineColor(ROOT.kBlack)
    #https://gitlab.in2p3.fr/comet/ICEDUST_packages/-/blob/master/SimG4/geometry_macros/MuonStoppingTarget.macro#L25
    disk = ROOT.TEllipse(0,0,100,100)
    disk.SetFillStyle(0)
    disk.SetLineColor(ROOT.kBlack)
    ewiret_o.Draw("SAME")
    ewiret_i.Draw("SAME")
    disk.Draw("SAME")

  if paramCirc[0] != None and paramCirc[1] != None and paramCirc[2] != None:
    circle = ROOT.TEllipse(paramCirc[0],paramCirc[1],paramCirc[2],paramCirc[2])
    circle.SetFillStyle(0)
    circle.SetLineColor(ROOT.kBlue)
    circle.SetLineStyle(2)
    circle.Draw("SAME")
    # print (paramCirc[0],paramCirc[1],paramCirc[2],paramCirc[2])

  canv.SaveAs(outDir+"/"+pdftitle+".pdf")

  del canv

def MakeDirIfDontExist(dirPath):
  if not os.path.exists(dirPath):
    os.makedirs(dirPath)


if __name__ == "__main__":
  main()