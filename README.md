# Microplastics Particle Size Distribution Analyzer

A command-line tool written in C for analyzing particle size data from microplastics sampling studies. Built as part of an independent research project at CIPET-IPT Ahmedabad to characterize microplastic distribution in industrial effluent channels.

---

## Research Context

This tool supports an ongoing study targeting effluent discharge points in **GIDC Naroda and Vatva industrial zones, Ahmedabad, India** — home to 17,000+ manufacturing units. A literature survey has confirmed this geography has no prior published microplastics characterization data, representing a concrete environmental research gap.

The analyzer is designed to process particle size measurements obtained from:
- **FTIR-ATR microscopy** (particle-by-particle identification)
- **SEM-EDS imaging** (morphology + size measurement)
- **Wet sieving** (convert mesh size to equivalent sphere diameter before input)

---

## Features

- Accepts raw particle size data in micrometers (μm)
- Classifies particles using the **GESAMP / NOAA microplastics size framework**:

| Class | Size Range |
|-------|-----------|
| Nano | < 1 μm |
| Small | 1 – 100 μm |
| Medium | 100 μm – 1 mm |
| Large | 1 mm – 5 mm |
| Macro | > 5 mm |

- Computes key statistical descriptors used in environmental characterization:
  - Mean, Median, Standard Deviation
  - D10, D90 percentiles
  - Polydispersity Span: `(D90 − D10) / D50`
- Renders ASCII histogram of size class distribution
- Outputs a D90-based pollution index category

---

## Build & Run

**Requirements:** GCC (or any C99-compatible compiler), standard math library

```bash
# Compile
gcc -o mp_analyzer main.c -lm

# Run (interactive input)
./mp_analyzer

# Run with piped data file
./mp_analyzer < sample_data.txt
```

---

## Sample Input / Output

```
Particle size (um) > 45.2
Particle size (um) > 312.5
Particle size (um) > 87.0
Particle size (um) > 1450.0
Particle size (um) > 23.8
Particle size (um) > -1

==========================================================
  DATASET SUMMARY
----------------------------------------------------------
  Total particles analysed : 5
  Mean (arithmetic)        : 383.700 um
  Median                   : 87.000 um
  Std deviation            : 503.819 um
  D10                      : 23.800 um
  D90                      : 1450.000 um
  Span [(D90-D10)/median]  : 16.395

  SIZE CLASS DISTRIBUTION  (GESAMP/NOAA Framework)
----------------------------------------------------------
  Small   (1 - 100 um)         count:    3  [##################        ]  60.0%
  Medium  (100 um - 1 mm)      count:    1  [######                     ]  20.0%
  Large   (1 mm - 5 mm)        count:    1  [######                     ]  20.0%
```

---

## Academic Background

**Mradul Dixit**
B.E. Plastics Engineering (2025–2029)
Central Institute of Plastics Engineering & Technology (CIPET-IPT), Ahmedabad
Affiliated: Gujarat Technological University (GTU)

Research interests: Microplastics characterization in industrial effluents, polymer degradation, FTIR-ATR and SEM-EDS characterization methods, environmental materials science.

---

## License

MIT License — free to use, modify, and build upon with attribution.
