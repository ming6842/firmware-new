#
#export abs path
export CMSIS=$(abspath $(WORKSPACE_DIR)/lib/CMSIS)
export ST=$(abspath $(WORKSPACE_DIR)/lib/STM32F4xx_StdPeriph_Driver)
export EXTERNAL_DEVICE=$(abspath $(WORKSPACE_DIR)/ext_device)
export MCU_PERIPH=$(abspath $(WORKSPACE_DIR)/mcu_periph)
export ESTIMATOR=$(abspath $(WORKSPACE_DIR)/estimator)
export ACTUATORS=$(abspath $(WORKSPACE_DIR)/actuators)
export COMMON=$(abspath $(WORKSPACE_DIR)/common)
export RADIO_CONTROLLER=$(abspath $(WORKSPACE_DIR)/radio_controller)
export CONTROLLER =$(abspath $(WORKSPACE_DIR)/controller)
export MAVLINK=$(abspath $(WORKSPACE_DIR)/mavlink)
export FREERTOS=$(abspath $(WORKSPACE_DIR)/lib/FreeRTOS)
export MAVLINK_LIB=$(abspath $(WORKSPACE_DIR)/lib/mavlink)
export TEST=$(abspath $(WORKSPACE_DIR)/test)

#
#inclue dir
BOARD_CONFIG=$(WORKSPACE_DIR)/vertigo_v2_config.h
DEBUG_CONFIG=$(WORKSPACE_DIR)/debug_config.h
