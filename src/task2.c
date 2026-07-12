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
        "║  ST5004CEM Task 2 — Memory Management Simulation    ║\n"
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

int main(void)
{
    printf("ST5004CEM Task 2 - Memory Management Simulation\n");
    return 0;
}
