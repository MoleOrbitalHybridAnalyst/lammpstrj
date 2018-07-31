#ifndef __LAMMPSTRJ_H__
#define __LAMMPSTRJ_H__

#include "Vector.h"
#include "snap.h"

#include <vector>
#include <string>

namespace LAMMPSTRJ_NS {

class LAMMPSTRJ {
   std::vector<Snap> snaps;
   std::string fname;
   std::ifstream fs;

public:
   LAMMPSTRJ() = default;
   LAMMPSTRJ(const std::string);
   LAMMPSTRJ(const std::string, int);
   Snap& operator[] (unsigned itimestep) { return snaps.at(itimestep); }

   ~LAMMPSTRJ();

   Snap read_snap() {return Snap(fs); }
   unsigned size() {return snaps.size(); }
};


}

#endif
