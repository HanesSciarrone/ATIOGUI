/*
 * ModuleControllerPump.h
 *
 *  Created on: Jan 10, 2021
 *      Author: Hanes
 */

#ifndef INC_MODULECONTROLLERPUMP_H_
#define INC_MODULECONTROLLERPUMP_H_

enum {
	RESTART_PUMP,
	DISPACHE_PUMP,
	STOP_PUMP,
	STATE_PUMP
};

enum type_fuel_t {
	REGULAR,
	PREMIUM,
	REGULAR_DIESEL,
	PREMIUM_DIESEL
};

typedef enum {
	PUMP_ERROR,
	PUMP_OK,
	PUMP_INSUFFICIENT_FUEL,
	PUMP_EMPTHY,
	PUMP_DISPACHING,
	PUMP_FINISH_CHARGE
}state_pump_t;

/**
 * @brief Initialize task and methods of synchronize as queues, mutexes and
 * semaphores.
 *
 * @return Return 1 if operation was success or 0 in ohrerwise.
 */
bool_t module_pump_controller_started(void);

#endif /* INC_MODULECONTROLLERPUMP_H_ */
