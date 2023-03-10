
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

// Returns 1 if the message is valid and 0 if the data is corrupted
bool status_check(Kvh1750Data* imuData, uint8_t *bytearray){
    
    uint8_t health;

    imuData->status = (uint8_t) bytearray[28];
    cout << "Message Status: "<< (uint32_t)imuData->status << endl; // should be hex
    
    health = imuData->status ^ 0x77;
    if(health != 0){
        cout << "DATA INVALID: Data is corrupted" << endl;  // modify to show which message is corrupted i.e. Gyro X, Acceleroemter Y, etc.
        return 0;
    }

    return 1;
}

// Returns 0 if message counter is greater than previous
// Returns -1 otherwise as data is invalid
int sequence_check(Kvh1750Data* imuData, uint8_t *bytearray, uint8_t prev_seq){
    
    imuData->sequence  = (uint8_t) bytearray[29];
    cout << "Message Counter: "<< (uint32_t)imuData->sequence << endl; // Wrong number I think the manual is wrong, either way we just need to verify tha tconsecutive mesages increment the counter

    if(imuData->sequence > prev_seq || imuData->sequence == 0){
        return 0;
    }
    
    cout << "DATA INVALID: Incorrect counter" << endl;
    return -1;
}

// Generate and compare checksum using generator polynomial
// void CRC_check(Kvh1750Data* imuData, uint8_t *bytearray){
// 
//     uint32_t polynomial = 0x04C11DB7;
//
// }

void convert_bytes(Kvh1750Data* imuData, uint8_t *bytearray){
    
    uint32_t Wx=0, Wy=0, Wz=0, Ax=0, Ay=0, Az=0;
    int16_t temp=0;

    Wx = ((uint32_t) bytearray[4]) << 24 | ((uint32_t) bytearray[5]) << 16 | ((uint32_t) bytearray[6]) << 8 | ((uint32_t) bytearray[7]);
    Wy = ((uint32_t) bytearray[8]) << 24 | ((uint32_t) bytearray[9]) << 16 | ((uint32_t) bytearray[10]) << 8 | ((uint32_t) bytearray[11]);
    Wz = ((uint32_t) bytearray[12]) << 24 | ((uint32_t) bytearray[13]) << 16 | ((uint32_t) bytearray[14]) << 8 | ((uint32_t) bytearray[15]);

    memcpy(&imuData->wx, &Wx, sizeof(imuData->wx));
    memcpy(&imuData->wy, &Wy, sizeof(imuData->wy));
    memcpy(&imuData->wz, &Wz, sizeof(imuData->wz));

    Ax = ((uint32_t) bytearray[16]) << 24 | ((uint32_t) bytearray[17]) << 16 | ((uint32_t) bytearray[18]) << 8 | ((uint32_t) bytearray[19]);
    Ay = ((uint32_t) bytearray[20]) << 24 | ((uint32_t) bytearray[21]) << 16 | ((uint32_t) bytearray[22]) << 8 | ((uint32_t) bytearray[23]);
    Az = ((uint32_t) bytearray[24]) << 24 | ((uint32_t) bytearray[25]) << 16 | ((uint32_t) bytearray[26]) << 8 | ((uint32_t) bytearray[27]);

    memcpy(&imuData->ax, &Ax, sizeof(imuData->ax));
    memcpy(&imuData->ay, &Ay, sizeof(imuData->ay));
    memcpy(&imuData->az, &Az, sizeof(imuData->az));

    //Convert from g to m/s^2
    imuData->ax = (imuData->ax)/10;
    imuData->ay = (imuData->ay)/10;
    imuData->az = (imuData->az)/10;

    imuData->temp = ((uint16_t) bytearray[30]) << 8 | ((uint16_t) bytearray[31]);

    cout << "Gyro X: " << imuData->wx << endl;
    cout << "Gyro Y: " << imuData->wy << endl;
    cout << "Gyro Z: " << imuData->wz << endl;

    cout << "Accel X: " << imuData->ax << endl;
    cout << "Accel Y: " << imuData->ay << endl;
    cout << "Accel Z: " << imuData->az << endl;

    cout << "Temp: " << imuData->temp << endl;

}

int main(){

    Kvh1750Data imuData;

    if(status_check(&imuData, current))
        convert_bytes(&imuData, current);
    
}
