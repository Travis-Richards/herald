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
      case "fill_objects":
        return fillObjects();
      case "set_background":
        return setBackground();
      case "update_axis":
        return updateAxis(scanner);
      case "update_button":
        return updateButton(scanner);
      default:
        // Unknown command
        System.err.println("unknown command: " + command);
        break;
    }

    return false;
  }
  /** Sets the background of the room.
   * */
  static boolean setBackground() {
    System.out.println("-1 -1");
    return true;
  }
  /** Handles an "update axis" command.
   * @param scanner Used to read the axis values from the game engine.
   * @return Always returns true.
   * */
  static boolean updateAxis(Scanner scanner) {

    int controller = scanner.nextInt();
    double x = scanner.nextDouble();
    double y = scanner.nextDouble();

    return updateAxis(controller, x, y);
  }
  /** Handles an axis update.
   * @param controller The ID of the controller being updated.
   * @param x The new X value.
   * @param y The new Y value.
   * @return Always returns true.
   * */
  static boolean updateAxis(int controller, double x, double y) {
    System.err.println("update axis: " + controller + " " + x + " " + y);
    System.err.println("here");
    return true;
  }
  /** Handles a button update.
   * @param scanner The scanner to read the button information from.
   * @return Always returns true.
   * */
  static boolean updateButton(Scanner scanner) {

    int controller = scanner.nextInt();
    int button = scanner.nextInt();
    boolean state =  scanner.nextBoolean();

    return updateButton(controller, button, state);
  }
  /** Updates a button.
   * @param controller The ID of the controller being updated.
   * @param button The ID of the button being updated.
   * @param state The new state of the button.
   * True means that it is currently being pressed,
   * false means that it is no longer being pressed.
   * @return Always returns true.
   * */
  static boolean updateButton(int controller, int button, boolean state) {
    System.err.println("updated button: " + controller + " " + button + " " + state);
    return true;
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

    System.out.println(response);

    return true;
  }
  /** Fills the room with objects.
   * @return Always returns true.
   * */
  static boolean fillObjects() {

    String response = "";

    System.out.println(response);

    return true;

  }
}
