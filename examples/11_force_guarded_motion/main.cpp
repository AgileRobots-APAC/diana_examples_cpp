// Copyright (c) 2026 Agile Robots SE
// SPDX-License-Identifier: MIT

#include "example_common.h"

#include <chrono>
#include <iostream>
#include <thread>

using namespace diana_examples;

namespace {

constexpr double kGuardedMoveDurationSeconds = 10.0;
constexpr int kForcePollPeriodMilliseconds = 10;
constexpr double kForceThresholdNewtons = 5.0;

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

    std::cout << "Guarded move: base X at 0.01 m/s, stop at |Fx| >= "
              << kForceThresholdNewtons << " N or after 10 s.\n";
    if (!run) {
        std::cout << "DRY RUN\n";
        destroyRobot(ip);
        return 0;
    }

    double velocity[6] = {0.01, 0.0, 0.0, 0.0, 0.0, 0.0};
    double acceleration[2] = {0.02, 0.05};
    double force[6] = {0.0};
    if (releaseBrake(ip) < 0) {
        destroyRobot(ip);
        return 5;
    }
    std::this_thread::sleep_for(std::chrono::seconds(2));

    const int ret = speedL(velocity, acceleration, kGuardedMoveDurationSeconds, nullptr, ip);
    const int max_polls = static_cast<int>(kGuardedMoveDurationSeconds * 1000 / kForcePollPeriodMilliseconds);
    bool stopped_by_force = false;
    bool has_force_sample = false;
    double last_fx = 0.0;
    for (int i = 0; ret == 0 && i < max_polls; ++i) {
        if (getTcpForce(force, ip) == 0) {
            last_fx = force[0];
            has_force_sample = true;
            if (last_fx >= kForceThresholdNewtons || last_fx <= -kForceThresholdNewtons) {
                stopped_by_force = true;
                break;
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(kForcePollPeriodMilliseconds));
    }

    stop(ip);
    holdBrake(ip);
    destroyRobot(ip);

    if (ret < 0) {
        std::cerr << "Guarded move command failed, ret=" << ret << ".\n";
    } else if (stopped_by_force) {
        std::cout << "Stopped by force threshold: Fx=" << last_fx << " N (threshold=+/-"
                  << kForceThresholdNewtons << " N).\n";
    } else if (has_force_sample) {
        std::cout << "Stopped by normal timeout after " << kGuardedMoveDurationSeconds
                  << " s; last sampled Fx=" << last_fx << " N (threshold=+/-"
                  << kForceThresholdNewtons << " N).\n";
    } else {
        std::cout << "Stopped by normal timeout after " << kGuardedMoveDurationSeconds
                  << " s; no valid Fx sample was available.\n";
    }
    return ret < 0 ? 6 : 0;
}
