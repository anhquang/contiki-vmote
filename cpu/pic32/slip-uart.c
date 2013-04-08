/*
 * Contiki PIC32 Port project
 * 
 * Copyright (c) 2012,
 *  Scuola Superiore Sant'Anna (http://www.sssup.it) and
 *  Consorzio Nazionale Interuniversitario per le Telecomunicazioni
 *  (http://www.cnit.it).
 *
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the Institute nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE INSTITUTE AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE INSTITUTE OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 */

/**
 * \addtogroup pic32 PIC32 Contiki Port
 *
 * @{
 */

/** 
 * \file   slip-uart.c
 * \brief  PIC32MX Slip interface routines
 * \author Giovanni Pellerano <giovanni.pellerano@evilaliv3.org>
 * \date   2012-03-23
 */

#include "contiki.h"
#include "dev/slip.h"

#include <pic32_uart.h>

#define DEBUG 1
#if DEBUG
#include <stdio.h>
#define PRINTF(...) printf(__VA_ARGS__)
#else
#define PRINTF(...)
#endif

#define SLIP_UART(XX)                                \
  void                                               \
  slip_arch_writeb(unsigned char c)                  \
  {                                                  \
    pic32_uart##XX##_write(c);                       \
  }                                                  \
                                                     \
  void                                               \
  slip_arch_init(unsigned long ubr)                  \
  {                                                  \
    pic32_uart##XX##_init(ubr, 0);                   \
                                                     \
    PRINTF("Initializing slip uart: %lubps\n", ubr); \
  }                                                  \
                                                     \
  UART_INTERRUPT(XX, 0, slip_input_byte);

#ifdef __USE_UART_PORT1A_FOR_SLIP__
SLIP_UART(1A);
#elif defined  __USE_UART_PORT1B_FOR_SLIP__
SLIP_UART(1B);
#elif defined  __USE_UART_PORT2A_FOR_SLIP__
SLIP_UART(2A);
#elif defined  __USE_UART_PORT2B_FOR_SLIP__
SLIP_UART(2B);
#elif defined  __USE_UART_PORT3A_FOR_SLIP__
SLIP_UART(3A);
#elif defined  __USE_UART_PORT3B_FOR_SLIP__
SLIP_UART(3B);
#else
SLIP_UART(1A);
#endif

/** @} */
