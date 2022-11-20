//
//
//	metal_demo
//	main.cpp
//

#define  NS_PRIVATE_IMPLEMENTATION
#define  CA_PRIVATE_IMPLEMENTATION
#define MTL_PRIVATE_IMPLEMENTATION

#include <iostream>
#include <vector>

#include "include/metal_ops.hpp"

#define SIZE 100


int main ()
{
        metal_ops mtl;

        std::vector< int > vec1( SIZE, 1 );
        std::vector< int > vec2( SIZE, 2 );

        auto sum = mtl.add_arrays( vec1, vec2 );

        ASSERT( sum.size() == SIZE, "hmmm" );

        for( auto val : sum )
        {
                std::cout << val << " ";
        }



        return 0;
}
