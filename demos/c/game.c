#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/** The width of a room,
 * in terms of tiles.
 * */
#define ROOM_WIDTH 10

/** The height of a room,
 * in terms of tiles.
 * */
#define ROOM_HEIGHT 10

/** A type definition for a room matrix.
 * */
typedef int room_matrix_z[ROOM_WIDTH][ROOM_HEIGHT];

/** Builds a room to be displayed.
 * @returns Always returns non-zero.
 * */
static int build_room();

/** Fills the room with objects.
 * @returns Always returns non-zero.
 * */
static int fill_objects();

/** Handles a command issued by the game engine.
 * @param command The command that was issued.
 * @returns A boolean value indicating whether
 * or not the game loop should continue executing.
 * If non-zero is returned, the loop should continue.
 * If zero is returned, the loop should exit.
 * */
static int handle_command(const char* command);

/** Prints an integer matrix.
 * This is used to respond to commands.
 * @param matrix The matrix to print.
 * */
static void print_matrix(const int matrix[][ROOM_HEIGHT]);

/** Reads from the standard input until a
 * line feed character is found.
 *
 * @returns A dynamically allocated string
 * containing the line data. The pointer returned
 * by this function may be null, in case of reaching
 * the end of file or a failed malloc call. If it
 * doesn't return a null pointer, the pointer must
 * be freed. On success, the string is null terminated.
 * */
static char* readline();

/** Updates a button state.
 * */
static int update_button();

/** Updates the axis values.
 * */
static int update_axis();

int main() {

  int keep_going = 1;

  while (keep_going) {

    char* line = readline();
    if (!line) {
      break;
    }

    keep_going = handle_command(line);

    free(line);
  }

  return EXIT_SUCCESS;
}

static int build_room() {

  const room_matrix_z textures = {
    { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
    { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
    { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
    { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
    { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
    { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
    { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
    { -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
    { 34, -1, -1, -1, -1, -1, -1, -1, -1, -1 },
    {  0,  1,  1,  1,  1,  1,  2, -1,  0,  1 }
  };

  const room_matrix_z frames = {
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
  };

  const room_matrix_z flags = {
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
  };

  printf("%d %d ", ROOM_WIDTH, ROOM_HEIGHT);
  print_matrix(textures);
  print_matrix(frames);
  print_matrix(flags);
  printf("\n");
  fflush(stdout);

  return 1;
}

static int fill_objects() {

  const room_matrix_z actions = {
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
  };

  const room_matrix_z flags = {
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
  };

  printf("%d %d ", ROOM_WIDTH, ROOM_HEIGHT);
  print_matrix(actions);
  print_matrix(flags);
  printf("\n");
  fflush(stdout);

  return 1;
}

static int handle_command(const char* command) {

  if (strcmp(command, "exit") == 0) {
    return 0;
  } else if (strcmp(command, "build_room") == 0) {
    return build_room();
  } else if (strcmp(command, "fill_objects") == 0) {
    return fill_objects();
  } else if (strcmp(command, "set_background") == 0) {
    printf("20 0\n");
    fflush(stdout);
  } else if (strcmp(command, "update_axis") == 0) {
    return update_axis();
  } else if (strcmp(command, "update_button") == 0) {
    return update_button();
  } else {
    fprintf(stderr, "Unknown command '%s'\n", command);
  }

  return 1;
}

static void print_matrix(const int matrix[][ROOM_HEIGHT]) {
  for (int y = 0; y < ROOM_HEIGHT; y++) {
    for (int x = 0; x < ROOM_WIDTH; x++) {
      printf("%d ", matrix[y][x]);
    }
  }
}

static char* readline() {

  char* line = NULL;

  size_t size = 0;

  while (!feof(stdin)) {

    char next_char = 0;

    if (fread(&next_char, 1, 1, stdin) != 1) {
      break;
    } else if (next_char == '\n') {
      break;
    }

    char* tmp = realloc(line, size + 2);
    if (!tmp) {
      return line;
    } else {
      line = tmp;
      line[size + 0] = next_char;
      line[size + 1] = 0;
    }

    size++;
  }

  return line;
}

static int update_button() {
  int controller = 0;
  int button = 0;
  int state = 0;
  scanf("%d %d %d", &controller, &button, &state);
  return 1;
}

static int update_axis() {
  int controller = 0;
  float x = 0;
  float y = 0;
  scanf("%d %f %f", &controller, &x, &y);
  return 1;
}
