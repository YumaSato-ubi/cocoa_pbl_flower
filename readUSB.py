### import ###
import serial
import os
import time
import json

ser = serial.Serial('/dev/ttyUSB0', 115200, timeout=0.1)

value = 0
addr_data = []
manu_data = []

cocoa_UUID = '0000fd6f-0000-1000-8000-00805f9b34fb'

value = {"cocoa":0, "all":0}
with open("cocoa_pbl_flower/cocoa_pbl_flower/num.json","w") as f:
                k = json.dumps(value)
                f.write(k)


while True:
    # print(ser.readline())
    # line = str(ser.readline())
    
    line = ser.readline().split()
    print(line)
    # print(str(line[6]))
    
    if line == []:
        continue
    
    if "manufacturer" in str(line[4]):
        ble_addr = str(line[3][:-1].decode())
        # print(str(line[3][:-1].decode()))
        if ble_addr not in manu_data:
            manu_data.append(ble_addr)
    
    if cocoa_UUID in str(line[5]):
        cocoa_addr = str(line[3][:-1].decode())
        if cocoa_addr not in addr_data:
            addr_data.append(cocoa_addr)
    print(manu_data)
    print(addr_data)
    
    value = {"cocoa":len(addr_data), "all":len(manu_data)}
    with open("cocoa_pbl_flower/cocoa_pbl_flower/num.json","w") as f:
                k = json.dumps(value)
                f.write(k)
    '''
    if line == []:
        continue
    if "ADDR" in str(line[0]):
        if line[1] not in addr:
            addr.append(line[1])
            print(addr)
            value = len(addr)
            value = {"value":value}
            with open("num.json","w") as f:
                k = json.dumps(value)
                f.write(k)
                
    '''

ser.close()
