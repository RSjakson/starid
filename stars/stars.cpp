#include "sensor.h"
#include "mnist.h"
#include <armadillo>
#include "cereal/archives/binary.hpp"

int main() {
    arma::arma_rng::set_seed_random();
    std::string fsky2000    = "/home/noah/dev/starid/data/SKYMAP_SKY2000_V5R4.txt";
    std::string f10stars    = "/home/noah/dev/starid/data/SKYMAP_10_stars.txt";
    std::string f8876stars  = "/home/noah/dev/starid/data/SKYMAP_8876_stars.txt";
    double mv               = 6.5;
    double fov              = 4.0 * arma::datum::pi / 180.0;

    if (1) { // catalog subset (800, 1600, 2400, ... 8000)
        stars::Sky stars;
        stars.init(fsky2000, mv);
        std::ofstream fout1(f10stars);
        for (int label = 0; label < 10; ++label) {
            int starndx = 800 * (label + 1);
            fout1 << stars.catalogLines[starndx] << std::endl;
        }
        fout1.close();
        std::ofstream fout2(f8876stars);
        for (auto it = stars.catalogLines.begin(); it != stars.catalogLines.end(); ++it) {
            fout2 << *it << std::endl;
        }
        fout2.close();
    }

    if (0) { // generate star images in mnist format
        int number_of_images  = 60000; // 60k for training file, 10k for test file
        std::string fimg1    = "/home/noah/dev/starid/data/mnist_format/mnist_imagesb.mnist";
        std::string flab1    = "/home/noah/dev/starid/data/mnist_format/mnist_labelsb.mnist";
        std::string fimg2    = "/home/noah/dev/starid/data/mnist_format/starid_imagesb.mnist";
        std::string flab2    = "/home/noah/dev/starid/data/mnist_format/starid_labelsb.mnist";
        data::Mnist mnist;
        std::vector<arma::mat> images;
        arma::colvec labels = arma::zeros<arma::colvec>(number_of_images);
        mnist.ReadMnistI(fimg1, images);
        mnist.ReadMnistL(flab1, labels);
        stars::Sensor sensor(f10stars, mv, fov);
        for (int tenscnt = 0; tenscnt < number_of_images/10; ++tenscnt) {
            for (int label = 0; label < 10; ++label) {
                int starndx = 800 * (label+1);
                arma::mat img = sensor.makeStarImage(starndx);
                labels(10*tenscnt + label) = (double)label;
                images[10*tenscnt + label] = img;
                //sensor.status();
            }
        }
        //mnist.WriteMnistI(fimg2, images, yaw);
        //mnist.WriteMnistL(flab2, labels);
    }

    return 0;
}

