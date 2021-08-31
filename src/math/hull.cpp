#include "hull.h"


using std::vector;
using Eigen::Vector2i;
using Eigen::Vector2f;
using Eigen::Matrix2f;
using Eigen::Matrix4f;
using Eigen::Vector4f;
typedef vector<Vector4f, Eigen::aligned_allocator<Vector4f>> Vectors4;


	float area2( Vector2f a, Vector2f b, Vector2f c){
		Matrix3f tmp;
		tmp << a, 1, b, 1, c, 1;
		return tmp.determinant();
	}

	bool left( Vector2f a, Vector2f b, Vector2f c){
		return area2(a,b,c) > 0;
	}

	bool leftOn( Vector2f a, Vector2f b, Vector2f c){
		return area2(a,b,c) >= 0;
	}

	bool insideHull( Vector2f p, Vectors2& hull){
		
		bool res = true;
		
		for(Vectors2::iterator it = hull.begin(); it != hull.end()-1; ++it)	res = res && left(*it, *(it+1), p);
		res = res && left(*(hull.end()-1), *(hull.begin()), p);
	    return res;
	}



	Vectors5 unitCube5(Matrix5f& bas_mat, Vector5f& offset){
		
		Vectors5 unit_cube5 = { bas_mat*offset };
		for(int i=0; i<5; i++){
			for(auto& v: unit_cube5){
				unit_cube5.push_back(v + bas_mat.col(i)/2 );
				unit_cube5.push_back(v - bas_mat.col(i)/2 );
			}
		}
		unit_cube5.erase(unit_cube5.begin());
		return unit_cube5;
	}

	Vectors3 projectTo345(Matrix5f& bas_mat, Vector5f& offset){
		
		Vectors5 cube = unitCube5(bas_mat, offset);
		Vectors3 cube3;
		for(auto& w: cube){
			cube3.push_back( Vector3f{w[2], w[3], w[4]} );
			//std::cout << cube3.back() << std::endl << std::endl;
		}

		return cube3;
	}


	Vectors2 Hull::makeHull2(Matrix5f& bas_mat, Vector5f& offset){
		
		Vectors3 cube3 = projectTo345(bas_mat, offset);
		Points points, result;
		for(auto& w: cube3)		points.push_back( Point_2(w[0],w[1]) );
		
		// std::cout << "Using CGAL on projected cube: " << std::endl;
		// for(auto& w: cube3)		std::cout <<  Point_2(w[0],w[1]) <<  std::endl <<  std::endl;


		CGAL::convex_hull_2( points.begin(), points.end(), std::back_inserter(result) );

		Vectors2 output;

		for(auto& c: result)	output.push_back(	Vector2f{c.x(),c.y()}	) ;
	
		// std::cout << "Found the following vectors: " << std::endl;
		// for(auto& w: result)		std::cout <<  w <<  std::endl <<  std::endl;

	
		return output;
	}
