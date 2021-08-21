#pragma once
#include "kipod.h"

namespace kipod::QuasiCrystals{
class PhysicalWindow
{
        std::vector< float > window_size_;
public:
    PhysicalWindow(): window_size_{-10,10,-10,10,-3,3,-3,3} {}
    PhysicalWindow(std::vector<float> window_size): window_size_(window_size) {}

    float operator[](int i){
        return window_size_[i];
    }

    float Left(){
        return (*this)[0];
    }
    float Right(){
        return (*this)[1];
    }
    float Bottom(){
        return (*this)[2];
    }
    float Top(){
        return (*this)[3];
    }
    float Near(){
        return (*this)[4];
    }
    float Far(){
        return (*this)[5];
    }
    float Near2(){
        return (*this)[6];
    }
    float Far2(){
        return (*this)[7];
    }

};

}
