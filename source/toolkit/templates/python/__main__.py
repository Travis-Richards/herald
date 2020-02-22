import sys

def main():
  while True:
    command = sys.stdin.readline()
    command = command.rstrip('\n')
    command = command.rstrip('\r')
    if command == "exit":
      return
    else:
      print("command: ", command)

if __name__ == "__main__":
  main()
