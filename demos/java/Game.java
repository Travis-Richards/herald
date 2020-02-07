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

    String response = "";
    // 5x5 matrix
    response += "5 5 ";
    // Texture matrix data
    response += "1 1 2 1 1 ";
    response += "0 0 0 0 0 ";
    response += "0 0 0 0 0 ";
    response += "3 3 3 3 3 ";
    response += "0 0 0 0 0 ";
    // Frame matrix data
    response += "0 0 0 0 0 ";
    response += "0 0 0 0 0 ";
    response += "0 0 0 0 0 ";
    response += "2 1 0 2 1 ";
    response += "0 0 0 0 0 ";
    // Flag matrix data
    response += "0 0 0 0 0 ";
    response += "0 0 0 0 0 ";
    response += "0 0 0 0 0 ";
    response += "0 0 0 0 0 ";
    response += "0 0 0 0 0";

    System.out.println(response);

    return true;
  }
}
