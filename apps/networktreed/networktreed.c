/*
 * Copyright (c) 2012, Nguyen Quoc Dinh
 * All rights reserved.
 */


/**
 * \file
 *         Collect network tree from leave to gateway generated by routing algo (i.e ROLL for current Contiki NWK layer)
 * \author
 *         Nguyen Quoc Dinh <nqdinh@hui.edu.vn>
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "contiki.h"

#include "networktreed.h"
#include "nwktr-dispatcher.h"

#define DEBUG DEBUG_PRINT
#include "net/uip-debug.h"

/* UDP connection */
static struct uip_udp_conn *udpconn;

PROCESS(networktreed_process, "NetworkTree daemon process");

static void udp_handler(process_event_t ev, process_data_t data){
	char rst;

	if (ev == tcpip_event && uip_newdata()){
		rst = nwkgraph_processing((uint8_t*)uip_appdata, uip_datalen(), udpconn);		//change this name

		PRINTF("exit with %d\n", rst);
	}
}

/*-----------------------------------------------------------------------------------*/
/*
 *  Entry point of the SNMP server.
 */
PROCESS_THREAD(networktreed_process, ev, data) {
	PROCESS_BEGIN();

	udpconn = udp_new(NULL, UIP_HTONS(0), NULL);
	udp_bind(udpconn, UIP_HTONS(LISTEN_PORT));

    /* init MIB */
	PRINTF("start the process\n");

	while(1) {
		PROCESS_YIELD();
		if(ev == tcpip_event) {
			udp_handler(ev, data);
		}
	}

	PROCESS_END();
}
