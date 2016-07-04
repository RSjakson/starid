#include "pointing.h"

#include "gtest/gtest.h"
#include <armadillo>

TEST(base, test1) {
    double ra = 0.0 * arma::datum::pi / 180.0; // cass
    double dec = 60.0 * arma::datum::pi / 180.0; // cass

    base::Pointing pointing(ra, dec, 0.0);
    EXPECT_EQ(norm(pointing.uv), 1.0);
}

TEST(base, test2) {
double ra = 0.0 * arma::datum::pi / 180.0; // cass
double dec = 60.0 * arma::datum::pi / 180.0; // cass

    base::Pointing pointing(ra, dec, 0.0);
    EXPECT_EQ(det(pointing.RotationMatrix()), 1.0);
}

