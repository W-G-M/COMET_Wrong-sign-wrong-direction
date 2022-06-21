import sys
import os
import glob
import ROOT
import enum
import math
import numpy
import array
import argparse
import collections

ROOT.gROOT.SetBatch()
ROOT.gROOT.LoadMacro("Analysis.C")

class CDCHits:
  def __init__(self, posX, posY, posZ, posT):
    self.posX = posX 
    self.posY = posY 
    self.posZ = posZ 
    self.posT = posT 

def main():

  cherryPick = []
  #
  # electron
  #
  inputFileName = "oa_g4_em_00000000-0001_yjvamvytt4k2_SG4BH_000_histos.root"
  outFileName = "outputGraph_electron_v2.root"
  # cherryPick = [12,13,24,47,52,98]
  MakeGraphs(inputFileName,outFileName,cherryPick)

  #
  # positron
  #
  inputFileName = "oa_g4_ep_00000000-0001_3mnkdxrzh5fq_SG4BH_000_histos.root"
  outFileName = "outputGraph_positron_v2.root"
  # cherryPick = [10,20,27,29,55]
  MakeGraphs(inputFileName,outFileName,cherryPick)

def MakeGraphs(inputFileName,outFileName, cherryPick):
  # file address
  treeName  = "outTree"

  # Setup tree
  tree = ROOT.TChain(treeName)
  tree.Add(inputFileName)

  nEvents = tree.GetEntries()

  analysis = ROOT.Analysis()
  analysis.Init(tree)

  graphs = collections.OrderedDict()
  histos = collections.OrderedDict()
  
  nEventsTrue = 0
  nEventsPos = 0
  nEventsNeg = 0
  nEventsUnd = 0

  #
  # Loop over the events
  #
  for iEvt in range(0, nEvents):
    # if iEvt > 500: break
    if iEvt > 2000: break
    # print ("\n\n")
    # print ("Event "+str(iEvt)+" / "+str(nEvents))

    analysis.fReader.SetEntry(iEvt)

    # primary_track_id = 0
    # primary_track_pointsIdx = []
    primary_track_points_posX = []
    primary_track_points_posY = []
    primary_track_points_posZ = []
    primary_track_hitsCDC_posT = []
    primary_track_hitsCDC_posX=[]
    primary_track_hitsCDC_posY=[]
    primary_track_hitsCDC_posZ=[]
    primary_track_hitsCDC_localposX = []
    primary_track_hitsCDC_localposY = []
    primary_track_hitsCDC_localposZ = []

    primary_track_hitsCDC = []

    if analysis.TurnNumber.Get()[0] != 1: continue
    if analysis.nG4HitsCDC.Get()[0] < 30: continue

    #
    # Tracks
    #
    # nG4Tracks =  analysis.nG4Tracks.Get()[0]
    # for iT in range(0,nG4Tracks):
    #   track_trackid   = analysis.G4Tracks_trackid.At(iT)
    #   track_parentid  = analysis.G4Tracks_parentid.At(iT)
    #   track_pid       = analysis.G4Tracks_pid.At(iT)

    #   if track_pid == 11 or track_pid == -11:
    #     primary_track_id = track_trackid 
      
    #   Loop through G4TrackPoints container and get the index 
    #   of the elements which is associated to this track.
      
    #   for iP in range(0,analysis.nG4TrackPoints.Get()[0]):
    #     if analysis.G4TrackPoints_trackIdx.At(iP) == iT:
    #       primary_track_pointsIdx.append(iP)
    #
    # Track points
    #
    # for idx in primary_track_pointsIdx:
    #   primary_track_points_posX.append(analysis.G4TrackPoints_posX[idx]-6400.)
    #   primary_track_points_posY.append(analysis.G4TrackPoints_posY[idx])
    #   primary_track_points_posZ.append(analysis.G4TrackPoints_posZ[idx]-7650.)


    #
    # G4HistsCDC
    #
    nG4HitsCDC =  analysis.nG4HitsCDC.Get()[0]
    for iH in range(0,nG4HitsCDC):
      # if analysis.G4HitsCDC_trackid.At(iH) != primary_track_id:
      if abs(analysis.G4HitsCDC_pid.At(iH)) == 11:
        primary_track_hitsCDC_posT.append(analysis.G4HitsCDC_posT.At(iH))
        primary_track_hitsCDC_localposX.append(analysis.G4HitsCDC_localposX.At(iH))
        primary_track_hitsCDC_localposY.append(analysis.G4HitsCDC_localposY.At(iH))
        primary_track_hitsCDC_localposZ.append(analysis.G4HitsCDC_localposZ.At(iH))

        # print(analysis.G4HitsCDC_localposZ.At(iH))
        # primary_track_hitsCDC.append(CDCHits(
        #   analysis.G4HitsCDC_posX.At(iH)-6400.,
        #   analysis.G4HitsCDC_posY.At(iH),
        #   analysis.G4HitsCDC_posZ.At(iH)-7650.,
        #   analysis.G4HitsCDC_posT.At(iH)
        #   )
        # )
    #
    #
    #
    # primary_track_hitsCDC_sorted = sorted(primary_track_hitsCDC, key=lambda x: x.posT)
    # for iH in range(0,len(primary_track_hitsCDC_sorted)):
    #   print (primary_track_hitsCDC_sorted[iH].posT)

    arr_points_posX = array.array('f', primary_track_points_posX)
    arr_points_posY = array.array('f', primary_track_points_posY)
    arr_points_posZ = array.array('f', primary_track_points_posZ)

    arr_hitsCDC_posX = array.array('f', primary_track_hitsCDC_posX)
    arr_hitsCDC_posY = array.array('f', primary_track_hitsCDC_posY)
    arr_hitsCDC_posZ = array.array('f', primary_track_hitsCDC_posZ)

    arr_hitsCDC_localposX = array.array('f', primary_track_hitsCDC_localposX)
    arr_hitsCDC_localposY = array.array('f', primary_track_hitsCDC_localposY)
    arr_hitsCDC_localposZ = array.array('f', primary_track_hitsCDC_localposZ)
  
    # if len(arr_points_posY) > 0:
    #   gName = "Graph2D_Points_Evt"+str(iEvt)
    #   graphs[gName] = ROOT.TGraph(len(arr_points_posY),    arr_points_posY, arr_points_posZ);
    #   graphs[gName].SetName(gName)
    
    xc = None
    yc = None
    radius = None
    xc, yc, radius = FitCircle(primary_track_hitsCDC_localposX,primary_track_hitsCDC_localposY)
    dirZ = CheckCharge(primary_track_hitsCDC_localposX,primary_track_hitsCDC_localposY,xc,yc,radius)
    
    strTAG = ""
    nEventsTrue += 1
    if dirZ > 1500:
      nEventsNeg += 1
      strTAG = "TagNeg"
    elif dirZ < 1500:
      nEventsPos += 1
      strTAG = "TagPos"
    else:
      nEventsUnd += 1 
      strTAG = "TagUnd"

    if len(arr_hitsCDC_localposX) > 0:
      gName = "Graph2D_HitsCDC_LOCAL_XY_Evt"+str(iEvt)+"_"+strTAG
      graphs[gName] = ROOT.TGraph(len(arr_hitsCDC_localposX), arr_hitsCDC_localposX, arr_hitsCDC_localposY);
      graphs[gName].SetName(gName)

    if xc != None and yc != None and radius != None:
      hName = "h_CircleParam_XY_Evt"+str(iEvt)
      histos[hName] = ROOT.TH1F(hName,hName,3,0,3)
      histos[hName].SetBinContent(1,xc)
      histos[hName].SetBinContent(2,yc)
      histos[hName].SetBinContent(3,radius)

    if len(arr_hitsCDC_localposX) > 0:
      gName = "Graph2D_HitsCDC_LOCAL_ZY_Evt"+str(iEvt)+"_"+strTAG
      graphs[gName] = ROOT.TGraph(len(arr_hitsCDC_localposZ), arr_hitsCDC_localposZ, arr_hitsCDC_localposY);
      graphs[gName].SetName(gName)

  print(str(dirZ))
  print ("nEventsTrue = "+ str(nEventsTrue))
  print ("nEventsPos  = "+ str(nEventsPos))
  print ("nEventsNeg  = "+ str(nEventsNeg))
  print ("nEventsUnd  = "+ str(nEventsUnd))
  print ("Efficiency")
  print ("nEventsPos  = {:.1f}%".format((nEventsPos/nEventsTrue)*100.))
  print ("nEventsNeg  = {:.1f}%".format((nEventsNeg/nEventsTrue)*100.))
  print ("nEventsUnd  = {:.1f}%".format((nEventsUnd/nEventsTrue)*100.))
  print ("\n\n")

  inFile = ROOT.TFile.Open(inputFileName)
  g_cdcWires = inFile.Get("CDCWires")
  g_cdcWiresC =  g_cdcWires.Clone()
  outFile = ROOT.TFile(outFileName,"RECREATE")
  outFile.cd()
  g_cdcWiresC.Write()
  for gName in graphs: graphs[gName].Write()
  for hName in histos: histos[hName].Write()
  outFile.Close()

def CheckCharge(xIn,yIn, xc, yc, R):

  vec3_OA = ROOT.TVector3(xIn[0],yIn[0],0)
  vec3_OB = ROOT.TVector3(xIn[1],yIn[1],0)
  vec3_OR = ROOT.TVector3(xc,yc,0)

  vec3_AB = vec3_OA-vec3_OB
  vec3_BR = vec3_OB-vec3_OR
  
  vec3_ABXBR = vec3_AB.Cross(vec3_BR)

  return vec3_ABXBR.Z()

def FitCircle(xIn,yIn):
  #
  # https://dtcenter.org/sites/default/files/community-code/met/docs/write-ups/circle_fit.pdf
  # https://scipy-cookbook.readthedocs.io/items/Least_Squares_Circle.html
  #
  # print("Fitting")
  
  x = numpy.r_[xIn]
  y = numpy.r_[yIn]

  # coordinates of the barycenter
  x_m = numpy.mean(x)
  y_m = numpy.mean(y)

  # calculation of the reduced coordinates
  u = x - x_m
  v = y - y_m

  # linear system defining the center in reduced coordinates (uc, vc):
  #    Suu * uc +  Suv * vc = (Suuu + Suvv)/2
  #    Suv * uc +  Svv * vc = (Suuv + Svvv)/2
  Suv  = numpy.sum(u*v)
  Suu  = numpy.sum(u**2)
  Svv  = numpy.sum(v**2)
  Suuv = numpy.sum(u**2 * v)
  Suvv = numpy.sum(u * v**2)
  Suuu = numpy.sum(u**3)
  Svvv = numpy.sum(v**3)

  # Solving the linear system
  A = numpy.array([ [ Suu, Suv ], [Suv, Svv]])
  B = numpy.array([ Suuu + Suvv, Svvv + Suuv ])/2.0
  uc, vc = numpy.linalg.solve(A, B)

  xc_1 = x_m + uc
  yc_1 = y_m + vc

  # Calculation of all distances from the center (xc_1, yc_1)
  Ri_1      = numpy.sqrt((x-xc_1)**2 + (y-yc_1)**2)
  R_1       = numpy.mean(Ri_1)
  residu_1  = numpy.sum((Ri_1-R_1)**2)
  residu2_1 = numpy.sum((Ri_1**2-R_1**2)**2)

  # print ("{:<5.3f} {:<5.3f} {:<5.3f}".format(xc_1, yc_1, R_1))

  return xc_1, yc_1, R_1

"""
def FitHelix(xIn,yIn,zIn):
  #
  # https://dtcenter.org/sites/default/files/community-code/met/docs/write-ups/circle_fit.pdf
  # https://scipy-cookbook.readthedocs.io/items/Least_Squares_Circle.html
  #
  print("Fitting")
  
  x = numpy.r_[xIn]
  y = numpy.r_[yIn]

  # coordinates of the barycenter
  x0 = numpy.mean(x)
  y0 = numpy.mean(y)

  # calculation of the reduced coordinates
  u = x - x0
  v = y - y0

  # linear system defining the center in reduced coordinates (uc, vc):
  #    Suu * uc +  Suv * vc = (Suuu + Suvv)/2
  #    Suv * uc +  Svv * vc = (Suuv + Svvv)/2
  Suv  = numpy.sum(u*v)
  Suu  = numpy.sum(u**2)
  Svv  = numpy.sum(v**2)
  Suuv = numpy.sum(u**2 * v)
  Suvv = numpy.sum(u * v**2)
  Suuu = numpy.sum(u**3)
  Svvv = numpy.sum(v**3)

  # Solving the linear system
  A = numpy.array([ [ Suu, Suv ], [Suv, Svv]])
  B = numpy.array([ Suuu + Suvv, Svvv + Suuv ])/2.0
  uc, vc = numpy.linalg.solve(A, B)

  xc_1 = x0 + uc
  yc_1 = y0 + vc

  # Calculation of all distances from the center (xc_1, yc_1)
  Ri_1      = numpy.sqrt((x-xc_1)**2 + (y-yc_1)**2)
  R_1       = numpy.mean(Ri_1)
  residu_1  = numpy.sum((Ri_1-R_1)**2)
  residu2_1 = numpy.sum((Ri_1**2-R_1**2)**2)

  FLT_EPSILON = 1e-5

  tmp = []
  p0 = numpy.arctan2(yIn[0] - y0, xIn[0] - x0)
  z0 = zIn[0]

  for i in range(1,len(xIn)): 
    p1 = numpy.arctan2(yIn[i] - y0, xIn[i] - x0)
    dp = p1 - p0
    dz = zIn[i] - z0
    p0 = p1 
    z0 = zIn[i]
    if numpy.fabs(dz) > FLT_EPSILON:
      tmp.append(dp / dz)

  ntmp = len(tmp)
  tmp.sort()
  # if (ntmp % 2)==0:
  #   w0 = 0.5 * (tmp[(ntmp / 2) - 1] + tmp[ntmp / 2])  
  # else:
  #   w0 = tmp[ntmp / 2]

  if (ntmp % 2)==0:
    w0 = 0.5 * (tmp[int((int(ntmp) / 2)-1)] + tmp[int(int(ntmp) / 2)])  
  else:
    w0 = tmp[int(int(ntmp) / 2)]

  p0 = numpy.arctan2(yIn[0] - y0, xIn[0] - x0)
  z0 = zIn[0]

  offset = 0
  sp = 0 
  sw = 0 
  spw = 0 
  sww = 0

  for i in range(0,len(xIn)): 

    p = numpy.arctan2(yIn[i] - y0, xIn[i] - x0) + offset
    p1 = p0 + w0 * (zIn[i] - z0)

    while numpy.fabs(p1 - p) > (1.5 * numpy.pi): 
      sgn =  1 if (p < p1) else  -1
      offset += sgn * 2 * numpy.pi;
      p += sgn * 2 * numpy.pi;

    z0 = zIn[i]
    p0 = p

    sp += p
    sw += zIn[i]
    spw += p * zIn[i]
    sww += zIn[i] * zIn[i]

  detz = sww * len(xIn) - sw * sw;
  # if (fabs(detz) < FLT_EPSILON) return false;
  w0 = (spw * len(xIn) - sp * sw) / detz;
  phi0 = (sww * sp - spw * sw) / detz;

  c0 = 299792458; # m / s 
  mm = 1E-03;  # in m 
  MeV = 1E+06; # in eV 
  B = 1.;      # T, TODO: read from EMField */
  momentum0 = c0 * B * (R_1 * mm) / MeV * numpy.sqrt(1 + 1 / (R_1 * R_1 * w0 * w0));

  print (momentum0, w0, phi0)
"""

      




if __name__ == "__main__":
  main()