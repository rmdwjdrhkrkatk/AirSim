// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

//in header only mode, control library is not available
#ifndef AIRLIB_HEADER_ONLY
//RPC code requires C++14. If build system like Unreal doesn't support it then use compiled binaries
#ifndef AIRLIB_NO_RPC
//if using Unreal Build system then include pre-compiled header file first

#include "vehicles/multirotor/api/MultirotorRpcLibServer.hpp"


#include "common/Common.hpp"
STRICT_MODE_OFF

#ifndef RPCLIB_MSGPACK
#define RPCLIB_MSGPACK clmdep_msgpack
#endif // !RPCLIB_MSGPACK
#include "common/common_utils/MinWinDefines.hpp"
#undef NOUSER

#include "common/common_utils/WindowsApisCommonPre.hpp"
#undef FLOAT
#undef check
#include "rpc/server.h"
//TODO: HACK: UE4 defines macro with stupid names like "check" that conflicts with msgpack library
#ifndef check
#define check(expr) (static_cast<void>((expr)))
#endif
#include "common/common_utils/WindowsApisCommonPost.hpp"

#include "vehicles/multirotor/api/MultirotorRpcLibAdapators.hpp"

STRICT_MODE_ON


namespace msr { namespace airlib {

typedef msr::airlib_rpclib::MultirotorRpcLibAdapators MultirotorRpcLibAdapators;

MultirotorRpcLibServer::MultirotorRpcLibServer(ApiProvider* api_provider, string server_address, uint16_t port)
        : RpcLibServerBase(api_provider, server_address, port)
{
    (static_cast<rpc::server*>(getServer()))->
        bind("takeoff", [&](float timeout_sec, const std::string& vehicle_name) -> bool { 
        return getVehicleApi(vehicle_name)->takeoff(timeout_sec); 
    });
    (static_cast<rpc::server*>(getServer()))->
        bind("land", [&](float timeout_sec, const std::string& vehicle_name) -> bool { 
        return getVehicleApi(vehicle_name)->land(timeout_sec); 
    });
    (static_cast<rpc::server*>(getServer()))->
        bind("goHome", [&](float timeout_sec, const std::string& vehicle_name) -> bool { 
        return getVehicleApi(vehicle_name)->goHome(timeout_sec); 
    });

    (static_cast<rpc::server*>(getServer()))->
        bind("moveByAngleZ", [&](float pitch, float roll, float z, float yaw, float duration, const std::string& vehicle_name) ->
        bool { return getVehicleApi(vehicle_name)->moveByAngleZ(pitch, roll, z, yaw, duration); });
    (static_cast<rpc::server*>(getServer()))->
        bind("moveByAngleThrottle", [&](float pitch, float roll, float throttle, float yaw_rate, float duration, 
            const std::string& vehicle_name) -> bool { 
                return getVehicleApi(vehicle_name)->moveByAngleThrottle(pitch, roll, throttle, yaw_rate, duration); 
    });
    (static_cast<rpc::server*>(getServer()))->
        bind("moveByVelocity", [&](float vx, float vy, float vz, float duration, DrivetrainType drivetrain, 
            const MultirotorRpcLibAdapators::YawMode& yaw_mode, const std::string& vehicle_name) -> bool { 
        return getVehicleApi(vehicle_name)->moveByVelocity(vx, vy, vz, duration, drivetrain, yaw_mode.to()); 
    });
    (static_cast<rpc::server*>(getServer()))->
        bind("moveByVelocityZ", [&](float vx, float vy, float z, float duration, DrivetrainType drivetrain, 
            const MultirotorRpcLibAdapators::YawMode& yaw_mode, const std::string& vehicle_name) -> bool {
            return getVehicleApi(vehicle_name)->moveByVelocityZ(vx, vy, z, duration, drivetrain, yaw_mode.to()); 
    });
    (static_cast<rpc::server*>(getServer()))->
        bind("moveByRotorSpeed", [&](float o0, float o1, float o2, float o3, float duration, const std::string& vehicle_name) -> bool {
            return getVehicleApi(vehicle_name)->moveByRotorSpeed(o0, o1, o2, o3, duration); 
    });
    (static_cast<rpc::server*>(getServer()))->
        bind("moveOnPath", [&](const vector<MultirotorRpcLibAdapators::Vector3r>& path, float velocity, float timeout_sec, DrivetrainType drivetrain, const MultirotorRpcLibAdapators::YawMode& yaw_mode,
        float lookahead, float adaptive_lookahead, const std::string& vehicle_name) -> bool {
            vector<Vector3r> conv_path;
            MultirotorRpcLibAdapators::to(path, conv_path);
            return getVehicleApi(vehicle_name)->moveOnPath(conv_path, velocity, timeout_sec, drivetrain, yaw_mode.to(), lookahead, adaptive_lookahead);
        });
    (static_cast<rpc::server*>(getServer()))->
        bind("moveToPosition", [&](float x, float y, float z, float velocity, float timeout_sec, DrivetrainType drivetrain,
        const MultirotorRpcLibAdapators::YawMode& yaw_mode, float lookahead, float adaptive_lookahead, const std::string& vehicle_name) -> bool {
        return getVehicleApi(vehicle_name)->moveToPosition(x, y, z, velocity, timeout_sec, drivetrain, yaw_mode.to(), lookahead, adaptive_lookahead); 
    });
    (static_cast<rpc::server*>(getServer()))->
        bind("moveToZ", [&](float z, float velocity, float timeout_sec, const MultirotorRpcLibAdapators::YawMode& yaw_mode, 
            float lookahead, float adaptive_lookahead, const std::string& vehicle_name) -> bool {
        return getVehicleApi(vehicle_name)->moveToZ(z, velocity, timeout_sec, yaw_mode.to(), lookahead, adaptive_lookahead); 
    });
    (static_cast<rpc::server*>(getServer()))->
        bind("moveByManual", [&](float vx_max, float vy_max, float z_min, float duration, DrivetrainType drivetrain, 
            const MultirotorRpcLibAdapators::YawMode& yaw_mode, const std::string& vehicle_name) -> bool {
        return getVehicleApi(vehicle_name)->moveByManual(vx_max, vy_max, z_min, duration, drivetrain, yaw_mode.to()); 
    });

    (static_cast<rpc::server*>(getServer()))->
        bind("rotateToYaw", [&](float yaw, float timeout_sec, float margin, const std::string& vehicle_name) -> bool {
        return getVehicleApi(vehicle_name)->rotateToYaw(yaw, timeout_sec, margin); 
    });
    (static_cast<rpc::server*>(getServer()))->
        bind("rotateByYawRate", [&](float yaw_rate, float duration, const std::string& vehicle_name) -> bool {
        return getVehicleApi(vehicle_name)->rotateByYawRate(yaw_rate, duration); 
    });
    (static_cast<rpc::server*>(getServer()))->
        bind("hover", [&](const std::string& vehicle_name) -> bool {
        return getVehicleApi(vehicle_name)->hover(); 
    });
    (static_cast<rpc::server*>(getServer()))->
        bind("moveByRC", [&](const MultirotorRpcLibAdapators::RCData& data, const std::string& vehicle_name) -> void {
        getVehicleApi(vehicle_name)->moveByRC(data.to()); 
    });

    (static_cast<rpc::server*>(getServer()))->
        bind("setSafety", [&](uint enable_reasons, float obs_clearance, const SafetyEval::ObsAvoidanceStrategy& obs_startegy,
        float obs_avoidance_vel, const MultirotorRpcLibAdapators::Vector3r& origin, float xy_length, 
            float max_z, float min_z, const std::string& vehicle_name) -> bool {
        return getVehicleApi(vehicle_name)->setSafety(SafetyEval::SafetyViolationType(enable_reasons), obs_clearance, obs_startegy,
            obs_avoidance_vel, origin.to(), xy_length, max_z, min_z); 
    });

    //getters
    (static_cast<rpc::server*>(getServer()))->
        bind("getMultirotorState", [&](const std::string& vehicle_name) -> MultirotorRpcLibAdapators::MultirotorState {
        return MultirotorRpcLibAdapators::MultirotorState(getVehicleApi(vehicle_name)->getMultirotorState()); 
    });
}

//required for pimpl
MultirotorRpcLibServer::~MultirotorRpcLibServer()
{
}


}} //namespace


#endif
#endif
