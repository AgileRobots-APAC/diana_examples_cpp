// Copyright (c) 2026 Agile Robots SE
// SPDX-License-Identifier: MIT

#include "example_common.h"

#include <chrono>
#include <cmath>
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
    const bool run = hasFlag(argc, argv, "--run");

    if (run && !profile) {
        std::cerr << "For safety, --run requires an explicit --model.\n";
        printSupportedRobotModels(std::cerr);
        return 2;
    }

    if (!initRobot(ip)) return 3;

    int joint_count = 0;
    if (!detectAndValidateRobot(ip, profile, joint_count)) {
        destroyRobot(ip);
        return 4;
    }

    double start[kMaxSupportedJointCount] = {0.0};
    if (getJointPos(start, ip) < 0) {
        std::cerr << "getJointPos failed\n";
        destroyRobot(ip);
        return 5;
    }

    double target[kMaxSupportedJointCount] = {0.0};
    for (int i = 0; i < joint_count; ++i) target[i] = start[i];

    const double deg2rad = 3.14159265358979323846 / 180.0;
    target[0] += 10.0 * deg2rad;  // Relative J1 demonstration; no model-specific home pose assumed.

    std::cout << "Current joints:\n";
    printJoints(start, joint_count);
    std::cout << "Example target (J1 + 10 deg):\n";
    printJoints(target, joint_count);

    if (!run) {
        std::cout << "DRY RUN: no motion command sent. Add --model <name> --run after checking the workspace.\n";
        destroyRobot(ip);
        return 0;
    }

    if (releaseBrake(ip) < 0) {
        std::cerr << "releaseBrake failed\n";
        destroyRobot(ip);
        return 6;
    }
    std::this_thread::sleep_for(std::chrono::seconds(2));

    const double vel = 0.10;  // rad/s
    const double acc = 0.20;  // rad/s^2
    const int ret = moveJToTarget(target, vel, acc, 0, 0.0, 0.0, ip);
    if (ret < 0) {
        std::cerr << "moveJToTarget failed, ret=" << ret << "\n";
        stop(ip);
        holdBrake(ip);
        destroyRobot(ip);
        return 7;
    }

    waitMove(ip);
    std::cout << "Motion finished.\n";

    holdBrake(ip);
    destroyRobot(ip);
    return 0;
}
