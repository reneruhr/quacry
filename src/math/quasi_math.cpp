#include "quasi_math.h"


using std::cout;
using std::endl;

using std::string;
using std::to_string;

using std::vector;
using std::map;

using std::begin;
using std::end;
using std::for_each;
using std::find_if;

using Eigen::Matrix2f;
using Eigen::Matrix4f;
using Eigen::Vector4f;
using Eigen::VectorXi;
using Eigen::Rotation2D;
using Eigen::HouseholderQR;


typedef vector<Vector4f, Eigen::aligned_allocator<Vector4f>> Vectors4;



bool isLagrangeReduced(const Vector4f& b1, const Vector4f& b2){
	return (b1.norm()<=b2.norm() && abs(b1.dot(b2)/b1.squaredNorm())<=0.5);
}

void GaussReduction(Vector4f& b1, Vector4f& b2){
	if(b1.norm()<b2.norm())	b1.swap(b2);
	do{
		b1.swap(b2);		
		b2-=floor(b1.dot(b2)/b1.squaredNorm())*b1;
	}while(b1.norm()>b2.norm());
}

void GramSchmidt(const Matrix4f& A, Matrix4f& Q, Matrix4f& R){
	HouseholderQR<Matrix4f> qr(A);
	Q = qr.householderQ();
	R = qr.matrixQR().triangularView<Eigen::Upper>();
}

void KAU(const Matrix4f& A, Matrix4f& k, Matrix4f& a, Matrix4f& u){
	GramSchmidt(A,k,u);
	a= Matrix4f::Identity();
	for(int i = 0;i<4;++i){
		a(i,i)=u(i,i);
		u.row(i)/=a(i,i);
	}
}

void GramSchmidtOrth(const Matrix4f& A, Matrix4f& Astar, Matrix4f& Mu){
	Matrix4f a;
	KAU(A, Astar, a, Mu);
	Astar*=a;
}

bool isWeaklyReduced(const Matrix4f& A){
	Matrix4f Astar, Mu;
	GramSchmidtOrth(A, Astar, Mu);
	bool mu = true;
	
	for(int j =0; j<4; ++j){
		for(int i = 0; i<j; ++i){
			mu= mu && (abs(Mu(i,j))<=0.5);
		}
	}
	return mu;
}

bool iscReduced(const Matrix4f& A, float c){
	Matrix4f Astar, Mu;
	GramSchmidtOrth(A, Astar, Mu);
	bool red = true;
	for(int i = 0; i<3; ++i){
		red = red && 
			(Astar.col(i).squaredNorm()<= c*Astar.col(i+1).squaredNorm())
			;
	}
	return red;
}

void cReduce(const Matrix4f& A, Matrix4f& Ared, float c){
	Matrix4f Astar, Mu;
	
	GramSchmidtOrth(A, Astar, Mu);
	Ared = A;
	for(int i = 0; i<3; ++i){
		if(Astar.col(i).squaredNorm()> c*Astar.col(i+1).squaredNorm()){
			Ared.col(i).swap(Ared.col(i+1));
			return;
		}
	}
}


void WeakReduction(const Matrix4f& A, Matrix4f& Ared){
	Ared = A;
	while(!isWeaklyReduced(Ared)){
		Matrix4f Astar, Mu;
		GramSchmidtOrth(Ared, Astar, Mu);

		std::map<string, float> mumap;
		for(int j =0; j<4; ++j){
			for(int i = 0; i<j; ++i){
				mumap[to_string(j)+' '+to_string(i)] = Mu(i,j);
			}
		}
		auto it = find_if(
			begin(mumap), end(mumap), 
				[&](const std::pair<string,float> &mu){ return (abs(mu.second)>0.5);}
					);

		if(it!=end(mumap)){
			cout << it->first << " val: " << it->second << endl;
			std::stringstream ss;
			ss << it->first;
			int i,j,c;
			ss >> j >> i;
			c = round(it->second);
			Ared.col(j)=Ared.col(j)-c*Ared.col(i);
		}else
			return;
	}
}

void WeakReductionWithMemory(const Matrix4f& A, Matrix4f& Ared, Matrix4f& u){
	Ared = A;
	u = Matrix4f::Identity();
	while(!isWeaklyReduced(Ared)){
		Matrix4f Astar, Mu;
		GramSchmidtOrth(Ared, Astar, Mu);

		std::map<string, float> mumap;
		for(int j =0; j<4; ++j){
			for(int i = 0; i<j; ++i){
				mumap[to_string(j)+' '+to_string(i)] = Mu(i,j);
			}
		}
		auto it = find_if(
			begin(mumap), end(mumap), 
				[&](const std::pair<string,float> &mu){ return (abs(mu.second)>0.5);}
					);

		if(it!=end(mumap)){
			cout << it->first << " val: " << it->second << endl;
			std::stringstream ss;
			ss << it->first;
			int i,j,c;
			ss >> j >> i;
			c = round(it->second);
			Ared.col(j)=Ared.col(j)-c*Ared.col(i);
			Matrix4f n = Matrix4f::Identity();
			n(i,j)=-c;
			u*= n;
		}else
			return;
	}
}

// p246 of Fundamental Problems of Algorithmic Algebra by Chee Keng Yap
void LLL(const Matrix4f& A, Matrix4f& Ared){
	float c = 4/3+0.0001;
	WeakReduction(A, Ared);
	while(!(isWeaklyReduced(Ared) && iscReduced(Ared,c)) ){
		Matrix4f Atemp = Ared;
		cReduce(Ared, Atemp, c);
		Matrix4f Atemp2 = Atemp;
		WeakReduction(Atemp, Atemp2);
		Ared = Atemp2;
	}
}

int quo(const Vector4f& a, const Vector4f& b){
	return floor(a.dot(b)/a.squaredNorm());
}

Vector4f rem(const Vector4f& a, const Vector4f& b){
	return b-quo(a,b)*a;
}



