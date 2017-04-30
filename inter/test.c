
#include "gradOpt.h"

void test(){
	//TF2 *f2 = new TF2("f2","sin(x)*sin(y)/(x*y)",0,5,0,5);
	TF2 *f2 = new TF2("f2","x*x+y*y",-5,5,-5,5);
	f2->Draw("surf2");
	gradOpt* g = new gradOpt(2);
	g->ff=f2;
	g->axis->push_back(2);
	g->axis->push_back(3);
	cout<<g->findMaximum()<<endl;
	cout<<"x= "<<g->axis->at(0)<<"; y= "<<g->axis->at(1)<<endl;
	return;
}
