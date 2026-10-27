// Copyright (c) 2026 Agile Robots SE
// SPDX-License-Identifier: MIT

#pragma once

namespace diana_examples {

enum class RobotFamily {
    Diana,
    Thor
};

struct RobotProfile {
    RobotFamily family;
    const char* key;          // CLI key, for example: diana7 / thor12
    const char* display_name; // Human-readable model name
    int joint_count;          // Diana series: 7, Thor series: 6
};

}  // namespace diana_examples
