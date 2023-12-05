all: run

run: PrintJobsFunctions.o HardwareComp.o ElectricComp.o pin.o PinConnections.o bit.o main.o
	g++ $^ -o $@

PrintJobsFunctions.o: ..\bitClasses\PrintJobsFunctions.cpp
	g++ -c $< -o $@

HardwareComp.o: ..\bitClasses\HardwareComp.cpp
	g++ -c $< -o $@

ElectricComp.o: ..\bitClasses\ElectricComp.cpp
	g++ -c $< -o $@

pin.o: ..\bitClasses\pin.cpp
	g++ -c $< -o $@

PinConnections.o: ..\bitClasses\PinConnections.cpp
	g++ -c $< -o $@

bit.o: .\bit.cpp
	g++ -c $< -o $@

main.o: .\main.cpp
	g++ -c $< -o $@

clean:
	del /Q *.o run
