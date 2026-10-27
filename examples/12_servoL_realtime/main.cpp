// Copyright (c) 2026 Agile Robots SE
// SPDX-License-Identifier: MIT

#include "example_common.h"

#include <chrono>
#include <cmath>
#include <iostream>
#include <thread>

using namespace diana_examples;

namespace {

constexpr int kServoPeriodMilliseconds = 10;
constexpr int kServoDurationSeconds = 20;
constexpr int kServoCycleCount = kServoDurationSeconds * 1000 / kServoPeriodMilliseconds;
constexpr double kPi = 3.14159265358979323846;
constexpr double kZAmplitudeMeters = 0.05;

}  // namespace

int main(int argc, char* argv[]) {
    const RobotProfile* profile = nullptr;
    if (!resolveRobotProfile(argc, argv, profile)) return 1;

    const bool run = hasFlag(argc, argv, "--run");
    if (run && !profile) {
        std::cerr << "--run requires --model\n";
        return 2;
    }

    const char* ip = getRobotIp(argc, argv);
    if (!initRobot(ip)) return 3;

    int joint_count = 0;
    if (!detectAndValidateRobot(ip, profile, joint_count)) {
        destroyRobot(ip);
        return 4;
    }

    double start_pose[6] = {0.0};
    if (getTcpPos(start_pose, ip) < 0) {
        destroyRobot(ip);
        return 5;
    }

    std::cout << "Servo trajectory: base Z sine wave centered at the current TCP, +/-"
              << kZAmplitudeMeters * 1000.0 << " mm, " << kServoCycleCount << " cycles at "
              << kServoPeriodMilliseconds << " ms (" << kServoDurationSeconds << " s).\n";
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
    for (int i = 0; i < kServoCycleCount && ret == 0; ++i) {
        const double phase = 2.0 * kPi * i / (kServoCycleCount - 1);
        double target_pose[6] = {
            start_pose[0], start_pose[1], start_pose[2] + kZAmplitudeMeters * std::sin(phase),
            start_pose[3], start_pose[4], start_pose[5],
        };
        ret = servoL_ex(target_pose, 0.01, 0.01, 2000, 1.0, false, nullptr, ip);
        std::this_thread::sleep_for(std::chrono::milliseconds(kServoPeriodMilliseconds));
    }

    stop(ip);
    holdBrake(ip);
    destroyRobot(ip);
    return ret < 0 ? 7 : 0;
}
