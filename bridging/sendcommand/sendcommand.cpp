
#include "Application.h"

/*
 * Main entry point
 */

int main(int argc,char **argv) {

  if(argc!=3) {
    puts("Usage: sendcommand <port-device> <command>");
    exit(1);
  }

  try {
    brewery::SerialPort port(argv[1],(speed_t)B9600);
    port.writeString(argv[2]);

    std::cout << port.readString();
  }
  catch(std::exception& ex) {
    std::cout << ex.what() << std::endl;
  }

  return 0;
}

