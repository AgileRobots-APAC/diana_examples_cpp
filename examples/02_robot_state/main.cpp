// Copyright (c) 2026 Agile Robots SE
// SPDX-License-Identifier: MIT

#include "example_common.h"

#include <iomanip>
#include <iostream>

using namespace diana_examples;

namespace {

void printDiagnosticCode(const char* label, int code, const char* ip) {
    std::cout << label << ": " << code;
    if (code != 0) {
        const char* message = formatError(code, ip);
        std::cout << " (" << (message ? message : "unknown") << ")";
    }
    std::cout << "\n";
}

}  // namespace

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
    double speeds[kMaxSupportedJointCount] = {0.0};
    double currents[kMaxSupportedJointCount] = {0.0};
    double torques[kMaxSupportedJointCount] = {0.0};
     double oritorque[kMaxSupportedJointCount] = {0.0};
    double pose[6] = {0.0};

    if (getJointPos(joints, ip) == 0) {
        std::cout << "Joint position:\n";
        printJoints(joints, joint_count);
    }

    if (getJointAngularVel(speeds, ip) == 0) {
        std::cout << "Joint angular velocity (rad/s):\n";
        for (int i = 0; i < joint_count; ++i) std::cout << "  J" << i + 1 << ": " << speeds[i] << "\n";
    }

    if (getJointCurrent(currents, ip) == 0) {
        std::cout << "Joint current (A):\n";
        for (int i = 0; i < joint_count; ++i) std::cout << "  J" << i + 1 << ": " << currents[i] << "\n";
    }

    if (getJointTorque(torques, ip) == 0) {
        std::cout << "Joint torque (Nm):\n";
        for (int i = 0; i < joint_count; ++i) std::cout << "  J" << i + 1 << ": " << torques[i] << "\n";
    }

//     if (getOriginalJointTorque(oritorque, ip) == 0) {
//         std::cout << "Joint getOriginalJointTorque (Nm):\n";
//         for (int i = 0; i < joint_count; ++i) std::cout << "  J" << i + 1 << ": " << oritorque[i] << "\n";
//     }
//     else
//     {
//  std::cout << "Joint has no (Nm):\n";
//     }


    if (getTcpPos(pose, ip) == 0) {
        std::cout << "TCP pose:\n";
        printPose(pose);
    }

    const double resultant_force = getTcpExternalForce(ip);
    std::cout << "TCP external resultant force: " << resultant_force << " N\n";

    const char state = getRobotState(ip);
    std::cout << "Robot state: " << static_cast<int>(state)
              << " (" << robotStateName(state) << ")\n";

    std::cout << "\nDiagnostics:\n";
    const int link_state = getLinkState(ip);
    std::cout << "Link state: "
              << (link_state == 0 ? "connected" : "disconnected or unavailable")
              << " (ret=" << link_state << ")\n";
    printDiagnosticCode("Last error", getLastError(ip), ip);
    printDiagnosticCode("Last warning", getLastWarning(ip), ip);
    std::cout << "Collision detected: " << (isCollision(ip) ? "yes" : "no") << "\n";

    double tcp_force[6] = {0.0};
    if (getTcpForce(tcp_force, ip) == 0) {
        std::cout << std::fixed << std::setprecision(6)
                  << "TCP external wrench:\n"
                  << "  Fx: " << tcp_force[0] << " N\n"
                  << "  Fy: " << tcp_force[1] << " N\n"
                  << "  Fz: " << tcp_force[2] << " N\n"
                  << "  Mx: " << tcp_force[3] << " Nm\n"
                  << "  My: " << tcp_force[4] << " Nm\n"
                  << "  Mz: " << tcp_force[5] << " Nm\n";
    } else {
        std::cout << "TCP external wrench: unavailable (data may be invalid near a singularity).\n";
    }

    double joint_force[kMaxSupportedJointCount] = {0.0};
    if (getJointForce(joint_force, ip) == 0) {
        std::cout << "Joint external force/torque:\n";
        for (int i = 0; i < joint_count; ++i) {
            std::cout << "  J" << (i + 1) << ": " << joint_force[i] << "\n";
        }
    } else {
        std::cout << "Joint external force/torque: unavailable.\n";
    }

    destroyRobot(ip);
    return 0;
}
