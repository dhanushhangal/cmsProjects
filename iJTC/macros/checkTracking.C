
#include "../lib/import_config.h"
#include "../lib/JTCSignalProducer.h"
#include "../lib/multiPad.h"
using namespace signal2D;

void checkTracking (){

		TString trk_tag[] = {"1 < p_{T}^{track} < 2 GeV",
				"2 < p_{T}^{track} < 3 GeV", "3 < p_{T}^{track} < 4 GeV",
				"4 < p_{T}^{track} < 8 GeV", "8 < p_{T}^{track} < 12 GeV", 
				"12 < p_{T}^{track} < 16 GeV", "p_{T}^{track} > 20 Gev"};
		TString cent_tag[]={"Cent. 0-30%", "Cent. 30-100%"};

		JTCSignalProducer *sp1 [7][2];
		JTCSignalProducer *sp2 [7][2];
		multiPad *cp = new multiPad("cp", "", 7, 2);
		for(int i=0; i<7; ++i){
				for(int j=0; j<2; ++j){
						sp1[i][j]= new JTCSignalProducer();
						sp1[i][j]->read(recgen_pb_f, Form("rec_gen_%d_%d",i,j));
						sp2[i][j]= new JTCSignalProducer();
						sp2[i][j]->read(recrec_pb_f, Form("rec_rec_%d_%d",i,j));
						TH1* h = sp1[i][j]->signal_X(); h->GetXaxis()->SetTitle("#Delta#eta"); h->SetTitle("");
						if(i==0 )h->SetAxisRange(h->GetMinimum()-h->GetMaximum()*0.1, h->GetMaximum()*1.5, "Y");
						else h->SetAxisRange(h->GetMinimum()-h->GetMaximum()*0.1, h->GetMaximum()*1.2, "Y");
						cp->addHist(h, i+1, 2-j);
						h = sp2[i][j]->signal_X(); h->SetLineColor(kRed);
						cp->addHist(h, i+1, 2-j);
				}
		}
		cp->addLine(-2.5, 0, 2.5, 0);
		cp->tl->SetLineStyle(3);
		cp->draw();
		auto tx = new TLatex();  tx->SetTextSize(.08);
		for(int i=0; i<7; ++i){
				for(int j=0; j<2; ++j){
						cp->cd(i*2-j+2);
						if( j==1) tx->DrawLatexNDC(0.2,0.87, trk_tag[i]);
						if( i==0) tx->DrawLatexNDC(0.6,0.87, cent_tag[j]);
				}
		}
		cp->SaveAs("RecGen_RecRec_overlay.pdf");
						
		
}
