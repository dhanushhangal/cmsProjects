
#ifndef xthf4_H
#define xthf4_H

#ifndef xAlgo_H
#include "xAlgo.h"
#endif

#include "TH2F.h"
#include <iomanip> // setprecision
#include <sstream> // stringstream

class xthf4 {
	public:
		xthf4(); 
		xthf4(TString name, TString title, int nxbin, float xmin,float xmax, \
				int nybin, float ymin, float ymax,\
				int nzbin, float *zbin, int nwbin, float *wbin);
		xthf4(TString name, TString title, int nxbin, float * xbin, int nybin, float *ybin,\
				int nzbin, float *zbin, int nwbin, float *wbin);
		TH2F* hist (int i, int j); //input the 2D index and return the corresponding 1 D index;
		void RebinZ(int n, float *bins);
		void RebinW(int n, float *bins);
		int Fill(float x, float y, float z, float w, float wit = 1);
		void Write();
		int Read(TString name,TString title, TFile *f , int nz, float *zbin, int nw, float *wbin );
		void init_setup(int nzbin, float *zbin, int nwbin, float *wbin);
		void ztitle_setup(int nzbin, float *zbin);
		void wtitle_setup(int nwbin, float *wbin);
		~xthf4();
	public:
		bool isempty;
		int nhist;
		TH2F** hf4;
		//std::vector<TH2F*> hf4;
		std::vector<float> zbin;
		std::vector<float> wbin;
		std::vector<std::string> ztitle;
		std::vector<std::string> wtitle;
		TString hname ;
		TString htitle ;
		int nz, nw;// this is the total number of histo in z or w (including overflow)
		int np;
		std::stringstream stream;
};

xthf4::xthf4(){
	isempty=1;
	np=1;
}

xthf4::xthf4(TString name, TString title, int nxbin, float xmin,float xmax, \
		int nybin, float ymin, float ymax,\
		int nzbin, float *zbin1, int nwbin, float *wbin1)
{
	hname = name;
	htitle = title;
	np=1;
	TString temp;
	init_setup(nzbin, zbin1,  nwbin, wbin1);
	hf4=new TH2F*[nz*nw];
	for(int jw=0; jw<nwbin+1; ++jw){
		for(int jz=0; jz<nzbin+1; ++jz){
			temp = title+ztitle[jz]+wtitle[jw];
			hf4[jw*nz+jz]= new TH2F(name+Form("_%d_%d",jz,jw), temp,
						nxbin, xmin, xmax, 
						nybin, ymin, ymax);
			hf4[jw*nz+jz]->Sumw2();
		}
	}
}


TH2F* xthf4::hist ( int i, int j){
	return hf4[i+j*nz];
}
void xthf4::RebinZ(int n, float *bins){
	std::vector<int > binIndx;
	for(int j=0; j<n+1; ++j){
		for(int i=j; i<zbin.size(); ++i){
			if(zbin[i]==bins[j]){
				binIndx.push_back(i);
				break;
			}
			std::cout<<"error:binning can't be match exactly!"<<std::endl;
			return ;
		}
	}
	TString temp;
	ztitle.clear();
	ztitle_setup(n, bins);
	TH2F ** hfnew = new TH2F*[nw*(n+1)];
	for(int l=0; l<nw; ++l){
		for(int j=0; j<n; ++j){
			hfnew[j+l*(n+1)]=hf4[binIndx[j]+l*nz];
			hfnew[j+l*(n+1)]->SetName(hname+Form("_%d_%d",j, l));
			temp = htitle+ztitle[j]+wtitle[l];
			hfnew[j+l*(n+1)]->SetTitle(temp);
			for(int k=binIndx[j]+1; k<binIndx[j+1]; ++k){
				hfnew[j+l*(n+1)]->Add(hf4[k+l*nz]);
				delete hf4[k+l*nz];
			}
		}
		hfnew[n+l*(n+1)] = hf4[nz-1+l*nz];//overflow
		hfnew[n+l*(n+1)]->SetName(hname+Form("_%d_%d", n, l));
	}
	delete [] hf4;
	hf4 = hfnew;
	nz=zbin.size();
}

void xthf4::RebinW(int n, float *bins){
	std::vector<int > binIndx;
	for(int j=0; j<n+1; ++j){
		for(int i=j; i<wbin.size(); ++i){
			if(wbin[i]==bins[j]){
				binIndx.push_back(i);
				break;
			}
			return;
		std::cout<<"error:binning can't be match exactly!"<<std::endl;
		}
	}
	TString temp;
	wtitle.clear();
	wtitle_setup(n, bins);
	TH2F ** hfnew = new TH2F*[nz*(n+1)];
	for(int l=0; l<nz; ++l){
		for(int j=0; j<n; ++j){
			hfnew[l+j*nz] = hf4[l+binIndx[j]*nz ];
			hfnew[l+j*nz]->SetName(hname+Form("_%d_%d",l, j));
			temp = htitle+ztitle[l]+wtitle[j];
			hfnew[l+j*nz]->SetTitle(temp);
			for(int k=binIndx[j]+1; k<binIndx[j+1]; ++k){
				hfnew[l+j*nz]->Add(hf4[l+k*nz]);
				delete hf4[l+k*nz];
			}
		}
		hfnew[l+n*nz] = hf4[l+(nw-1)*nz];//overflow
		hfnew[l+n*nz]->SetName(hname+Form("_%d_%d", l, n));
	}
	delete [] hf4;
	hf4 = hfnew;
	nw=wbin.size();
}
int xthf4::Fill(float x, float y, float z, float w, float wit){
	int jz = xAlgo::binarySearch(z, zbin, zbin.size()-1, 0); // size-1 is the overflow bin
	int jw = xAlgo::binarySearch(w, wbin, wbin.size()-1, 0); // size-1 is the overflow bin
	if(jz <0) jz = nz-1;
	if(jw <0) jw = nw-1;
	hist(jz, jw)->Fill(x,y,wit); 
	if(jz== nz-1 || jw == nw-1) return 1;
	else return 0;
}

void xthf4::Write(){
	for(int i=0; i<nz*nw; ++i){
		hf4[i]->Write();
	}
}

int xthf4::Read(TString name, TString title, TFile *f , int nz1, float *zbin1, int nw1, float *wbin1){
	if(!isempty ) {
		std::cout<<"thf4 is filled by other histograms, load failed!"<<std::endl;
		return 0;
	}
	init_setup(nz1, zbin1,nw1, wbin1);
	int nn=0;
	hname = name;
	htitle = title;
	hf4 = new TH2F*[(nz1+1)*(nw1+1)];
	for(int j=0; j<nw; ++j){
		for(int i=0;i<nz; ++i){
			hf4[j*nz+i] = (TH2F*) f->Get(name+Form("_%d_%d",i,j));
			nn++;
		}
	}
	return nn;
}

void xthf4::init_setup(int nzbin, float *zbin1, int nwbin, float *wbin1){
	nw = nwbin+1;
	nz = nzbin+1;
	wbin.reserve(nwbin+1);
	zbin.reserve(nzbin+1);
	ztitle.reserve(nzbin+1);
	wtitle.reserve(nwbin+1);
	wtitle_setup(nwbin,wbin1);
	ztitle_setup(nzbin,zbin1);
}

void xthf4::wtitle_setup(int nwbin, float *wbin1){
	wbin.clear();
	wbin.reserve(nw);
	for(int j=0; j<nwbin+1;++j) wbin.push_back(wbin1[j]);
	for(int jw=0; jw<nwbin+1; ++jw){
		stream.str("");
		if(jw<nwbin ){
			stream<<", "<<fixed<<setprecision(np)<<wbin1[jw]<<"<= w-axis <"<<fixed<<setprecision(np)<<wbin1[jw+1];
		}
		else {
			stream<<", w-axis >= "<<fixed<<setprecision(np)<<wbin1[jw];
		}
		wtitle.push_back(stream.str());
	}
}

void xthf4::ztitle_setup(int nzbin, float *zbin1){
	zbin.clear();
	for(int j=0; j<nzbin+1;++j) zbin.push_back(float(zbin1[j]));
	for(int jz=0; jz<nzbin+1; ++jz){
		stream.str("");
		if( jz<nzbin) {
			stream<<" : "<<fixed<<setprecision(np)<<zbin1[jz]<<"<= z-aixs<"
				<<fixed<<setprecision(np)<<zbin1[jz+1];
		}
		else{
			stream<<" : z-aixs >= "<<fixed<<setprecision(np)<<zbin1[jz];
		}
		ztitle.push_back(stream.str());
	}
}

xthf4::~xthf4(){
	if(!isempty){
		for(int i=0; i<nz*nw; ++i) delete hf4[i];
	}
	delete [] hf4;
	zbin.clear();
	wbin.clear();
	ztitle.clear();
	wtitle.clear();
}

#endif

/*
   void h4(){
   float bin[] = {0,1,2,3,4,5};
   float zbin[] = {0,2,4,5, 6};
   float newzbin[] = {0,2,5, 6};
   TFile* f = TFile::Open("test.root");
   auto a = new xthf4();
   a->Read("test",f, 3, newzbin, 4, zbin);
//a->hist(1,2)->Draw("colz");
TFile* wf = TFile::Open("test.root","recreate");
xthf4* a = new xthf4("gen","tracks", 5, bin, 5, bin ,4, zbin, 4, zbin);
xthf4* b = new xthf4("rec","tracks", 5, bin, 5, bin ,4, zbin, 4, zbin);
a->Fill(3,2,7.4, 10);
a->Fill(3,2,4, 4);
a->Fill(3,2,3, 4);
a->RebinZ(3,newzbin);
a->Write();
b->Write();
}
*/
