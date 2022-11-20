//
//
//      metal_demo
//      util.hpp
//

#pragma once

#include <iostream>


#define ASSERT(cond, ... )                                             \
         do                                                             \
         {                                                               \
                 if( !( cond ) )                                          \
                 {                                                         \
                         std::cerr << "mdemo: '" << #cond << "' failed in " \
                                   << __FILE__ << ":" << __LINE__            \
                                   << " - " << __VA_ARGS__ << std::endl;      \
                         std::abort();                                         \
                 }                                                              \
         } while( 0 )
