
#include "../lib/import_config.h"
#ifndef JTCSignalProducer_H
#include "../lib/JTCSignalProducer.h"
#endif
#ifndef multiPad_H
#include "../lib/multiPad.h"
#endif
#ifndef xPlotStyle_H
#include "../lib/xPlotStyle.h"
#endif

//const int nPt = 8; const int nCent =2 ;
TString dataDumpPath = "/Users/tabris/cmsProjects/iJTC/dataSet/correlation/";

TString trk_tag[] = {"0.7 < p_{T}^{track} < 1 GeV","1 < p_{T}^{track} < 2 GeV",
		"2 < p_{T}^{track} < 3 GeV", "3 < p_{T}^{track} < 4 GeV",
		"4 < p_{T}^{track} < 8 GeV", "8 < p_{T}^{track} < 12 GeV", 
		"12 < p_{T}^{track} < 16 GeV", "p_{T}^{track} > 16 Gev"};
TString cent_tag[]={"Cent. 0-30%", "Cent. 30-100%"};
namespace input_raw2D{
		void pullSig(TString fname){
				cout<<"pulling singal for "<<fname;
				TString trkbin [] = {"0.7", "1", "2", "3", "4", "8", "12", "16", "999"};
				TString centbin [] = {"0", "30", "100"};
				JTCSignalProducer *sp1[8][2];
				TFile * wf = new TFile(dataDumpPath+fname+"_JTCSignal.root","recreate");
				TString title;
				cout<<".";
				for(int i=0; i<nPt ; ++i){
						cout<<".";
						for(int j=0; j<nCent ; ++j){
								title = "track pt in ["+trkbin[i]+", "+trkbin[i+1]\
										 +"), cent in ["+centbin[j]+", "+centbin[j+1]+"), jet pt in [120, 1000]";
								raw_sig[i][j]->SetTitle(title);
								mixing [i][j]->SetTitle(title);
								sp1[i][j] = new JTCSignalProducer(raw_sig[i][j], mixing[i][j]);
								sp1[i][j]->getSignal(fname+Form("_%d_%d", i, j));
								sp1[i][j]->WriteTH2();
						}
				}
				cout<<"."<<endl;
				wf->Close();
//				clearInput();
				cout<<"signal dumped to "<<dataDumpPath+fname+"_JTCSignal.root"<<endl;
		}
}

namespace signal2D{
		void drawTableWithRatio(TString name1, TString name2, TFile *f1, TFile *f2){
				JTCSignalProducer *sp1 [nPt][nCent];
				JTCSignalProducer *sp2 [nPt][nCent];
				auto *cp = new doublePanelFig("cp_"+name1+"_"+name2, "", nPt,nCent );
				TH1D* hr[nPt][nCent];
				auto tx = new TLatex();  tx->SetTextSize(.08);
				auto tl = new TLine();   tl->SetLineStyle(2); tl->SetLineWidth(2);
				TString tmp;
				for(int i=0; i<nPt; ++i){
						for(int j=0; j<nCent; ++j){
								sp1[i][j]= new JTCSignalProducer();
								sp1[i][j]->read(f1, name1+Form("_%d_%d", i,j));
								sp2[i][j]= new JTCSignalProducer();
								sp2[i][j]->read(f2, name2+Form("_%d_%d", i,j));
								TH1* h = sp1[i][j]->signal_X(); h->GetXaxis()->SetTitle("#Delta#eta"); h->SetTitle("");
								if(i==0 )h->SetAxisRange(h->GetMinimum()-h->GetMaximum()*0.1, h->GetMaximum()*1.5, "Y");
								else h->SetAxisRange(h->GetMinimum()-h->GetMaximum()*0.1, h->GetMaximum()*1.4, "Y");
								h->SetMarkerColor(kBlue+2);
								h->SetLineWidth(1);
								hr[i][j]=(TH1D*)h->Clone(Form("hr_%d_%d",i,j));
								cp->addHist(h, i+1, 2-j);
								h = sp2[i][j]->signal_X(); h->SetLineColor(kRed);  h->SetMarkerColor(kRed);
								cp->addHist(h, i+1, 2-j); 
								hr[i][j]->Divide(h); hr[i][j]->GetYaxis()->SetNdivisions(505);
								hr[i][j]->SetAxisRange(0, 2, "Y");  cp->addHist(hr[i][j], i+1, 2-j, 1);
								cp->CD(i+1, 2-j, 0); tl->DrawLine(-2.5, 0, 2.5, 0);
								tmp = trk_tag[i]+", "+cent_tag[j];
								tx->DrawLatexNDC(0.15,0.87, tmp); 
								cp->CD(i+1, 2-j, 1); tl->DrawLine(-2.5, 1, 2.5, 1);
								cp->draw95Area(i+1,2-j, -3, 3);
						}
				}
				cp->SaveAs(name1+"_"+name2+"_overlay.gif");
		}

		void drawTable(TString name1, TString name2, TFile *f1, TFile *f2){
				JTCSignalProducer *sp1 [nPt][nCent];
				JTCSignalProducer *sp2 [nPt][nCent];
				multiPad *cp = new multiPad("cp_"+name1+"_"+name2, "", nPt,nCent );
				for(int i=0; i<nPt; ++i){
						for(int j=0; j<nCent; ++j){
								sp1[i][j]= new JTCSignalProducer();
								sp1[i][j]->read(f1, name1+Form("_%d_%d", i,j));
								sp2[i][j]= new JTCSignalProducer();
								sp2[i][j]->read(f2, name2+Form("_%d_%d", i,j));
								TH1* h = sp1[i][j]->signal_X(); h->GetXaxis()->SetTitle("#Delta#eta"); h->SetTitle("");
								if(i==0 )h->SetAxisRange(h->GetMinimum()-h->GetMaximum()*0.1, h->GetMaximum()*1.5, "Y");
								else h->SetAxisRange(h->GetMinimum()-h->GetMaximum()*0.1, h->GetMaximum()*1.4, "Y");
								cp->addHist(h, i+1, 2-j);
								h = sp2[i][j]->signal_X(); h->SetLineColor(kRed);
								cp->addHist(h, i+1, 2-j);
						}
				}
				cp->addLine(-2.5, 0, 2.5, 0);
				cp->tl->SetLineStyle(3);
				cp->draw();
				auto tx = new TLatex();  tx->SetTextSize(.08);
				for(int i=0; i<nPt; ++i){
						for(int j=0; j<nCent; ++j){
								cp->cd(i*2+2-j);
								if( j==1) tx->DrawLatexNDC(0.2,0.87, trk_tag[i]);
								if( i==0) tx->DrawLatexNDC(0.6,0.87, cent_tag[j]);
						}
				}
				cp->SaveAs(name1+"_"+name2+"_overlay.gif");
		}
}
