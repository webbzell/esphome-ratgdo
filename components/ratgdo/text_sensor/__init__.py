import esphome.codegen as cg
from esphome.components import text_sensor
import esphome.config_validation as cv
from esphome.const import CONF_ID
from esphome.types import ConfigType

from .. import RATGDO_CLIENT_SCHMEA, ratgdo_ns, register_ratgdo_child, validate_unique

DEPENDENCIES = ["ratgdo"]

RATGDOTextSensor = ratgdo_ns.class_(
    "RATGDOTextSensor", text_sensor.TextSensor, cg.Component
)


def validate_single_text_sensor(config: ConfigType) -> ConfigType:
    """Validate that only one RATGDO GDO-version text_sensor is configured."""
    validate_unique(
        "text_sensor", "ratgdo_gdo_version", "Only one RATGDO text_sensor is allowed"
    )
    return config


CONFIG_SCHEMA = cv.All(
    text_sensor.text_sensor_schema(RATGDOTextSensor)
    .extend(
        {
            cv.GenerateID(): cv.declare_id(RATGDOTextSensor),
        }
    )
    .extend(RATGDO_CLIENT_SCHMEA),
    validate_single_text_sensor,
)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await text_sensor.register_text_sensor(var, config)
    await cg.register_component(var, config)
    await register_ratgdo_child(var, config)
