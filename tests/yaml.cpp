#include <stdint.h>
#include <yaml.h>
#include <assert.h>

int main(int argc, char **argv) {
    FILE *file;
    yaml_parser_t parser;
    yaml_parser_initialize(&parser);

    file = fopen(argv[1], "rb");
    assert(file);

    yaml_parser_set_input_file(&parser, file);

    int done = 0;
    while (!done) {
        yaml_event_t event;
        if (!yaml_parser_parse(&parser, &event)) {
            break;
        }
        done = (event.type == YAML_STREAM_END_EVENT);
        yaml_event_delete(&event);
    }
    yaml_parser_delete(&parser);
    return 0;
}
