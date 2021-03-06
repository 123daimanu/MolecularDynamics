#include "initialize.hpp"

#include <cmath>
#include <iostream>
#include <random>

#include "../maths/Vector3.hpp"

#define INF std::numeric_limits<float>::infinity();

void initialize::Initial() {
  std::mt19937 g1;
  std::default_random_engine g2;
  std::ranlux48 g3;
  std::normal_distribution<float> d1(0, 2);
  std::normal_distribution<float> d3(0, 2);
  std::normal_distribution<float> d2(0, 2);
  float noLattice = std::ceil(pow(number, 1 / 3.));
  float a = length / float(noLattice);
  latConst = a;
  int i = 1;
  while (i < number) {
    for (int ix = 0; ix <= noLattice; ix++) {
      for (int iy = 0; iy <= noLattice; iy++) {
        for (int iz = 0; iz <= noLattice; iz++) {
          Vector3 coord(ix, iy, iz);
          if (i > number) break;
          systemAtoms[i - 1].setnumberOfAtoms(number);
          systemAtoms[i - 1].setAtomTag(i - 1);
          if (i % 4 == 0)
            systemAtoms[i - 1].setPosition((coord + Vector3(0.0, 0.0, 0.0)) *
                                           a);
          else if (i % 3 == 0)
            systemAtoms[i - 1].setPosition((coord + Vector3(0.0, 0.5, 0.5)) *
                                           a);
          else if (i % 2)
            systemAtoms[i - 1].setPosition((coord + Vector3(0.5, 0.5, 0.0)) *
                                           a);
          else
            systemAtoms[i - 1].setPosition((coord + Vector3(0.5, 0, 0.5)) * a);
          Vector3 velocity(d1(g1), d2(g2), d3(g3));
          systemAtoms[i - 1].setVelocity(velocity);
          i = i + 1;
        }
      }
    }
  }

  for (int i = 0; i < number; i++) setForce(systemAtoms[i]);
}

void initialize::setSystemAtoms() { systemAtoms = new atom[number]; }

void initialize::setForce(atom &A) {
  A.force = Vector3(0, 0, 0);
  for (int i = 0; i < number; i++) {
    if (i != A.atomTag) {
      Vector3 F_AB = calcForce(A.pos, systemAtoms[i].pos);
      A.force = A.force + F_AB;
    }
  }
}

Vector3 initialize::calcForce(Vector3 r1, Vector3 r2) {
  Vector3 r_12 = r1.V12(r2);
  float energyConvert = 4.184E-4;  //! convertskcal/mol to  amu A^2/fs^2
  float r = sqrt(r_12.square());
  Vector3 runit = r_12 / r;
  float sigbr = sigma / r;
  float r3 = sigbr * sigbr;
  float r6 = r3 * r3;
  float r12 = r6 * r6;
  float forcScalar = energyConvert * 4 * (epsilon / r) * (-6 * r6 + 12 * r12);
  return runit * forcScalar;
}
