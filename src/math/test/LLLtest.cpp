#define CATCH_CONFIG_MAIN
#include "catch2/catch_amalgamated.hpp"

#include "Eigen/Dense"
#include "../LLL.h"
using namespace Eigen;
using namespace latred;


#include <iostream>

TEST_CASE("Catch2 works","[Catch]")
{
    REQUIRE(1 == 1);
}

TEST_CASE( "Finds Eigen and create a Matrix", "[Found and Works]" )
{
    Matrix2d mat;
    mat << 2,0,0,3;

    REQUIRE( mat(0,0) == 2 );
    REQUIRE( mat(1,1) == 3 );
}

TEST_CASE("Gram Schmidt", "Gram Schmidt" )
{
    using Mat3 = Matrix3d;
    Mat3 A;
    A << 1,1,0,
         1,0,1,
         0,1,1;
    auto [k,U] = GramSchmidt(A);

    REQUIRE( (k*U-A).squaredNorm()<0.000000001 );
    REQUIRE( (k*k.transpose()-Mat3::Identity()).squaredNorm()<0.0001 );
}

TEST_CASE("KAN decomposition", "KAN")
{
    using Mat2 = Matrix2d;
    Mat2 g;
    g << 3,2,1,1;
    auto [k,a,u] = KAN(g);

    REQUIRE( (k*a*u-g).squaredNorm()<0.000000001 );

    Mat2 h,l,b,n;
    l << 0,-1,1,0;
    b << 2.,0,0,1./2.;
    n << 1,10,0,1;
    h = l*b*n;  
    
    std::tie(k,a,u) = KAN(h);
    auto IsSame = [](auto a, auto b){ return (a-b).squaredNorm()<0.0000001; };
    REQUIRE( ( IsSame(k,l) && IsSame(a,b) && IsSame(u,n) ) ); 
}

TEST_CASE("isApprox","")
{
    Matrix2d a; a << 2,0,1,0;
    Matrix2d b = a+0.00001*Matrix2d::Identity();
    REQUIRE( !a.isApprox(b,0.00000001) );
    REQUIRE( !a.isApprox(b) );
}

TEST_CASE("PN decomposition", "PN")
{
    using Mat2 = Matrix2d;
    Mat2 g;
    g << 3,2,1,1;
    auto [p,u] = PN(g);

    REQUIRE( g.isApprox(p*u, 0.0000001));

    Mat2 h,l,b,n;
    l << 0,-1,1,0;
    b << 2.,0,0,1./2.;
    n << 1,10,0,1;
    h = l*b*n;  
    
    std::tie(p,u) = PN(h);
    REQUIRE(u.isApprox(n,0.000001));
    REQUIRE(p.isApprox(l*b,0.000001)); 
}

TEST_CASE("IsWeaklyReduced", "IsWeaklyReduced")
{
    using Mat3 = Matrix3d;

    Mat3 a,b;
    a << 1,0.5,0.4,  0,1,-0.2,  0,0,1;
    b << 1,0.6,0,  0,1,0,  0,0,1;

    REQUIRE( IsWeaklyReduced(a));
    REQUIRE( !IsWeaklyReduced(b));
}

TEST_CASE("ColumnReduce", "")
{
    using Mat3 = Matrix3d;

    Mat3 a,b,c, d;
    a << 2,0,0,0,1,0,0,0,1;
    d = a; d.col(0).swap(d.col(1));
    b = ColumnReduce(a, 1./5);
    c = ColumnReduce(a, 1./1.1);
    REQUIRE( IsColumnReduced(a, 0.2));
    REQUIRE( !IsColumnReduced(a, 0.9));
    REQUIRE( b.isApprox(a));
    REQUIRE( c.isApprox(d));

    ColumnReduceInPlace(a,1./1.1);
    REQUIRE( a.isApprox(d));
}

TEST_CASE("Weak Reduce", "")
{
    using Mat3 = Matrix3d;

    Mat3 a,b;
    a << 0,0,1,
         0,1,0,
         1,0,0;
    b << 2,10,4,
         0,0.5,3,
         0,0,1;
    auto c = a*b;
    REQUIRE( !IsWeaklyReduced(c) );
    auto d = WeakReduce(b);
    REQUIRE( IsWeaklyReduced(d) );
}

TEST_CASE("LLL", "")
{
   Matrix3d a,b,b2;
   a << 1,-1,3,
        1,0,5,
        1,2,6;

   b << 0,1,-1,
        1,0,0,
        0,1,2;

  b2 << 0,1,-2,
        1,0,0,
        0,1,1;  

   auto c = LLL(a);

   REQUIRE((c.isApprox(b) || c.isApprox(b2)));
}
