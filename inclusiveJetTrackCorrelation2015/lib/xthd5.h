

#ifndef xthd5_H
#define xthd5_H
#include <iomanip> // setprecision
#include <sstream> // stringstream
#include "xAlgo.h"

#ifndef ROOT_TH2D
#include "TH2D.h"
#endif

class xthd5 {
	public:

		xthd5(){
			nhist =0;
		}
		xthd5(TString name, TString title, int nxbin, float xmin,float xmax, \
				int nybin, float ymin, float ymax, int nzbin, float *zbin1, int nwbin, float *wbin1, int nrbin,
				float * rbin1, TString ztitle="z-axis",TString wtitle="w-axis",TString rtitle="r-axis");
		void axis_title_setup(TString name,TString *&aname, int nbin, float *bins, float *& bins2);
		int Fill(float x,float y,float z,float w,float r,float wit=1);
		TH2D* hist (int i, int j, int k); //input the 2D index and return the corresponding 1 D index;
		int checkRebin(int n, float *bins, int n2, float*bins2, std::vector<int> &indx);
		void RebinZ(int n, float *bins);
		void RebinW(int n, float *bins);
		void RebinR(int n, float *bins);
		void Write();
		int Read(TString name, TString title, TFile *f , int nz1, float *zbin1, int nw1, float *wbin1, int nr1, float *rbin1, TString ztitle="z-axis",TString wtitle="w-axis",TString rtitle="r-axis");
		~xthd5();

	public:
		int nhist;
		TH2D** hd5;
		float* zbins,* wbins, *rbins;
		TString hname, htitle;
		TString *zname, *wname, *rname;
		int nz, nw, nr;
		std::stringstream stream;
		int np;
};


xthd5::xthd5(TString name, TString title, int nxbin, float xmin,float xmax, \
		int nybin, float ymin, float ymax, int nzbin, float *zbin1, int nwbin, float *wbin1, int nrbin,
		float * rbin1, TString ztitle,TString wtitle,TString rtitle)
{
	hname = name;
	htitle = title;
	nz=nzbin+1;
	nw=nwbin+1;
	nr=nrbin+1;
	nhist = nz*nw*nr;
	np=1;
	zbins = NULL;
	wbins = NULL;
	rbins = NULL;
	zname = NULL;
	wname = NULL;
	rname = NULL;
	axis_title_setup(ztitle, zname, nzbin, zbin1, zbins);
	axis_title_setup(wtitle, wname, nwbin, wbin1, wbins);
	axis_title_setup(rtitle, rname, nrbin, rbin1, rbins);
//	cout<<zbins<<endl;

//	cout<<zbins[0]<<endl;
	TString tmp;
	hd5 = new TH2D*[nhist];
	for(int i=0; i<nz; ++i){
		for(int j=0; j<nw; ++j){
			for(int k=0; k<nr; ++k){
				tmp = " :"+zname[i]+wname[j]+rname[k];
				hd5[i+j*nz+k*nz*nw]= new TH2D(hname+Form("_%d_%d_%d",i,j, k), tmp,
						nxbin, xmin, xmax, nybin, ymin, ymax);
				hd5[i+j*nz+k*nz*nw]->Sumw2();
			}
		}
	}
}

void xthd5::axis_title_setup(TString name, TString *&aname, int nbin, float *bins, float *&bins2){
	if(bins2!=NULL) delete [] bins2;
	if(aname!=NULL) delete [] aname;
	bins2 = new float[nbin+1];
	aname = new TString[nbin+1];
	for(int i=0;i<nbin+1;++i){
		bins2[i]=bins[i];
		stream.str("");
		if(i<nbin)
			stream<<fixed<<setprecision(np)<<bins[i]<<"<="<<name<<"<"<<fixed<<setprecision(np)<<bins[i+1];
		else 
			stream<<*name<<">"<<fixed<<setprecision(np)<<bins[i];
		aname[i]=stream.str();
	}
}

TH2D* xthd5::hist(int i, int j, int k){
	return hd5[i+j*nz+k*nz*nw];
}
int xthd5::Fill(float x,float y,float z, float w, float r,float wit){
	int jz = xAlgo::binarySearch(z, zbins, nz-1, 0); // size-1 is the overflow bin
	int jw = xAlgo::binarySearch(w, wbins, nw-1, 0); // size-1 is the overflow bin
	int jr = xAlgo::binarySearch(r, rbins, nr-1, 0); // size-1 is the overflow bin
	if(jz <0) jz = nz-1;
	if(jw <0) jw = nw-1;
	if(jr <0) jr = nr-1;
	//cout<<"i="<<jz<<", j="<<jw<<", k="<<jr<<endl;
	hist(jz,jw,jr)->Fill(x,y,wit);
	if(jz == nz-1||jw==nw-1||jr==nr-1) return 1;
	return 0;
}

int xthd5::checkRebin(int n, float *bins, int n2, float*bins2, std::vector <int > & indx){
	indx.clear();
	if(bins[0]!=bins2[0]|| bins[n]!=bins2[n2]){
		std::cout<<"error:rebin range can't cover the origin range"<<std::endl;
		return 1;
	}
	for(int j=0; j<n+1; ++j){
		for(int i=j; i<n2+1; ++i){
			if(bins2[i]==bins[j]){
				indx.push_back(i);
				break;
			}
		}
	}
	if(indx.size()!=n+1){
		std::cout<<"error:bin can't be match exactly!"<<std::endl;
		return 1;
	}
	return 0;
}

void xthd5::RebinZ(int n, float *bins){
	std::vector<int > binIndx;
	if(checkRebin(n, bins, nz-1,zbins, binIndx)) return;
	delete [] zname;
	axis_title_setup("z-axis",zname, n, bins, zbins);
	TH2D ** hfnew = new TH2D*[(n+1)*nw*nr];
	TString temp;
	for(int j=0; j<nw; ++j){
		for(int k=0;k<nr; ++k){
			for(int i=0;i<n; ++i){
				hfnew[i+j*nz+k*nz*nw] =hist(i,j,k);
				hfnew[i+j*nz+k*nz*nw]->SetName(hname+Form("_%d_%d_%d",i,j,k));
				temp = " : "+zname[i]+", "+wname[j]+", "+rname[k];
				hfnew[i+j*nz+k*nz*nw]->SetTitle(temp);
				for(int l=binIndx[i]+1;l<binIndx[i+1]; ++l){
					hfnew[i+j*nz+k*nz*nw]->Add(hist(l,j,k));
					delete hist(l,j,k);
				}
			}
			hfnew[n+j*nz+k*nz*nw]= hist(nz-1,j,k);//overflow
			hfnew[n+j*nz+k*nz*nw]->SetName(hname+Form("_%d_%d_%d", n, j,k));
		}
	}
	delete [] hd5;
	hd5= hfnew;
	nz=n+1;
}
void xthd5::RebinW(int n, float *bins){
	std::vector<int > binIndx;
	if(checkRebin(n, bins, nw-1,wbins, binIndx)) return;
	delete [] wname;
	axis_title_setup("w-axis",wname, n, bins, wbins);
	TH2D ** hfnew = new TH2D*[nz*(n+1)*nr];
	TString temp;
	for(int i=0;i<nz; ++i){
		for(int k=0;k<nr; ++k){
			for(int j=0; j<n; ++j){
				hfnew[i+j*nz+k*nz*nw] =hist(i,j,k);
				hfnew[i+j*nz+k*nz*nw]->SetName(hname+Form("_%d_%d_%d",i,j,k));
				temp = " : "+zname[i]+", "+wname[j]+", "+rname[k];
				hfnew[i+j*nz+k*nz*nw]->SetTitle(temp);
				for(int l=binIndx[i]+1;l<binIndx[i+1]; ++l){
					hfnew[i+j*nz+k*nz*nw]->Add(hist(i,l,k));
					delete hist(i,l,k);
				}
			}
			hfnew[i+n*nz+k*nz*nw]= hist(i,nw-1,k);//overflow
			hfnew[i+n*nz+k*nz*nw]->SetName(hname+Form("_%d_%d_%d", i, n,k));
		}
	}
	delete [] hd5;
	hd5= hfnew;
	nw=n+1;
}
void xthd5::RebinR(int n, float *bins){
	std::vector<int > binIndx;
	if(checkRebin(n, bins, nr-1,rbins, binIndx)) return;
	delete [] rname;
	axis_title_setup("r-axis",rname, n, bins, rbins);
	TH2D ** hfnew = new TH2D*[nz*nw*(n+1)];
	TString temp;
	for(int i=0;i<nz; ++i){
		for(int j=0; j<nw; ++j){
			for(int k=0;k<n; ++k){
				hfnew[i+j*nz+k*nz*nw] =hist(i,j,k);
				hfnew[i+j*nz+k*nz*nw]->SetName(hname+Form("_%d_%d_%d",i,j,k));
				temp = " : "+zname[i]+", "+wname[j]+", "+rname[k];
				hfnew[i+j*nz+k*nz*nw]->SetTitle(temp);
				for(int l=binIndx[i]+1;l<binIndx[i+1]; ++l){
					hfnew[i+j*nz+k*nz*nw]->Add(hist(i,j,l));
					delete hist(i,j,l);
				}
			}
			hfnew[i+j*nz+n*nz*nw]= hist(i,j,n);//overflow
			hfnew[i+j*nz+n*nz*nw]->SetName(hname+Form("_%d_%d_%d", i, j,n));
		}
	}
	delete [] hd5;
	hd5= hfnew;
	nr=n+1;
}

int xthd5::Read(TString name, TString title, TFile *f , int nz1, float *zbin1, int nw1, float *wbin1, int nr1, float *rbin1, TString ztitle,TString wtitle,TString rtitle){
	
	if(!nhist) {
		std::cout<<"thd5 is filled by other histograms, load failed!"<<std::endl;
		return 0;
	}
	nz = nz1+1;
	nw = nw1+1;
	nr = nr1+1;
	nhist = 0;
	hname = name;
	htitle = title;
	zbins = NULL;
	wbins = NULL;
	rbins = NULL;
	zname = NULL;
	wname = NULL;
	rname = NULL;
	axis_title_setup(ztitle, zname, nz1, zbin1, zbins);
	axis_title_setup(wtitle, wname, nw1, wbin1, wbins);
	axis_title_setup(rtitle, rname, nr1, rbin1, rbins);
	hd5 = new TH2D*[nhist];
	TString tmp;
	for(int i=0; i<nz; ++i){
		for(int j=0; j<nw; ++j){
			for(int k=0; k<nr; ++k){
				nhist++;
				tmp = " :"+zname[i]+wname[j]+rname[k];
				hd5[i+j*nz+k*nz*nw]= (TH2D*)f->Get(hname+Form("_%d_%d_%d",i,j, k));
			}
		}
	}
	return nhist;
}

void xthd5::Write(){
	for(int i=0; i<nz*nw*nr; ++i){
		hd5[i]->Write();
	}
}

xthd5::~xthd5(){
	delete [] zname;
	delete [] wname;
	delete [] rname;
	delete [] hd5;
	delete [] zbins;
	delete [] wbins;
	delete [] rbins;
}

#endif
