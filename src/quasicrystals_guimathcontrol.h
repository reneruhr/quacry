#pragma once
#include "kipod.h"
#include "math/lattice.h"
#include "math/sl.h"
#include "Eigen/Dense"

using std::vector;
using std::string;

typedef glm::mat4 Mat4;
typedef glm::mat2 Mat2;
typedef glm::vec4 Vec4;
typedef glm::vec2 Vec2;

typedef vector<Vec4> Vectors4;
typedef vector<Mat4> Matrices4;
typedef vector<Vec2> Vectors2;

using Mat5f = Eigen::Matrix<float,5,5>;
using Vec5f = Eigen::Matrix<float,5,1>;

void DrawColumnMatrix4(const Mat4& m);
void DrawRowMatrix4(const Mat4& m);
void DrawMatrix5(const Mat5f& m);

struct SampleSize4
        {
            int x,y,z,w;
            SampleSize4(int x,int y,int z,int w):x(x),y(y),z(z),w(w){}
        };

enum mode {
       NEW_SAMPLE,
       IDLE
   };

void EmbeddingMap(std::array<int, 16> &selected, SL2Embedding &currEmbedding);
void EmbeddingMapReverse(std::array<int, 16> &selected, SL2Embedding &currEmbedding);
bool SL2Control(Mat4& current_transform, MatrixWalk &SL4walk, SL2Embedding currEmbedding);
void SL4Control(MatrixWalk &SL4walk);
bool DrawSL2Matrix(float& s, SL2Type sl2type, int id);
void DrawEmbeddings(std::array<int, 16> &selected, SL2Embedding &currEmbedding);
void EmbeddingsView(std::array<int, 16> &selected);
void EmbeddingsView5(std::array<int, 16> &selected);
void MatrixWalkCandidate(MatrixWalk&);

auto Mat4ToEigen5(Mat4) -> Mat5f;

