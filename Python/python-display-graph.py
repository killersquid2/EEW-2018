import matplotlib.pyplot as plt

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
        temperature.append(elements[3][1:])
        humidity.append(elements[5][1:])
    return (time, temperature, humidity)

def drawGraph(x, y1, y2):   
    plt.plot(x, y1, "r--", x, y2, "b--")
    print(y2)
    plt.show()

def main():
    data = readFile("logs\\13-07-2018-10-40-53.log")
    times, temps, hums = getDataLists(data)
    print(hums)
    drawGraph(times, temps, hums)


if __name__ == "__main__":
    main()