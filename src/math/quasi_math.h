
#ifndef QUASI_MATH_H
#define QUASI_MATH_H

#include <GL/glew.h>

#include <cmath>

#include <string>
#include <sstream>

#include <iostream>
#include <vector>
#include <map>
#include <algorithm>

//#include <ranges>

#include <Eigen/Dense>
#include <Eigen/StdVector>
#include <Eigen/Geometry>
#include <Eigen/QR>


using std::vector;
using Eigen::Matrix2f;
using Eigen::Matrix4f;
using Eigen::Vector4f;
typedef vector<Vector4f, Eigen::aligned_allocator<Vector4f>> Vectors4;

static const Vector4f zero = Vector4f::Zero();


bool isLagrangeReduced(const Vector4f& b1, const Vector4f& b2);

void GaussReduction(Vector4f& b1, Vector4f& b2);

void GramSchmidt(const Matrix4f& A, Matrix4f& Q, Matrix4f& R);

void KAU(const Matrix4f& A, Matrix4f& k, Matrix4f& a, Matrix4f& u);

void GramSchmidtOrth(const Matrix4f& A, Matrix4f& Astar, Matrix4f& Mu);

bool isWeaklyReduced(const Matrix4f& A);

bool iscReduced(const Matrix4f& A, float c);

void cReduce(const Matrix4f& A, Matrix4f& Ared, float c);


void WeakReduction(const Matrix4f& A, Matrix4f& Ared);

void WeakReductionWithMemory(const Matrix4f& A, Matrix4f& Ared, Matrix4f& u);

void LLL(const Matrix4f& A, Matrix4f& Ared);

int quo(const Vector4f& a, const Vector4f& b);

Vector4f rem(const Vector4f& a, const Vector4f& b);

//}
#endif