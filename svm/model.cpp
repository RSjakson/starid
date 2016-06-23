#include "model.h"

svm::Model::Model(base::Training &trainingset) {
    using namespace arma;
    gamma = 1;
    epsilon = 0.001;
    Cval = 1;

    xvecs = trainingset.examples;
    yvec = trainingset.labels;
    lval = yvec.n_rows;

    alphavec.zeros(lval);
    Kmat.zeros(lval,lval);
    Qmat = Kmat;
    for (uint i = 0; i < lval; ++i) {
        for (uint j = i; j < lval; ++j) {

            vecdiff = xvecs.row(i) - xvecs.row(j);
            Kmat(i,j) = exp( -gamma * dot( vecdiff, vecdiff ) ); // rbf kernel
            Qmat(i,j) = yvec(i) * yvec(j) * Kmat(i,j);

            Kmat(j,i) = Kmat(i,j);
            Qmat(j,i) = Qmat(i,j);
        }
    }

}
