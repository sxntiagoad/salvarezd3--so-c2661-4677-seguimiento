CXX      = g++
CXXFLAGS = -Wall -std=c++11
LDLIBS   = -lyaml-cpp

SRC_DIR  = src
BIN      = banker

SRCS = $(SRC_DIR)/banker.cpp $(SRC_DIR)/main-banker.cpp
OBJS = $(SRCS:.cpp=.o)

.PHONY: all clean run-examples

all: $(BIN)

$(BIN): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDLIBS)

$(SRC_DIR)/%.o: $(SRC_DIR)/%.cpp $(SRC_DIR)/banker.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

run-examples: $(BIN)
	@for f in examples/example-*.yaml; do \
	    echo "=========================================="; \
	    echo "Ejemplo: $$f"; \
	    echo "=========================================="; \
	    echo "-1" | ./$(BIN) $$f || true; \
	done

clean:
	rm -f $(OBJS) $(BIN)
