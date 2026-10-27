// Copyright (c) 2026 Agile Robots SE
// SPDX-License-Identifier: MIT

#include "example_common.h"

#include <iomanip>
#include <iostream>

using namespace diana_examples;

int main(int argc, char* argv[]) {
    if (hasFlag(argc, argv, "--list-models")) {
        printSupportedRobotModels(std::cout);
        return 0;
    }

    const RobotProfile* profile = nullptr;
    if (!resolveRobotProfile(argc, argv, profile)) return 1;

    const char* ip = getRobotIp(argc, argv);
    if (!initRobot(ip)) return 2;

    int joint_count = 0;
    if (!detectAndValidateRobot(ip, profile, joint_count)) {
        destroyRobot(ip);
        return 3;
    }

    double joints[kMaxSupportedJointCount] = {0.0};
    if (getJointPos(joints, ip) < 0) {
        std::cerr << "getJointPos failed\n";
        destroyRobot(ip);
        return 4;
    }

    double pose[6] = {0.0};
    if (forward(joints, pose, nullptr, ip) == 0) {
        std::cout << "Forward kinematics result:\n";
        printPose(pose);
    }

    double inverse_joints[kMaxSupportedJointCount] = {0.0};
    if (inverse(pose, inverse_joints, nullptr, ip) == 0) {
        std::cout << "Inverse kinematics result:\n";
        printJoints(inverse_joints, joint_count);
    }

    double reference_solution[kMaxSupportedJointCount] = {0.0};
    if (inverse_ext(joints, pose, reference_solution, nullptr, ip) == 0) {
        std::cout << "Inverse solution near current joints (inverse_ext):\n";
        printJoints(reference_solution, joint_count);
    } else {
        std::cout << "inverse_ext failed.\n";
    }

    const double pi = 3.14159265358979323846;
    double rotation[3] = {pi, pi / 3.0, pi / 6.0};
    std::cout << std::fixed << std::setprecision(6)
              << "RPY input: " << rotation[0] << ", " << rotation[1] << ", " << rotation[2] << "\n";

    if (rpy2Axis(rotation) == 0) {
        std::cout << "Axis-angle vector: " << rotation[0] << ", " << rotation[1] << ", " << rotation[2] << "\n";
    }

    if (axis2RPY(rotation) == 0) {
        std::cout << "Converted back to RPY: " << rotation[0] << ", " << rotation[1] << ", " << rotation[2] << "\n";
    }

    destroyRobot(ip);
    return 0;
}
