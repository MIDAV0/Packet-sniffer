#ifndef CS241_ANALYSIS_H
#define CS241_ANALYSIS_H

#include <pcap.h>

typedef struct packet_info {
  unsigned long ip;
  int syn_pac;
  int arp_pac;
  int blv_pac_google;
  int blv_pac_facebook;
} packet_info;

packet_info * analyse(const unsigned char *packet);


#endif
