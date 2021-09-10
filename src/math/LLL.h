#pragma once

#include <Eigen/Dense>

#include <iostream>
namespace latred{

template <typename Derived>
auto GramSchmidt(const Eigen::MatrixBase<Derived>& A) 
		-> std::pair<typename Derived::PlainObject,typename Derived::PlainObject>  
{
	typedef typename Derived::PlainObject AType;
	Eigen::HouseholderQR<AType> qr(A);
	return {qr.householderQ(), qr.matrixQR().template triangularView<Eigen::Upper>()};
}

template <typename Derived>
auto KAN(const Eigen::MatrixBase<Derived>& g)
  -> std::tuple<typename Derived::PlainObject, typename Derived::PlainObject, typename Derived::PlainObject>  
{
	typedef typename Derived::PlainObject Type;
	auto [k,u] = GramSchmidt(g);
	Type a= Type::Identity();
	for(int i = 0; i< g.rows(); ++i){
		a(i,i)=u(i,i);
		u.row(i)/=a(i,i);
		if(a(i,i)<0){
			a(i,i) = -a(i,i);
			k.col(i) = - k.col(i);
		}
	}
 return {k,a,u};
}

template <typename Derived>
auto PN(const Eigen::MatrixBase<Derived>& g)
  -> std::pair<typename Derived::PlainObject, typename Derived::PlainObject>  
{
	typedef typename Derived::PlainObject Type;
	typedef typename Derived::Scalar Scalar;
	auto [k,u] = GramSchmidt(g);
	for(int i = 0; i< g.rows(); ++i){
		Scalar a = u(i,i);
		u.row(i)/= a;
		k.col(i)*=a;
	}
 return {k,u};
}

template <typename Derived>
bool IsWeaklyReduced(const Eigen::MatrixBase<Derived>& A)
{
	typedef typename Derived::PlainObject Type;
	auto [k,a,n] = KAN(A);
	return (n-Type::Identity()).minCoeff()>= -.5 && (n-Type::Identity()).maxCoeff() <= 0.5;
}

template <typename Derived>
bool IsColumnReduced(const Eigen::MatrixBase<Derived>& A, double theta)
{
	auto [p,n] = PN(A);
	for(int i = 0; i<A.rows()-1; ++i){
		if( theta * p.col(i).squaredNorm() > p.col(i+1).squaredNorm())
			return false;
	}
	return true;
}

template <typename Derived>
auto ColumnReduce(const Eigen::MatrixBase<Derived>& A, double theta)
{
	typedef typename Derived::PlainObject Type;
	
	auto [p,n] = PN(A);
	Type a_red = A;
	for(int i = 0; i<A.rows()-1; ++i){
		if(theta * p.col(i).squaredNorm() > p.col(i+1).squaredNorm()){
			a_red.col(i).swap(a_red.col(i+1));
			break;
		}
	}
	return a_red;
}

template <typename Derived>
void ColumnReduceInPlace(Eigen::MatrixBase<Derived>& A, double theta)
{
	typedef typename Derived::PlainObject Type;
	
	auto [p,n] = PN(A);
	for(int i = 0; i<A.rows()-1; ++i){
		if( theta * p.col(i).squaredNorm() > p.col(i+1).squaredNorm()){
			A.col(i).swap(A.col(i+1));
			break;
		}
	}
}

template <typename Derived>
auto WeakReduce(Eigen::MatrixBase<Derived>& A)
{
	int N = A.rows();
	auto [p,n] = PN(A);
	typedef typename Derived::PlainObject Type;
	Type a_red = A;
  for(int i = 1; i < N; ++i)
		for(int j = i-1; j>=0; --j){
			a_red.col(i) -= round(n(j,i))*a_red.col(j);
		}
	return a_red;
}

template <typename Derived>
auto LLL(const Eigen::MatrixBase<Derived>& A) 
		-> typename Derived::PlainObject
{
	typedef typename Derived::PlainObject Type;
	Type a_red = A;
	int N = A.rows();
	double theta = 0.5;

	for(bool reduced = false; !reduced; ){
		auto [p,n] = PN(a_red);
		for(int i = 1; i < N; ++i)
			for(int j = i-1; j>=0; --j)
				a_red.col(i) -= round(n(j,i))*a_red.col(j);

		reduced = true;
		for(int i = 0; i<N-1; ++i)
			if( theta * p.col(i).squaredNorm() > p.col(i+1).squaredNorm()){
				a_red.col(i).swap(a_red.col(i+1));
				reduced = false;
				break;
			}
	}
	return a_red;
}
}

