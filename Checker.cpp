#include <cassert>
#include <iostream>
#include <string>

enum ParameterStatus {
    LOW_BREACH,
    LOW_WARNING,
    NORMAL,
    HIGH_WARNING,
    HIGH_BREACH
};

struct ParameterRange {
    float lowBreach;
    float lowWarning;
    float highWarning;
    float highBreach;
};

ParameterStatus getParameterStatus(float value, const ParameterRange& range) {
    if (value < range.lowBreach) {
        return LOW_BREACH;
    } else if (value < range.lowWarning) {
        return LOW_WARNING;
    } else if (value < range.highWarning) {
        return NORMAL;
    } else if (value < range.highBreach) {
        return HIGH_WARNING;
    } else {
        return HIGH_BREACH;
    }
}

std::string translateStatusToMessage(const std::string& parameter, ParameterStatus status) {
    switch (status) {
        case LOW_BREACH:
            return parameter + " is below the safe range!";
        case LOW_WARNING:
            return "Warning: " + parameter + " is approaching discharge.";
        case NORMAL:
            return parameter + " is normal.";
        case HIGH_WARNING:
            return "Warning: " + parameter + " is approaching charge-peak.";
        case HIGH_BREACH:
            return parameter + " is above the safe range!";
        default:
            return "Unknown status for " + parameter;
    }
}

bool checkTemperature(float temperature) {
    ParameterRange temperatureRange = {0, 0 + 45 * 0.05, 45 - 45 * 0.05, 45};
    ParameterStatus status = getParameterStatus(temperature, temperatureRange);
    std::cout << translateStatusToMessage("Temperature", status) << std::endl;
    return status != LOW_BREACH && status != HIGH_BREACH;
}

bool checkSoc(float soc) {
    ParameterRange socRange = {20, 20 + 80 * 0.05, 80 - 80 * 0.05, 80};
    ParameterStatus status = getParameterStatus(soc, socRange);
    std::cout << translateStatusToMessage("State of Charge", status) << std::endl;
    return status != LOW_BREACH && status != HIGH_BREACH;
}

bool checkChargeRate(float chargeRate) {
    ParameterRange chargeRateRange = {0, 0, 0.8 - 0.8 * 0.05, 0.8};
    ParameterStatus status = getParameterStatus(chargeRate, chargeRateRange);
    std::cout << translateStatusToMessage("Charge Rate", status) << std::endl;
    return status != LOW_BREACH && status != HIGH_BREACH;
}

bool batteryIsOk(float temperature, float soc, float chargeRate) {
    bool tempOk = checkTemperature(temperature);
    bool socOk = checkSoc(soc);
    bool chargeRateOk = checkChargeRate(chargeRate);
    return tempOk && socOk && chargeRateOk;
}

int main() {
    // Test cases to verify battery safety checks and warning messages
    assert(batteryIsOk(25, 70, 0.7) == true);  // Within normal range
    assert(batteryIsOk(50, 85, 0) == false);   // Temperature and SoC out of range
    assert(batteryIsOk(-1, 50, 0.5) == false); // Temperature out of range
    assert(batteryIsOk(25, 10, 0.5) == false); // SoC out of range
    assert(batteryIsOk(25, 50, 0.9) == false); // Charge rate out of range
    assert(batteryIsOk(42, 78, 0.6) == true);  // Within warning range
    assert(batteryIsOk(1, 21, 0.76) == true);  // Within warning range

    std::cout << "All tests passed!\n";
    return 0;
}
