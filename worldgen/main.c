#include <getopt.h>
#include <libgen.h>
#include <stdio.h>
#include <stdlib.h>
#include <starjumper/starjumper.h>


struct options
{
  char *name;
  struct sj_hex_coordinate hex_coordinate;
};


static void
initialize_options(struct options *options);

static void
finalize_options(struct options *options);

static void
parse_options(int argc, char **argv, struct options *options);

static void
print_usage_and_exit(int argc, char **argv);


static void
initialize_options(struct options *options)
{
  options->name = sj_strdup("No Name");
  options->hex_coordinate = (struct sj_hex_coordinate) { .horizontal=1, .vertical=1, };
}


static void
finalize_options(struct options *options)
{
  sj_free(options->name);
}


int
main(int argc, char **argv)
{
  struct options options;
  initialize_options(&options);
  parse_options(argc, argv, &options);
  
  struct sj_random *random = sj_random_alloc_nrand48();
  struct sj_world *world = sj_world_alloc(options.name, options.hex_coordinate, random);
  
  char *description = sj_string_from_world(world);
  fprintf(stdout, "%s\n", description);
  sj_free(description);
  
  sj_random_free(random);
  sj_memory_expect_alloc_count_zero();
  return EXIT_SUCCESS;
}


static void
parse_options(int argc, char **argv, struct options *options)
{
  static struct option longOptions[] = {
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
  
  int longOptionIndex;
  int ch;
  while (-1 != (ch = getopt_long(argc, argv, "hn:x:", longOptions, &longOptionIndex))) {
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
