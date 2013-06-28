#pragma once

#include "common.hh"
#include "cmdline.h"

int getNumThreads();

class Config
{
public:
  Config() {}
  ~Config() { cmdline_parser_free(&args_info); }
  int getopt(int &argc, char* argv[]) {
    if (cmdline_parser(argc, argv, &args_info) != 0)
      return 1;
    nthreads = args_info.nthreads_arg;
    samples = args_info.samples_arg;
    outfile = args_info.outfile_arg;
    mode = args_info.mode_arg;
    gammaR = args_info.gamma_r_arg;
    gammaG = args_info.gamma_g_arg;
    gammaB = args_info.gamma_b_arg;
    if (nthreads <= 0) nthreads = getNumThreads();
    printf("nthreads: %d\n", nthreads);
    printf("outfile: %s\n", outfile ? outfile : "*OpenCV imshow*");
    return 0;
  }
  int nthreads, samples;
  const char *outfile, *mode;
  ft gammaR, gammaG, gammaB;
protected:
  gengetopt_args_info args_info;
};

extern Config cfg;
