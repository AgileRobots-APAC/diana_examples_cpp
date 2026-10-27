// Copyright (c) 2026 Agile Robots SE
// SPDX-License-Identifier: MIT

#pragma once

#include "DianaAPI.h"
#include "robot_registry.h"

#include <string>

namespace diana_examples {

std::string getOptionValue(int argc, char* argv[], const std::string& option);
const char* getRobotIp(int argc, char* argv[], const char* default_ip = "192.168.10.75");
bool hasFlag(int argc, char* argv[], const std::string& flag);

// Resolves --model. Returns false only when the user supplied an unknown model.
bool resolveRobotProfile(int argc, char* argv[], const RobotProfile*& profile);

void errorCallback(int error_code, const char* ip);
void stateCallback(StrRobotStateInfo* state, const char* ip);

bool initRobot(const char* ip);
void destroyRobot(const char* ip);

// Calls getJointCount() and, when a model is selected, validates that the
// connected robot has the expected number of axes.
bool detectAndValidateRobot(const char* ip,
                            const RobotProfile* profile,
                            int& joint_count);

// Wait until the controller leaves the running state. The API manual's sample
// uses getRobotState() to poll for motion completion.
void waitMove(const char* ip);

void printJoints(const double* joints, int count);
void printPose(const double pose[6]);
const char* robotStateName(char state);

}  // namespace diana_examples
