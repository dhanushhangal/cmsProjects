

#include "config.h"
#include "dataInput.h"
#include "jetShapeProducer.h"

void mainPath(){
TString inFile ;
TString step1File;
TFile *f;
getCorr(); // get residual correction
/*
*/
inFile= "/Users/tabris/Research/HIN_5.02TeV/dataFile/Correlation/jetPtbin/PbPb_data_jetTrackCorrelation_jtPt120to150.root";
step1File= "jetShapeSignal_jtPt120_150.root";
f = TFile::Open(inFile);
getDataHist(f);
signalFinder(step1File);
f->Close();
inFile= "/Users/tabris/Research/HIN_5.02TeV/dataFile/Correlation/jetPtbin/PbPb_data_jetTrackCorrelation_jtpt150to200.root";
step1File= "jetShapeSignal_jtPt150_200.root";
f = TFile::Open(inFile);
getDataHist(f);
signalFinder(step1File);
f->Close();

inFile= "/Users/tabris/Research/HIN_5.02TeV/dataFile/Correlation/jetPtbin/PbPb_data_jetTrackCorrelation_jtptAbove200.root";
step1File= "jetShapeSignal_jtPtAbove200.root";
f = TFile::Open(inFile);
getDataHist(f);
signalFinder(step1File);
f->Close();

inFile= "/Users/tabris/Research/HIN_5.02TeV/dataFile/Correlation/jetPtbin/PbPb_data_jetTrackCorrelation_alljtpt.root";
step1File= "jetShapeSignal_alljtPt.root";
f = TFile::Open(inFile);
getDataHist(f);
signalFinder(step1File);
f->Close();
/*
inFile= "/Users/tabris/Research/HIN_5.02TeV/dataFile/Correlation/jetPtbin/PbPb_2015_jetTrack_withMixing_meptrig60.root";
step1File= "jetShapeSignal_jtPt200_999_meptrig60.root";
f = TFile::Open(inFile);
getDataHist(f);
signalFinder(step1File);
f->Close();
*/
}
