#pragma once
#include <cmath>

#include <valarray>

#include "../../kipod/src/utils/mat.h"

using O_K = std::valarray<int>;
using O_K2 = std::valarray<O_K>;

class MinkowskiEmbedding
{
public:
    MinkowskiEmbedding(int D) : D(D), tau(sqrt(D)){}

    int D;
    float tau;

    O_K Bar(O_K x){
        return O_K{x[0],-x[1]};
    }

    O_K2 Bar(O_K2 v){
        return O_K2{Bar(v[0]),Bar(v[1])};
    }

    float Real(O_K x){
        return x[0]+x[1]*tau;
    }

    vec2 Real(O_K2 x){
        return vec2(Real(x[0]), Real(x[1]));
    }

//    vec2 R2(O_K x){
//        return vec2(Real(x), Real(Bar(x)));
//    }

    vec4 R4(O_K2 v){
        return vec4(Real(v),Real(Bar(v)));
    }


    mat4 Embedding(O_K2 e1={{1,0},{0,0}},
                            O_K2 e2={{0,1},{0,0}},
                            O_K2 e3={{0,0},{1,0}},
                            O_K2 e4={{0,0},{0,1}})
    {

        return transpose(mat4(R4(e1),R4(e2),R4(e3),R4(e4)));
    }
};
