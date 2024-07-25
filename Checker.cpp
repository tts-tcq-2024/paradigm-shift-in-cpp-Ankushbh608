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

ParameterStatus getBreachStatus(float value, const ParameterRange& range) {
    if (value < range.lowBreach) {
        return LOW_BREACH;
    }
    if (value >= range.highBreach) {
        return HIGH_BREACH;
    }
    return NORMAL; // Not in breach
}

ParameterStatus getWarningStatus(float value, const ParameterRange& range) {
    if (value < range.lowWarning) {
        return LOW_WARNING;
    }
    if (value >= range.highWarning) {
        return HIGH_WARNING;
    }
    return NORMAL; // Normal if not in any warning range
}

ParameterStatus getParameterStatus(float value, const ParameterRange& range) {
    ParameterStatus breachStatus = getBreachStatus(value, range);
    if (breachStatus != NORMAL) {
        return breachStatus;  // Return if it's a breach
    }
    return getWarningStatus(value, range); // Check warning status if not a breach
}

std::string translateStatusToMessage(const std::string& parameter, ParameterStatus status) {
    static const std::string messages[] = {
        parameter + " is below the safe range!",   // LOW_BREACH
        "Warning: " + parameter + " is approaching discharge.", // LOW_WARNING
        parameter + " is normal.",                // NORMAL
        "Warning: " + parameter + " is approaching charge-peak.", // HIGH_WARNING
        parameter + " is above the safe range!"   // HIGH_BREACH
    };

    if (status < LOW_BREACH || status > HIGH_BREACH) {
        return "Unknown status for " + parameter;
    }

    return messages[status];
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

    assert(batteryIsOk(28, 74, 0.6) == true); 
    assert(batteryIsOk(48, 83, 0.4) == false);
    assert(batteryIsOk(-5, 52, 0.6) == false);
    assert(batteryIsOk(30, 10, 0.5) == false);
    assert(batteryIsOk(25, 60, 0.85) == false);
    assert(batteryIsOk(43, 76, 0.6) == true);
    assert(batteryIsOk(2, 22, 0.78) == true);

    std::cout << "All tests passed!\n";
    return 0;
}
