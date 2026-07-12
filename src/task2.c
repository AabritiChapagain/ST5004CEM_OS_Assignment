#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define PAGE_SIZE        4     // bytes per page
#define FRAME_COUNT      4     // physical frames in RAM
#define REF_LENGTH       12    // length of the reference string


#define RESET   "\033[0m"
#define BOLD    "\033[1m"
#define DIM     "\033[2m"

#define RED     "\033[91m"   // Bright Red
#define GREEN   "\033[92m"   // Bright Green
#define YELLOW  "\033[93m"   // Bright Yellow
#define BLUE    "\033[94m"   // Bright Blue
#define MAGENTA "\033[95m"   // Bright Magenta
#define CYAN    "\033[96m"   // Bright Cyan
#define WHITE   "\033[97m"   // Bright White


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

/* Returns index of page_id in frames[], or -1 if not present */
int find_page(Frame *frames, int page_id) {
    for (int i = 0; i < FRAME_COUNT; i++)
        if (frames[i].page_id == page_id) return i;
    return -1;
}

/* Returns 1 if any frame is empty */
int has_empty(Frame *frames) {
    for (int i = 0; i < FRAME_COUNT; i++)
        if (frames[i].page_id == -1) return 1;
    return 0;
}

/* Returns index of first empty frame */
int first_empty(Frame *frames) {
    for (int i = 0; i < FRAME_COUNT; i++)
        if (frames[i].page_id == -1) return i;
    return -1;
}
void print_frames(Frame *frames, int ref, int tick, int is_fault) {
    printf("  Ref %-3d │ ", ref);
    for (int i = 0; i < FRAME_COUNT; i++) {
        if (frames[i].page_id == -1)
            printf(DIM " [ — ]" RESET);
        else if (frames[i].page_id == ref)
            printf(MAGENTA " [P%-2d]" RESET, frames[i].page_id);
        else
            printf(BLUE " [P%-2d]" RESET, frames[i].page_id);
    }
    if (is_fault)
        printf("  " RED "PAGE FAULT" RESET "\n");
    else
        printf("  " BLUE "HIT" RESET "\n");
}
void load_page(Frame *f, int page_id, int tick) {
    f->page_id   = page_id;
    f->loaded_at = tick;
    f->last_used = tick;

    for (int b = 0; b < PAGE_SIZE; b++)
        f->data[b] = (char)('A' + page_id);

    printf(CYAN "    → Loading P%d into frame (data: " RESET, page_id);

    for (int b = 0; b < PAGE_SIZE; b++)
        printf("%c ", f->data[b]);

    printf(CYAN ")\n" RESET);
}
void print_header(const char *algo_name, int *ref_string) {
    printf(MAGENTA
        "\n╔══════════════════════════════════════════════════════╗\n"
        "║  ST5004CEM Task 2 — Memory Management Simulation     ║\n"
        "╚══════════════════════════════════════════════════════╝\n"
        RESET);

    printf(BOLD "\n  Algorithm     : " RESET "%s\n", algo_name);
    printf(BOLD "  Page size     : " RESET "%d bytes\n", PAGE_SIZE);
    printf(BOLD "  Frame count   : " RESET "%d\n", FRAME_COUNT);
    printf(BOLD "  Reference str : " RESET);

    for (int i = 0; i < REF_LENGTH; i++)
        printf("P%d ", ref_string[i]);

    printf("\n\n");

    printf(BOLD "  Ref     │ Frames%-*s│ Result\n" RESET,
           FRAME_COUNT * 6 - 6, "");

    printf(DIM "  ────────┼");

    for (int i = 0; i < FRAME_COUNT * 6; i++)
        printf("─");

    printf("┼──────────\n" RESET);
}

/* FIFO PAGE REPLACEMENT
   Evicts whichever page was loaded FIRST (oldest).
*/
Stats run_fifo(int *ref_string) {
    Frame frames[FRAME_COUNT];

    for (int i = 0; i < FRAME_COUNT; i++) {
        frames[i].page_id   = -1;
        frames[i].loaded_at = 0;
        frames[i].last_used = 0;
        memset(frames[i].data, 0, PAGE_SIZE);
    }

    Stats s = {0, 0};
    int tick = 0;

    print_header("FIFO (First-In First-Out)", ref_string);

    for (int i = 0; i < REF_LENGTH; i++) {

        int ref = ref_string[i];
        tick++;

        int idx = find_page(frames, ref);

        if (idx != -1) {

            /* HIT */
            frames[idx].last_used = tick;
            s.hits++;

            print_frames(frames, ref, tick, 0);

        } else {

            /* PAGE FAULT */
            s.faults++;

            if (has_empty(frames)) {

                int slot = first_empty(frames);
                load_page(&frames[slot], ref, tick);

            } else {

                int oldest = 0;

                for (int j = 1; j < FRAME_COUNT; j++)
                    if (frames[j].loaded_at < frames[oldest].loaded_at)
                        oldest = j;

                printf(RED
                       "	 ✕ Evicting P%d (loaded earliest at tick %d)\n"
                       RESET,
                       frames[oldest].page_id,
                       frames[oldest].loaded_at);

                load_page(&frames[oldest], ref, tick);
            }

            print_frames(frames, ref, tick, 1);
        }
    }

    return s;
}
int main(void)
{

	/* Reference string: the sequence of page numbers
     * 	the CPU requests. This simulates a program
     * 	accessing pages in this order.*/
int ref_string[REF_LENGTH] =
    {
        0, 1, 2, 3,
        0, 1, 4, 0,
        1, 2, 3, 4
    };
/*
Pages 0–4 represent 5 distinct virtual pages.
     * With only 4 frames, the simulator must decide
     * which page to evict when all frames are full.*/

    Stats fifo_stats = run_fifo(ref_string);

    printf("\n");
    printf("Hits        : %d\n", fifo_stats.hits);
    printf("Page Faults : %d\n", fifo_stats.faults);
  return 0;
}
