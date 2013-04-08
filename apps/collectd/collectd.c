/**
 * \file
 *         Collect network tree from mote to gateway generated by routing algo
 *         (i.e ROLL for current Contiki NWK layer)
 * \author
 *         Jan 2013 Nguyen Quoc Dinh <nqdinh@hui.edu.vn>
 */

#include <stdio.h>
#include <string.h>
#include "collectd.h"
#include "colld-dispatcher.h"

#define DEBUG NONE
#include "net/uip-debug.h"

static struct uip_udp_conn *client_conn;
static collectd_conf_t collectd_conf;

/*---------------------------------------------------------------------------*/
PROCESS(collectd_process, "Collectd sending process");

/*---------------------------------------------------------------------------*/
static void collectd_udp_handler(void) {
	if(uip_newdata()) {
		char rst;
		rst = collectd_processing((uint8_t*)uip_appdata, uip_datalen(), &collectd_conf);
		PRINTF("Collectd udp handler return with %d\n", rst);
	}
}
/*---------------------------------------------------------------------------*/
void collectd_conf_init(collectd_conf_t *conf){
	conf->send_active = SEND_ACTIVE_NO;
	conf->update_freq_in_sec = DEFAULT_UPDATE_PERIOD;	//this value may update via snmp
}
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(collectd_process, ev, data)
{
	static struct etimer period_timer, wait_timer;
	PROCESS_BEGIN();

	collectd_conf_init(&collectd_conf);

	/* new connection with remote host */
	client_conn = udp_new(NULL, UIP_HTONS(0), NULL);
	udp_bind(client_conn, UIP_HTONS(COLLECTD_CLIENT_PORT));

	PRINTF("Created a connection with the server ");

	/* Send a packet every 60-62 seconds. */
	etimer_set(&period_timer, CLOCK_SECOND * DEFAULT_UPDATE_PERIOD);

	while(1) {
		PROCESS_WAIT_EVENT();
		//send update (collected data)
		if(ev == PROCESS_EVENT_TIMER) {
			if (data == &period_timer) {
				etimer_reset(&period_timer);		//TODO: reset the period from collectd_conf.update freq
				if (collectd_conf.send_active == SEND_ACTIVE_YES)
					etimer_set(&wait_timer, random_rand() % (CLOCK_SECOND * RANDWAIT));

			} else if(data == &wait_timer) {
				/* Time to send the data */
				PRINTF("Time to send the data\n");
				collectd_common_send(client_conn, &collectd_conf);
			}
		}
		//receive a request (here, expected a request to begin to send update)
		if(ev == tcpip_event) {
			collectd_udp_handler();
		}
	}

	PROCESS_END();
}
/*---------------------------------------------------------------------------*/
