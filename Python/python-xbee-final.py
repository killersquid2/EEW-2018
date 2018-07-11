import time
from digi.xbee.devices import XBeeDevice

def main():
    coord = XBeeDevice('COM7', 9600) # Create XBeeDevice object for the coordinator at COM7 with Baud Rate 9600 
    try:
        coord.open() # Open communication with the XBee device
        coord.flush_queues()

        xbee_network = coord.get_network() # Get the network that coord is connected to
        router2 = xbee_network.discover_device('R2') # Find the remote device on the network with ID 'R2'

        if router2 is None: # If we could not find R2 then the program will continue to run
            print("Could not find the remote device")
        else :
            print("Remote device found")

        while True:
            xbee_message = coord.read_data() # Read the data from R1 on the Sensor Arduino
            if xbee_message is not None:
                timestamp = round(time.time()) # Get the timestamp
                data_raw = xbee_message.data.decode().split(",") # Turn the sensor data into a list
                data = [float(element) for element in data_raw] # Turn each element of data_raw into a float

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
        if coord is not None and coord.is_open(): # Closes the communications to the coordinator when the program closes
            coord.close()

if __name__== '__main__':
    main()