#pragma once
#include "kipod.h"
#include "math/lattice.h"
#include "math/sl.h"

using std::vector;
using std::string;

typedef glm::mat4 Mat4;
typedef glm::mat2 Mat2;
typedef glm::vec4 Vec4;
typedef glm::vec2 Vec2;

typedef vector<Vec4> Vectors4;
typedef vector<Mat4> Matrices4;
typedef vector<Vec2> Vectors2;

void DrawColumnMatrix4(const Mat4& m);
void DrawRowMatrix4(const Mat4& m);

struct SampleSize
        {
            int x,y,z,w;
            SampleSize(int x,int y,int z,int w):x(x),y(y),z(z),w(w){}
        };

enum mode {
       NEW_SAMPLE,
       IDLE
   };

void EmbeddingMap(std::array<int, 16> &selected, SL2Embedding &currEmbedding);
void EmbeddingMapReverse(std::array<int, 16> &selected, SL2Embedding &currEmbedding);
void SL2Control(Mat4& current_transform, MatrixWalk &SL4walk, SL2Embedding currEmbedding);
void SL4Control(MatrixWalk &SL4walk);
bool DrawSL2Matrix(float& s, SL2Type sl2type, int id);
void DrawEmbeddings(std::array<int, 16> &selected, SL2Embedding &currEmbedding);
void EmbeddingsView(std::array<int, 16> &selected);
void MatrixWalkCandidate(MatrixWalk&);

void matrix(float* f, int size);
void SetMat4(Mat4& m);
void matrix4i(int* f);
void mat4selectable(string* b, std::array<int, 16> &selected);
void SetMat2(float* f);
void SL4matrix(Mat4& m);

void verify4i(SampleSize &sampleSize, mode &currentMode);
