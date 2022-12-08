#include "analysis.h"
#include "sniff.h"

#include <stdlib.h>
#include <pcap.h>
#include <netinet/if_ether.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <string.h>



packet_info * analyse(const unsigned char *packet) {
  // Allocate memory for the packet_info struct
  struct packet_info *pi = malloc(sizeof(struct packet_info));
  pi->arp_pac = 0;
  pi->syn_pac = 0;
  pi->blv_pac_google = 0;
  pi->blv_pac_facebook = 0;
  pi->ip = NULL;

  // Get the ethernet header
  struct ether_header *ether_header = (struct ether_header *) packet;

  // Define IP header and TCP header structs
  struct ip *ip_header;
  struct tcphdr *tcp_header;
  struct ether_arp *arp_header;

  // Payload for ip header
  unsigned char *payload_ip = (unsigned char *) packet + ETH_HLEN;
  unsigned char *payload_tcp;

  // Initialize headers
  if (ether_header != NULL){
    // Check if the packet has an IP proocol
    if (ntohs(ether_header->ether_type) == ETHERTYPE_IP){
      ip_header = (struct ip *) payload_ip;
      payload_tcp = (unsigned char *) payload_ip + (ip_header->ip_hl * 4);

      // Check if the packet has TCP protocol
      if (ip_header->ip_p == IPPROTO_TCP){
        tcp_header = (struct tcphdr *) payload_tcp;
      }
    }

    // Check if the packet has ARP protocol
    // If this true then we get arp_header and check if its opcode is set to 2(int 512) because this is ARP reply
    // This way we can detect unsilicited ARP replies and ignore ARP requests because they are not posses any threat
    // To test this I changed the opcode to 1(int 256) in provided python script
    else if (ntohs(ether_header->ether_type) == ETHERTYPE_ARP){
      arp_header = (struct ether_arp *) payload_ip;
      if (arp_header->ea_hdr.ar_op == 512){
        pi->arp_pac = 1;
      }
    }
  }

  // Check for SYN and Blacklisted URLs
  if (tcp_header != NULL){
    // Check if the header has SYN flag set to 1 and ACK flag set to 0 to only count incoming requests that might be associated with a SYN flood attack
    if (tcp_header->syn == 1 && tcp_header->ack == 0){
      pi->syn_pac = 1;

      // Set the IP address
      pi->ip = ip_header->ip_src.s_addr;

    }

    // Check if the packet has a blacklisted URL
    // Check if packet sent to port 80
    if (ntohs(tcp_header->dest) == 80){
      // Initialize the HTTP request header
      unsigned char *http_header = (unsigned char *) payload_tcp + (tcp_header->doff * 4);

      // Check if the header has a GET/POST request
      if (strstr(http_header, "POST") != NULL || strstr(http_header, "GET") != NULL){

        // Check if the header has a blacklisted URL
        if (strstr(http_header, "google.co.uk") != NULL){
          pi->blv_pac_google = 1;
          printf("===============\n");
          printf("Blacklist URL vialation detected\n");
          printf("Source IP address: %s\n", inet_ntoa(ip_header->ip_src));
          printf("Destination IP address: %s\n", inet_ntoa(ip_header->ip_dst));
          printf("===============\n");
        }
        else if (strstr(http_header, "facebook.com") != NULL){
          pi->blv_pac_facebook = 1;
          printf("===============\n");
          printf("Blacklist URL vialation detected\n");
          printf("Source IP address: %s\n", inet_ntoa(ip_header->ip_src));
          printf("Destination IP address: %s\n", inet_ntoa(ip_header->ip_dst));
          printf("===============\n");
        }
      }
    }
  }

  // Return the packet_info struct
  return(pi);
}
