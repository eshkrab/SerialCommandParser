#include "Arduino.h"

typedef void (*callback)(char cmd, int cmd_idx, void * data); //user-defined callback funct for dealing with commands

class SerialCmdParser{
  public:
    HardwareSerial * port=NULL; //pointer to the serial port used, if not default
    int num_cmds; //number of unique, one-char commands
    char * cmds; //array containing commands, filled at runtime

    callback callback_f = NULL; //pointer to callback function
    
    //Constructors
    SerialCmdParser(){
    }
    SerialCmdParser(char * _cmds, int _num_cmds){
      num_cmds = _num_cmds;
      // cmds = (char *)calloc(num_cmds, sizeof(char));
      cmds = new char[num_cmds]();
      memcpy(cmds, _cmds, num_cmds*sizeof(char));
    }
    SerialCmdParser(char * _cmds, int _num_cmds, callback _function){
      num_cmds = _num_cmds;
      // cmds = (char *)calloc(num_cmds, sizeof(char));
      cmds = new char[num_cmds]();
      memcpy(cmds, _cmds, num_cmds*sizeof(char));
      callback_f = _function;
    }
    ~SerialCmdParser(){
      delete cmds;
    }

    //Functions
    void setCallback(callback _function){ callback_f = _function; }
    void setPort(HardwareSerial * _port){ port = _port; }

    void setCmds(char * _cmds, int _num_cmds);

    void update();
};
