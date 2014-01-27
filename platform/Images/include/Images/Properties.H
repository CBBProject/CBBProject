#pragma once

#include <string>
#include <sstream>
#include <map>

namespace Images {

    class Properties: public std::map<std::string,std::string> {

        typedef std::map<std::string,std::string> base;

    public:
    
        typedef base::iterator       iterator;
        typedef base::const_iterator const_iterator;

        Properties(): base() { }

        bool has_property(const std::string& id) const { return base::find(id)!=base::end(); }

        const std::string& find(const std::string& id) const { return base::find(id)->second; }

        template <typename T>
        const T find(const std::string& id) const {
            T res;
            std::istringstream iss(find(id));
            iss >> res;
            return res;
        }

        template <typename T>
        void define(const std::string& id,const T& prop) {
            std::ostringstream ost;
            ost.precision(10);
            ost << prop;
            define(id,ost.str());
        }

        void define(const std::string& id,const std::string& prop) {
            if (has_property(id)) {
                throw "Double definition";
            } else {
                base::insert(base::value_type(id,prop));
            }
        }
    };
}
