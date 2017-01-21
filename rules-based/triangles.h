#ifndef TRIPLETS_H
#define TRIPLETS_H

#include "image.h"
#include "pairs_over_whole_sky.h"
#include <eigen/Core>

namespace rules {

class Triangles
{

public:

    Triangles(stars::Image& image,
              rules::PairsOverWholeSky& pairs,
              double triTol,
              int triMaxCnt);

    int identifyCentralStar();

private:

    Eigen::Matrix<int, 1000, 2> findRelatedPairs(Eigen::Matrix<int, 1000, 2>&, Eigen::Matrix<int, 1000, 2>&);

    Eigen::Matrix<int, 1000, 1> findCandidates(Eigen::Matrix<int, 1000, 2>&, Eigen::Matrix<int, 1000, 2>&);

    bool isPairNew(int, int, Eigen::Matrix<int, 1000, 2>&);

    rules::PairsOverWholeSky pairsOverWholeSky;
    stars::Image& image;
    double triTol;
    int triMaxCnt;
    int starsCnt;
    int triCur;
};

}

#endif
