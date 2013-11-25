// ============================================================================
// Copyright Jean-Charles LAMBERT - 2007-2013
// e-mail:   Jean-Charles.Lambert@oamp.fr
// address:  Dynamique des galaxies
//           Laboratoire d'Astrophysique de Marseille
//           Pôle de l'Etoile, site de Château-Gombert
//           38, rue Frédéric Joliot-Curie
//           13388 Marseille cedex 13 France
//           CNRS U.M.R 7326
// ============================================================================
// See the complete license in LICENSE and/or "http://www.cecill.info".
// ============================================================================

#ifndef CATMULL_ROM_SPLINE_H
#define CATMULL_ROM_SPLINE_H

#include <glm/glm.hpp>
#include <vector>

namespace glnemo {
class CRSpline
{
public:

  // Constructors and destructor
  CRSpline();
  CRSpline(const CRSpline&);
  ~CRSpline();

  // Operations
  void AddSplinePoint(const glm::vec3& v);
  glm::vec3 GetInterpolatedSplinePoint(float t);   // t = 0...1; 0=vp[0] ... 1=vp[max]
  int GetNumPoints();
  glm::vec3& GetNthPoint(int n);

  // Static method for computing the Catmull-Rom parametric equation
  // given a time (t) and a vector quadruple (p1,p2,p3,p4).
  static glm::vec3 Eq(float t, const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3, const glm::vec3& p4);

  // Clear ctrl points
  void clearCPoints() { vp.clear();}
private:
  std::vector<glm::vec3> vp;
  float delta_t;
};
}
#endif
