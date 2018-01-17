##  @file: extract.py
##  @brief: Python script to extract and display the data transmitted by the binary logger
##  @Authors: Gunj Manseta, Ashish Tak (University of Colorado, Boulder)
##  @Date: 11/12/2017

import sys
import struct
import time

print("\n\nTimestamp\t\tTx Checksum\tRx Checksum\tLog\t\t\tDetails\n");

Log=['HEARTBEAT','LOGGER INITIALZED','GPIO INITIALZED','SYSTEM INITIALIZED', \
     'SYSTEM HALTED','INFO','WARNING','ERROR','PROFILING STARTED', \
     'PROFILING RESULT', 'PROFILING COMPLETED', 'DATA RECEIVED', 'DATA ANALYSIS STARTED', \
     'DATA ALPHA COUNT', 'DATA NUMERIC COUNT', 'DATA PUNCTUATION COUNT', 'DATA MISC COUNT' \
     'DATA ANALYSIS COMPLETED']

with open('KL25Z.log', 'rb') as f:
    try:
        while(1):
            #Variable for calculating the checksum at Receiver
            check=0;

            #Read LogID and add to checksum
            data=f.read(4);
            for j in range(4):
                check^=ord(data[j]);
            LogID = struct.unpack("<i",data)[0];
            if(LogID>18):
                break;

            #Read Timestamp
            data=f.read(4);
            for j in range(4):
                check^=ord(data[j]);
            Timestamp = struct.unpack("<i",data)[0];

            #Read payload size and capture the payload accordingly
            data=f.read(4);
            for j in range(4):
                check^=ord(data[j]);
            size = struct.unpack("<i",data)[0];

            payload=f.read(size);
            for j in range(size):
                check^=ord(payload[j]);

            #Read checksum byte from the sent data for verification
            checksum=ord(f.read(1));

            #Mask off 8 bits for the calculated checksum
            check&=0xff;
            
            print(time.strftime('%Y-%m-%d %H:%M:%S', time.localtime(Timestamp)) + '\t' + str(checksum) + '\t\t' + str(check) + '\t\t' + Log[LogID] + '\t\t\t' + payload + "\n");
    except:
        print('Error in reading the file further');
