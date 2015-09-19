#ifndef DRIVER_TIMER_H
#define DRIVER_TIMER_H

#include <memory/interrupt_request.h>
#include <memory/interrupt_service_routines.h>
#include <memory/interrupt_descriptor_table.h>
#include <process/process.h>

#define DEBUG

void timer_init();

void timer_handler(struct registers *r);

void update_time();

void get_system_time();

int get_from_chip(char a);

#endif