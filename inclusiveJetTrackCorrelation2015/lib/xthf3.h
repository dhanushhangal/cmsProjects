
#ifndef xthf3_H
#define xthf3_H

#ifndef xAlgo_H
#include "xAlgo.h"
#endif

#include <iomanip> // setprecision
#include <sstream> // stringstream

class xthf3 {
	public: 
		xthf3(int nnp=1){
			nhist=0;
			np=nnp;
		};
		xthf3(TString name, TString ztitle, int nxbin, float xmin,float xmax,int nybin, float ymin, float ymax,\
				int nzbin, float zmin, float zmax);
		void createHist(int nxbin, float xmin,float xmax,int nybin, float ymin, float ymax);
		void init_setup(int nzbin, float *zbin1);
		void ztitle_setup();
		void RebinZ(int nbin, float *bins);
		int Fill(float x, float y, float z, float wit = 1);
		void Write();
		TH2F* hist(int z);
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
init_setup(int nzbin, float *zbin1){
	createHist(nxbin, xmin, xmax, nybin, ymin,ymax);
}

void xthf3::init_setup(int nzbin, float *zbin1){
	nhist = nzbin+1;
	zbin = new float[nhist];
	zbin[nhist-1] = zmax;
	float binwidth  = (zmax-zmin)/nzbin;
	ztitle_setup();
}

void xthf3::ztitle_setup(){
	TString stmp;
	for(int i=0;i<nhist-1;++i) zbin[i]= i*binwidth;	
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
	float* binIndx = new float[n+1];
	if( bins[0] != zbin[0] ){
		std::cout<<"error: rebin should have the same start point!"<<std::endl;
		return;
	}
	for(int j=0; j<n+1; ++j){
		for(int i=j; i<nhist; ++i){
			if(zbin[i]==bins[j]){
				binIndx[j]=i;
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
		hfnew[j]=hf4[binIndx[j]];
		hfnew[j]->SetName(hname+Form("_%d",j));
		temp = htitle+ztitle[j];
		hfnew[j]->SetTitle(temp);
		for(int k=binIndx[j]+1; k<binIndx[j+1]; ++k){
			hfnew[j]->Add(hf3[k]);
			delete hf3[k];
		}
	}
	hfnew[n] = hf3[binIndx[n]];//overflow
	hfnew[n]->SetName(hname+Form("_%d", n));
	//sum all the rest into over flow;
	for(int j=binIndx[n]+1; j<nhist; ++j){
		hfnew[n] ->Add(hf3[j]); 
		delete hf3[j];
	}
	delete [] hf3;
	delete [] binIndx;
	hf3 = hfnew;
	nhist=n+1;
}
int xthf3::Fill(float x, float y, float z, float wit = 1){
	int jz = xAlgo::binarySearch(z, zbin, nhist-1, 0); 
	if(jz <0) jz = nhist-1;
	hist(jz)->Fill(x,y,wit); 
	if(jz== hist-1 ) return 1;
	else return 0;
}

TH2F* xthf3::hist(int z){
	if(z<nhist){
		return hf3[z];
	}
	else{
		std::cout<<"hist out of the range!"<<std::endl;
		return NULL;
	}
}
void xthf3::Write(){
	for(int i=0; i<nhist; ++i){
		hf3[i]->Write();
	}
}
