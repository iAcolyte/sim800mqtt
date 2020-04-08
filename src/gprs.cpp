#include "gprs.h"

GPRSClient::GPRSClient(){}

bool GPRSClient::begin(uint8_t pinRX, uint8_t pinTX, String apn){
  this->gprsSerial = SoftwareSerial(pinRX,pinTX);
  this->gprsSerial.begin(9600); 
  while (this->sendATCommand("AT",10).indexOf("OK")==-1){ delay(100);}
  this->sendATCommand("AT+CFUN=1",10);
  this->sendATCommand("AT+CIPMODE=1",1000);
  this->sendATCommand("AT+CPIN?",10);
  this->sendATCommand("AT+CSTT=\""+apn+"\",\"\",\"\"",100);
  delay(100);
  this->sendATCommand("AT+CIICR",10000);
  delay(100);
  String res = this->sendATCommand("AT+CIFSR",10000);
  return res.indexOf("ERROR")==-1;
}
bool GPRSClient::begin(uint8_t pinRX, uint8_t pinTX, String apn, String login, String password){
  this->gprsSerial = SoftwareSerial(pinRX,pinTX);
  this->gprsSerial.begin(9600);
  while (this->sendATCommand("AT",10).indexOf("OK")==-1){ delay(100);}
  this->sendATCommand("AT+CFUN=1",10);
  this->sendATCommand("AT+CPIN?",10);
  this->sendATCommand("AT+CIPMODE=1",1000);
  this->sendATCommand("AT+CSTT=\""+apn+"\",\""+login+"\",\""+password+"\"",100);
  delay(100);
  this->sendATCommand("AT+CIICR",10000);
  delay(100);
  String res = this->sendATCommand("AT+CIFSR",10000);
  return res.indexOf("ERROR")==-1;
}

bool GPRSClient::_connect(String address, String port){

  String res = this->sendATCommand("AT+CIPSTART=\"TCP\",\""+address+"\","+port,10000);
  Serial.println(res);
  if (res.indexOf("CONNECT")!=-1){
    isConnected=true;
    return true;
  }

  while(true) {
    Serial.print('0');
    if (gprsSerial.available()){
      delay(100);
      res = gprsSerial.readString();
      Serial.println(res);
      if (res.indexOf("CONNECT")!=-1) {
        isConnected=true;
        delay(10);
        return true;
      }
    }
    delay(10);
  }
  return false;
}

int GPRSClient::connect(IPAddress ip, uint16_t port){
  String addr = String(ip[0])+"."+String(ip[1])+"."+String(ip[2])+"."+String(ip[3]);
  String prt = String(port);
  return this->_connect(addr,prt);
}
int GPRSClient::connect(const char *host, uint16_t port){
  String addr = String(host);
  String prt = String(port);
  return this->_connect(addr,prt);
}
size_t GPRSClient::write(uint8_t data){
   this->gprsSerial.write(data);
}
size_t GPRSClient::write(const uint8_t *buf, size_t size){

  for (size_t i = 0; i < size; i++)
  {   
    Serial.print("w: ");
    Serial.print(buf[i],HEX);
    Serial.print(" ");
    Serial.println((char)buf[i]);
    this->gprsSerial.write(buf[i]);
  }
  return size;
}
int GPRSClient::available(){
  return this->gprsSerial.available();
}
int GPRSClient::read(){
  if (this->gprsSerial.available()){
    uint8_t byt = this->gprsSerial.read();
    Serial.print("r: ");
    Serial.print(byt,HEX);
    Serial.print(" ");
    Serial.println((char)byt);
    return byt;
  }
  return -1;
}
int GPRSClient::read(uint8_t *buf, size_t size){
  return 0;
}
int GPRSClient::peek(){
  return this->gprsSerial.peek();
}
void GPRSClient::flush(){
  this->gprsSerial.flush();
}
void GPRSClient::stop(){
  gprsSerial.write(0x1A);
  this->sendATCommand("AT+CIPCLOSE",1000);
  isConnected=false;
}

uint8_t GPRSClient::connected(){
    //TODO: Check connected with CIPMODE=1
    return isConnected;
}

String GPRSClient::sendATCommand(String command, uint16_t timeout) {

  this->gprsSerial.println(command);
  while (!this->gprsSerial.available()) { if (timeout==0) return "null"; timeout--; delay(1); }
  String res = this->gprsSerial.readString();
  Serial.println(res);
  if( res[0]=='\0')
    res[0] = 'A';
  return res;
}
