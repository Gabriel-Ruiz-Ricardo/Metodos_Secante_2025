CXX = g++
CXXFLAGS = -std=c++17 -O2 -Wa,-mbig-obj
TARGET = programa
SRC = Secante.cpp

$(TARGET): $(SRC)
	$(CXX) $(SRC) -o $(TARGET) $(CXXFLAGS)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET) $(TARGET).exe