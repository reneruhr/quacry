#pragma once
//#include <Eigen/Dense>
//#include <Eigen/StdVector>
//#include <Eigen/Geometry>
//#include <Eigen/QR>
#include <glm/matrix.hpp>
#include <vector>
#include <string>
#include <algorithm>
#include <unordered_map>

using std::vector;
using std::string;

typedef glm::mat4 Mat4;
typedef glm::mat2 Mat2;
typedef glm::vec4 Vec4;
typedef glm::vec2 Vec2;

typedef vector<Vec4> Vectors4;
typedef vector<Mat4> Matrices4;
typedef vector<Vec2> Vectors2;

inline Mat4 Transpose (const Mat4& m){ return glm::transpose(m); }

enum SL2Embedding {
    TOP_LEFT, OUTER, BOTTOM_RIGHT, TOP_RIGHT, BOTTOM_LEFT,
    XZ, YZ, YW,
    NOT_DEFINED = -1
};

enum SL2Type {
    DILATE, SHEAR_U, SHEAR_L, ROTATE
};

typedef std::tuple<SL2Type, SL2Embedding, float>  SL2InSL4;

const char* EmbeddingName(SL2Embedding e);
Mat4 SL2ToSL4(SL2InSL4);

struct EmbeddingDescription {
    SL2Embedding embed;
    int pos[4];
    std::string name;

    EmbeddingDescription(SL2Embedding em, int pos_in[], std::string name_in)
        : embed(em), name(name_in)
    {
        for(int i = 0; i < 4; ++i) pos[i] = pos_in[i];
    }
};

class SL2{
    public:
        constexpr static Mat2 i = Mat2(1.0f);

        static Mat2 Dilate(float t);
        static Mat2 Shear(float s);
        static Mat2 Shearl(float s);
        static Mat2 Rotate(float t);

        static Mat2 FromType(const SL2Type type, float t);
};

class SL4{
    public:
        constexpr static Mat4 i = Mat4(1.0f);

        static Mat4 DilateTopLeft(float t);
        static Mat4 ShearTopLeft(float t);
        static Mat4 ShearlTopLeft(float t);
        static Mat4 RotateTopLeft(float t);
};

class MatrixWalk{
    public:
        MatrixWalk() {}
        MatrixWalk(vector<SL2InSL4> init_matrices);

        vector<SL2InSL4> SL2_stack_ = {};
        Matrices4 SL4_stack_ = {};
        Mat4 current_ = SL4::i;
        Mat4 candidate_ = current_;

        void Append(Mat4 m);
        void Append(Mat4 m, SL2InSL4 n);
        void Append(SL2InSL4 n);
        Mat4& Current();
        void Modify(int i, SL2InSL4 n);
        Mat4& Candidate(){ return candidate_;}

    private:
        bool updated_ = true;
};
