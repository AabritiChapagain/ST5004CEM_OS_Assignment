#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define PAGE_SIZE        4     // bytes per page
#define FRAME_COUNT      4     // physical frames in RAM
#define REF_LENGTH       12    // length of the reference string


#define RESET   "\033[0m"
#define BOLD    "\033[1m"
#define DIM     "\033[2m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define CYAN    "\033[36m"
#define WHITE   "\033[37m"


typedef struct {
    int  page_id;    /* which page is loaded (-1 = empty) */
    int  loaded_at;  /* tick when page was loaded (for FIFO) */
    int  last_used;  /* tick of last access        (for LRU)  */
    char data[PAGE_SIZE]; /* simulated page content            */
} Frame;

typedef struct {
    int hits;
    int faults;
} Stats;

int main(void)
{
    printf("ST5004CEM Task 2 - Memory Management Simulation\n");
    return 0;
}
