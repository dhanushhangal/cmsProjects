
#include "xAlgo.h"

class xthf4 {
	public:
		xthf4(){ isempty = 1;}; xthf4(TString name, int nxbin, float * xbin, int nybin, float *ybin,\
				int nzbin, float *zbin, int nwbin, float *wbin);
		int order(int i, int j, int m, int n);
		TH2F* hist (int i, int j); //input the 2D index and return the corresponding 1 D index;
		void RebinZ(int n, float *bins);
		void RebinW(int n, float *bins);
		int Fill(float x, float y, float z, float w, float wit = 1);
		void Write();
		int Read(TString name,TFile *f , int nz, float *zbin, int nw, float *wbin );
	public:
		bool isempty;
		std::vector<TH2F*> hf4;
		std::vector<float> zbin;
		std::vector<float> wbin;
		TString hname ;
		int nz, nw;// this is the total number of histo in z or w (including overflow)
};


xthf4::xthf4(TString name, int nxbin, float * xbin, int nybin, float *ybin,\
		int nzbin, float *zbin1, int nwbin, float *wbin1):
	nz(nzbin+1), nw(nwbin+1)
{
	hf4.reserve(nz*nw);
	hname = name;
	for(int jw=0; jw<nwbin+1; ++jw){
		for(int jz=0; jz<nzbin+1; ++jz){
			if(jw==nwbin )
				hf4.push_back(new TH2F(name+Form("_%d_%d",jz,jw), Form("overflow at the %dth in z",jz),
							nxbin, xbin, nybin,ybin));
			else if(jz == nzbin) 
				hf4.push_back(new TH2F(name+Form("_%d_%d",jz,jw), Form("overflow at the %dth in w",jw),
							nxbin, xbin, nybin,ybin));
			else hf4.push_back(new TH2F(name+Form("_%d_%d",jz,jw), "", nxbin, xbin, nybin,ybin));
			hf4.back()->Sumw2();
		}
	}
	for(int j=0; j<nzbin+1;++j) zbin.push_back(zbin1[j]);
	for(int j=0; j<nwbin+1;++j) wbin.push_back(wbin1[j]);
}

TH2F* xthf4::hist ( int i, int j){
	return hf4[i+j*nz];
}
int xthf4::order ( int i, int j, int m, int n){
	return i+j*m;
}
void xthf4::RebinZ(int n, float *bins){
	std::vector<int > binIndx;
	for(int j=0; j<n+1; ++j){
		for(int i=j; i<zbin.size(); ++i){
			if(zbin[i]==bins[j]){
				binIndx.push_back(i);
				break;
			}
		}
	}
	if(binIndx.size()<n+1){
		std::cout<<"error:binning can't be match exactly!"<<std::endl;
		return;
	}
	std::vector<TH2F* > htmp;
	htmp.resize((n+1)*nw);
	for(int l=0; l<nw; ++l){
		for(int j=0; j<binIndx.size()-1; ++j){
			htmp[order(j,l,n+1, nw)] = hf4[order(binIndx[j],l, nz, nw)];
			htmp[order(j,l,n+1, nw)]->SetName(hname+Form("_%d_%d",j, l));
			for(int k=binIndx[j]+1; k<binIndx[j+1]; ++k){
				htmp[order(j,l,n+1, nw)]->Add(hf4[order(k,l,nz, nw)]);
				delete hf4[order(k,l,nz, nw)];
			}
		}
		//cout<<binIndx.size()<<endl;
		htmp[order(binIndx.size()-1,l,n+1, nw)] = hf4[order(nz-1, l , nz,nw)];//overflow
		htmp[order(binIndx.size()-1,l,n+1, nw)]->SetName(hname+Form("_%d_%d", int(binIndx.size()-1), l));
	}
	hf4.resize(binIndx.size()*nw);
	for(int i=0; i<htmp.size(); ++i) hf4[i]=htmp[i];
	zbin.clear();
	for(auto &it : binIndx) zbin.push_back(it);
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
		}
	}
	if(binIndx.size()<n+1){
		std::cout<<"error:binning can't be match exactly!"<<std::endl;
		return;
	}
	std::vector<TH2F* > htmp;
	htmp.resize((n+1)*nw);
	for(int l=0; l<nz; ++l){
		for(int j=0; j<binIndx.size()-1; ++j){
			htmp[order(l,j,nz, n+1)] = hf4[order(l, binIndx[j], nz, nw)];
			htmp[order(l,j,nz, n+1)]->SetName(hname+Form("_%d_%d",l, j));
			for(int k=binIndx[j]+1; k<binIndx[j+1]; ++k){
				htmp[order(l,j,nz, n+1)]->Add(hf4[order(l,k, nz, nw)]);
				delete hf4[order(l, k,nz, nw)];
			}
		}
		htmp[order(l, binIndx.size()-1,nz, n+1)] = hf4[order(l, nz-1, nz,nw)];//overflow
		htmp[order(l, binIndx.size()-1,nz, n+1)]->SetName(hname+Form("_%d_%d", l, int(binIndx.size()-1)));
	}
	hf4.resize(binIndx.size()*nz);
	for(int i=0; i<htmp.size(); ++i) hf4[i]=htmp[i];
	wbin.clear();
	for(auto &it : binIndx) wbin.push_back(it);
	nw = wbin.size();
}

int xthf4::Fill(float x, float y, float z, float w, float wit = 1){
	int jz = xAlgo::binarySearch(z, zbin, zbin.size()-1, 0); // size-1 is the overflow bin
	int jw = xAlgo::binarySearch(w, wbin, wbin.size()-1, 0); // size-1 is the overflow bin
	if(jz <0) jz = nz-1;
	if(jw <0) jw = nw-1;
	hist(jz, jw)->Fill(x,y,wit); 
	if(jz== nz-1 || jw == nw-1) return 1;
	else return 0;
}

void xthf4::Write(){
	for(auto &it: hf4){
		it->Write();
	}
}

int xthf4::Read(TString name, TFile *f , int nz1, float *zbin1, int nw1, float *wbin1){
	if(!isempty ) {
		std::cout<<"thf4 is filled by other histograms, load failed!"<<std::endl;
		return 0;
	}
	nz = nz1+1;
	nw = nw1+1;
	hf4.reserve(nz*nw);
	int nn=0;
	for(int j=0; j<nw; ++j){
		for(int i=0;i<nz; ++i){
			zbin.push_back(zbin1[i]);
			hf4.push_back((TH2F*) f->Get(name+Form("_%d_%d",i,j)));
			wbin.push_back(wbin1[j]);
			nn++;
		}
	}
	zbin.push_back(zbin1[nz]);
	wbin.push_back(wbin1[nw]);
	return nn;
}
/*
void h4(){
	float bin[] = {0,1,2,3,4,5};
	float zbin[] = {0,2,4,5, 6};
	float newzbin[] = {0,2,5, 6};
	TFile* f = TFile::Open("test.root");
	auto a = new xthf4();
	a->Read("test",f, 3, newzbin, 4, zbin);
	a->hist(1,2)->Draw("colz");
	//TFile* wf = TFile::Open("test.root","recreate");
	//auto b = new TH1F("b", "", 5,0,5);
	//b->SetName("bb");
	//b->Write();
	//xthf4* a = new xthf4("test", 5, bin, 5, bin ,4, zbin, 4, zbin);
	//a->Fill(3,2,7.4, 10);
	//a->Fill(3,2,4, 4);
	//a->Fill(3,2,3, 4);
	//a->RebinZ(3,newzbin);
	//a->Write();
}
*/
