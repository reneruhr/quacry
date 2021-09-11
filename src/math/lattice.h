#pragma once

template <typename BasisType>
class Lattice
{
public:
    Lattice(BasisType basis = BasisType(1)) :  basis_(basis){}

    BasisType GetBasis() { return basis_; }
    void BaseChange(const BasisType& new_basis) { basis_ = new_basis; }
private:
    BasisType basis_;
};

