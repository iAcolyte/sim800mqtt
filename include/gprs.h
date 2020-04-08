#pragma once

#include <Arduino.h>
#include <SoftwareSerial.h>
#include <IPAddress.h>
#include <Client.h>

class GPRSClient : public Client {
    private:
        bool _connect(String address, String port);
        bool isConnected;
    public:
        SoftwareSerial gprsSerial = SoftwareSerial(8,9);
        bool begin(uint8_t pinRX, uint8_t pinTX, String apn);
        bool begin(uint8_t pinRX, uint8_t pinTX, String apn, String login, String password);
        int connect(IPAddress, uint16_t);
        int connect(const char*, uint16_t);
        size_t write(uint8_t);
        size_t write(const uint8_t*, size_t);
        int available();
        int read();
        int read(uint8_t*, size_t);
        int peek();
        void flush();
        void stop();
        uint8_t connected();
        operator bool() { return this->connected()==1; }

        String sendATCommand(String, uint16_t);
        String sendATCommand(const char*, uint16_t);
        GPRSClient();
};