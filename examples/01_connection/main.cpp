// Copyright (c) 2026 Agile Robots SE
// SPDX-License-Identifier: MIT

#include "example_common.h"

#include <chrono>
#include <cstring>
#include <iostream>
#include <thread>

using namespace diana_examples;

int main(int argc, char* argv[]) {
    if (hasFlag(argc, argv, "--list-models")) {
        printSupportedRobotModels(std::cout);
        return 0;
    }

    const RobotProfile* profile = nullptr;
    if (!resolveRobotProfile(argc, argv, profile)) return 1;

    const char* ip = getRobotIp(argc, argv);
    const bool enable = hasFlag(argc, argv, "--enable");

    std::cout << "Robot IP: " << ip << "\n";
    if (profile) std::cout << "Requested model: " << profile->display_name << "\n";

    // 1) Initialize the network structure as recommended by the API manual.
    srv_net_st net_info;
    initSrvNetInfo(&net_info);
    std::strcpy(net_info.SrvIp, ip);

    // 2) Connect and register callbacks.
    const int init_ret = initSrv(errorCallback, stateCallback, &net_info);
    if (init_ret < 0) {
        std::cerr << "initSrv failed, ret=" << init_ret << "\n";
        return 2;
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(200));

    int joint_count = 0;
    if (!detectAndValidateRobot(ip, profile, joint_count)) {
        destroySrv(ip);
        return 3;
    }

    // 3) Read basic information.
    const unsigned short version = getLibraryVersion();
    std::cout << "Diana API library version(raw): " << version << "\n";

    const char state = getRobotState(ip);
    std::cout << "Robot state: " << static_cast<int>(state)
              << " (" << robotStateName(state) << ")\n";

    // 4) Brake operation is opt-in.
    if (enable) {
        std::cout << "Calling releaseBrake()...\n";
        const int ret = releaseBrake(ip);
        if (ret < 0) {
            std::cerr << "releaseBrake failed, ret=" << ret << "\n";
            destroySrv(ip);
            return 4;
        }
        std::this_thread::sleep_for(std::chrono::seconds(2));
        std::cout << "Calling holdBrake()...\n";
        holdBrake(ip);
    } else {
        std::cout << "Brake state is not changed. Use --enable to demonstrate releaseBrake/holdBrake.\n";
    }

    destroySrv(ip);
    return 0;
}
