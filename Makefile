-include .env

# Compilers
CXX   := g++
GLSLC := /usr/bin/glslc

# Target
TARGET := a.out

# Flags
CXXFLAGS := -std=c++17 -I. -I$(VULKAN_SDK_PATH)/include
LDFLAGS  := -L$(VULKAN_SDK_PATH)/lib \
            $(shell pkg-config --static --libs glfw3) \
            -lvulkan

# Source files
CPP_SOURCES := $(wildcard *.cpp)
CPP_HEADERS := $(wildcard *.hpp)

# Shader sources
VERT_SOURCES := $(shell find shaders -type f -name '*.vert')
FRAG_SOURCES := $(shell find shaders -type f -name '*.frag')

# Shader outputs
VERT_SPV := $(VERT_SOURCES:.vert=.vert.spv)
FRAG_SPV := $(FRAG_SOURCES:.frag=.frag.spv)
SHADERS  := $(VERT_SPV) $(FRAG_SPV)

# Default target
all: $(TARGET)

# Build executable
$(TARGET): $(CPP_SOURCES) $(CPP_HEADERS) $(SHADERS)
	$(CXX) $(CXXFLAGS) -o $@ $(CPP_SOURCES) $(LDFLAGS)

# Shader compilation rules
%.vert.spv: %.vert
	$(GLSLC) $< -o $@

%.frag.spv: %.frag
	$(GLSLC) $< -o $@


test: $(TARGET)
	./$(TARGET)

shaders: $(SHADERS)

clean:
	rm -f $(TARGET)
	find shaders -type f -name '*.spv' -delete

# Utility targets
.PHONY: all run test clean shaders
