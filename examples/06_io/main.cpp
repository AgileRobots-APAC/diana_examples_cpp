// Copyright (c) 2026 Agile Robots SE
// SPDX-License-Identifier: MIT

#include "example_common.h"

#include <cstdlib>
#include <iostream>
#include <string>

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

    int di = 0;
    int dout = 0;
    int ai_mode = 0;
    int ao_mode = 0;
    double ai = 0.0;
    double ao = 0.0;

    if (readDI("board", "di0", di, ip) == 0)
        std::cout << "board/di0 = " << di << "\n";
    else
        std::cout << "readDI(board, di0) failed (signal may not exist in this configuration).\n";

    if (readDO("board", "do0", dout, ip) == 0)
        std::cout << "board/do0 = " << dout << "\n";
    else
        std::cout << "readDO(board, do0) failed (signal may not exist in this configuration).\n";

    if (readAI("board", "ai0", ai_mode, ai, ip) == 0)
        std::cout << "board/ai0 mode=" << ai_mode << " value=" << ai << "\n";

    if (readAO("board", "ao0", ao_mode, ao, ip) == 0)
        std::cout << "board/ao0 mode=" << ao_mode << " value=" << ao << "\n";

    double bus_current = 0.0;
    double bus_voltage = 0.0;
    if (readBusCurrent(bus_current, ip) == 0)
        std::cout << "Bus current: " << bus_current << " A\n";
    if (readBusVoltage(bus_voltage, ip) == 0)
        std::cout << "Bus voltage: " << bus_voltage << " V\n";

    const std::string prefix = "--set-do=";
    for (int i = 1; i < argc; ++i) {
        const std::string arg = argv[i];
        if (arg.compare(0, prefix.size(), prefix) == 0) {
            const int value = std::atoi(arg.substr(prefix.size()).c_str()) ? 1 : 0;
            std::cout << "Writing board/do0 = " << value << "\n";
            const int ret = writeDO("board", "do0", value, ip);
            if (ret < 0) std::cerr << "writeDO failed, ret=" << ret << "\n";
        }
    }

    const std::string ao_value = getOptionValue(argc, argv, "--set-ao");
    if (!ao_value.empty()) {
        const std::string::size_type colon = ao_value.find(':');
        if (colon == std::string::npos) {
            std::cerr << "Use --set-ao=<mode>:<value>, for example --set-ao=2:0.0\n";
        } else {
            const int mode = std::atoi(ao_value.substr(0, colon).c_str());
            const double output = std::atof(ao_value.substr(colon + 1).c_str());
            const int ret = writeAO("board", "ao0", mode, output, ip);
            std::cout << "writeAO(board, ao0) ret=" << ret << "\n";
        }
    }

    destroyRobot(ip);
    return 0;
}
