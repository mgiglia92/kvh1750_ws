
#include <iostream>
#include <fstream>
#include <vector>
#include <string.h>
#include <cctype>
#include <algorithm>
#include <stdint.h>
#include <stdio.h>
//#include <string_view>
using namespace std;

// string a = "FE 81 FF 55 37 A9 6A 6E 38 58 6C 1F B7 5B F8 62 BF 80 3E 78 BB 65 0D 28 3B 0A 37 AC 77 3D 00 28 4B FA 34 D8"; //sample data
struct Kvh1750Data
{
    float header;
    float wx;
    float wy;
    float wz;
    float ax;
    float ay;
    float az;
    uint8_t status;
    uint8_t sequence;
    int16_t temp;
};

Kvh1750Data imuData;

uint8_t current[] = { // actually size 36
     0xFE,  0x81,  0xFF,  0x55, // header 
     0x37,  0xA9,  0x6A,  0x6E, // wx
     0x38,  0x58,  0x6C,  0x1F, // wy 
     0xB7,  0x5B,  0xF8,  0x62, // wz
     0xBF,  0x80,  0x3E,  0x78, // ax
     0xBB,  0x65,  0x0D,  0x28, // ay
     0x3B,  0x0A,  0x37,  0xAC, // az
     0x77,                      // status
     0x3D,                      // sequence  
     0x00,  0x28,               // temperature
     0x4B,  0xFA,  0x34,  0xD8  // CRC
};

// uint32_t data1 = 0x37A96A6E;
// uint32_t data2 = 0x38586C1F;

void convert_bytes(Kvh1750Data* imudata, uint8_t *bytearray, size_t len){
uint32_t Wx, Wy, Wz, Ax, Ay, Az;
uint8_t stat, seq;
int16_t Temp;

for (int i = 0; i<4; i++){
    uint32_t temp1 = ((uint32_t) current[4 + i]) << (24 - 8*i);
    Wx |= temp1;
    uint32_t temp2 = ((uint32_t) current[8 + i]) << (24 - 8*i);
    Wy |= temp2;
    uint32_t temp3 = ((uint32_t) current[12 + i]) << (24 - 8*i);
    Wz |= temp3;
    uint32_t temp4 = ((uint32_t) current[16 + i]) << (24 - 8*i);
    Ax |= temp4;
    uint32_t temp5 = ((uint32_t) current[20 + i]) << (24 - 8*i);
    Ay |= temp5;
    uint32_t temp6 = ((uint32_t) current[24 + i]) << (24 - 8*i); 
    Az |= temp6; 
    } // lmao

    stat = ((uint8_t current[29]));
    seq = ((uint8_t current[30]));
    
for (int i = 0; i < 2; i++){
    int16_t temp7 = ((int16_t) current[31 + i]) << (8 - 8*i);
    Temp |= temp7;
}


    
memcpy(&imuData.wx, &Wx, sizeof(imuData.wx));
cout << imuData.wx << endl;
memcpy(&imuData.wy, &Wy, sizeof(imuData.wy));
cout << imuData.wy << endl;
memcpy(&imuData.wz, &Wz, sizeof(imuData.wz));
cout << imuData.wz << endl;

memcpy(&imuData.ax, &Ax, sizeof(imuData.ax));
cout << imuData.ax << endl;
memcpy(&imuData.ay, &Ay, sizeof(imuData.ay));
cout << imuData.ay << endl;
memcpy(&imuData.az, &Az, sizeof(imuData.az));
cout << imuData.az << endl;

memcpy(&imuData.status, &stat, sizeof(imuData.status));
cout << imuData.status << endl;
memcpy(&imuData.sequence, &seq, sizeof(imuData.sequence));
cout << imuData.sequence << endl;
memcpy(&imuData.temp, &Temp, sizeof(imuData.temp));
cout << imuData.temp << endl;
}

// 

int main(){

    convert_bytes(&imuData, current, sizeof(current));

    // make this for loop bruh
    // uint32_t Wx = ((uint32_t)(current[4])) << 24;
    // Wx |= ((uint32_t)current[5]) << 16;
    // Wx |= ((uint32_t)current[6]) << 8;
    // Wx |= (uint32_t)(current[7]); 

    // memcpy(&imuData.wx, &Wx, sizeof(imuData.wx));
    // cout << imuData.wx << endl;

    // uint32_t Wy = ((uint32_t)(current[8])) << 24;
    // Wy |= ((uint32_t)current[9]) << 16;
    // Wy |= ((uint32_t)current[10]) << 8;
    // Wy |= (uint32_t)(current[11]); 

    // memcpy(&imuData.wx, &Wx, sizeof(imuData.wx));
    // cout << imuData.wx << endl;

    // memcpy(&imuData.wx, &data1, sizeof(imuData.wx));
    // cout << imuData.wx << endl; 
    // memcpy(&imuData.wy, &data2, sizeof(imuData.wy));
    // cout << imuData.wy << endl; 
}

