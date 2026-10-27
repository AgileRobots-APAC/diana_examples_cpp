// Copyright (c) 2026 Agile Robots SE
// SPDX-License-Identifier: MIT

#include "example_common.h"

#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string>

using namespace diana_examples;

namespace {

const double kPi = 3.14159265358979323846;

bool parseOffset(const std::string& text, double offset[6]) {
    if (text.empty()) {
        offset[0] = 0.0; offset[1] = 0.0; offset[2] = 0.010;
        offset[3] = 0.0; offset[4] = 0.0; offset[5] = 0.0;
        return true;
    }
    std::stringstream stream(text);
    std::string item;
    for (int i = 0; i < 6; ++i) {
        if (!std::getline(stream, item, ',')) return false;
        offset[i] = std::atof(item.c_str());
    }
    return !std::getline(stream, item, ',');
}

void multiply4x4(const double lhs[16], const double rhs[16], double out[16]) {
    for (int row = 0; row < 4; ++row) for (int col = 0; col < 4; ++col) {
        out[row * 4 + col] = 0.0;
        for (int k = 0; k < 4; ++k) out[row * 4 + col] += lhs[row * 4 + k] * rhs[k * 4 + col];
    }
}

}  // namespace

int main(int argc, char* argv[]) {
    if (hasFlag(argc, argv, "--list-models")) { printSupportedRobotModels(std::cout); return 0; }
    const RobotProfile* profile = nullptr;
    if (!resolveRobotProfile(argc, argv, profile)) return 1;
    double offset[6] = {0.0};
    if (!parseOffset(getOptionValue(argc, argv, "--offset"), offset)) {
        std::cerr << "Use --offset=x,y,z,roll,pitch,yaw. Angles are degrees.\n";
        return 2;
    }
    const char* ip = getRobotIp(argc, argv);
    if (!initRobot(ip)) return 2;
    int joint_count = 0;
    if (!detectAndValidateRobot(ip, profile, joint_count)) { destroyRobot(ip); return 3; }

    double tcp_pose[6] = {0.0};
    if (getTcpPos(tcp_pose, ip) == 0) { std::cout << "Current TCP pose:\n"; printPose(tcp_pose); }
    double active_tcp_pose[6] = {0.0};
    if (getDefaultActiveTcpPose(active_tcp_pose, ip) == 0) {
        std::cout << "Default active TCP pose relative to flange:\n"; printPose(active_tcp_pose);
    }
    double active_tcp[16] = {0.0};
    if (getDefaultActiveTcp(active_tcp, ip) == 0) {
        std::cout << "Default active TCP homogeneous matrix:\n";
        for (int row = 0; row < 4; ++row) {
            std::cout << "  ";
            for (int col = 0; col < 4; ++col) std::cout << active_tcp[row * 4 + col] << (col == 3 ? '\n' : ' ');
        }
    }
    // The SDK writes into a 256-byte coordinate-name buffer (even for short names).
    char tcp_name[256] = {0};
    if (getDefaultTcpCoordinate(tcp_name, ip) == 0) std::cout << "Default TCP coordinate: " << tcp_name << "\n";
    char workpiece_name[256] = {0};
    if (getDefaultWorkPieceCoordinate(workpiece_name, ip) == 0) std::cout << "Default workpiece coordinate: " << workpiece_name << "\n";

    double payload[10] = {0.0};
    if (getActiveTcpPayload(payload, ip) == 0) {
        std::cout << "Active TCP payload:\n"
                  << "  mass: " << payload[0] << " kg\n"
                  << "  center of mass: " << payload[1] << ", " << payload[2] << ", " << payload[3] << " m\n"
                  << "  inertia: " << payload[4] << ", " << payload[5] << ", " << payload[6] << ", "
                  << payload[7] << ", " << payload[8] << ", " << payload[9] << "\n";
    }

    if (getTcpPos(tcp_pose, ip) == 0) {
        double offset_axis[6] = {offset[0], offset[1], offset[2], offset[3] * kPi / 180.0,
                                 offset[4] * kPi / 180.0, offset[5] * kPi / 180.0};
        double current_matrix[16] = {0.0}, offset_matrix[16] = {0.0}, target_matrix[16] = {0.0}, target_pose[6] = {0.0};
        if (rpy2Axis(offset_axis + 3) == 0 && pose2Homogeneous(tcp_pose, current_matrix) == 0 &&
            pose2Homogeneous(offset_axis, offset_matrix) == 0) {
            multiply4x4(current_matrix, offset_matrix, target_matrix);
            if (homogeneous2Pose(target_matrix, target_pose) == 0) {
                std::cout << "Tool-relative offset: " << offset[0] << ", " << offset[1] << ", " << offset[2]
                          << ", " << offset[3] << ", " << offset[4] << ", " << offset[5] << " (m, deg)\n"
                          << "Equivalent base-frame target (axis-angle):\n";
                printPose(target_pose);
            }
        }
    }
    std::cout << "Read-only example: no TCP, payload, or coordinate configuration is modified.\n";
    destroyRobot(ip);
    return 0;
}
