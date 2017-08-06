
#ifndef xthf3_H
#define xthf3_H

#ifndef xAlgo_H
#include "xAlgo.h"
#endif

#ifndef SSTREAM_H
#include <iomanip> // setprecision
#include <sstream> // stringstream
#endif

class xthf3 {
	public: 
		xthf3(int nnp=1){
			nhist=0;
			np=nnp;
		};
		xthf3(TString name, TString ztitle, int nxbin, float xmin,float xmax,int nybin, float ymin, float ymax,\
				int nzbin, float zmin, float zmax);
		void createHist(int nxbin, float xmin,float xmax,int nybin, float ymin, float ymax);
		void ztitle_setup();
		void RebinZ(int nbin, float *bins);
	public :
		TH2F** hf3;
		float *zbin;
		int nhist;
		std::vector<std::string> ztitle;
		TString hname, htitle;
		int np;
		std::stringstream stream;
}
xthf3::xthf3(TString name, TString title, int nxbin, float xmin,float xmax,int nybin, float ymin, float ymax,\
		int nzbin, float zmin, float zmax):
	hname(name),
	htitle(title)
{
	np = 1;
	nhist = nzbin+1;
	zbin = new float[nhist];
	zbin[nhist-1] = zmax;
	float binwidth  = (zmax-zmin)/nzbin;
	for(int i=0;i<nhist-1;++i) zbin[i]= i*binwidth;	
	ztitle_setup();
	createHist(nxbin, xmin, xmax, nybin, ymin,ymax);
}

void xthf3::ztitle_setup(){
	TString stmp;
	for(int i=0;i<nhist;++i){
		stream.str("");
		if( i<nhist) {
			stream<<" : "<<fixed<<setprecision(np)<<zbin1[i]<<"<= "<<htitle<<" <"<<
				<<fixed<<setprecision(np)<<zbin1[i+1];
		}
		else {
			stream<<" : "<<htitle<<" >= "<<fixed<<setprecision(np)<<zbin1[i];
		}
		ztitle.push_back(stream.str());
	}
}

void xthf3::createHist(int nxbin, float xmin,float xmax,int nybin, float ymin, float ymax){
	for(int j=0; j<nhist; ++j){
		hf3[j]= new TH2F(hname+Form("_%d",j), ztitle[j], nxbin, xmin, xmax, nybin, ymin, ymax );
		hf3[j]->Sumw2();
	}
}
void xthf3::RebinZ(int n, float *bins){
	if( bins[0] != zbin[0] ){
		std::cout<<"error: rebin should have the same start point!"<<std::endl;
	       	return;
	}
	for(int j=0; j<n+1; ++j){
		for(int i=j; i<nhist; ++i){
			if(zbin[i]==bins[j]){
				break;
			}
			std::cout<<"new bins can't be fit into the origin one!"<<std::endl;
			return ;
		}
	}
	TString temp;
	delete [] zbin;
	ztitle_setup(n, bins);
	TH2F ** hfnew = new TH2F*[n+1];
	for(int j=0; j<n; ++j){
		hfnew[j]=hf4[bins[j]];
		hfnew[j]->SetName(hname+Form("_%d",j));
		temp = htitle+ztitle[j];
		hfnew[j]->SetTitle(temp);
		for(int k=bins[j]+1; k<bins[j+1]; ++k){
			hfnew[j]->Add(hf3[k]);
			delete hf3[k];
		}
	}
	hfnew[n] = hf3[bins[n]];//overflow
	hfnew[n]->SetName(hname+Form("_%d", n));
	//sum all the rest into over flow;
	for(int j=bins[n]+1; j<nhist; ++j){
		hfnew[n] ->Add(hf3[j]); 
		delete hf3[j];
	}
	delete [] hf3;
	hf3 = hfnew;
	nhist=n+1;
}