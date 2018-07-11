import time
from digi.xbee.devices import XBeeDevice

def main():
    coord = XBeeDevice('COM7', 9600) #create Xbeedevice Coord at COM7 & baud rate 9600 
    try:
        coord.open() #calling method to open communication with Xbee device
        coord.flush_queues()

        xbee_network = coord.get_network() #getting a newtwork and assigning it to the xbee
        router2 = xbee_network.discover_device('R3') # find the remote device on the network at R1

        if router2 is None:
            print("Could not find the remote device")
        else :
            print("Remote device found")

        while True:
            xbee_message = coord.read_data()
            if xbee_message is not None:
                timestamp = round(time.time())
                data_raw = xbee_message.data.decode().split(",")
                data = [float(element) for element in data_raw]
                # Temperature
                print("At {0} Temperature is {1}".format(timestamp, data[0]), end=": ")
                if data[0] > 23.7:
                    print("Unsafe")
                    if router2 is not None:
                        coord.send_data(router2, "T1")
                else:
                    print("Safe")
                    if router2 is not None:
                        coord.send_data(router2, "T0")
    finally:
        if coord is not None and coord.is_open():
            coord.close()

if __name__== '__main__':
    main()