// Copyright (c) 2026 Agile Robots SE
// SPDX-License-Identifier: MIT

#include "example_common.h"

#include <chrono>
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

    double start[6] = {0.0};
    if (getTcpPos(start, ip) < 0) {
        std::cerr << "getTcpPos failed\n";
        destroyRobot(ip);
        return 5;
    }

    double target[6];
    for (int i = 0; i < 6; ++i) target[i] = start[i];
    target[2] += 0.010;  // Base Z + 10 mm.

    std::cout << "Current TCP pose:\n";
    printPose(start);
    std::cout << "Example target (Base Z + 10 mm):\n";
    printPose(target);

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

    const double vel = 0.02;  // m/s
    const double acc = 0.05;  // m/s^2
    const bool avoid_singular = false;
    const int ret = moveLToPose(target, vel, acc, nullptr, 0, 0.0, 0.0,
                                avoid_singular, ip);
    if (ret < 0) {
        std::cerr << "moveLToPose failed, ret=" << ret << "\n";
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
