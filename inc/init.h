#ifndef INIT_H
#define INIT_H

#include <stdio.h>
#ifdef _WIN32
#include <Windows.h>
#else
#include <signal.h>
#endif

/**
 * Initializes program to handle Ctrl+C.
 */
void initialize();

#endif