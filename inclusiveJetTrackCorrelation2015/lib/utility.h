
#ifndef xUtility
#define xUtility

#ifndef ROOT_TCanvas
#include "TCanvas.h"
#endif

#ifndef ROOT_TString
#include "TString.h"
#endif

#ifndef ROOT_TLegend
#include "TLegend.h"
#endif

#ifndef ROOT_TLine
#include "TLine.h"
#endif

#include <iomanip> // setprecision
#include <sstream>

class padPolisher{
	public : 
		padPolisher(){
			tl=new TLegend();
			tx=new TLatex();
			line=new TLine();
		};
		void cent4Labels(TCanvas *, float x=0.65, float y=0.8);
		void centLabel(int k, TPad *pad=NULL, float x=0.65, float y=0.8);
		void ptLabel( int k, float *ptbins, TString name="p_{T}", TPad *pad = NULL, float x=0.6, float y=0.8);
		~padPolisher(){
			delete tl;
			delete tx;
			delete line;
		}

	public :
		TLegend* tl;
		TLatex* tx;
		TLine * line;
		std::stringstream stream;
		TString str;
};

void padPolisher::cent4Labels(TCanvas *c, float x, float y){
	TString l[4]= {"0-10%", "10-30%","30-50%","50-100%"};
	for(int i=0;i<4;++i){
		c->cd(4-i);
		tx->DrawLatexNDC(x,y,l[i]);
	}
}

void padPolisher::ptLabel(int k, float *ptbins, TString name, TPad *pad, float x, float y){
	if(pad!=NULL) pad->cd();
	int np1=0, np2=0;
	if(ceil(ptbins[k])!=floor(ptbins[k])) np1 = 1;
	if(ceil(10*ptbins[k])!=floor(10*ptbins[k])) np1 = 2;
	if(ceil(ptbins[k+1])!=floor(ptbins[k+1])) np2 = 1;
	if(ceil(10*ptbins[k+1])!=floor(10*ptbins[k+1])) np2 = 2;
	stream.str("");
	stream<<fixed<<setprecision(np1)<<ptbins[k]<<"< "<<name<<"< "<<fixed<<setprecision(np2)<<ptbins[k+1];
	str=stream.str();
	tx->DrawLatexNDC(x,y,str);	
}

void padPolisher::centLabel(int k, TPad *pad, float x, float y){
	TString l[4]= {"0-10%", "10-30%","30-50%","50-100%"};
	if(pad!=NULL) pad->cd();
	str="cent:"+l[k];
	tx->DrawLatexNDC(x,y,str);
}
#endif
