// Copyright (c) 2026 Agile Robots SE
// SPDX-License-Identifier: MIT

#pragma once

#include "robot_profile.h"

#include <iosfwd>
#include <string>

namespace diana_examples {

// Current models covered by this example repository.
constexpr int kMaxSupportedJointCount = 7;

const RobotProfile* findRobotProfile(const std::string& model_key);
void printSupportedRobotModels(std::ostream& os);
const char* robotFamilyName(RobotFamily family);
const char* robotFamilyNameFromJointCount(int joint_count);

}  // namespace diana_examples
