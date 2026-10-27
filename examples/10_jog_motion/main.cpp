// Copyright (c) 2026 Agile Robots SE
// SPDX-License-Identifier: MIT

#include "example_common.h"

#include <chrono>
#include <cstdlib>
#include <iostream>
#include <string>
#include <thread>

using namespace diana_examples;

namespace {

enum class JogMode {
    Joint,
    Tcp,
    Rotate,
};

bool parseJogMode(const std::string& value, JogMode& mode) {
    if (value.empty() || value == "joint") {
        mode = JogMode::Joint;
        return true;
    }
    if (value == "tcp") {
        mode = JogMode::Tcp;
        return true;
    }
    if (value == "rotate") {
        mode = JogMode::Rotate;
        return true;
    }
    return false;
}

}  // namespace

int main(int argc, char* argv[]) {
    const RobotProfile* profile = nullptr;
    if (!resolveRobotProfile(argc, argv, profile)) return 1;

    const bool run = hasFlag(argc, argv, "--run");
    if (run && !profile) {
        std::cerr << "--run requires --model\n";
        return 2;
    }

    JogMode mode = JogMode::Joint;
    if (!parseJogMode(getOptionValue(argc, argv, "--mode"), mode)) {
        std::cerr << "--mode must be joint, tcp, or rotate\n";
        return 2;
    }

    const char* ip = getRobotIp(argc, argv);
    if (!initRobot(ip)) return 3;

    int joint_count = 0;
    if (!detectAndValidateRobot(ip, profile, joint_count)) {
        destroyRobot(ip);
        return 4;
    }

    double seconds = 0.5;
    const std::string duration = getOptionValue(argc, argv, "--duration");
    if (!duration.empty()) seconds = std::atof(duration.c_str());
    if (seconds < 0.1 || seconds > 2.0) {
        std::cerr << "--duration must be 0.1..2.0 seconds\n";
        destroyRobot(ip);
        return 5;
    }

    if (mode == JogMode::Joint) {
        std::cout << "Jog J1 positive for " << seconds << " s at low speed.\n";
    } else if (mode == JogMode::Tcp) {
        std::cout << "Jog TCP along base +X for " << seconds << " s at low speed.\n";
    } else {
        std::cout << "Rotate TCP about base +Z for " << seconds << " s at low speed.\n";
    }
    if (!run) {
        std::cout << "DRY RUN\n";
        destroyRobot(ip);
        return 0;
    }

    if (releaseBrake(ip) < 0) {
        destroyRobot(ip);
        return 6;
    }
    std::this_thread::sleep_for(std::chrono::seconds(2));

    int ret = 0;
    if (mode == JogMode::Joint) {
        ret = moveJoint(T_MOVE_UP, 0, 0.05, 0.10, ip);
    } else if (mode == JogMode::Tcp) {
        ret = moveTcp_ex(E_BASE_COORDINATE, T_MOVE_X_UP, 0.01, 0.02, ip);
    } else {
        ret = rotationTCP_ex(E_BASE_COORDINATE, T_MOVE_Z_UP, 0.10, 0.20, ip);
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<int>(seconds * 1000)));
    stop(ip);
    holdBrake(ip);
    destroyRobot(ip);
    return ret < 0 ? 7 : 0;
}
