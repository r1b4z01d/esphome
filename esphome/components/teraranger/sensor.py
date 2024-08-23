import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import i2c, sensor
from esphome.const import (
    STATE_CLASS_MEASUREMENT,
    UNIT_METER,
    ICON_ARROW_EXPAND_VERTICAL,
)

CODEOWNERS = ["@r1b4z01d"]
DEPENDENCIES = ["i2c"]

teraranger_ns = cg.esphome_ns.namespace("teraranger")
TERARANGERSensor = teraranger_ns.class_(
    "TERARANGERSensor", sensor.Sensor, cg.PollingComponent, i2c.I2CDevice
)

CONFIG_SCHEMA = (
    sensor.sensor_schema(
        TERARANGERSensor,
        unit_of_measurement=UNIT_METER,
        icon=ICON_ARROW_EXPAND_VERTICAL,
        accuracy_decimals=3,
        state_class=STATE_CLASS_MEASUREMENT,
    )
    .extend(cv.polling_component_schema("0.25s"))
    .extend(i2c.i2c_device_schema(0x31))
)


async def to_code(config):
    var = await sensor.new_sensor(config)
    await cg.register_component(var, config)
    await i2c.register_i2c_device(var, config)
