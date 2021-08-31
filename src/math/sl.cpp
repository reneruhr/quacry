#include "sl.h"

const char* EmbeddingName(SL2Embedding e){
    static bool constructed = false;
    static std::unordered_map<SL2Embedding, std::string> embedding_names;
    if(!constructed){
        embedding_names[TOP_LEFT] = "Top Left";
        embedding_names[OUTER] = "Outer";
        embedding_names[BOTTOM_RIGHT] = "Bottom Right";
        embedding_names[TOP_RIGHT] = "Top Right";
        embedding_names[BOTTOM_LEFT] = "Bottom Left";
        embedding_names[XZ] = "XZ";
        embedding_names[YZ] = "YZ";
        embedding_names[YW] = "YW";
        embedding_names[NOT_DEFINED] = "Embedding?";
        constructed = true;
    }
    return embedding_names[e].c_str();
}

void TopLeftCorner(Mat4& m4, const Mat2& m)
{
    m4[0][0] = m[0][0];
    m4[1][0] = m[1][0];
    m4[0][1] = m[0][1];
    m4[1][1] = m[1][1];
}

void BottomLeftCorner(Mat4& m4, const Mat2& m)
{
    m4[0+2][0] = m[0][0];
    m4[1+2][0] = m[1][0];
    m4[0+2][1] = m[0][1];
    m4[1+2][1] = m[1][1];
}

void TopRightCorner(Mat4& m4, const Mat2& m)
{
    m4[0][0+2] = m[0][0];
    m4[1][0+2] = m[1][0];
    m4[0][1+2] = m[0][1];
    m4[1][1+2] = m[1][1];
}

void BottomRightCorner(Mat4& m4, const Mat2& m)
{
    m4[0+2][0+2] = m[0][0];
    m4[1+2][0+2] = m[1][0];
    m4[0+2][1+2] = m[0][1];
    m4[1+2][1+2] = m[1][1];
}

Mat4 PermutationMatrix(const std::vector<int>& order)
{
    Mat4 id = SL4::i;
    Mat4 permutation_matrix;

    for(int i = 0; i<4; ++i)
        permutation_matrix[order[i]] = id[i];
    return permutation_matrix;
}

Mat4 SL2ToSL4(SL2InSL4 m2){
    Mat4 m = SL4::i;
    auto [type, embed, val] = m2;

    if (embed == TOP_RIGHT) {
        TopRightCorner(m, SL2::FromType(type, val)-SL2::i);
        return m;
    }
    if (embed == BOTTOM_LEFT){
        BottomLeftCorner(m, SL2::FromType(type, val)-SL2::i);
        return m;
    }
    if(embed == TOP_LEFT) {
        TopLeftCorner(m, SL2::FromType(type, val));
        return m;
    }
    if(embed == BOTTOM_RIGHT){
        BottomRightCorner(m, SL2::FromType(type, val));
        return m;
    }

    TopLeftCorner(m, SL2::FromType(type, val));
    std::vector<int> perm_vec = {0,1,2,3};

    if (embed == OUTER) perm_vec = std::vector<int>{0,3,2,1};
    else if (embed == BOTTOM_RIGHT) perm_vec = std::vector<int>{2,3,0,1};
    else if (embed == XZ) perm_vec = std::vector<int>{0,2,1,3};
    else if (embed == YZ) perm_vec = std::vector<int>{1,2,0,3};
    else if (embed == YW) perm_vec = std::vector<int>{1,3,2,0};

    Mat4 P = PermutationMatrix(perm_vec);
    m = P*m*Transpose(P);
    return m;
}

Mat2 SL2::FromType(const SL2Type type,float t)
{
    switch(type){
        case(DILATE):
            return Dilate(t);
        case(SHEAR_U):
            return Shear(t);
        case(SHEAR_L):
            return Shearl(t);
        case(ROTATE):
            return Rotate(t);
        default:
            return i;
    }
}

Mat2 SL2::Dilate(float t)
{
    Mat2 j = i;
    j[0][0] = exp(t);
    j[1][1] = exp(-t);
    return  j;
}

Mat2 SL2::Shear(float s)
{
    Mat2 j = i;
    j[1][0] = s;
    return  j;
}

Mat2 SL2::Shearl(float s)
{
    Mat2 j = i;
    j[0][1] = s;
    return  j;
}

Mat2 SL2::Rotate(float t)
{
    float angle = t*M_PI;
    Mat2 j = Mat2(cos(angle), -sin(angle),
                  sin(angle), cos(angle));
    return  j;
}

Mat4 SL4::DilateTopLeft(float t)
{
    Mat4 m = i;
    TopLeftCorner(m, SL2::Dilate(t));
    return m;
}

Mat4 SL4::ShearTopLeft(float t)
{
    Mat4 m = i; 
    TopLeftCorner(m, SL2::Shear(t));
    return m;
}

Mat4 SL4::ShearlTopLeft(float t)
{
    Mat4 m = i; 
    TopLeftCorner(m, SL2::Shearl(t));
    return m;
}

Mat4 SL4::RotateTopLeft(float t)
{
    Mat4 m = i; 
    TopLeftCorner(m, SL2::Rotate(t));
    return m;
}

MatrixWalk::MatrixWalk(vector<SL2InSL4> init_matrices)
{
    SL2_stack_= init_matrices;
    for(auto n: SL2_stack_) {
        SL4_stack_.push_back(SL2ToSL4(n));
        current_ = SL4_stack_.back()*current_;
    }
}

void MatrixWalk::Append(SL2InSL4 n)
{
    SL4_stack_.push_back(SL2ToSL4(n));
    SL2_stack_.push_back(n);
    current_ = SL4_stack_.back()*current_;
}

void MatrixWalk::Append(Mat4 m){
    SL4_stack_.push_back(m);
    SL2_stack_.push_back(std::make_tuple(DILATE, NOT_DEFINED, 0));
    current_ = m*current_;
}

void MatrixWalk::Append(Mat4 m, SL2InSL4 n)
{
    SL4_stack_.push_back(m);
    SL2_stack_.push_back(n);
    current_ = m*current_;
}

Mat4& MatrixWalk::Current()
{
    if(updated_ == false){
        current_ = SL4::i; 
        for(auto& m: SL4_stack_) current_ = m*current_;
        updated_ = true;
    }
    return current_;
}

void MatrixWalk::Modify(int i, SL2InSL4 n)
{
    assert(i < SL4_stack_.size());
    SL4_stack_[i] = SL2ToSL4(n);
    SL2_stack_[i] = n;
    updated_ = false;
}
