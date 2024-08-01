#include "BatteryParametersCheck.h"
#include <iostream>

WarningStatus getWarningStatus(float value, const ParameterRange& range) {
    if (value < range.lowWarning) {
        return LOW_WARNING;
    }
    if (value >= range.highWarning) {
        return HIGH_WARNING;
    }
    return NO_WARNING; // Normal if not in any warning range
}

BreachStatus getBreachStatus(float value, const ParameterRange& range) {
    if (value < range.lowBreach) {
        return LOW_BREACH;
    }
    if (value >= range.highBreach) {
        return HIGH_BREACH;
    }
    return NO_BREACH; // Not in breach
}

std::string getBreachMessage(const std::string& parameter, BreachStatus breachStatus) {
    if (breachStatus == LOW_BREACH) {
        return parameter + " is below the safe range!";
    }
    if (breachStatus == HIGH_BREACH) {
        return parameter + " is above the safe range!";
    }
    return parameter + " is normal.";
}

std::string getWarningMessage(const std::string& parameter, WarningStatus warningStatus) {
    if (warningStatus == LOW_WARNING) {
        return "Warning: " + parameter + " is approaching discharge.";
    }
    if (warningStatus == HIGH_WARNING) {
        return "Warning: " + parameter + " is approaching charge-peak.";
    }
    return parameter + " is normal.";
}

std::string statusToMessageTranslation(const std::string& parameter, BreachStatus breachStatus, WarningStatus warningStatus) {
    std::string message = getWarningMessage(parameter, warningStatus);
    if (message.empty()) {
        message = getBreachMessage(parameter, breachStatus);
    }
    if (message.empty()) {
        message = parameter + " is normal.";
    }
    return message;
}

bool checkTemperature(float temperature, BreachStatus& breachStatus, WarningStatus& warningStatus) {
    ParameterRange temperatureRange = {0, 0 + 45 * 0.05, 45 - 45 * 0.05, 45};
    warningStatus = getWarningStatus(temperature, temperatureRange);
    breachStatus = getBreachStatus(temperature, temperatureRange);
    return breachStatus == NO_BREACH;
}

bool checkSoc(float soc, BreachStatus& breachStatus, WarningStatus& warningStatus) {
    ParameterRange socRange = {20, 20 + 80 * 0.05, 80 - 80 * 0.05, 80};
    warningStatus = getWarningStatus(soc, socRange);
    breachStatus = getBreachStatus(soc, socRange);
    return breachStatus == NO_BREACH;
}

bool checkChargeRate(float chargeRate, BreachStatus& breachStatus, WarningStatus& warningStatus) {
    ParameterRange chargeRateRange = {0, 0 + 0.8 * 0.05, 0.8 - 0.8 * 0.05, 0.8};
    warningStatus = getWarningStatus(chargeRate, chargeRateRange);
    breachStatus = getBreachStatus(chargeRate, chargeRateRange);
    return breachStatus == NO_BREACH;
}

bool batteryIsOk(float temperature, float soc, float chargeRate) {
    BreachStatus tempBreachStatus, socBreachStatus, chargeRateBreachStatus;
    WarningStatus tempWarningStatus, socWarningStatus, chargeRateWarningStatus;

    bool tempOk = checkTemperature(temperature, tempBreachStatus, tempWarningStatus);
    bool socOk = checkSoc(soc, socBreachStatus, socWarningStatus);
    bool chargeRateOk = checkChargeRate(chargeRate, chargeRateBreachStatus, chargeRateWarningStatus);

    std::cout << statusToMessageTranslation("Temperature", tempBreachStatus, tempWarningStatus) << std::endl;
    std::cout << statusToMessageTranslation("State of Charge", socBreachStatus, socWarningStatus) << std::endl;
    std::cout << statusToMessageTranslation("Charge Rate", chargeRateBreachStatus, chargeRateWarningStatus) << std::endl;

    return tempOk && socOk && chargeRateOk;
}
