//  This macro is ugly, but the result is much more compact and readable
//  than any other alternative I know.

#define NAME_SPECIALIZATION(type,name)              \
namespace Images {                                  \
    namespace Inrimage5 {                           \
        namespace Tools {                           \
            template <>                             \
            struct Name<type> {                     \
                static const char string[];         \
            };                                      \
                                                    \
            const char Name<type>::string[] = name; \
        }                                           \
    }                                               \
}
