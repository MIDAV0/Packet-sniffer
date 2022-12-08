#ifndef CS241_DISPATCH_H
#define CS241_DISPATCH_H

#include <pcap.h>


void dispatch(u_char *args,
              const struct pcap_pkthdr *header,
              const unsigned char *packet);
void start_threads();
void *thread_code();
void kill_threads();
#endif
