/*
 * ============================================================
 *  Microplastics Particle Size Distribution Analyzer
 * ============================================================
 *  Author  : Mradul Dixit
 *  Degree  : B.E. Plastics Engineering, CIPET-IPT Ahmedabad
 *  Purpose : Analyzes particle size data from microplastics
 *            sampling studies, classifies particles by ISO/GESAMP
 *            size categories, and computes key statistical
 *            descriptors used in environmental characterization.
 *
 *  Context : Part of an independent research project to
 *            characterize microplastic distribution in GIDC
 *            industrial effluent channels, Ahmedabad, India.
 * ============================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define MAX_PARTICLES 10000
#define NUM_CLASSES   5

/* ── Size classification (micrometers) ── */
/* Based on GESAMP / NOAA microplastics size framework */
typedef struct {
    const char *label;
    double      lower;   /* micrometers */
    double      upper;
} SizeClass;

static const SizeClass CLASSES[NUM_CLASSES] = {
    { "Nano    (< 1 um)",         0.0,    1.0   },
    { "Small   (1 - 100 um)",     1.0,    100.0 },
    { "Medium  (100 um - 1 mm)",  100.0,  1000.0},
    { "Large   (1 mm - 5 mm)",    1000.0, 5000.0},
    { "Macro   (> 5 mm)",         5000.0, 1e18  }
};

/* ── Comparison function for qsort ── */
static int cmp_double(const void *a, const void *b) {
    double x = *(const double *)a;
    double y = *(const double *)b;
    return (x > y) - (x < y);
}

/* ── Statistical functions ── */
static double mean(const double *data, int n) {
    double sum = 0.0;
    for (int i = 0; i < n; i++) sum += data[i];
    return sum / n;
}

static double median(double *sorted, int n) {
    if (n % 2 == 0)
        return (sorted[n/2 - 1] + sorted[n/2]) / 2.0;
    return sorted[n/2];
}

static double std_dev(const double *data, int n, double avg) {
    double sum = 0.0;
    for (int i = 0; i < n; i++)
        sum += (data[i] - avg) * (data[i] - avg);
    return sqrt(sum / n);
}

static double percentile(double *sorted, int n, double p) {
    double idx = (p / 100.0) * (n - 1);
    int    lo  = (int)idx;
    double frac = idx - lo;
    if (lo + 1 >= n) return sorted[n - 1];
    return sorted[lo] + frac * (sorted[lo + 1] - sorted[lo]);
}

/* ── Print a horizontal bar for distribution histogram ── */
static void print_bar(double fraction, int width) {
    int filled = (int)(fraction * width);
    printf("  [");
    for (int i = 0; i < width; i++)
        putchar(i < filled ? '#' : ' ');
    printf("]  %.1f%%\n", fraction * 100.0);
}

/* ── Main ── */
int main(void) {
    double particles[MAX_PARTICLES];
    int    n = 0;
    int    class_count[NUM_CLASSES] = {0};

    printf("==========================================================\n");
    printf("  Microplastics Particle Size Distribution Analyzer\n");
    printf("  CIPET-IPT Ahmedabad  |  Mradul Dixit\n");
    printf("==========================================================\n\n");

    /* ── Input ── */
    printf("Enter particle sizes in micrometers (um).\n");
    printf("Type -1 when done, or provide a filename as argument.\n\n");

    /* Accept data from stdin or redirect */
    double val;
    printf("Particle size (um) > ");
    while (n < MAX_PARTICLES && scanf("%lf", &val) == 1) {
        if (val < 0) break;
        if (val == 0) {
            printf("Warning: zero-size particle skipped.\n");
        } else {
            particles[n++] = val;
        }
        printf("Particle size (um) > ");
    }

    if (n == 0) {
        fprintf(stderr, "\nNo data entered. Exiting.\n");
        return 1;
    }

    /* ── Sort a copy for median / percentile ── */
    double sorted[MAX_PARTICLES];
    memcpy(sorted, particles, n * sizeof(double));
    qsort(sorted, n, sizeof(double), cmp_double);

    /* ── Classify particles ── */
    for (int i = 0; i < n; i++) {
        for (int c = 0; c < NUM_CLASSES; c++) {
            if (particles[i] >= CLASSES[c].lower &&
                particles[i] <  CLASSES[c].upper) {
                class_count[c]++;
                break;
            }
        }
    }

    /* ── Statistics ── */
    double avg = mean(particles, n);
    double med = median(sorted, n);
    double sd  = std_dev(particles, n, avg);
    double p10 = percentile(sorted, n, 10.0);
    double p90 = percentile(sorted, n, 90.0);

    /* ── Output ── */
    printf("\n----------------------------------------------------------\n");
    printf("  DATASET SUMMARY\n");
    printf("----------------------------------------------------------\n");
    printf("  Total particles analysed : %d\n", n);
    printf("  Minimum size             : %.3f um\n", sorted[0]);
    printf("  Maximum size             : %.3f um\n", sorted[n-1]);
    printf("  Mean (arithmetic)        : %.3f um\n", avg);
    printf("  Median                   : %.3f um\n", med);
    printf("  Std deviation            : %.3f um\n", sd);
    printf("  10th percentile (D10)    : %.3f um\n", p10);
    printf("  90th percentile (D90)    : %.3f um\n", p90);
    printf("  Span [(D90-D10)/median]  : %.3f\n",
           (med > 0) ? (p90 - p10) / med : 0.0);

    printf("\n----------------------------------------------------------\n");
    printf("  SIZE CLASS DISTRIBUTION  (GESAMP/NOAA Framework)\n");
    printf("----------------------------------------------------------\n");
    for (int c = 0; c < NUM_CLASSES; c++) {
        double frac = (n > 0) ? (double)class_count[c] / n : 0.0;
        printf("  %-28s  count: %4d  ", CLASSES[c].label, class_count[c]);
        print_bar(frac, 30);
    }

    printf("\n----------------------------------------------------------\n");
    printf("  POLLUTION INDEX (D90 heuristic)\n");
    printf("----------------------------------------------------------\n");
    if      (p90 < 100)   printf("  Category : FINE microplastics dominant\n");
    else if (p90 < 1000)  printf("  Category : MEDIUM microplastics dominant\n");
    else if (p90 < 5000)  printf("  Category : LARGE microplastics dominant\n");
    else                   printf("  Category : MACROPLASTICS present\n");

    printf("\n  Note: This tool supports field data from FTIR-ATR and\n");
    printf("  microscopy-based sizing. For wet-sieving data, convert\n");
    printf("  sieve mesh size to equivalent sphere diameter before\n");
    printf("  input.\n");
    printf("\n==========================================================\n");

    return 0;
}
