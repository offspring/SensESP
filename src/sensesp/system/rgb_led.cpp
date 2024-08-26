#include "rgb_led.h"

#include "pwm_output.h"

namespace sensesp {

RgbLed::RgbLed(int led_r_pin, int led_g_pin, int led_b_pin, String config_path,
               long int led_on_rgb, long int led_off_rgb, bool common_anode)
    : Configurable(config_path),
      led_r_channel_((led_r_pin < 0) ? -1
                                     : PWMOutput::assign_channel(led_r_pin)),
      led_g_channel_((led_g_pin < 0) ? -1
                                     : PWMOutput::assign_channel(led_g_pin)),
      led_b_channel_((led_b_pin < 0) ? -1
                                     : PWMOutput::assign_channel(led_b_pin)),
      led_on_rgb_{led_on_rgb},
      led_off_rgb_{led_off_rgb},
      common_anode_{common_anode} {
  this->load_configuration();
}

// Calculate the PWM value to send to analogWrite() based on the specified
// color value. When using analogWrite(), the closer to zero, the
// brighter the color. The closer to PWMRANGE, the darker the
// color.
static float get_pwm(int64_t rgb, int shift_right, bool common_anode) {
  int color_val = (rgb >> shift_right) & 0xFF;
  float const color_pct = color_val / 255.0;

  if (common_anode) {
    return 1.0 - color_pct;
  }
  return color_pct;
}

void RgbLed::set(const long& new_value) {
  if (led_r_channel_ >= 0) {
    float r = get_pwm(new_value, 16, common_anode_);
    PWMOutput::set_pwm(led_r_channel_, r);
  }

  if (led_g_channel_ >= 0) {
    float g = get_pwm(new_value, 8, common_anode_);
    PWMOutput::set_pwm(led_g_channel_, g);
  }

  if (led_b_channel_ >= 0) {
    float b = get_pwm(new_value, 0, common_anode_);
    PWMOutput::set_pwm(led_b_channel_, b);
  }
}

void RgbLed::set(const bool& new_value) {
  if (new_value) {
    set(led_on_rgb_);
  } else {
    set(led_off_rgb_);
  }
}

void RgbLed::get_configuration(JsonObject& root) {
  root["led_on_rgb"] = led_on_rgb_;
  root["led_off_rgb"] = led_off_rgb_;
}

static const char kSchema[] = R"({
    "type": "object",
    "properties": {
        "led_on_rgb": { "title": "RGB color for led ON", "type": "integer" },
        "led_off_rgb": { "title": "RGB color for led OFF", "type": "integer" }
    }
  })";

String RgbLed::get_config_schema() { return kSchema; }

bool RgbLed::set_configuration(const JsonObject& config) {
  String expected[] = {"led_on_rgb", "led_off_rgb"};
  for (auto str : expected) {
    if (!config.containsKey(str)) {
      return false;
    }
  }
  led_on_rgb_ = config["led_on_rgb"];
  led_off_rgb_ = config["led_off_rgb"];
  return true;
}

}  // namespace sensesp
