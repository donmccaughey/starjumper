#include "options.h"

#include <getopt.h>
#include <libgen.h>
#include <stdio.h>
#include <stdlib.h>


static struct option long_options[] = {
  {
    .name = "help",
    .has_arg = no_argument,
    .flag = NULL,
    .val = 'h',
  },
  {
    .name = "name",
    .has_arg = required_argument,
    .flag = NULL,
    .val = 'n',
  },
  {
    .name = "hex",
    .has_arg = required_argument,
    .flag = NULL,
    .val = 'x',
  },
  {
    .name = NULL,
    .has_arg = no_argument,
    .flag = NULL,
    .val = 0,
  },
};


static void
print_usage_and_exit(int argc, char **argv);


struct options *
options_alloc(int argc, char *argv[])
{
  struct options *options = sj_malloc(sizeof(struct options));
  
  options->name = sj_strdup("No Name");
  options->hex_coordinate = (struct sj_hex_coordinate) { .horizontal=1, .vertical=1, };
  
  int long_options_index;
  int ch;
  while (-1 != (ch = getopt_long(argc, argv, "hn:x:", long_options, &long_options_index))) {
    switch (ch) {
      case 'h':
        print_usage_and_exit(argc, argv);
        break;
      case 'n':
        sj_free(options->name);
        options->name = sj_strdup(optarg);
        break;
      case 'x':
      {
        bool valid = sj_hex_coordinate_from_string(optarg, &options->hex_coordinate);
        if ( ! valid) {
          fprintf(stderr, "ERROR: \"%s\" is not a valid hex coordinate\n", optarg);
          print_usage_and_exit(argc, argv);
        }
      }
        break;
      case '?':
        print_usage_and_exit(argc, argv);
        break;
      default:
        print_usage_and_exit(argc, argv);
        break;
    }
  }
  
  return options;
}


void
options_free(struct options *options)
{
  sj_free(options->name);
  sj_free(options);
}


static void
print_usage_and_exit(int argc, char **argv)
{
  fprintf(stderr,
          "usage: %s [OPTIONS]\n"
          "  -h, --help                       display this help message\n"
          "  -n, --name NAME                  set the world name\n"
          "  -x, --hex XXYY                   set the world hex coordinate\n"
          "                                   in the format \"0101\"\n"
          ,
          basename(argv[0])
          );
  exit(EXIT_FAILURE);
}
