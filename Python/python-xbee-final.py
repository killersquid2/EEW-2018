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
            print("Could not find the remote alert device (R2)")
        else :
            print("Remote alert device (R2) found")

        with open("log.txt", mode="a") as myFile: # Opens the log file
            while True:
                xbee_message = coord.read_data() # Read the data from R1 on the Sensor Arduino
                if xbee_message is not None:
                    timestamp = round(time.time()) # Get the timestamp
                    data_raw = xbee_message.data.decode().split(",") # Turn the sensor data into a list
                    data = [float(element) for element in data_raw] # Turn each element of data_raw into a float
                    dataToSend = ""

                    # Temperature
                    print("\nAt {0} Temperature is {1}".format(timestamp, data[0]), end=": ")
                    if data[0] > 24.65:
                        print("Unsafe")
                        dataToSend += "T1"
                        stateT = "U"
                    else:
                        print("Safe")
                        dataToSend += "T0"
                        stateT = "S"

                    # Pressure
                    print("At {0} Pressure is {1}".format(timestamp, data[1]), end=": ")
                    if data[1] > 1000:
                        print("Unsafe")
                        dataToSend += "P1"
                        stateP = "U"
                    else:
                        print("Safe")
                        dataToSend += "P0"
                        stateP = "S"
                    
                    # Humidity
                    print("At {0} Humidity is {1}".format(timestamp, data[2]), end=": ")
                    if data[2] > 70:
                        print("Unsafe")
                        dataToSend += "H1"
                        stateH = "U"
                    else:
                        print("Safe")
                        stateH = "S"
                        dataToSend += "H0"
                    
                    if router2 is not None:
                        coord.send_data(router2, dataToSend)

                    text = "<{0}>\tT{1}{2}\tP{3}{4}\tH{5}{6}\n".format(timestamp, stateT, data[0], stateP, data[1], stateH, data[2])
                    myFile.write(text)
    finally:
        if coord is not None and coord.is_open(): # Closes the communications to the coordinator when the program closes
            coord.close()

if __name__== '__main__':
    main()