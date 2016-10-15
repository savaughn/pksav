#ifndef INCLUDED_GETOPT_PORT_H
#define INCLUDED_GETOPT_PORT_H

#if defined(__cplusplus)
extern "C" {
#endif

#define no_argument 0
#define required_argument 1
#define optional_argument 2

extern char* optarg;
extern int optind, opterr, optopt;

struct option {
  const char* name;
  int has_arg;
  int* flag;
  int val;
};

int getopt(int argc, char* const argv[], const char* optstring);

int getopt_long(int argc, char* const argv[],
  const char* optstring, const struct option* longopts, int* longindex);

#if defined(__cplusplus)
}
#endif

#endif // INCLUDED_GETOPT_PORT_H
