/// **triangles**
///
/// find star triangles in the image, and candidate stars that meet the constraints implicit within the triangle. these constraints are effectively the rules for candidate stars. for example, if a triangle consists of star pairs ab and bc, then the third side is ac.
///
#ifndef TRIPLETS_H
#define TRIPLETS_H
#include "globals.h"
#include "image.h"
#include "pairs_over_whole_sky.h"
#include <eigen/Core>
#include <unordered_map>

namespace rules {

class Triangles
{

public:

    Triangles(stars::Image& image, rules::PairsOverWholeSky& pairs,
              double tol_radius, int triMaxCnt);

    int identifyCentralStar();

private:

    /// *constrain side* the length of a triangle side is constrained by the lengths of the other two sides. star pairs in the side must have one member in constraint side a, and the other member in constraint side b.
    ///
    void constrainSide(std::unordered_multimap<int, int>& side,
                       const std::unordered_multimap<int, int>& cona,
                       const std::unordered_multimap<int, int>& conb);

    /// *find candidates using constrained side* candidate stars are members of pairs where the other member is in the constrained side. they also must be present in both sidea and sideb.
    ///
    std::unordered_map<int,int> findCansUsingConstrainedSide(const std::unordered_multimap<int, int>& con,
                                                             const std::unordered_multimap<int, int>& sidea,
                                                             const std::unordered_multimap<int, int>& sideb);

    rules::PairsOverWholeSky pairsOverWholeSky;
    stars::Image& image;
    double tol_radius;
    int triMaxCnt;
    int starsCnt;
    int triCur;
};

}

#endif
