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
      case "build_room":
        return buildRoom();
      case "exit":
        return false;
      default:
        break;
    }

    // Unknown command

    return false;
  }
  /** Builds the room.
   * @return Always returns true.
   * */
  static boolean buildRoom() {
    System.out.println("2 2  0 0 0 0  0 0 0 0");
    return true;
  }
}
