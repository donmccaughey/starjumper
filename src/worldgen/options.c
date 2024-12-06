#include "options.h"

#include <getopt.h>
#include <libgen.h>
#include <lrnd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <xmalloc.h>
#include <xstring.h>


static struct option long_options[] = {
  {
    .name = "subsector",
    .has_arg = no_argument,
    .flag = NULL,
    .val = 'b',
  },
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
    .name = "rng",
    .has_arg = required_argument,
    .flag = NULL,
    .val = 'r',
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


static bool
lrnd_from_type(char const *type, struct lrnd **lrnd_out);

static void
print_usage_and_exit(int argc, char **argv);


struct options *
options_alloc(int argc, char *argv[])
{
  struct options *options = xcalloc(1, sizeof(struct options));
  
  options->hex_coordinate = (struct sj_hex_coordinate) { .horizontal=1, .vertical=1, };
  
  int long_options_index;
  int ch;
  while (-1 != (ch = getopt_long(argc, argv, "bhn:r:x:", long_options, &long_options_index))) {
    switch (ch) {
      case 'b':
        options->subsector = true;
        break;
      case 'h':
        print_usage_and_exit(argc, argv);
        break;
      case 'n':
        free(options->name);
        options->name = xstrdup(optarg);
        break;
      case 'r':
      {
        bool valid = lrnd_from_type(optarg, &options->lrnd);
        if ( ! valid) {
          fprintf(stderr, "ERROR: \"%s\" is not a valid random number generator type\n", optarg);
          print_usage_and_exit(argc, argv);
        }
      }
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

  if ( ! options->lrnd) {
    options->lrnd = lrnd_best;
  }

  if ( ! options->name) {
    options->name = xstrdup("No Name");
  }

  return options;
}


void
options_free(struct options *options)
{
  lrnd_free(options->lrnd);
  free(options->name);
  free(options);
}


static bool
lrnd_from_type(char const *type, struct lrnd **lrnd_out)
{
  if (0 == strcmp("arc4", type)) {
    *lrnd_out = lrnd_best;
    return true;
  }
  if (0 == strcmp("asc", type)) {
    *lrnd_out = lrnd_alloc_fake_start_step(0, 1);
    return true;
  }
  if (0 == strcmp("desc", type)) {
    *lrnd_out = lrnd_alloc_fake_start_step(UINT32_MAX, -1);
    return true;
  }
  if (0 == strcmp("fixed", type)) {
    *lrnd_out = lrnd_alloc_fake_fixed(0);
    return true;
  }
  return false;
}


static void
print_usage_and_exit(int argc, char **argv)
{
  fprintf(stderr,
          "usage: %s [OPTIONS]\n"
          "  -h, --help                       display this help message\n"
          "  -b, --subsector                  generate a subsector\n"
          "  -n, --name NAME                  set the world name\n"
          "  -r, --rng TYPE                   set the random number generator type\n"
          "                                     [arc4|asc|desc]\n"
          "  -x, --hex XXYY                   set the world hex coordinate\n"
          "                                     in the format \"0101\"\n"
          ,
          basename(argv[0])
          );
  exit(EXIT_FAILURE);
}
