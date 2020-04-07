/*
 * Copyright (C) 2020 Robert Olsson
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @ingroup     examples
 * @{
 *
 * @file
 * @brief       rime application
 *
 * @author      Robert Olsson <roolss@kth.se>
 *
 * @}
 */

#include <stdio.h>
#include <string.h>
#include "thread.h"
#include "xtimer.h"
#include "timex.h"

#ifdef MODULE_NETIF
#include "net/gnrc/pktdump.h"
#include "net/gnrc.h"
#endif

#define SENSD_TAG   "&:  "

/* set interval to 10 second */
#define INTERVAL (30U * US_PER_SEC)
#define COUNT 10
#define DEF_TTL 0xF

uint8_t addr[2];
uint8_t lqi;
int16_t rssi;
uint32_t count;
#define R_TXT        ((uint64_t) ((uint64_t) 1)<<0)  
#define R_EPC        ((uint64_t) ((uint64_t) 1)<<1)

#define MAXTXTSIZE 10
//unsigned char txt [MAXTXTSIZE];
char *txt = "RIOT";
int8_t seqno;
#define MAX_BCAST_SIZE 99

struct payload {
  uint16_t channel;
  unsigned char dummy[2];
  uint8_t head; /* version << 4 + ttl */
  uint8_t seqno;
  uint8_t buf[MAX_BCAST_SIZE+20];  /* Check for max payload 20 extra to be able to test */
};

struct payload p;

extern kernel_pid_t rime_pid;

void print_report(gnrc_pktsnip_t *pkt)
{
  gnrc_netif_hdr_t *hdr;
  char *p;

  hdr = pkt->data;

  if( pkt->type == GNRC_NETTYPE_NETIF) {
    if (hdr->src_l2addr_len == 2) {
      uint8_t *laddr;

      laddr = gnrc_netif_hdr_get_src_addr(hdr);
      addr[0] = *laddr;
      laddr++;
      addr[1] = *laddr;
    }
    if (hdr->src_l2addr_len == 8) {
      uint8_t *laddr;

      laddr = gnrc_netif_hdr_get_src_addr(hdr);
      addr[0] = *laddr;
      laddr++;
      addr[1] = *laddr;
    }
    lqi = hdr->lqi;
    rssi = hdr->rssi;
    return;
  }

  p = pkt->data;

  struct payload *pl =(struct payload *)  &p[0];  

  printf("&: %s ", &pl->buf);

  printf(" [ADDR=%-d.%-d SEQ=%-d TTL=%-u RSSI=%-d LQI=%-u]\n", addr[0], addr[1], pl->seqno,
	 (pl->head & 0x0F), (signed) rssi, (unsigned) lqi);
  
#if 0
      printf(" [ADDR=%-d.%-d SEQ=%-d TTL=%-u RSSI=%-u LQI=%-u DRP=%-d.%02d DELAY=%d]",
	     p->addr[0], p->addr[1], p->seqno, p->ttl, p->rssi, p->lqi,  p->drp[0], p->drp[1], 
	     (uptime - p->time));
#endif
}

void rime_msg(gnrc_pktsnip_t *pkt)
{
  gnrc_pktsnip_t *snip = pkt;

  pkt = gnrc_pktsnip_search_type(snip, GNRC_NETTYPE_NETIF);
  if(pkt)
    print_report(pkt);

  pkt = gnrc_pktsnip_search_type(snip, GNRC_NETTYPE_UNDEF);
  if(pkt)
    print_report(pkt);
  
  gnrc_pktbuf_release(snip);
}

static int rime_send(gnrc_netif_t *iface, struct payload* payload, int payload_len)
{
    gnrc_netif_hdr_t *hdr;
    gnrc_pktsnip_t *pkt = gnrc_pktbuf_add(NULL, payload, payload_len, GNRC_NETTYPE_UNDEF);
    gnrc_pktsnip_t *netif;
    uint16_t l2addr[2];
    uint16_t src_len = 2U;
    signed res;

    src_len = 8;

    if(!pkt) 
      return -1;

    res = gnrc_netapi_set(iface->pid, NETOPT_SRC_LEN,0, &src_len, sizeof(src_len));

    if(res < 0 )
      printf("ERR SRC_LEN\n");

    /* Compat w. Contiki/sensd */
    l2addr[0] = iface->l2addr[7];
    l2addr[1] = iface->l2addr[6];

    res = gnrc_netapi_set(iface->pid, NETOPT_ADDRESS,0, &l2addr, sizeof(src_len));
    
    if(res < 0 )
      printf("ERR ADDr\n");

    netif = gnrc_netif_hdr_build(NULL, 0, NULL, 0);
    
    if(!netif)
      return -1;

    hdr = netif->data;
    hdr->flags |= GNRC_NETIF_HDR_FLAGS_BROADCAST;
    LL_PREPEND(pkt, netif);
    gnrc_netif_send(iface, pkt);
    return 1;
}

char *epc_str= "34323001D900000000000001";

int len_exceeded(int len)
{
  if(len > MAX_BCAST_SIZE) {
      printf("Error TX Length=%d\n", len);
    return 1;
  }
  return 0;
}

void send_sensd_pkt(gnrc_netif_t *iface)
{
  struct payload msg;
  uint16_t len = 0;
  uint64_t mask;
  uint8_t ttl = DEF_TTL;
  
  mask = (R_TXT|R_EPC); /* For test */

  if(mask & R_TXT) {
    len += snprintf((char *) &msg.buf[len], sizeof(msg.buf), 
		    "TXT=%s ", txt);
    
    if( len_exceeded(len)) 
      return;
  }
  if(mask & R_EPC) {
    len += snprintf((char *) &msg.buf[len], sizeof(msg.buf), 
		    "EPC=%s ", epc_str);
    
    if( len_exceeded(len)) 
      return;
  }
  msg.channel = 0x81; /* 129 for Contiki */
  msg.dummy[0] = 0xAB;
  msg.dummy[1] = 0xBA;
  msg.head = (1<<4); /* Version 1 for sensd */
  msg.head |= ttl;
  msg.seqno = seqno++;
  msg.buf[len++] = 0;
  rime_send(iface, &msg, strlen(msg.buf)+7);
}

int main(void)
{
  int res;

  xtimer_ticks32_t last_wakeup = xtimer_now();
 
#ifdef MODULE_NETIF
  gnrc_netreg_entry_t dump = GNRC_NETREG_ENTRY_INIT_PID(GNRC_NETREG_DEMUX_CTX_ALL, rime_pid);
  gnrc_netreg_register(GNRC_NETTYPE_UNDEF, &dump);
#endif
    
  gnrc_pktdump_init();
  gnrc_netif_t *iface = gnrc_netif_get_by_pid(4);

  uint16_t pan = 0xabcd;
  res = netif_set_opt(&iface->netif, NETOPT_NID, 0, &pan, sizeof(pan));
  if(res < 0)
    printf("Setting PAN failed\n");

  uint16_t chan = 26;
  res = netif_get_opt(&iface->netif, NETOPT_CHANNEL, 0, &chan, sizeof(chan));
  if (res < 0)
    printf("Setting Chan failed\n");
  
  while(1) {
    xtimer_periodic_wakeup(&last_wakeup, INTERVAL);
    send_sensd_pkt(iface);
  }
}
