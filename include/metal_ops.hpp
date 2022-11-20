//
//
//      metal_demo
//      metal_ops.hpp
//

#pragma once

#include <vector>
#include <string>
#include <map>

#include "Metal.hpp"

#include "util.hpp"

#define DEFAULT_LIB_PATH "./include/ops.metallib"


class metal_ops
{
public:
        metal_ops ( MTL::Device * device );
        metal_ops (                      );

        std::vector< int > add_arrays ( std::vector< int > const & lhs, std::vector< int > const & rhs );
private:
        MTL::Device * device_;

        std::map< std::string, MTL::Function * > function_map_;
        std::map< std::string, MTL::ComputePipelineState * > func_pipeline_map_;

        MTL::CommandQueue * command_q_;

        void _exec_blocking ( std::vector< MTL::Buffer * > buffers, std::size_t const arr_len, char const * method );

        MTL::Buffer * _create_buffer ( std::size_t const size );

        template< typename T >
        MTL::Buffer * _create_buffer ( std::vector< T > const & src );

        template< typename T >
        void _read_buffer ( MTL::Buffer * buffer, std::vector< T > & dest, std::size_t const count );

        void _add_arrays ( MTL::Buffer * lhs, MTL::Buffer * rhs, MTL::Buffer * dest, std::size_t const arr_len );
};

template< typename T >
MTL::Buffer * metal_ops::_create_buffer ( std::vector< T > const & src )
{
        MTL::Buffer * gpu_buffer = this->device_->newBuffer( src.size() * sizeof( T ), MTL::ResourceStorageModeManaged );

        T * buff_ptr = static_cast< T * >( gpu_buffer->contents() );

        std::memcpy( buff_ptr, src.data(), src.size() * sizeof( T ) );

        return gpu_buffer;
}

template< typename T >
void metal_ops::_read_buffer ( MTL::Buffer * buffer, std::vector< T > & dest, std::size_t const count )
{
        T * buff_ptr = static_cast< T * >( buffer->contents() );

        std::memcpy( dest.data(), buff_ptr, count * sizeof( T ) );
}
