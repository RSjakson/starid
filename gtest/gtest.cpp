#include "gtest/gtest.h"


int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);

    ::testing::GTEST_FLAG(filter) = "geometry.basic"
                                    ":geometry.rotation"
                                    ":base.basic";
    RUN_ALL_TESTS();

    return 0;
}
