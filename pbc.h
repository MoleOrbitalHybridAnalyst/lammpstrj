#ifndef __PBC_H__
#define __PBC_H__

#include "Vector.h"

namespace PBC_NS {

class Pbc {
   typedef VECTOR_NS::Vector<double> vd;
   vd box_sizes;

public:
   Pbc(const vd& b) { box_sizes = b; }
   VECTOR_NS::Vector<double> minimum_image(const vd& v) const;
   VECTOR_NS::Vector<double> delta(const vd& v1, const vd& v2) const;
   double modulo2(const vd& v) const {return VECTOR_NS::dotProduct(v, v); }
   double distance2(const vd&, const vd& ) const;
   double distance(const vd& v1, const vd& v2) const 
                      {return std::sqrt(distance2(v1, v2)); }
};

inline
VECTOR_NS::Vector<double> Pbc::minimum_image(
      const VECTOR_NS::Vector<double>& v) const
{
   auto n = VECTOR_NS::closest_int(v / box_sizes);
   return v - box_sizes * n;
}

inline 
VECTOR_NS::Vector<double> Pbc::delta(
      const VECTOR_NS::Vector<double>& v1, 
      const VECTOR_NS::Vector<double>& v2) const
{
   return minimum_image(v2 - v1);
}

inline
double Pbc::distance2(
      const VECTOR_NS::Vector<double>& v1, 
      const VECTOR_NS::Vector<double>& v2) const
{
   return delta(v1, v2).modulo2();
}

}

#endif
