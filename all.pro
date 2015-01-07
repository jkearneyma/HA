!include( ./common.pri ) {
    error("Couldn't find the common.pri file!")
}

TEMPLATE = subdirs
SUBDIRS = \
    ha_jworks_service \
    ha_zwave_service \
    ha_timer_service \
    ha_gpio_service \
    ha_map_service

