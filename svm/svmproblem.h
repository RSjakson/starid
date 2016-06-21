#ifndef SVMPROBLEM_H
#define SVMPROBLEM_H

#include "training.h"
#include <armadillo>

namespace svm {

class SvmProblem {

public:
    SvmProblem(base::Training& trainingset);

    arma::mat examples;
    arma::vec labels;
    double gamma;
    double eps;
    double C;

private:

};

}

#endif // SVMPROBLEM_H
