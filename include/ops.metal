//
//
//      metal_demo
//      ops.metal
//

#include <metal_stdlib>

using namespace metal;


kernel void add_arrays ( device int const * x, device int const * y, device int * r, unsigned index [[ thread_position_in_grid ]] )
{
        r[ index ] = x[ index ] + y[ index ];
}
