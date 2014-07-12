#ifndef FILE_FLIGHT_CONTROLLER_H
#define FILE_FLIGHT_CONTROLLER_H

#include <stdio.h>
#include "attitude_estimator.h"
#include "vertical_estimator.h"
#include "estimator.h"
#include "controller.h"
#include "pwm.h"
#include "radio_control.h"
#include "test_common.h"
#include "hmc5983.h"
#include "lea6h_ubx.h"
#include "usart.h"

#include "FreeRTOS.h"
#include "task.h"

#include "global.h"

void flight_control_task(void);




#endif