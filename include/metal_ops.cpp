//
//
//      metal_demo
//      metal_ops.cpp
//

#include "metal_ops.hpp"


metal_ops::metal_ops ( MTL::Device * device )
{
        this->device_ = device;

        NS::Error * err = nullptr;

        auto filepath = NS::String::string( DEFAULT_LIB_PATH, NS::ASCIIStringEncoding );

        MTL::Library * op_lib = device_->newLibrary( filepath, &err );

        ASSERT( op_lib != nullptr, "metal_ops::ctor: failed to find default library with error: " << err->description()->utf8String() );

        auto fn_names = op_lib->functionNames();

        for( std::size_t i = 0; i < fn_names->count(); ++i )
        {
                auto name_nsstring = fn_names->object( i )->description();
                auto name_utf8     = name_nsstring->utf8String();

                this->function_map_[ name_utf8 ] =
                        ( op_lib->newFunction( name_nsstring ) );

                this->func_pipeline_map_[ name_utf8 ] =
                        this->device_->newComputePipelineState( this->function_map_[ name_utf8 ], &err );

                ASSERT( this->func_pipeline_map_[ name_utf8 ] != nullptr, "metal_ops::ctor: failed to create pipeline state object for " << name_utf8 << " with error: " << err->description()->utf8String() );
        }

        this->command_q_ = device_->newCommandQueue();

        ASSERT( this->command_q_ != nullptr, "metal_ops::ctor: failed to create command queue" );
}

metal_ops::metal_ops ()
        : metal_ops( MTL::CreateSystemDefaultDevice() )
{}

std::vector< int > metal_ops::add_arrays ( std::vector< int > const & lhs, std::vector< int > const & rhs )
{
        std::size_t count = lhs.size();

        MTL::Buffer * lhs_buffer = _create_buffer(                 lhs   );
        MTL::Buffer * rhs_buffer = _create_buffer(                 rhs   );
        MTL::Buffer * res_buffer = _create_buffer( count * sizeof( int ) );

        _add_arrays( lhs_buffer, rhs_buffer, res_buffer, lhs.size() );

        std::vector< int > result( count );

        _read_buffer( res_buffer, result, count );

        return result;
}

void metal_ops::_exec_blocking ( std::vector< MTL::Buffer * > buffers, std::size_t const arr_len, char const * method )
{
        // create new buffer
        MTL::CommandBuffer * command_buffer = this->command_q_->commandBuffer();

        ASSERT( command_buffer != nullptr, "metal_ops::_exec_blocking: failed getting command buffer" );

        // create new encoder
        MTL::ComputeCommandEncoder * compute_encoder = command_buffer->computeCommandEncoder();
        // assert compute_encoder != nullptr

        ASSERT( compute_encoder != nullptr, "metal_ops::_exec_blocking: failed getting compute command encoder" );

        // set pipeline
        compute_encoder->setComputePipelineState( this->func_pipeline_map_[ method ] );

        for( std::size_t i = 0; i < buffers.size(); ++i )
        {
                compute_encoder->setBuffer( buffers[ i ], 0, i );
        }

        // compute thread_group_size
        NS::UInteger potential_thread_group_size =
                this->func_pipeline_map_[ method ]->maxTotalThreadsPerThreadgroup();

        if( potential_thread_group_size > arr_len )
        {
                potential_thread_group_size = arr_len;
        }

        MTL::Size thread_group_size = MTL::Size::Make( potential_thread_group_size, 1, 1 );
        MTL::Size grid_size         = MTL::Size::Make(                     arr_len, 1, 1 );

        // encode compute command
        compute_encoder->dispatchThreads( grid_size, thread_group_size );
        compute_encoder->endEncoding();

        command_buffer->commit();
        command_buffer->waitUntilCompleted();
}

MTL::Buffer * metal_ops::_create_buffer ( std::size_t const size )
{
        return this->device_->newBuffer( size, MTL::ResourceStorageModeManaged );
}

void metal_ops::_add_arrays ( MTL::Buffer * lhs, MTL::Buffer * rhs, MTL::Buffer * dest, std::size_t const arr_len )
{
        std::vector< MTL::Buffer * > buffers( { lhs, rhs, dest } );

        char const * method = "add_arrays";

        _exec_blocking( buffers, arr_len, method );
}
