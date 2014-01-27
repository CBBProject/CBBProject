#ifndef INFOTAG_H
#define INFOTAG_H

#include <typeinfo>
#include <cstring>

namespace Types {

    //  This is absolutely necessary as typeinfo cannot be copied.
    //  This implements just what is necessary and allows the usage of STL containers.
    
    struct info_tag {

        info_tag(const std::type_info& t): info(t) { }

#if 0
        //  This version cannot be used because loading multiple dso leads to multiple
        //  type_info for the same type. Thus type_infos for identical types would be 
        //  considered as different with the current gcc implementation (that only
        //  compares pointers).

        bool operator<(const info_tag& t)        const { return info.before(t.info); }
        bool operator!=(const info_tag& t)       const { return info != t.info;      }
        bool operator!=(const std::type_info& t) const { return info != t;           }
#endif

        bool operator<(const info_tag& t)        const { return strcmp(info.name(),t.info.name())<0; }
        bool operator!=(const info_tag& t)       const { return operator!=(t.info);                  }
        bool operator!=(const std::type_info& t) const { return strcmp(info.name(),t.name());        }
        bool operator==(const std::type_info& t) const { return !operator!=(t);                      }

        const std::type_info& info;
    };
}

#endif  //  ! INFOTAG_H
