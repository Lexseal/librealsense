// License: Apache 2.0. See LICENSE file in root directory.
// Copyright(c) 2022 Intel Corporation. All Rights Reserved.

#pragma once
#include <librealsense2/rs.hpp>  // Include RealSense Cross Platform API
#include <unordered_map>

namespace tools {

// This class is in charge of handling a RS device: streaming, control..
class lrs_device_controller
{
public:
    
    lrs_device_controller( rs2::device dev );
    ~lrs_device_controller();
    void start_stream( rs2::stream_profile sp, std::function< void( const std::string& stream_name, uint8_t* frame, int size) > cb );
    void stop_stream( rs2_stream stream );
    void stop_all_streams();

private:
    class lrs_sensor_streamer;

    rs2::device _rs_dev;
    std::string _device_sn;
    std::unordered_map<rs2_stream, std::shared_ptr<lrs_sensor_streamer>> stream_to_rs2_sensor;
};  // class lrs_device_controller
}  // namespace tools
