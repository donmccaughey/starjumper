#include "options.h"

#include <getopt.h>
#include <libgen.h>
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
die_from_type(char const *type, struct die *die_out, int *die_value_out);

static void
print_usage_and_exit(int argc, char **argv);


struct options *
options_alloc(int argc, char *argv[])
{
    struct options *options = xcalloc(1, sizeof(struct options));

    options->die = random_die;

    options->hex_coordinate = (struct sj_hex_coordinate) {
        .horizontal=1,
        .vertical=1,
    };

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
            case 'r': {
                bool valid = die_from_type(optarg, &options->die, &options->die_value);
                if ( ! valid) {
                    fprintf(stderr, "ERROR: \"%s\" is not a valid random number generator type\n", optarg);
                    print_usage_and_exit(argc, argv);
                }
            }
                break;
            case 'x': {
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

    if (!options->name) {
        options->name = xstrdup("No Name");
    }

    return options;
}


void
options_free(struct options *options)
{
    free(options->name);
    free(options);
}


static bool
die_from_type(char const *type, struct die *die_out, int *die_value_out)
{
    if (0 == strcmp("arc4", type)) {
        *die_value_out = 0;
        *die_out = random_die;
        return true;
    }
    if (0 == strcmp("ascending", type)) {
        *die_value_out = 0;
        *die_out = die_make_ascending(die_value_out);
        return true;
    }
    if (0 == strcmp("fixed", type)) {
        *die_value_out = 0;
        *die_out = die_make_fixed(die_value_out);
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
            "                                     [arc4|ascending|fixed]\n"
            "  -x, --hex XXYY                   set the world hex coordinate\n"
            "                                     in the format \"0101\"\n",
            basename(argv[0])
    );
    exit(EXIT_FAILURE);
}
