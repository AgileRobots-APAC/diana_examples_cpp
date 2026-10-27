// Copyright (c) 2026 Agile Robots SE
// SPDX-License-Identifier: MIT

#include "robot_registry.h"

#include "diana/diana3.h"
#include "diana/diana7.h"
#include "diana/diana7_g2.h"
#include "thor/thor7.h"
// #include "thor/thor7_pro.h"  // Temporarily disabled: Thor7 Pro is unavailable.
#include "thor/thor12.h"
#include "thor/thor20.h"

#include <algorithm>
#include <cctype>
#include <ostream>

namespace diana_examples {
namespace {

std::string normalize(std::string value) {
    std::transform(value.begin(), value.end(), value.begin(), [](unsigned char c) {
        if (c == '_') return '-';
        return static_cast<char>(std::tolower(c));
    });
    return value;
}

const RobotProfile* const kProfiles[] = {
    &kDiana3,
    &kDiana7,
    &kDiana7G2,
    &kThor7,
    // &kThor7Pro,  // Temporarily disabled: Thor7 Pro is unavailable.
    &kThor12,
    &kThor20,
};

}  // namespace

const RobotProfile* findRobotProfile(const std::string& model_key) {
    const std::string key = normalize(model_key);

    for (const RobotProfile* profile : kProfiles) {
        if (key == profile->key) return profile;
    }

    // Friendly aliases.
    if (key == "diana7g2") return &kDiana7G2;
    // if (key == "thor7pro") return &kThor7Pro;  // Temporarily disabled.
    return nullptr;
}

void printSupportedRobotModels(std::ostream& os) {
    os << "Supported robot models:\n"
       << "  Diana series (7 axes):\n"
       << "    diana3\n"
       << "    diana7\n"
       << "    diana7-g2\n"
       << "  Thor series (6 axes):\n"
       << "    thor7\n"
       // << "    thor7-pro\n"  // Temporarily disabled: Thor7 Pro is unavailable.
       << "    thor12\n"
       << "    thor20\n";
}

const char* robotFamilyName(RobotFamily family) {
    return family == RobotFamily::Diana ? "Diana series" : "Thor series";
}

const char* robotFamilyNameFromJointCount(int joint_count) {
    if (joint_count == 7) return "Diana series";
    if (joint_count == 6) return "Thor series";
    return "Unknown series";
}

}  // namespace diana_examples
