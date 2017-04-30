

#ifndef GRADOPT_H
#define GRADOPT_H

class gradOpt {
	public:
		std::vector<float> *grad;
		std::vector<float> *axis;
		TF1* ff;
		int nDim;
		int maxIteration = 10000;
		double convergeCut = 0.000001;
		bool changeSign;

		gradOpt(int ndim){
			grad = new vector<float>();
			axis = new vector<float>();
			grad->reserve(ndim);
			axis->reserve(ndim);
			nDim = ndim;
			for(int i=0;i<ndim;++i ) grad->push_back(0);
		}
		float updateGrad(float step);
		float func(std::vector<float>*axis);
		double getGrad();
		float findMaximum();
};

float gradOpt::updateGrad(float step){
	for(auto i=0;i<axis->size();++i){
		axis->at(i)-=step*grad->at(i);
	}
	return 1;
}

double gradOpt::getGrad(){
	changeSign = false;
	double epsilon = 0.00001;
	double tmp;
	double norm=0;
	for(auto i=0;i<grad->size();++i){
		float f1= func(axis);
		axis->at(i)+=epsilon;
		float f2= func(axis);
		axis->at(i)-=epsilon;
		tmp = (f2-f1)/epsilon;
		if(tmp*grad->at(i) <0) changeSign=true;
		grad->at(i)=tmp;
		norm+= pow(tmp,2);
	}
	return pow(norm,0.5);
}

float gradOpt::findMaximum(){
	double f1, f0 = func(axis);
	double step=1;
	double length;
	for(int n=0;n<maxIteration;++n){
		length = getGrad();
		if(changeSign) step = step/2;
		updateGrad(step);
		f1 = func(axis);
		if(length<convergeCut) return f1; 
	}
	return f1;
}

float gradOpt::func(std::vector<float>*axis){
	return (float)ff->Eval(axis->at(0),axis->at(1));
}

#endif
