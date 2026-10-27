// Copyright (c) 2026 Agile Robots SE
// SPDX-License-Identifier: MIT

#include "example_common.h"

#include <iostream>

using namespace diana_examples;

int main(int argc, char* argv[]) {
    if (hasFlag(argc, argv, "--list-models")) { printSupportedRobotModels(std::cout); return 0; }
    const RobotProfile* profile = nullptr;
    if (!resolveRobotProfile(argc, argv, profile)) return 1;
    const char* ip = getRobotIp(argc, argv);
    if (!initRobot(ip)) return 2;
    int count = 0;
    if (!detectAndValidateRobot(ip, profile, count)) { destroyRobot(ip); return 3; }
    double min_pos[kMaxSupportedJointCount] = {0.0}, max_pos[kMaxSupportedJointCount] = {0.0};
    double max_vel[kMaxSupportedJointCount] = {0.0}, max_acc[kMaxSupportedJointCount] = {0.0};
    if (getJointsPositionRange(min_pos, max_pos, ip) == 0 && getMaxJointsVel(max_vel, ip) == 0 && getMaxJointsAcc(max_acc, ip) == 0) {
        std::cout << "Joint limits (rad, rad/s, rad/s^2):\n";
        for (int i = 0; i < count; ++i) std::cout << "  J" << i + 1 << ": [" << min_pos[i] << ", " << max_pos[i] << "], vel=" << max_vel[i] << ", acc=" << max_acc[i] << "\n";
    }
    double translation_vel = 0.0, translation_acc = 0.0, rotation_vel = 0.0, rotation_acc = 0.0;
    const bool has_cartesian_limits =
        getMaxCartTranslationVel(translation_vel, ip) == 0 &&
        getMaxCartTranslationAcc(translation_acc, ip) == 0 &&
        getMaxCartRotationVel(rotation_vel, ip) == 0 &&
        getMaxCartRotationAcc(rotation_acc, ip) == 0;
    if (has_cartesian_limits) {
        std::cout << "Cartesian limits: translation vel=" << translation_vel << " m/s, acc="
                  << translation_acc << " m/s^2; rotation vel=" << rotation_vel << " rad/s, acc="
                  << rotation_acc << " rad/s^2\n";
    }
    destroyRobot(ip);
    return 0;
}
