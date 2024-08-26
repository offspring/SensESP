#include "voltage_multiplier.h"

namespace sensesp {

VoltageMultiplier::VoltageMultiplier(uint16_t R1, uint16_t R2,
                                     const String& config_path)
    : FloatTransform(config_path), R1_{R1}, R2_{R2} {}

void VoltageMultiplier::set(const float& input) {
  // Ohms Law at work!
  this->emit(input * (((float)R1_ + (float)R2_) / (float)R2_));
}

void VoltageMultiplier::get_configuration(JsonObject& root) {
  root["R1"] = R1_;
  root["R2"] = R2_;
};

static const char kSchema[] = R"###({
    "type": "object",
    "properties": {
        "R1": { "title": "R1", "type": "number", "description": "The measured value of resistor R1" },
        "R2": { "title": "R2", "type": "number", "description": "The measured value of resistor R2" }
    }
  })###";

String VoltageMultiplier::get_config_schema() { return (kSchema); }

bool VoltageMultiplier::set_configuration(const JsonObject& config) {
  String const expected[] = {"R1", "R2"};
  for (auto str : expected) {
    if (!config.containsKey(str)) {
      return false;
    }
  }
  R1_ = config["R1"];
  R2_ = config["R2"];
  return true;
}

}  // namespace sensesp
