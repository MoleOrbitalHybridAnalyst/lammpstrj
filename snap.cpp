#include "snap.h"

namespace LAMMPSTRJ_NS {

//Snap::~Snap()
//{
//   if(pbc) delete pbc;
//}

void Snap::update_fields2values(std::stringstream& ss)
{
   for(const auto& f : names) {
      double value; ss >> value;
      field2values[f].push_back(value);
   }
}

Snap::Snap(std::ifstream& fs) :
natoms(0),
pbc(nullptr)
{
   // parse timestep boxlo boxhi natoms
   unsigned line_count = 0;
   natoms = std::numeric_limits<unsigned>::max() - 9;
   std::string line;
   std::string field_name;
   while(std::getline(fs, line)) {
      line_count++;
      std::stringstream ss(line);
      if(line_count >= natoms + 9) {
         update_fields2values(ss); break;
      }
      if(line_count >= 10) {
         update_fields2values(ss);
      } else if(line_count == 1) {
         ss >> field_name;
         if(field_name != "ITEM:")
            throw std::runtime_error("cannot find ITEM");
      } else if(line_count == 2) {
         ss >> timestep;
      } else if(line_count == 3) {
         ss >> field_name;
         if(field_name != "ITEM:")
            throw std::runtime_error("cannot find ITEM");
      } else if(line_count == 4) {
         ss >> natoms;
      } else if(line_count == 5) {
         ss >> field_name;
         if(field_name != "ITEM:")
            throw std::runtime_error("cannot find ITEM");
      } else if(line_count == 6 or line_count == 7 or line_count == 8) {
         ss >> boxlo[line_count - 6];
         ss >> boxhi[line_count - 6];
      } else if(line_count == 9) {
         ss >> field_name >> field_name; // "ITEM: ATOMS"
         if(field_name != "ATOMS")
            throw std::runtime_error("cannot find ATOM");
         while(ss >> field_name) {
            names.push_back(field_name);
            field2values[field_name];
            field2values[field_name].reserve(natoms);
            itx = field2values.find("x");
            ity = field2values.find("y");
            itz = field2values.find("z");
            itvx = field2values.find("vx");
            itvy = field2values.find("vy");
            itvz = field2values.find("vz");
            ittype = field2values.find("type");
            itmol = field2values.find("mol");
            itid = field2values.find("id");
         }
      }
   }
   if(line_count == 0) {natoms = 0; return;}
   pbc = new PBC_NS::Pbc(boxhi - boxlo);
}

}
