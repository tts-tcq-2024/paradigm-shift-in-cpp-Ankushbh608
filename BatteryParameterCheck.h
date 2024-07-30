#ifndef BATTERY_PARAMETER_CHECK_H
#define BATTERY_PARAMETER_CHECK_H

#include <string>

enum BreachStatus {
    NO_BREACH,
    LOW_BREACH,
    HIGH_BREACH
};

enum WarningStatus {
    NO_WARNING,
    LOW_WARNING,
    HIGH_WARNING
};

struct ParameterRange {
    float lowBreach;
    float lowWarning;
    float highWarning;
    float highBreach;
};

BreachStatus getBreachStatus(float value, const ParameterRange& range);
WarningStatus getWarningStatus(float value, const ParameterRange& range);
std::string statusToMessageTranslation(const std::string& parameter, BreachStatus breachStatus, WarningStatus warningStatus);

bool checkTemperature(float temperature, BreachStatus& breachStatus, WarningStatus& warningStatus);
bool checkSoc(float soc, BreachStatus& breachStatus, WarningStatus& warningStatus);
bool checkChargeRate(float chargeRate, BreachStatus& breachStatus, WarningStatus& warningStatus);
bool batteryIsOk(float temperature, float soc, float chargeRate);

#endif // BATTERY_PARAMETER_CHECK_H
