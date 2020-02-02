import java.util.Scanner;

class Game {
  public static void main(String[] args) {

    Scanner scanner = new Scanner(System.in);

    while (iteration(scanner));
  }
  /** Performs one iteration of the game.
   * @param scanner The input scanner.
   * This is used to receive commands
   * from the game engine.
   * @return If the loop should continue
   * executing, then true is returned.
   * Otherwise, false is returned and
   * the loop should exit.
   * */
  static boolean iteration(Scanner scanner) {

    String command = scanner.nextLine();

    switch (command) {
      case "build_menu":
        return buildMenu();
      case "build_level":
        // TODO
        break;
      case "get_level_count":
        // TODO
        break;
      case "exit":
        return false;
      default:
        break;
    }

    // Unknown command

    return false;
  }
  /** Builds the game menu.
   * @return Always returns true.
   * */
  static boolean buildMenu() {

    System.out.println("load_image_texture 'background.png'");
    System.out.println("decl_color_texture 1.0 1.0 1.0 0.5");

    System.out.println("add_polygon_object 1 20 20 70 20 70 70 20 70");
    System.out.println("set_background 0");

    System.out.println("finish");
    return true;
  }
}
