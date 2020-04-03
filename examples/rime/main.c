/*
 * Copyright (C) tik2008, 2009, 2010 Kaspar Schleiser <kaspar@schleiser.de>
 * Copyright (C) 2013 INRIA
 * Copyright (C) 2013 Ludwig Knüpfer <ludwig.knuepfer@fu-berlin.de>
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
 * @brief       Default application that shows a lot of functionality of RIOT
 *
 * @author      Kaspar Schleiser <kaspar@schleiser.de>
 * @author      Oliver Hahm <oliver.hahm@inria.fr>
 * @author      Ludwig Knüpfer <ludwig.knuepfer@fu-berlin.de>
 *
 * @}
 */

#include <stdio.h>
#include <string.h>
#include "thread.h"
#include "xtimer.h"
#include "timex.h"
#include "shell.h"
#include "shell_commands.h"
#include "od.h"

#ifdef MODULE_NETIF
#include "net/gnrc/pktdump.h"
#include "net/gnrc.h"
#endif

#define SENSD_TAG   "&:  "

/* set interval to 10 second */
#define INTERVAL (30U * US_PER_SEC)
#define COUNT 10
#define DEF_TTL 0xF

struct payload {
  uint8_t head; /* version << 4 + ttl */
  uint8_t seqno;
  unsigned char buf[40];
  //unsigned char epc[20];
};

// &: TXT=TREE ID=28be51ce02000031 PS=1 T=14.25  V_MCU=3.60 UP=1969060 RH=33.6 V_IN=5.61  [ADDR=190.49 SEQ=181 TTL=1

struct payload p;

extern kernel_pid_t rime_pid;

uint8_t addr[2];
uint8_t lqi;
int16_t rssi;
uint32_t count;
#define R_TXT        ((uint64_t) ((uint64_t) 1)<<0)  
#define R_EPC        ((uint64_t) ((uint64_t) 1)<<1)

#define MAXTXTSIZE 10
//unsigned char txt [MAXTXTSIZE];
char *txt = "RIOT";
uint8_t seqno;
#define MAX_BCAST_SIZE 99

void print_report(gnrc_pktsnip_t *pkt)
{
  gnrc_netif_hdr_t *hdr;
  char *p;

  hdr = pkt->data;

  if( pkt->type == GNRC_NETTYPE_NETIF) {
    if (hdr->src_l2addr_len == 2) {
      uint8_t *laddr;

      printf("HDR2L\n");
      laddr = gnrc_netif_hdr_get_src_addr(hdr);
      addr[0] = *laddr;
      laddr++;
      addr[1] = *laddr;
    }
    if (hdr->src_l2addr_len == 8) {
      uint8_t *laddr;

      printf("HDRL8\n");

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
  //struct payload *pl =(struct payload *)  &p[0];
  struct payload *pl =(struct payload *)  &p[4];  
  printf("&: %s ", &p[6]);

  printf(" [ADDR=%-d.%-d SEQ=%-d TTL=%-u RSSI=%-d LQI=%-u]\n", addr[0], addr[1], pl->seqno,
	 (pl->head & 0x0F), (signed) rssi, (unsigned) lqi);
  
#if 0
      printf(" [ADDR=%-d.%-d SEQ=%-d TTL=%-u RSSI=%-u LQI=%-u DRP=%-d.%02d DELAY=%d]",
	     p->addr[0], p->addr[1], p->seqno, p->ttl, p->rssi, p->lqi,  p->drp[0], p->drp[1], 
	     (uptime - p->time));
#endif
}

static void _dump_snip(gnrc_pktsnip_t *pkt)
{
    size_t hdr_len = pkt->size;
    
    switch (pkt->type) {
        case GNRC_NETTYPE_UNDEF:
            printf("NETTYPE_UNDEF (%i)\n", pkt->type);
            od_hex_dump(pkt->data, pkt->size, OD_WIDTH_DEFAULT);
            break;
#ifdef MODULE_GNRC_NETIF
    case GNRC_NETTYPE_NETIF:
            printf("NETTYPE_NETIF (%i)\n", pkt->type);
            od_hex_dump(pkt->data, pkt->size, OD_WIDTH_DEFAULT);	    
            gnrc_netif_hdr_print(pkt->data);
            break;
#endif
        default:
            printf("NETTYPE_UNKNOWN (%i)\n", pkt->type);
            od_hex_dump(pkt->data, pkt->size, OD_WIDTH_DEFAULT);
            break;
    }
    if (hdr_len < pkt->size) {
        size_t size = pkt->size - hdr_len;

        od_hex_dump(((uint8_t *)pkt->data) + hdr_len, size, OD_WIDTH_DEFAULT);
    }
}

void __rime_msg(gnrc_pktsnip_t *pkt)
{
    int snips = 0;
    int size = 0;
    gnrc_pktsnip_t *snip = pkt;
    count++;
#if 0
    printf("pkts=%lu\n", count);
    gnrc_pktbuf_release(pkt);
    return;
#endif    
    while (snip != NULL) {
        printf("~~ SNIP %2i - size: %3u byte, type: ", snips,
               (unsigned int)snip->size);
        _dump_snip(snip);
        ++snips;
        size += snip->size;
        snip = snip->next;
    }

    printf("~~ PKT    - %2i snips, total size: %3i byte\n", snips, size);
    gnrc_pktbuf_release(pkt);
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

// 00000000  81  00  38  5A  1F  9A  54  58  54  3D  56  5F  52  55  4D  20
// 00000000  1F  36  05  34  32  30  01  D9  00  00  00  00  00  00  01

static int rime_send(gnrc_netif_t *iface, struct payload* payload, int payload_len)
{
    gnrc_netif_hdr_t *hdr;
    gnrc_pktsnip_t *pkt = gnrc_pktbuf_add(NULL, payload, payload_len, GNRC_NETTYPE_UNDEF);
    gnrc_pktsnip_t *netif;
    uint16_t src_len = 2U;
    signed res;

    src_len = 8;

    if(!pkt) 
      return -1;

    res = gnrc_netapi_set(iface->pid, NETOPT_SRC_LEN,0, &src_len, sizeof(src_len));

    if(res < 0 )
      printf("ERR 1\n");

    res = gnrc_netapi_set(iface->pid, NETOPT_ADDRESS,0, &iface->l2addr, sizeof(src_len));
    
    if(res < 0 )
      printf("ERR 1\n");


    netif = gnrc_netif_hdr_build(NULL, 0, NULL, 0);
    
    if(!netif)
      return -1;

    hdr = netif->data;
    hdr->flags |= GNRC_NETIF_HDR_FLAGS_BROADCAST;
    LL_PREPEND(pkt, netif);
    gnrc_netif_send(iface, pkt);

    return 1;
}

struct broadcast_message {
  unsigned char contiki_rime[4]; //0 1 2 3 4 
  uint8_t head; /* version << 4 + ttl */
  uint8_t seqno;
  uint8_t buf[MAX_BCAST_SIZE+20];  /* Check for max payload 20 extra to be able to test */
};

unsigned char epc[13] = {0x34, 0x32, 0x30, 0x01, 0xD9, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01};
char *epc_str= "34323001D900000000000001";

//int8_t seqno = 0; 

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
  struct broadcast_message msg;
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
  msg.head = (1<<4); /* Version 1 */
  msg.head |= ttl;
  msg.seqno = seqno++;
  //packetbuf_copyfrom(&msg, sizeof(struct broadcast_message));
  msg.buf[len++] = 0;
  rime_send(iface, &msg, strlen(msg.buf)+7);
}

int main(void)
{
  uint32_t pkt_us;
  int cnt;
  int res;

  xtimer_ticks32_t last_wakeup = xtimer_now();
 
#ifdef MODULE_NETIF
  gnrc_netreg_entry_t dump = GNRC_NETREG_ENTRY_INIT_PID(GNRC_NETREG_DEMUX_CTX_ALL, rime_pid);
  gnrc_netreg_register(GNRC_NETTYPE_UNDEF, &dump);
#endif
    
  //char line_buf[SHELL_DEFAULT_BUFSIZE];
  //shell_run(NULL, line_buf, SHELL_DEFAULT_BUFSIZE);

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
  
  cnt = COUNT;

  while(1) {
    uint32_t t1, t2, pps;
    uint8_t ttl = DEF_TTL;
    xtimer_periodic_wakeup(&last_wakeup, INTERVAL);
    
    t1 = xtimer_usec_from_ticks(xtimer_now());

    while(0 && cnt && cnt--) {
      p.head = (1<<4); /* Version 1 */
      p.head |= ttl;
      p.seqno++;
      memcpy(p.buf, epc, 12); 
      rime_send(iface, &p, sizeof(p));
    }

    send_sensd_pkt(iface);
    
    t2 = xtimer_usec_from_ticks(xtimer_now());
    cnt = COUNT;
    pkt_us = (t2-t1)/COUNT;
    pps = 1000000/pkt_us;
    //printf("sent=%d packet_us=%lu pps=%lu\n", cnt, pkt_us, pps);
  }
}
