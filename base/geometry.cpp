#include "geometry.h"
#include <string>
#include <iostream>

geometry::Pointing::Pointing() {}
geometry::Pointing::Pointing(double ra=0.0, double dec=0.0, double yaw=0.0) : yaw(yaw) {
    uv.set_size(3);
    uv(0) = cos(ra)*cos(dec);
    uv(1) = sin(ra)*cos(dec);
    uv(2) = sin(dec);
    uv = normalise(uv);
    assert(norm(uv) == 1.0);
}

mat geometry::Pointing::RotationMatrix() {
    mat rm;
    rm.set_size(3,3);
    vec bz = uv;
    vec iz = { 0.0, 0.0, 1.0 };
    vec b1x = cross(bz,iz);
    b1x = normalise(b1x);
    vec b1y = cross(bz,b1x);
    b1y = normalise(b1y);
    vec bx = cos(yaw)*b1x + sin(yaw)*b1y;
    vec by = sin(yaw)*b1x + cos(yaw)*b1y;
    rm.col(0) = bx;
    rm.col(1) = by;
    rm.col(2) = bz;
    assert(det(rm) == 1.0);
    return rm;
}

vec geometry::Pointing::Quaternion() {
    mat rm = RotationMatrix();
    vec q;
    q.set_size(4);
    q = rm2q(rm);
    q = normalise(q);
    assert(norm(q) == 1.0);
    return q;
}

vec geometry::qmult(vec& q1, vec& q2) {
    vec q3 {
        q1(0)*q2(3) + q1(1)*q2(2) + -q1(2)*q2(1) + q1(3)*q2(0),
                -q1(0)*q2(2) + q1(1)*q2(3) + q1(2)*q2(0) + q1(3)*q2(1),
                q1(0)*q2(1) + -q1(1)*q2(0) + q1(2)*q2(3) + q1(3)*q2(2),
                -q1(0)*q2(0) + -q1(1)*q2(1) + -q1(2)*q2(2) + q1(3)*q2(3)
    };
    q3 = normalise(q3);
    if (q3(3) < 0.0) q3 = -q3;
    return q3;
}

vec geometry::qconj(vec& q) {
    vec q2 { -q(0), -q(1), -q(2), q(3) };
    return q2;
}

vec geometry::rv2q(vec& rv) {
    vec q { rv(0)/2, rv(1)/2, rv(2)/2, 1 };
    q = normalise(q);
    if (q(3) < 0.0) q = -q;
    return q;
}

vec geometry::q2rv(vec& q) {
    vec rv { 2*q(0), 2*q(1), 2*q(2) };
    return rv;
}

vec geometry::qdif2rv(vec& q1, vec& q2a) {
    vec q2 { -q2a(0), -q2a(1), -q2a(2), q2a(3) };
    vec q3 {
        q1(0)*q2(3) + q1(1)*q2(2) + -q1(2)*q2(1) + q1(3)*q2(0),
                -q1(0)*q2(2) + q1(1)*q2(3) + q1(2)*q2(0) + q1(3)*q2(1),
                q1(0)*q2(1) + -q1(1)*q2(0) + q1(2)*q2(3) + q1(3)*q2(2),
                -q1(0)*q2(0) + -q1(1)*q2(1) + -q1(2)*q2(2) + q1(3)*q2(3)
    };
    q3 = normalise(q3);
    if ( q3(3) < 0 ) { q3 = -q3; }
    vec rv { 2.0*q3(0), 2.0*q3(1), 2.0*q3(2) };
    return rv;
}

mat geometry::q2rm(vec& q) {
    mat rm(3,3);
    rm(0,0) =  q(0)*q(0) - q(1)*q(1) - q(2)*q(2) + q(3)*q(3);
    rm(0,1) = 2.0*( q(0)*q(1) + q(2)*q(3) );
    rm(0,2) = 2.0*( q(0)*q(2) - q(1)*q(3) );
    rm(1,0) = 2.0*( q(0)*q(1) - q(2)*q(3) );
    rm(1,1) = -q(0)*q(0) + q(1)*q(1) - q(2)*q(2) + q(3)*q(3);
    rm(1,2) = 2.0*( q(1)*q(2) + q(0)*q(3) );
    rm(2,0) = 2.0*( q(0)*q(2) + q(1)*q(3) );
    rm(2,1) = 2.0*( q(1)*q(2) - q(0)*q(3) );
    rm(2,2) = -q(0)*q(0) - q(1)*q(1) + q(2)*q(2) + q(3)*q(3);
    return rm;
}

vec geometry::rm2q(mat& rm) {
    vec q { 0.0, 0.0, 0.0, 1.0 };
    q(0) = 0.5 * sqrt(1.0 + rm(0,0) - rm(1,1) - rm(2,2)) * sgn( rm(1,2)-rm(2,1) );
    q(1) = 0.5 * sqrt(1.0 - rm(0,0) + rm(1,1) - rm(2,2)) * sgn( rm(2,0)-rm(0,2) );
    q(2) = 0.5 * sqrt(1.0 - rm(0,0) - rm(1,1) + rm(2,2)) * sgn( rm(0,1)-rm(1,0) );
    q(3) = 0.5 * sqrt(1.0 + rm(0,0) + rm(1,1) + rm(2,2));
    q = normalise(q);
    if (q(3) < 0.0) q = -q;
    return q;
}

double geometry::sgn(double x) { // nonstandard signum return +1 for 0
    if (x >= 0.0) return 1.0;
    if (x < 0.0) return -1.0;
    return 1.0;
}
