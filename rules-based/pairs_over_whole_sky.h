#ifndef PAIRS_H
#define PAIRS_H

#include "sensor.h"
#include "float_int_table.h"

#include <unordered_map>
#include <cereal/access.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/tuple.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/unordered_map.hpp>

namespace rules {

class PairsOverWholeSky {

public:

    void init(stars::Sensor &sensor);

    std::vector<int> starsFromPairs(double angle, double tolerance);

    void Status();

private:

    util::FloatIntTable angletable; // angle, starpairs ndx

    std::vector<std::tuple<double, int, int>> starpairs; // angle, catndx1, catndx2

    std::unordered_map<std::string, int> starpairs_map; // starpairkey, starpairsndx

    std::string pairsKey(int catndx1, int catndx2); // hash key

    friend class cereal::access;
    template <class Archive> void serialize(Archive& ar) {
        ar(starpairs, starpairs_map);
    }
};

}

#endif
