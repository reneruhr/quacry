#ifndef HULL_H
#define HULL_H

#include "quasi_math.h"
#include "sample.h"
#include <Eigen/Core>

#include "render.h"

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/convex_hull_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef K::Point_2 Point_2;
typedef std::vector<Point_2> Points;

using std::vector;
using Eigen::Vector2i;
using Eigen::Vector2f;
using Eigen::Vector3f;
using Eigen::Matrix2f;
using Eigen::Matrix3f;
using Eigen::Matrix4f;
using Eigen::Vector4f;
typedef vector<Vector4f, Eigen::aligned_allocator<Vector4f>> Vectors4;
typedef vector<Vector3f, Eigen::aligned_allocator<Vector3f>> Vectors3;
typedef Eigen::Vector<float, 5> Vector5f;
typedef Eigen::Matrix<float, 5, 5> Matrix5f;
typedef vector<Vector5f, Eigen::aligned_allocator<Vector5f>> Vectors5;
typedef vector<Vector2f, Eigen::aligned_allocator<Vector2f>> Vectors2;


float 	area2( Vector2f a, Vector2f b, Vector2f c);
bool 	left( Vector2f a, Vector2f b, Vector2f c);
bool 	leftOn( Vector2f a, Vector2f b, Vector2f c);
bool 	insideHull( Vector2f p, Vectors2& hull);


Vectors5 	unitCube5(Matrix5f& bas_mat, Vector5f& offset);

Vectors3 	projectTo345(Matrix5f& bas_mat, Vector5f& offset);


class 		Hull{

	public:
	Vectors2 makeHull2(Matrix5f& bas_mat, Vector5f& offset);

};


#endif