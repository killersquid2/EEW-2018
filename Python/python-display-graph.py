import matplotlib.pyplot as plt

def getGraphInfo():
    finished = False
    while not finished:
        try:
            filename = input("Please enter filename of log file: ")
            with open("logs\\{0}".format(filename), mode="r") as myFile:
                finished = True
        except:
            print("Invalid File Name.")
    finished = False
    while not finished:
        try:
            datatype = input("Would you like a graph of temperature of humidity (T/H): ").lower()
            if datatype in ("t", "h"):
                finished = True
        except:
            print("Invalid Data Type.")
    return (filename, datatype)

def readFile(filename):
    data = []
    with open(filename, mode="r") as myFile:
        lines = myFile.read().split("\n")
        for line in lines:
            if len(line) > 0  and line[0] != "#":
                elements = line.split("\t")
                data.append(elements)
    return data

def getDataLists(data):
    time = []
    temperature = []
    humidity = []
    for elements in data:
        time.append(elements[1])
        temperature.append(float(elements[3][1:]))
        humidity.append(float(elements[5][1:]))
    return (time, temperature, humidity)

def drawGraph(x, y, dataName):   
    plt.plot(x, y, "b--")
    plt.xticks(rotation=90)
    plt.xlabel("Time")
    plt.ylabel(dataName)
    plt.show()

def main():
    filename, datatype = getGraphInfo()
    data = readFile("logs\\{0}".format(filename))
    times, temps, hums = getDataLists(data)
    if datatype == "t":
        drawGraph(times, temps, "Temperature")
    else:
        drawGraph(times, hums, "Humidity")

if __name__ == "__main__":
    main()