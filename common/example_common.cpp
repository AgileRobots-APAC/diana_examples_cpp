// Copyright (c) 2026 Agile Robots SE
// SPDX-License-Identifier: MIT

#include "example_common.h"

#include <chrono>
#include <cstring>
#include <iomanip>
#include <iostream>
#include <thread>

namespace diana_examples {

std::string getOptionValue(int argc, char* argv[], const std::string& option) {
    const std::string prefix = option + "=";
    for (int i = 1; i < argc; ++i) {
        const std::string arg = argv[i];
        if (arg == option && i + 1 < argc) {
            return argv[i + 1];
        }
        if (arg.compare(0, prefix.size(), prefix) == 0) {
            return arg.substr(prefix.size());
        }
    }
    return std::string();
}

const char* getRobotIp(int argc, char* argv[], const char* default_ip) {
    // Preferred syntax: --ip 192.168.10.75 or --ip=192.168.10.75
    for (int i = 1; i < argc; ++i) {
        const std::string arg = argv[i];
        if (arg == "--ip" && i + 1 < argc) return argv[i + 1];
        if (arg.compare(0, 5, "--ip=") == 0) return argv[i] + 5;
    }

    // Backward-compatible positional IP. Skip option values.
    for (int i = 1; i < argc; ++i) {
        const std::string arg = argv[i];
        if (arg == "--model" || arg == "--ip") {
            ++i;
            continue;
        }
        if (!arg.empty() && arg[0] != '-') return argv[i];
    }
    return default_ip;
}

bool hasFlag(int argc, char* argv[], const std::string& flag) {
    for (int i = 1; i < argc; ++i) {
        if (argv[i] == flag) return true;
    }
    return false;
}

bool resolveRobotProfile(int argc, char* argv[], const RobotProfile*& profile) {
    profile = nullptr;
    const std::string model = getOptionValue(argc, argv, "--model");
    if (model.empty()) return true;

    profile = findRobotProfile(model);
    if (!profile) {
        std::cerr << "Unknown robot model: " << model << "\n";
        printSupportedRobotModels(std::cerr);
        return false;
    }
    return true;
}

void errorCallback(int error_code, const char* ip) {
    const char* msg = formatError(error_code, ip ? ip : "");
    std::cerr << "[Diana API error] code=" << error_code
              << " ip=" << (ip ? ip : "")
              << " message=" << (msg ? msg : "unknown") << std::endl;
}

void stateCallback(StrRobotStateInfo*, const char*) {
    // Keep callbacks non-blocking. Query examples use the API's cached getter functions.
}

bool initRobot(const char* ip) {
    srv_net_st net_info;
    initSrvNetInfo(&net_info);
    std::strcpy(net_info.SrvIp, ip);

    const int ret = initSrv(errorCallback, stateCallback, &net_info);
    if (ret < 0) {
        std::cerr << "initSrv failed, ret=" << ret << ", ip=" << ip << std::endl;
        return false;
    }

    // Give the background state receiver a short time to populate cached data.
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    return true;
}

void destroyRobot(const char* ip) {
    const int ret = destroySrv(ip);
    if (ret < 0) {
        std::cerr << "destroySrv failed, ret=" << ret << std::endl;
    }
}

bool detectAndValidateRobot(const char* ip,
                            const RobotProfile* profile,
                            int& joint_count) {
    joint_count = getJointCount(ip);
    if (joint_count <= 0 || joint_count > kMaxSupportedJointCount) {
        std::cerr << "getJointCount failed or returned unsupported count: "
                  << joint_count << "\n";
        return false;
    }

    std::cout << "Detected joint count: " << joint_count
              << " (" << robotFamilyNameFromJointCount(joint_count) << ")\n";

    if (!profile) {
        std::cout << "Exact model: not selected. Use --model <name> to select and validate it.\n";
        return true;
    }

    std::cout << "Selected model: " << profile->display_name
              << " (expected " << profile->joint_count << " axes)\n";

    if (joint_count != profile->joint_count) {
        std::cerr << "Robot model mismatch: selected " << profile->display_name
                  << " expects " << profile->joint_count
                  << " axes, but the connected robot reports " << joint_count << ".\n";
        return false;
    }
    return true;
}

void waitMove(const char* ip) {
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    while (getRobotState(ip) == 0) {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

void printJoints(const double* joints, int count) {
    std::cout << std::fixed << std::setprecision(6);
    for (int i = 0; i < count; ++i) {
        std::cout << "  J" << (i + 1) << ": " << joints[i] << " rad\n";
    }
}

void printPose(const double pose[6]) {
    std::cout << std::fixed << std::setprecision(6)
              << "  x : " << pose[0] << " m\n"
              << "  y : " << pose[1] << " m\n"
              << "  z : " << pose[2] << " m\n"
              << "  rx: " << pose[3] << " rad\n"
              << "  ry: " << pose[4] << " rad\n"
              << "  rz: " << pose[5] << " rad\n";
}

const char* robotStateName(char state) {
    switch (state) {
        case 0: return "running";
        case 1: return "paused";
        case 2: return "idle";
        case 3: return "free-driving";
        case 4: return "zero-space-free-driving";
        case 5: return "hold-brake";
        case 6: return "error";
        case 7: return "handling-error";
        case 8: return "rescue-mode";
        default: return "unknown";
    }
}

}  // namespace diana_examples
