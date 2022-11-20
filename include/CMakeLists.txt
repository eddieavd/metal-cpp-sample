message( "-- Finding Metal dependencies..." )

find_library( MetalLib      Metal      )
find_library( MetalKitLib   MetalKit   )
find_library( FoundationLib Foundation )

message( "-- LIB : Metal      : ${MetalLib}"      )
message( "-- LIB : MetalKit   : ${MetalKitLib}"   )
message( "-- LIB : Foundation : ${FoundationLib}" )

add_custom_command(
        OUTPUT "ops.metallib"

        COMMAND xcrun -sdk macosx metal -c ${CMAKE_SOURCE_DIR}/include/ops.metal -o ${CMAKE_SOURCE_DIR}/include/ops.air
        COMMAND xcrun -sdk macosx metallib ${CMAKE_SOURCE_DIR}/include/ops.air -o ${CMAKE_SOURCE_DIR}/include/ops.metallib
        COMMAND cp ${CMAKE_SOURCE_DIR}/include/ops.metallib ${PROJECT_BINARY_DIR}/ops.metallib
        COMMAND rm ${CMAKE_SOURCE_DIR}/include/ops.air

        VERBATIM
)
add_custom_target(
        metal_lib
        DEPENDS "ops.metallib"
)

add_dependencies( metal_demo metal_lib )

target_link_libraries( metal_demo PUBLIC
        "-framework Metal"
        "-framework MetalKit"
        "-framework Foundation"
)
