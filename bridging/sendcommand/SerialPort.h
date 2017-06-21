#pragma once


namespace brewery {

  /*
   * Class to manage a serial port
   */
  
  class SerialPort {

  protected:
    int _fd;

  public:
    SerialPort(const char *port,speed_t speed);
    ~SerialPort();
    
    void writeString(const std::string& command);
    std::string readString();
  };


  /*
   * Constructor
   */
  
  inline SerialPort::SerialPort(const char *port,speed_t speed) {

    // open the port
    
    if((_fd = open(port, O_RDWR | O_NOCTTY | O_SYNC))<0)
      throw OpenPortException(port,strerror(errno));

    // set attributes
    
    struct termios tty;
  
    if (tcgetattr(_fd, &tty) < 0)
      throw GetTerminalAttributesException(strerror(errno));

    // baud rate
    
    cfsetospeed(&tty, speed);
    cfsetispeed(&tty, speed);

    tty.c_cflag |= (CLOCAL | CREAD);    // ignore modem controls 
    tty.c_cflag &= ~CSIZE;
    tty.c_cflag |= CS8;         // 8-bit characters 
    tty.c_cflag &= ~PARENB;     // no parity bit 
    tty.c_cflag &= ~CSTOPB;     // only need 1 stop bit 
    tty.c_cflag &= ~CRTSCTS;    // no hardware flowcontrol 

    // setup for non-canonical mode
    
    tty.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP | INLCR | IGNCR | ICRNL | IXON);
    tty.c_lflag &= ~(ECHO | ECHONL | ICANON | ISIG | IEXTEN);
    tty.c_oflag &= ~OPOST;
  
    // fetch bytes as they become available
    
    tty.c_cc[VMIN] = 1;
    tty.c_cc[VTIME] = 1;

    if (tcsetattr(_fd, TCSANOW, &tty) != 0)
      throw SetTerminalAttributesException(strerror(errno));
  }


  /*
   * Destructor
   */
  
  inline SerialPort::~SerialPort() {
    if(_fd>0)
      close(_fd);
  }


  /*
   * Write a string
   */
  
  inline void SerialPort::writeString(const std::string& command) {

    std::string fullcmd=command+"\r\n";
    size_t written=write(_fd,fullcmd.c_str(),fullcmd.length());

    if(written!=fullcmd.length())
      throw WriteCommandException(strerror(errno));
  }


  /*
   * Read a response string
   */
  
  inline std::string SerialPort::readString() {

    std::string response;
    char buf[100];
    bool finished;
    
    do {

      size_t rdlen=read(_fd,buf,sizeof(buf)-1);

      if(rdlen<0)
	throw ReadCommandException(strerror(errno));
      else if(rdlen>0) {
	buf[rdlen]='\0';
	response+=buf;
	finished=buf[rdlen-1]=='\n';
      }
    } while(!finished);

    return response;
  }
}
