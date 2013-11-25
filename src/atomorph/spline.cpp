
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

#include "atomorph/spline.h"

namespace glnemo {
CRSpline::CRSpline()
  : vp(), delta_t(0)
{
}

CRSpline::CRSpline(const CRSpline& s)
{
  for (int i = 0; i < (int)s.vp.size(); i++)
    vp.push_back(s.vp[i]);
  delta_t = s.delta_t;
}

CRSpline::~CRSpline()
{}

// Solve the Catmull-Rom parametric equation for a given time(t) and vector quadruple (p1,p2,p3,p4)
glm::vec3 CRSpline::Eq(float t, const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3, const glm::vec3& p4)
{
  float t2 = t * t;
  float t3 = t2 * t;

  float b1 = .5f * (  -t3 + 2*t2 - t);
  float b2 = .5f * ( 3*t3 - 5*t2 + 2);
  float b3 = .5f * (-3*t3 + 4*t2 + t);
  float b4 = .5f * (   t3 -   t2    );

  return (p1*b1 + p2*b2 + p3*b3 + p4*b4);
}

void CRSpline::AddSplinePoint(const glm::vec3& v)
{
  vp.push_back(v);
  delta_t = (float)1 / (float)vp.size();
}

glm::vec3 CRSpline::GetInterpolatedSplinePoint(float t)
{
  // Find out in which interval we are on the spline
  int p = (int)(t / delta_t);
  // Compute local control point indices
#define BOUNDS(pp) {      if ((pp) < 0)               (pp) = vp.size() - 1; \
                     else if ((pp) >= (int)vp.size()) (pp) = (pp) - (int)vp.size(); }
  int p0 = p - 1;     BOUNDS(p0);
  int p1 = p;         BOUNDS(p1);
  int p2 = p + 1;     BOUNDS(p2);
  int p3 = p + 2;     BOUNDS(p3);
  // Relative (local) time
  float lt = (t - delta_t*(float)p) / delta_t;
  // Interpolate
  return CRSpline::Eq(lt, vp[p0], vp[p1], vp[p2], vp[p3]);
}

int CRSpline::GetNumPoints()
{
  return vp.size();
}

glm::vec3& CRSpline::GetNthPoint(int n)
{
  return vp[n];
}
}

