CC=g++

main.exe: main.o Xml.o Parser.o
	$(CC) main.o Xml.o Parser.o -o main.exe
main.o: main.cpp
	$(CC) -c main.cpp -o main.o
Xml.o: Xml.cpp
	$(CC) -c Xml.cpp -o Xml.o
Parser.o: Parser.cpp
	$(CC) -c Parser.cpp -o Parser.o
clean:
	del *.o a.exe