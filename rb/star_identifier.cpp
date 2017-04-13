#include "star_identifier.h"

rules::StarIdentifier::StarIdentifier(stars::Images& image,
                                      stars::Pairs& pairs,
                                      double tolrad) :
    pairs(pairs), image(image), tolerance(tolrad) {
}

int rules::StarIdentifier::identify_central_star(int teststar = 1) {

    std::vector<TriangleSide> abs;
    for (ndxb = 1; ndxb < image.uvecs.rows(); ++ndxb) {

        uveca = image.uvecs.row(0);
        uvecb = image.uvecs.row(ndxb);
        TriangleSide ab(std::acos(uveca.transpose() * uvecb), tolerance, pairs, teststar);

        int prev_stars = 0;
        int repeatcnt = 0;
        bool converged = false;
        for (ndxc = 1; ndxc < image.uvecs.rows(); ++ndxc) {
            if (converged || !get_angs_c()) continue;

            Triangle abca(angs_c[0], angs_c[1], angs_c[2], tolerance, pairs, teststar, image.uvecs.row(ndxc).transpose());
            abca.side1.stars = ab.stars;
            abca.close_loops_abca();
            ab.append_iterations(abca.side1);

            std::vector<Triangle> triangles;
            triangles.push_back(abca);
            for (ndxd = 1; ndxd < image.uvecs.rows(); ++ndxd) {
                if (converged || !get_angs_d()) continue;

                Triangle abda(angs_d[0], angs_d[4], angs_d[3], tolerance, pairs, teststar, image.uvecs.row(ndxd).transpose());
                abda.side1.stars = ab.stars;
                abda.close_loops_abda(triangles);
                ab.append_iterations(abda.side1);
                triangles.push_back(abda);

                if (prev_stars == ab.stars.size()) ++repeatcnt; else repeatcnt = 0;
                if (repeatcnt > 3) converged = true;
                prev_stars = ab.stars.size();
                // std::cout << ndxb << ", " << ndxc << ", " << ndxd << ", " << ab.stars.size() << ", " << ab.has_teststar << ", " << repeatcnt << std::endl;
                if (ab.stars.size() == 1) break;
            }

            if (ab.stars.size() == 1) break;
        }

        if (ab.stars.size() == 1) {
            auto starsit = ab.stars.begin();
            return starsit->first;
        }
        abs.push_back(ab);
    }
    return -1;
}

bool rules::StarIdentifier::get_angs_d() {
    if (ndxd == ndxb || ndxd == ndxc) return false;
    bool angsok = true;
    angs_d = angs_c;
    uvecd = image.uvecs.row(ndxd);
    angs_d.push_back(std::acos(uvecd.transpose() * uveca));
    angs_d.push_back(std::acos(uvecd.transpose() * uvecb));
    angs_d.push_back(std::acos(uvecd.transpose() * uvecc));
    if (angs_d[3] < min_ang) angsok = false; // da
    if (angs_d[4] < min_ang) angsok = false; // db
    if (angs_d[5] < min_ang) angsok = false; // dc
    if (std::abs(angs_d[4]-angs_d[3]) < min_ang) angsok = false; // db-da
    //if (std::abs(angs_d[4]-angs_d[0]) < min_ang) angsok = false; // db-ab
    //if (std::abs(angs_d[4]-angs_d[5]) < min_ang) angsok = false; // db-dc
    return angsok;
}

bool rules::StarIdentifier::get_angs_c() {
    if (ndxc == ndxb) return false;
    bool angsok = true;
    angs_c.clear();
    uvecc = image.uvecs.row(ndxc);
    angs_c.push_back(std::acos(uveca.transpose() * uvecb));
    angs_c.push_back(std::acos(uvecb.transpose() * uvecc));
    angs_c.push_back(std::acos(uvecc.transpose() * uveca));
    min_ang = 3000.0 * stars::arcseconds_to_radians;
    if (angs_c[0] < min_ang) angsok = false; // ab
    if (angs_c[1] < min_ang) angsok = false; // bc
    if (angs_c[2] < min_ang) angsok = false; // ca
    if (std::abs(angs_c[0]-angs_c[1]) < min_ang) angsok = false; // ab-bc
    if (std::abs(angs_c[0]-angs_c[2]) < min_ang) angsok = false; // ab-ca
    if (std::abs(angs_c[1]-angs_c[2]) < min_ang) angsok = false; // bc-ca
    return angsok;
}







