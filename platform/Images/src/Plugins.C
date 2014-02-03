#include <dlfcn.h>

#include <fstream>
#include <iostream>
#include <string>

#include <Utils/IOUtils.H>
#include <Utils/Plugins.H>
#include <Utils/Verbose.H>

#include <boost/filesystem.hpp>

namespace Plugins {

    //  Load a plugin from a single file.
    //  TODO: Replace the error messages by exceptions.

    plugin::plugin(const char* file) {

        //  Test if it is an "ordinary file" (see mknod(2)).

        boost::filesystem::path path(file);
        if (!exists(path)) {
            std::cerr << "      Error: plugin " << file << " does not exist" << std::endl;
            return;
        }

        if (is_directory(path)) {
            std::cerr << "      Error when loading the image io plugins: " << file << " is not a plugin" << std::endl;
            return;
        }

        //  Dlopen the io plugin and keep its handle in memory for future unload.
        //  Do not put RTLD_GLOBAL here (because the identity symbol of each plugin will then be merged) but
        //  use rdynamic flag instead.

        handle = dlopen(file,RTLD_LAZY);

        if (const char* error = dlerror()) {
            std::cerr << "      Error when dlopening the image io plugin " << file << ":\n\t" << error << std::endl;
            return;
        }

        //  Ask the plugin for its identity.

        const void* object = dlsym(handle,"identity");

        if (const char* error = dlerror()) {
            std::cerr << "      Error when dlopening the image io plugin " << file << ":\n\t" << error << std::endl;
            return;
        }

        name = reinterpret_cast<const char*>(object);
    }

    //  Load plugins from a plugins list.

    plugins::plugins(const char* list,const unsigned verb) {

        const bool FileP = verb & FILENAME;

        io_utils::VerboseStream Verbose(verb!=0); 

        Verbose << "Loading plugins from file " << list << " {\n";

        std::ifstream liststream(list,std::ios::binary);
        if (!liststream) {
            std::cerr << "\tError when loading plugins: " << list << " could not be opened." << std::endl;
            return;
        }

        std::string file;
        while (liststream >> io_utils::skip_comments('#') >> file) {
            push_back(plugin(file.c_str()));
            Verbose << "\tLoaded: ";
            if (const char* identity = back().identity()) {
                Verbose << "identity = " << identity;
                if (FileP)
                    Verbose << '(' << file << ')';
            } else {
                Verbose << file;
            }
            Verbose << '\n';
        }

        Verbose << "}\n";
    }

    //  The destructor unloads the dynamic I/O plugins.

    plugins::~plugins() {
        for (iterator i=begin();i!=end();++i)
            dlclose(i->handle);
    }
}
