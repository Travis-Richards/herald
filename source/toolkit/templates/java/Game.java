import java.util.Scanner;

public class Game {
  public static void main(String[] args) {

    Scanner scanner = new Scanner(System.in);

    while (true) {

      String command = scanner.nextLine();

      if (!run_command(command)) {
        break;
      }
    }
  }
  static boolean run_command(String command) {

    if (command == "exit") {
      /* False indicates that we should break
       * from the main loop and exit the program. */
      return false;
    }

    return true;
  }
}
