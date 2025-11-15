CXX      := g++
CXXFLAGS := -std=c++17 
LDFLAGS  := -lglfw -lvulkan -ldl -pthread
TARGET   := VulkanTest
SRC      := main.cpp

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

clean:
	rm -f $(TARGET)

run: $(TARGET)
	./$(TARGET)

.PHONY: all clean

