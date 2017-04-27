#include "libstars.h"

libstars::libstars() {
    std::string datadir = "/home/noah/starid/stars/data/";
    std::ifstream is1(std::string(datadir + "sky"));
    cereal::BinaryInputArchive iarchive1(is1);
    iarchive1(sky);
}

void libstars::load_pairs() {
    std::string datadir = "/home/noah/starid/stars/data/";
    std::ifstream is2(std::string(datadir + "pairs"));
    cereal::BinaryInputArchive iarchive2(is2);
    iarchive2(pairs);
}

Eigen::MatrixXd libstars::image(int starndx) {
    Eigen::MatrixXd img = stars::pointing_vectors::new_image_matrix(starndx, sky);;
    return img;
}

int libstars::id(stars::image_matrix imgmat) {
    id::star_identifier ider(imgmat, pairs);
    int result = ider.id();
    return result;
}
