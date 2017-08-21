//Parser expects commands to be separated by semicolons (;) and will ignore input that is not terminated this way
//To test, send commands like "p;" or "s;"
//Parser also processes command indices (ie "p1;" "t5;")
//and space-separated data (ie "p 123;")
//or all of the above (ie "p1 1234,64356;")
#include "SerialCmdParser.h"
#define LED 13

//Init the parser
SerialCmdParser parser = SerialCmdParser();
//Specify which commands to listen for
char cmds[] = {'t','p','s'};

void dataHandler(char cmd, int idx, void * data){
  //print our incoming command data
  Serial.print("cmd: ");
  Serial.println(cmd);
  Serial.print("idx: ");
  Serial.println(idx);
  if (data != NULL) {
    Serial.print("data: ");
    Serial.println((char *)data);
  }

  //let's do something with this data
  switch (cmd) {
    case 't':
      Serial.println(millis());
      break;
    case 'p':
      Serial.println("PLAY:");
      digitalWrite(LED,HIGH);
      break;
    case 's':
      Serial.println("STOP:");
      digitalWrite(LED,LOW);
      break;
  }

}

void setup() {
  Serial.begin(115200);
  delay(1000); //waiting for Serial to init, for teensy

  parser.setCmds(cmds, sizeof(cmds));
  parser.setCallback(dataHandler);

  Serial.println("Parser handles the following commands: (don't forget ';')");
  for (int i=0; i<sizeof(cmds); i++) {
    Serial.print(cmds[i]);
    Serial.println(";");
  }

  pinMode(13, OUTPUT);
  
}

void loop() {
  parser.update();
  
}
