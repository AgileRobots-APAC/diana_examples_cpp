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
    const bool run = hasFlag(argc, argv, "--run");
    const bool complex = hasFlag(argc, argv, "--complex");
    if (run && !profile) {
        std::cerr << "For safety, --run requires an explicit --model.\n";
        return 2;
    }
    const char* ip = getRobotIp(argc, argv);
    if (!initRobot(ip)) return 3;
    int count = 0;
    if (!detectAndValidateRobot(ip, profile, count)) {
        destroyRobot(ip);
        return 4;
    }
    double start[kMaxSupportedJointCount] = {0.0};
    double target[kMaxSupportedJointCount] = {0.0};
    if (getJointPos(start, ip) < 0) {
        destroyRobot(ip);
        return 5;
    }
    for (int i = 0; i < count; ++i) target[i] = start[i];
    const double deg2rad = 3.14159265358979323846 / 180.0;
    target[0] += 10.0 * deg2rad;
    target[0] += complex ? 5.0 * deg2rad : 0.0;
    std::cout << (complex ? "Complex-path" : "Path") << " waypoint 1 (J1 + "
              << (complex ? 15 : 10) << " deg):\n";
    printJoints(target, count);
    std::cout << "Path waypoint 2 (return to current joints):\n";
    printJoints(start, count);
    if (!run) {
        std::cout << "DRY RUN: no " << (complex ? "complex path" : "path")
                  << " is created or executed. Add --model <name> --run after checking the workspace.\n";
        destroyRobot(ip);
        return 0;
    }
    if (releaseBrake(ip) < 0) {
        destroyRobot(ip);
        return 6;
    }
    std::this_thread::sleep_for(std::chrono::seconds(2));
    unsigned int path_id = 0;
    int ret = complex ? createComplexPath(0, path_id, ip) : createPath(1, path_id, ip);
    if (ret == 0 && complex) ret = addMoveJByTarget(path_id, target, 0.10, 0.20, 0.0, 0, 0.0, 0.0, ip);
    if (ret == 0 && complex) ret = addMoveJByTarget(path_id, start, 0.10, 0.20, 0.0, 0, 0.0, 0.0, ip);
    if (ret == 0 && !complex) ret = addMoveJ(path_id, target, 0.10, 0.20, 0.0, ip);
    if (ret == 0 && !complex) ret = addMoveJ(path_id, start, 0.10, 0.20, 0.0, ip);
    if (ret == 0) ret = complex ? runComplexPath(path_id, ip) : runPath(path_id, ip);
    if (ret < 0) {
        std::cerr << "Path setup or execution failed, ret=" << ret << "\n";
        stop(ip);
    } else {
        waitMove(ip);
        std::cout << "Path finished.\n";
    }
    if (complex) {
        destroyComplexPath(path_id, ip);
    } else {
        destroyPath(path_id, ip);
    }
    holdBrake(ip);
    destroyRobot(ip);
    return ret < 0 ? 7 : 0;
}
