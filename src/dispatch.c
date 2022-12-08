#include <pcap.h>
#include <signal.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

#include "dispatch.h"
#include "analysis.h"
#include "sniff.h"
#include "queue.h"
#include "dynamicArray.h"

#define NUMTHREADS 8


// Global variables

// The queue of packets to be processed
struct queue *work_queue;

// The dynamic array that stores ip addresses
struct dynamicArray *dArray;

// Report data
int syn_pac = 0;
int arp_pac = 0;
int blv_pac_google = 0;
int blv_pac_facebook = 0;

// Mutex lock
pthread_mutex_t queue_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t queue_cond = PTHREAD_COND_INITIALIZER;

// Threads
pthread_t tid[NUMTHREADS];

// Flag for stopping thread loops
int FLAGRUN = 0;


// Start the threads
void start_threads(){

  // Create the work queue
  work_queue = create_queue();

  // Create the dynamic array with initial size 20
  dArray = create_array(20);

  // Set the flag to 1 to run the threads loop
  FLAGRUN = 1;

  // Create the threads
  for(int i=0; i<NUMTHREADS; i++){
    pthread_create(&tid[i], NULL, &thread_code, NULL);
  }
}


void dispatch(u_char *args,
              const struct pcap_pkthdr *header,
              const unsigned char *packet) {

  // Acquire lock and add packet to queue 
  // Signal the waiting threads and release lock
  pthread_mutex_lock(&queue_mutex);
  enqueue(work_queue, packet);
  pthread_cond_broadcast(&queue_cond);
  pthread_mutex_unlock(&queue_mutex);
}


// Thread code
void *thread_code() {
  unsigned char *packetForProcessing = NULL;

  // Execute thread code until program is terminated using CTRL+C
  while (FLAGRUN) {
    
    packetForProcessing = NULL;

    // Acquire lock and check if queue is empty
    pthread_mutex_lock(&queue_mutex);
    
    while (isempty(work_queue)) {
      // Wait for the condition variable
      pthread_cond_wait(&queue_cond, &queue_mutex);
    }
    
    // Get the packet from the queue 
    packetForProcessing = work_queue->head->item;  
    dequeue(work_queue);

    // Release lock
    pthread_mutex_unlock(&queue_mutex);
  
    // Analyse the packet
    if (packetForProcessing != NULL){
      packet_info *pi = analyse(packetForProcessing);

      // Aquire lock and update the report data
      pthread_mutex_lock(&queue_mutex);    

      syn_pac += pi->syn_pac;
      arp_pac += pi->arp_pac;
      blv_pac_google += pi->blv_pac_google;
      blv_pac_facebook += pi->blv_pac_facebook;
 
      if (pi->ip){
        insert_array(dArray, pi->ip);
      }

      // Release lock and free the packet
      pthread_mutex_unlock(&queue_mutex);
      free(pi);
    }
  }
  return NULL;
}

// Stop the threads and print the report
void kill_threads() {
  // Set flog to 0 to stop thread loops
  FLAGRUN = 0;

  // Count number of distinct IP addresses;
  sort_array(dArray);
  int count = count_distinct_elements(dArray);

  // Print the report
  printf("\nIntrusion Detection Report:\n");
  if (count > 0){
      printf("%d SYN packets detected from %d different IPs (syn attack)\n", syn_pac, count);
  } else {
      printf("%d SYN packets detected from 0 IP (syn attack)\n", syn_pac);
  }
  printf("%d ARP responses (cache poisoning)\n", arp_pac);
  printf("%d URL Blacklist violations (%d google and %d facebook)\n", blv_pac_google+blv_pac_facebook, blv_pac_google, blv_pac_facebook);

  // Free the work queue and dynamic array
  destroy_queue(work_queue);
  free_array(dArray);
}

