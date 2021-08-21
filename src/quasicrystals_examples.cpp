#include "quasicrystals_examples.h"
#include "math/minkowski_embedding.h"

namespace kipod::QuasiCrystals{

    QuasiCrystal AmmannBeenker()
    {
        Basis square_root_matrix = transpose(MinkowskiEmbedding(2).Embedding());

        Window octagon = kipod::Shapes::Shape( kipod::Shapes::Octagon(sqrt(2)) );

        Sample sample_range = {-20,20,-20,20,-10,10,-10,10};

        QuasiCrystal quacryOctagon = {square_root_matrix, octagon, sample_range};

        return quacryOctagon;
    }


}
