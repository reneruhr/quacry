#ifndef PENROSE_H
#define PENROSE_H

#include "quasi_math.h"
#include "sample.h"
#include "hull.h"
#include "minkowski_embedding.h"
#include <Eigen/Core>

#include "render.h"


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



class Penrose: public SampleGeneric<5>, public RenderGeneric, public Hull{
public:
	Eigen::Matrix<float, 5, 5> g;
	Eigen::Vector<float, 5> gamma;
	Vectors2 hull;
	const float u = 2.0*M_PI/5;
	int samplesize[5] = {5, 5, 1, 1, 1};

	int sampleEdgesCount = 0;
	int sampleHullCount = 0;

	GLuint vaoHull;
	
	Penrose(): SampleGeneric<5>(samplesize){
		
		for(int j = 0; j<5; ++j){
			g.col(j) = sqrt(2.0/5)*Eigen::Vector<float, 5>{cos(j*u), sin(j*u), cos(j*2*u), sin(j*2*u), pow(2.0,-0.5)};
		}

		std::cout << "Choice of g: " << std::endl  << g << std::endl;


		gamma << 0.12, -0.12, 0.143, 0.132, (0.12-0.12+0.143+0.132)*(-1)+0.5;
		std::cout << "Choice of gamma: " << std::endl  << gamma << std::endl;

		hull = makeHull2(g, gamma);
		sampleHullCount = hull.size();
	}


	bool inWindow(VectorXf& v);
	bool notInWindow(VectorXf& v);


	void reduce_sample_test();
	void make_sample();
	void make_sample_with_edges();
	void reduce_sample_with_edges();

	void setupRender();
	void setupRenderWithEdges();
	void setupRenderWithHull();

	void draw(float scale);
	void drawEdges(float scale);
	void drawHull(float scale);

};



#endif