#include "lammpstrj.h"

namespace LAMMPSTRJ_NS {

LAMMPSTRJ::LAMMPSTRJ(const std::string fn, int junk) :
fs(fn)
{
   (void) junk;
   fname = fn;
   if(!fs.is_open())
      throw std::runtime_error("cannot open file" + fn);
}

LAMMPSTRJ::LAMMPSTRJ(const std::string fn) :
fs(fn)
{
   fname = fn;
   if(!fs.is_open())
      throw std::runtime_error("cannot open file" + fn);

   while(1) {
      snaps.push_back(Snap(fs));
      if(snaps.back().get_natoms() == 0) {
         snaps.pop_back(); break;
      }
   }

   fs.close();
}

LAMMPSTRJ::~LAMMPSTRJ()
{
   if(fs.is_open()) fs.close();
}


}
