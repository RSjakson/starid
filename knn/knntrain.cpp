#include "knntrain.h"

uint knn::KnnTrain::Classify(arma::vec& fv) {
    using namespace arma;
    uint catndx = 0;
    rowvec distances = trans(fv) * fvs;
    uvec ndxs = sort_index(trans(distances), "descend");
    catndx = ndxs(0);
    return catndx;
}

knn::KnnTrain::KnnTrain(base::Catalog &catalog, base::Sensor &sensor) {
    fvs.zeros(1e2, catalog.stars.size());
    for (uint i = 0; i < catalog.stars.size(); ++i) {
        base::Pointing p(catalog.stars[i].ra, catalog.stars[i].dec, 0.0);
        sensor.Click(catalog, p);
        fvs.col(i) = sensor.l2a.fv;
    }
}

void knn::KnnTrain::StatusFeatures() {
    using namespace arma;
    rowvec nfeaturesa = sum(fvs);
    std::cout << hist(nfeaturesa, linspace<vec>(0,60,10)) << "\n";
}

void knn::KnnTrain::StatusOrthogonality() {
    using namespace arma;
    mat dota = trans(normalise(fvs)) * normalise(fvs);
    vec tmpa = vectorise(dota);
    uvec hista = hist(tmpa, linspace<vec>(0,1,10))/2;
    hista(9) = hista(9) - 4439;
    std::cout << trans(hista) << "\n";
}

