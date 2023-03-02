
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
     0x38,  0x58,  0x6C,  0x1F // wy
};

// uint32_t data1 = 0x37A96A6E;
// uint32_t data2 = 0x38586C1F;

void convert_bytes(Kvh1750Data* imudata, uint8_t *bytearray, size_t len){
uint32_t Wx;
for (int i = 0; i<4; i++){
    uint32_t temp = ((uint32_t) current[4 + i]) << (24 - 8*i);
    Wx |= temp;
    } 
memcpy(&imuData.wx, &Wx, sizeof(imuData.wx));
cout << imuData.wx << endl;

}

// 

int main(){
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

    convert_bytes(&imuData, current, sizeof(current));

    // memcpy(&imuData.wx, &data1, sizeof(imuData.wx));
    // cout << imuData.wx << endl; 
    // memcpy(&imuData.wy, &data2, sizeof(imuData.wy));
    // cout << imuData.wy << endl; 
}


// /* 
//     header = <first four bytes>

//     GyroX = <decode next 4>
//     GyroY = <decode next 4>
//     GyroZ = ...

//     turn this into imu.msg
// */
// }
