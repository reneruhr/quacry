#include "penrose.h"


using std::vector;
using Eigen::Vector2i;
using Eigen::Vector2f;
using Eigen::Matrix2f;
using Eigen::Matrix4f;
using Eigen::Vector4f;
typedef vector<Vector4f, Eigen::aligned_allocator<Vector4f>> Vectors4;




	bool Penrose::inWindow(VectorXf& v){
		
		float eps = 0.01;
		bool res = insideHull( Vector2f{ v[2],v[3] }, hull) && (abs( fmod( sqrt(5)*v[4], 1 ) ) < eps);
		
		return res;
	}

	bool Penrose::notInWindow(VectorXf& v){
		
		return !inWindow(v);
	}

	void Penrose::make_sample(){
		
		initialize_sample(MatrixXf(g));
		//reduce_sample();
		this->reduce_sample_test();
		//for(auto& v: sample) std::cout << "...." << v << endl;
	}

	void Penrose::make_sample_with_edges(){
		
		initialize_sample_with_edges(MatrixXf(g));
		//reduce_sample();
		this->reduce_sample_with_edges();
		//for(auto& v: sample) std::cout << "...." << v << endl;
	}

	void Penrose::setupRender(){
		
			int reduced_dim = 2;
			
			GLfloat vertices_position[sample_num*reduced_dim];
			this->set_vertices_position(vertices_position, reduced_dim);
	
			initializeWithShaders(vao, vertices_position, sample_num, reduced_dim, "shaders/penrose_vert.shader.glsl", "shaders/penrose_frag.shader.glsl");

	}



	void Penrose::setupRenderWithEdges(){
		
			int reduced_dim = 2;
			
			GLfloat edges_position[sampleEdgesCount*reduced_dim*2];
			this->set_edges_position(edges_position, reduced_dim);
	
			initializeEdgesWithShaders(vao, edges_position, sampleEdgesCount*2, reduced_dim, "shaders/penroseEdges_vert.shader.glsl", "shaders/penrose_frag.shader.glsl");

	}

	void Penrose::setupRenderWithHull(){
		
			int reduced_dim = 2;
			
			GLfloat edges_position[sampleHullCount*reduced_dim];
			setEdges(hull, edges_position, reduced_dim);
	
			initializeHullWithShaders(vaoHull, edges_position, sampleHullCount, reduced_dim, "shaders/penroseHull_vert.shader.glsl", "shaders/penrose_frag.shader.glsl");

	}

	void Penrose::draw(float scale){
		
		setUniform(getProgram(), "scale", scale);

		glClear(GL_COLOR_BUFFER_BIT);
		display(vao, sample_num);	

	}

	void Penrose::drawEdges(float scale){
		
		setUniform(getProgram(), "scale", scale);

		glClear(GL_COLOR_BUFFER_BIT);
		displayEdges(vao, sampleEdgesCount);	

	}

	void Penrose::drawHull(float scale){
		
		setUniform(shaderList[0].m_programID, "scale", scale);

		glClear(GL_COLOR_BUFFER_BIT);
		displayHull(vaoHull, sampleHullCount);	

	}


void Penrose::reduce_sample_test(){
	
	//auto baseMethod = [this](VectorXf& v){ return Penrose::inWindow(v); };
	sample.erase( std::remove_if( sample.begin(), sample.end(), std::bind(&Penrose::inWindow, this, std::placeholders::_1) ), sample.end() );
	//sample.erase( std::remove_if( sample.begin(), sample.end(), baseMethod ), sample.end() );
	
	sample_num = sample.size();
	cout << " Updated Sample size to " << sample_num << endl;
} // reduce_sample

void Penrose::reduce_sample_with_edges(){
	

    auto iter = sample_container.latticeEdges.begin();
    auto endIter = sample_container.latticeEdges.end();
	for(; iter != endIter; ) {
    if (notInWindow(iter->second[0])) {
          iter = sample_container.latticeEdges.erase(iter);
     } else {
     	std::cout << "Test nbs of : " << iter->second[0] << std::endl ;

     		for(auto& v : iter->second){
			
				if(inWindow(v)) {
					std::cout << "The vector: "  << std::endl << v << std::endl << " is ";
					std::cout << " inside!"<< std::endl;
				}
     		}

			iter->second.erase( std::remove_if( iter->second.begin(), iter->second.end(), std::bind(&Penrose::notInWindow, this, std::placeholders::_1) ), iter->second.end() );
          	sampleEdgesCount+= iter->second.size();
          	std::cout << " The following survived: " << std::endl;
          	for(auto& v : iter->second) std::cout << v << std::endl;
          ++iter;
     }
}

	cout << " Updated Sample size to " << sampleEdgesCount << endl;
} // reduce_sample
