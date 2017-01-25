#include "triangles.h"

rules::Triangles::Triangles(stars::Image& image,
                            rules::PairsOverWholeSky& pairs,
                            double triTol,
                            int triMaxCnt) :
    pairsOverWholeSky(pairs),
    image(image),
    tol_radius(triTol),
    triMaxCnt(triMaxCnt),
    starsCnt(image.uvecs.n_rows),
    triCur(0)
{}

int rules::Triangles::identifyCentralStar() {
    using namespace Eigen;
    arma::mat mata;
    arma::mat matb;
    arma::mat matc;
    mata.zeros(triMaxCnt,3);
    matb.zeros(triMaxCnt,3);
    matc.zeros(triMaxCnt,3);

    std::unordered_map<int, int> cans;
    int i, j, k, dj, dk;
    triCur = 0;
    for (dj = 1; dj <= starsCnt-2; ++dj) {
        for (dk = 1; dk <= starsCnt-dj-1; ++dk) {
            for (i = 1; i <= starsCnt-dj-dk; ++i) {
                if (i != 1) continue; // center star
                j = i + dj;
                k = j + dk;

                double angab = std::acos( arma::dot( arma::trans( image.uvecs.row(i-1) ) , arma::trans( image.uvecs.row(j-1) ) ) );
                double angac = std::acos( arma::dot( arma::trans( image.uvecs.row(i-1) ) , arma::trans( image.uvecs.row(k-1) ) ) );
                double angbc = std::acos( arma::dot( arma::trans( image.uvecs.row(j-1) ) , arma::trans( image.uvecs.row(k-1) ) ) );
                if (angab >= stars::imageRadiusRadians) continue;
                if (angac >= stars::imageRadiusRadians) continue;
                if (angbc >= stars::imageRadiusRadians) continue;
                if ( std::abs( angab - angac ) < 2.0 * tol_radius ) continue;
                if ( std::abs( angab - angbc ) < 2.0 * tol_radius ) continue;
                if ( std::abs( angac - angbc ) < 2.0 * tol_radius ) continue;
                std::unordered_multimap<int, int> ab = pairsOverWholeSky.pairsMap(angab, tol_radius);
                std::unordered_multimap<int, int> ac = pairsOverWholeSky.pairsMap(angac, tol_radius);
                std::unordered_multimap<int, int> bc = pairsOverWholeSky.pairsMap(angbc, tol_radius);
                std::unordered_map<int, int> cansab = matchMaps(ab, bc);
                std::unordered_map<int, int> cansac = matchMaps(ac, bc);
                std::unordered_map<int, int> cansmerge;
                for (auto itab = cansab.begin(), end = cansab.end(); itab != end; ++itab) {
                    auto itac = cansac.find(itab->first);
                    if (itac != cansac.end()) {
                        cansmerge.emplace(itab->first, itab->second + itac->second);
                    }
                }
                for (auto itmerge = cansmerge.begin(), end = cansmerge.end(); itmerge != end; ++itmerge) {
                    auto itcan = cans.find(itmerge->first);
                    if (itcan == cans.end()) {
                        cans.emplace(itmerge->first, itmerge->second);
                    } else {
                        itcan->second += itmerge->second;
                    }
                }

                ++triCur;
                if (triCur == triMaxCnt-1) break;
            }
            if (triCur == triMaxCnt-1) break;
        }
        if (triCur == triMaxCnt-1) break;
    }
    int starndx = 0;
    int maxcnt = 0;
    for (auto itcan = cans.begin(), end = cans.end(); itcan != end; ++itcan) {
        if (itcan->second > maxcnt) {
            starndx = itcan->first;
            maxcnt = itcan->second;
        }
    }
    return starndx;
}

std::unordered_map<int,int> rules::Triangles::matchMaps(std::unordered_multimap<int, int>& ab,
                                                        std::unordered_multimap<int, int>& bc) {
    std::unordered_map<int, int> cans;
    for (auto itab = ab.begin(), end = ab.end(); itab != end; ++itab) {
        auto itbc = bc.find(itab->first);
        if (itbc != bc.end() && itbc->second != itab->second) {
            int can = itab->second;
            auto itcans = cans.find(can);
            if (itcans != cans.end()) {
                ++itcans->second;
                continue;
            }
            cans.emplace(can, 1);
        }
    }
    return cans;
}

Eigen::Matrix<int, Eigen::Dynamic, 1> rules::Triangles::findCans(Eigen::Matrix<int, Eigen::Dynamic, 2>& ab,
                                                                 std::unordered_multimap<int, int>& bc) {
    using namespace Eigen;
    Matrix<int, Dynamic, 1> cans;
    cans.resize(1000,1);
    cans.setZero();
    int cansndx = 0;
    bool okab = false; // ok to break on 0
    for (int ndxab = 0; ndxab < ab.rows(); ++ndxab) {
        if (okab && ab(ndxab,0) == 0) break;
        if (!okab && ab(ndxab,0) > 0) okab = true;

        int can = 0;
        auto it1 = bc.find(ab(ndxab,0));
        auto it2 = bc.find(ab(ndxab,1));
        if (it1 != bc.end() && it2 == bc.end()) {
            can = ab(ndxab,1);
        }
        else if (it1 == bc.end() && it2 != bc.end()) {
            can = ab(ndxab,0);
        }
        else continue;

        if (isCanNew(can, cans)) {
            if (cans.rows() < cansndx+1) {
                cans.conservativeResize(cans.rows()+1000, cans.cols());
                for (int i = cans.rows() - 1000; i < cans.rows(); ++i) {
                    cans(i,0) = 0;
                }
            }
            cans(cansndx,0) = can;
            ++cansndx;
        }
    }
    return cans;
}

Eigen::Matrix<int, Eigen::Dynamic, 1> rules::Triangles::findCans(Eigen::Matrix<int, Eigen::Dynamic, 2>& ab,
                                                                 Eigen::Matrix<int, Eigen::Dynamic, 2>& bc) {
    using namespace Eigen;
    Matrix<int, Dynamic, 1> cans;
    cans.resize(1000,1);
    cans.setZero();
    int cansndx = 0;
    bool okab = false; // ok to break on 0
    for (int ndxab = 0; ndxab < ab.rows(); ++ndxab) {
        if (okab && ab(ndxab,0) == 0) break;
        if (!okab && ab(ndxab,0) > 0) okab = true;
        bool okbc = false;
        for (int ndxbc = 0; ndxbc < bc.rows(); ++ndxbc) {
            if (okbc && bc(ndxbc,0) == 0) break;
            if (!okbc && bc(ndxbc,0) > 0) okbc = true;
            int can = 0;
            if (ab(ndxab,0) == bc(ndxbc,0) && ab(ndxab,1) != bc(ndxbc,1)) {
                can = ab(ndxab,1);
            }
            else if (ab(ndxab,0) != bc(ndxbc,0) && ab(ndxab,1) == bc(ndxbc,1)) {
                can = ab(ndxab,0);
            }
            else {
                continue;
            }
            if (isCanNew(can, cans)) {
                if (cans.rows() < cansndx+1) {
                    cans.conservativeResize(cans.rows()+1000, cans.cols());
                    for (int i = cans.rows() - 1000; i < cans.rows(); ++i) {
                        cans(i,0) = 0;
                    }
                }
                cans(cansndx,0) = can;
                ++cansndx;
            }
            break;
        }
    }
    return cans;
}

bool rules::Triangles::isCanNew(int int1, Eigen::Matrix<int, Eigen::Dynamic, 1>& mat) {
    using namespace Eigen;
    bool ok = false;
    for (int ndx = 0; ndx < mat.rows(); ++ndx) {
        if (ok && mat(ndx,0) == 0) break;
        if (!ok && mat(ndx,0) > 0) ok = true;
        if (mat(ndx,0) == int1) return false;
    }
    return true;
}

Eigen::Matrix<int, Eigen::Dynamic, 2> rules::Triangles::findRelatedPairs(Eigen::Matrix<int, Eigen::Dynamic, 2>& pairsa,
                                                                         Eigen::Matrix<int, Eigen::Dynamic, 2>& pairsb,
                                                                         bool strict) {
    using namespace Eigen;
    Matrix<int, Dynamic, 2> pairs;
    pairs.resize(1000,2);
    pairs.setZero();

    int pairsndx = 0;
    for (int ndxa = 0; ndxa < pairsa.rows(); ++ndxa) {
        if (pairsa(ndxa,0) == 0) break;
        for (int ndxb = 0; ndxb < pairsb.rows(); ++ndxb) {
            if (pairsb(ndxb,0) == 0) break;
            bool pairsAreRelated = false;
            bool pairsAreTheSame = false;
            bool apairIsNew = false;
            bool bpairIsNew = false;
            if (pairsa(ndxa,0) == pairsb(ndxb,0) || pairsa(ndxa,0) == pairsb(ndxb,1)) pairsAreRelated = true;
            if (pairsa(ndxa,1) == pairsb(ndxb,0) || pairsa(ndxa,1) == pairsb(ndxb,1)) pairsAreRelated = true;
            if (pairsa(ndxa,0) == pairsb(ndxb,0) && pairsa(ndxa,1) == pairsb(ndxb,1)) pairsAreTheSame = true;
            if (pairsa(ndxa,1) == pairsb(ndxb,0) && pairsa(ndxa,0) == pairsb(ndxb,1)) pairsAreTheSame = true;
            if (strict == true && !pairsAreTheSame) continue;
            if (pairsAreRelated)
                apairIsNew = isPairNew(pairsa(ndxa,0), pairsa(ndxa,1), pairs);
            if (pairsAreRelated && !pairsAreTheSame)
                bpairIsNew = isPairNew(pairsb(ndxb,0), pairsb(ndxb,1), pairs);
            if (pairsAreRelated && pairsAreTheSame && apairIsNew) {
                if (pairs.rows() < pairsndx+1) {
                    pairs.conservativeResize(pairs.rows()+1000, pairs.cols());
                    for (int i = pairs.rows() - 1000; i < pairs.rows(); ++i) {
                        pairs(i,0) = 0;
                        pairs(i,1) = 0;
                    }
                }
                pairs(pairsndx,0) = pairsa(ndxa,0);
                pairs(pairsndx,1) = pairsa(ndxa,1);
                ++pairsndx;
            }
            if (pairsAreRelated && !pairsAreTheSame && apairIsNew) {
                if (pairs.rows() < pairsndx+1) {
                    pairs.conservativeResize(pairs.rows()+1000, pairs.cols());
                    for (int i = pairs.rows() - 1000; i < pairs.rows(); ++i) {
                        pairs(i,0) = 0;
                        pairs(i,1) = 0;
                    }
                }
                pairs(pairsndx,0) = pairsa(ndxa,0);
                pairs(pairsndx,1) = pairsa(ndxa,1);
                ++pairsndx;
            }
            if (pairsAreRelated && !pairsAreTheSame && bpairIsNew) {
                if (pairs.rows() < pairsndx+1) {
                    pairs.conservativeResize(pairs.rows()+1000, pairs.cols());
                    for (int i = pairs.rows() - 1000; i < pairs.rows(); ++i) {
                        pairs(i,0) = 0;
                        pairs(i,1) = 0;
                    }
                }
                pairs(pairsndx,0) = pairsb(ndxb,0);
                pairs(pairsndx,1) = pairsb(ndxb,1);
                ++pairsndx;
            }
        }
    }
    return pairs;
}

bool rules::Triangles::isPairNew(int int1, int int2, Eigen::Matrix<int, Eigen::Dynamic, 2>& mat) {
    using namespace Eigen;
    for (int ndx = 0; ndx < mat.rows(); ++ndx) {
        if (mat(ndx,0) == 0) break;
        if (mat(ndx,0) == int1 && mat(ndx,1) == int2) return false;
        if (mat(ndx,1) == int1 && mat(ndx,0) == int2) return false;
    }
    return true;
}


