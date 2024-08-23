#include "teraranger_sensor.h"
#include "esphome/core/log.h"
#include "esphome/core/hal.h"
#include <cinttypes>

// Very basic support for Teraranger distance sensor

namespace esphome {
namespace teraranger {

static const char *const TAG = "teraranger";
static const uint8_t TERARANGER_READ_DISTANCE_CMD[] = {0x00};
static const uint8_t TERARANGER_DEFAULT_DELAY = 30;

static const uint8_t TERARANGER_DISTANCE_REGISTER = 0x00;

static const uint16_t TERARANGER_OUT_OF_RANGE_VALUE = 15000;

void TERARANGERSensor::dump_config() {
  LOG_SENSOR("", "TERARANGER", this);
  LOG_UPDATE_INTERVAL(this);
  LOG_I2C_DEVICE(this);
}

void TERARANGERSensor::setup() {
}

void TERARANGERSensor::update() {
  if (!this->write_bytes(TERARANGER_DISTANCE_REGISTER, TERARANGER_READ_DISTANCE_CMD, sizeof(TERARANGER_READ_DISTANCE_CMD))) {
    ESP_LOGE(TAG, "Communication with TERARANGER failed on write");
    this->status_set_warning();
    return;
  }

  uint8_t data[2];
  if (this->write(&TERARANGER_DISTANCE_REGISTER, 1) != i2c::ERROR_OK) {
    this->status_set_warning();
    return;
  }

  delay(TERARANGER_DEFAULT_DELAY);

  if (this->read(data, 3) != i2c::ERROR_OK) {
    ESP_LOGE(TAG, "Communication with TERARANGER failed on read");
    this->status_set_warning();
    return;
  }

  uint32_t distance_mm = (data[0] << 8) + data[1];

  ESP_LOGI(TAG, "Data read: %" PRIu32 "mm", distance_mm);

  if (distance_mm == TERARANGER_OUT_OF_RANGE_VALUE) {
    ESP_LOGW(TAG, "Distance measurement out of range");
    this->publish_state(NAN);
  } else {
    this->publish_state(distance_mm / 1000.0f);
  }
  this->status_clear_warning();
}

}  // namespace teraranger
}  // namespace esphome
