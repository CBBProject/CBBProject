#include <cstdlib>
#include <Utils/Plugins.H>

//  We use the build system to provide a default plugins list.
//  However, if it has been forgotten:

#ifndef IMAGE_IO_PLUGINS_DEFAULT_LIST
#error "No default plugin list"
#endif

namespace Images {

    inline const char*
    plugin_list() {
        const char* var = getenv("IMAGE_IO_PLUGINS");
        return (var ? var : IMAGE_IO_PLUGINS_DEFAULT_LIST);
    }

    inline unsigned
    plugin_verbose() {
        if (const char* var = getenv("IMAGE_IO_PLUGINS_VERBOSE")) {
            const unsigned val = atoi(var);
            return (val ? val : 1);
        }
        return 0;
    }

    static Plugins::plugins PluginsRegistery(plugin_list(),plugin_verbose());
}
