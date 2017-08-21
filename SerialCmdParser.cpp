#include "SerialCmdParser.h"

void SerialCmdParser::setCmds(char * _cmds, int _num_cmds){
  num_cmds = _num_cmds;
  cmds = (char *)calloc(num_cmds, sizeof(char));
  memcpy(cmds, _cmds, num_cmds*sizeof(char));
}

void SerialCmdParser::update(){
  int num_bytes = 0;

  //get serial data
  if (port == NULL) num_bytes = Serial.available(); //if using default Serial port
  else num_bytes = port->available();               //user specified another port
  if (num_bytes == 0) return;

  char bytes[num_bytes];
  if (port == NULL) Serial.readBytes(bytes, num_bytes); //if using default Serial port
  else port->readBytes(bytes, num_bytes);               //user specified another port

  //split string into commands, separated by ";"
  char * cmd_ptr;
  cmd_ptr = strchr(bytes, ';');
  while (cmd_ptr != NULL){
    int cmd_size = cmd_ptr - bytes +1;
    char cmd_str[cmd_size];
    strncpy(cmd_str, bytes, cmd_size-1);

    //parse out cmd_str into cmd, cmd_idx, and data
    //find space separated tokens, first part is the cmd+cmd index, second is data
    char hdr[255] = {0};
    char * data = NULL;
    char cmd_char = ' ';
    int cmd_idx = 0;
    int hdr_len = strcspn(cmd_str, " ");
    char * tok_ptr = strtok(cmd_str, " ");
    sprintf(hdr, "%s\n", tok_ptr);
    
    //look for characters in the command list
    char * c_ptr = NULL;
    for (int i=0; i<num_cmds; i++) {
      c_ptr = strchr(hdr, cmds[i]);
      if (c_ptr != NULL) {
        cmd_char = cmds[i];
        //look for command index
        char * ptr_end;
        cmd_idx = strtol(c_ptr+1, &ptr_end, 10);
        break;
      }
    }

    //get data if there is any 
    //BUG:(everything until next space, doesn't handle spaces in data)
    tok_ptr = strtok(NULL, " ");
    if (tok_ptr != NULL) {
      int data_size = strlen(tok_ptr);
      data = (char*)calloc(data_size, sizeof(char));
      sprintf(data, "%s\n", tok_ptr);
    }

    if (cmd_char != ' ') {
      //YOU HAVE COMMANDS!
      //send everything to the callback function
      if (callback_f != NULL) {
        (*callback_f)(cmd_char, cmd_idx,(void*)data);
      }
    }
    
    //parse other commands in the buffer if there are any
    int bytes_left = num_bytes-cmd_size+1;
    if (bytes_left > 0) {
      char temp[bytes_left];
      memcpy(temp, cmd_ptr+1, bytes_left);
      Serial.println(temp);
      memset(bytes, 0, num_bytes);
      memcpy(bytes, temp, bytes_left);
      cmd_ptr = strchr(bytes, ';');
    }else {
      cmd_ptr = NULL;
    }
  }

}
