#ifndef __SNAP_H__
#define __SNAP_H__

#include "Vector.h"
#include "pbc.h"

#include <vector>
#include <unordered_map>
#include <string>
#include <fstream>
#include <sstream>
#include <limits>

namespace LAMMPSTRJ_NS {

class Snap {
   VECTOR_NS::Vector<double> boxlo, boxhi;
   unsigned timestep;
   unsigned natoms;
   PBC_NS::Pbc* pbc;

   std::vector<std::string> names;
   std::unordered_map<std::string, std::vector<double>> field2values;

   std::unordered_map<std::string, std::vector<double>>::iterator
      itx, ity, itz, itvx, itvy, itvz, ittype, itmol, itid, itfx, itfy, itfz;

   void update_fields2values(std::stringstream&);

public:
   Snap() = default;
   Snap(std::ifstream&);
   //~Snap();

   VECTOR_NS::Vector<double> const get_boxlo() {return boxlo; }
   VECTOR_NS::Vector<double> const get_boxhi() {return boxhi; }
   unsigned get_timestep() const {return timestep; }
   unsigned get_natoms() const {return natoms; }

   std::vector<double>& get_field(const std::string& field_name)
                                 {return field2values[field_name]; }
   double& get_field(const std::string& field_name, unsigned iatom)
#ifndef NDEBUG
                          {return field2values[field_name].at(iatom); }
#else
                          {return field2values[field_name][iatom]; }
#endif
   double& get_field(unsigned iatom, const std::string& field_name)
#ifndef NDEBUG
                          {return field2values[field_name].at(iatom); }
#else
                          {return field2values[field_name][iatom]; }
#endif
   //std::vector<double> get_field(unsigned iatom, 
   //            std::vector<std::string>& field_names) const;
   VECTOR_NS::Vector<double> get_coord(unsigned iatom) const;
   VECTOR_NS::Vector<double> get_veloc(unsigned iatom) const;
   VECTOR_NS::Vector<double> get_force(unsigned iatom) const;
   unsigned get_type(unsigned iatom) const;
   unsigned get_mol(unsigned iatom) const;
   unsigned get_id(unsigned iatom) const;
   void set_coord(unsigned iatom, const VECTOR_NS::Vector<double>&);
   void set_veloc(unsigned iatom, const VECTOR_NS::Vector<double>&);
   void set_force(unsigned iatom, const VECTOR_NS::Vector<double>&);

   VECTOR_NS::Vector<double> delta(unsigned i, unsigned j) const
                  { return pbc->delta(get_coord(i), get_coord(j)); }
   double distance2(unsigned i, unsigned j) const
                  { return pbc->distance2(get_coord(i), get_coord(j)); }
   double distance(unsigned i, unsigned j) const
                  { return pbc->distance(get_coord(i), get_coord(j)); }
/// cos of angle i-j-k
   double cos_angle(unsigned i, unsigned j, unsigned k) const;
   unsigned find_atom_by_id(unsigned id) const;
};
//inline
//std::vector<double> Snap::get_field(unsigned iatom,
//         std::vector<std::string>& field_names) const
//{
//   if(iatom >= natoms) throw std::out_of_range("atom out of range");
//   std::vector<double> result;
//   for(std::string& f : field_names) {
//      result.push_back(field2values[f][iatom]);
//   }
//   return result;
//}

inline
VECTOR_NS::Vector<double> Snap::get_coord(unsigned iatom) const
{
#ifndef NDEBUG
   if(iatom >= natoms) throw std::out_of_range("atom out of range");
   if(itx != field2values.end() && 
      ity != field2values.end() &&
      itz != field2values.end() ) {
#endif
      return VECTOR_NS::Vector<double>(
            itx->second[iatom], ity->second[iatom], itz->second[iatom]);
#ifndef NDEBUG
   } else {
      throw std::runtime_error("missing x or y or z");
   }
#endif
}

inline
VECTOR_NS::Vector<double> Snap::get_veloc(unsigned iatom) const
{
#ifndef NDEBUG
   if(iatom >= natoms) throw std::out_of_range("atom out of range");
   if(itvx != field2values.end() && 
      itvy != field2values.end() &&
      itvz != field2values.end() ) {
#endif
      return VECTOR_NS::Vector<double>(
            itvx->second[iatom], itvy->second[iatom], itvz->second[iatom]);
#ifndef NDEBUG
   } else {
      throw std::runtime_error("missing vx or vy or vz");
   }
#endif
}

inline
VECTOR_NS::Vector<double> Snap::get_force(unsigned iatom) const
{
#ifndef NDEBUG
   if(iatom >= natoms) throw std::out_of_range("atom out of range");
   if(itfx != field2values.end() && 
      itfy != field2values.end() &&
      itfz != field2values.end() ) {
#endif
      return VECTOR_NS::Vector<double>(
            itfx->second[iatom], itfy->second[iatom], itfz->second[iatom]);
#ifndef NDEBUG
   } else {
      throw std::runtime_error("missing fx or fy or fz");
   }
#endif
}

inline
unsigned Snap::get_type(unsigned iatom) const
{
#ifndef NDEBUG
   if(iatom >= natoms) throw std::out_of_range("atom out of range");
   if(ittype != field2values.end()) {
#endif
      return static_cast<unsigned>(ittype->second[iatom]);
#ifndef NDEBUG
   } else {
      throw std::runtime_error("missing type");
   }
#endif
}

inline
unsigned Snap::get_mol(unsigned iatom) const
{
#ifndef NDEBUG
   if(iatom >= natoms) throw std::out_of_range("atom out of range");
   if(itmol != field2values.end()) {
#endif
      return static_cast<unsigned>(itmol->second[iatom]);
#ifndef NDEBUG
   } else {
      throw std::runtime_error("missing mol");
   }
#endif
}

inline
unsigned Snap::get_id(unsigned iatom) const
{
#ifndef NDEBUG
   if(iatom >= natoms) throw std::out_of_range("atom out of range");
   if(itid != field2values.end()) {
#endif
      return static_cast<unsigned>(itid->second[iatom]);
#ifndef NDEBUG
   } else {
      throw std::runtime_error("missing id");
   }
#endif
}

inline
void Snap::set_coord(unsigned iatom,
      const VECTOR_NS::Vector<double>& x)
{
#ifndef NDEBUG
   if(iatom >= natoms) throw std::out_of_range("atom out of range");
   if(itx != field2values.end() && 
      ity != field2values.end() &&
      itz != field2values.end() ) {
#endif
      itx->second[iatom] = x[0];
      ity->second[iatom] = x[1];
      itz->second[iatom] = x[2];
#ifndef NDEBUG
   } else {
      throw std::runtime_error("missing x or y or z");
   }
#endif
}

inline
void Snap::set_veloc(unsigned iatom,
      const VECTOR_NS::Vector<double>& v)
{
#ifndef NDEBUG
   if(iatom >= natoms) throw std::out_of_range("atom out of range");
   if(itvx != field2values.end() && 
      itvy != field2values.end() &&
      itvz != field2values.end() ) {
#endif
      itvx->second[iatom] = v[0]; 
      itvy->second[iatom] = v[1];
      itvz->second[iatom] = v[2];
#ifndef NDEBUG
   } else {
      throw std::runtime_error("missing vx or vy or vz");
   }
#endif
}

inline
void Snap::set_force(unsigned iatom,
      const VECTOR_NS::Vector<double>& v)
{
#ifndef NDEBUG
   if(iatom >= natoms) throw std::out_of_range("atom out of range");
   if(itfx != field2values.end() && 
      itfy != field2values.end() &&
      itfz != field2values.end() ) {
#endif
      itfx->second[iatom] = v[0]; 
      itfy->second[iatom] = v[1];
      itfz->second[iatom] = v[2];
#ifndef NDEBUG
   } else {
      throw std::runtime_error("missing fx or fy or fz");
   }
#endif
}

inline
double Snap::cos_angle(unsigned i, unsigned j, unsigned k) const
{
   const auto& ri = get_coord(i), rj = get_coord(j), rk = get_coord(k);
   const auto& vji = pbc->delta(rj, ri);
   const auto& vjk = pbc->delta(rj, rk);
   return VECTOR_NS::dotProduct(vji, vjk) / vji.modulo() / vjk.modulo();
}

inline
unsigned Snap::find_atom_by_id(unsigned id) const
{
   for(unsigned i = 0; i < natoms; ++i)
      if(itid->second[i] == id) return i;
   std::runtime_error("cannot find id " + std::to_string(id));
}

}

#endif
