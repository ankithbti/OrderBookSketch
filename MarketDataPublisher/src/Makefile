###############
## Makefile
###############

BINARY=../bin/MarketDataPublisherBinary
BOOST_BASE=${BOOST_INSTALL_ROOT}

ARCH = $(shell uname -m)	# ARCH becomes the output from the uname -m command.
CXX=/usr/bin/g++
CXXFLAGS=-g -O2 -std=c++11 -Wall $(INCS)

SRCS = \

MAIN_SRC = Main.cpp

OBJS = $(patsubst %.cpp, %.o, $(SRCS))
MAIN_OBJ = $(patsubst %.cpp, %.o, $(MAIN_SRC))

INCS = -I../include \
	   -I../../Common/include \
	   -I../../third_party/spdlog/include \
       -I$(BOOST_BASE)/include \

LIBS = \
        -L$(BOOST_BASE)/lib -lboost_thread \
        -L$(BOOST_BASE)/lib -lboost_system \
        -L$(BOOST_BASE)/lib -lboost_iostreams \
        -L$(BOOST_BASE)/lib -lboost_chrono \
        -L$(BOOST_BASE)/lib -lboost_regex \
		-lpthread

all: $(BINARY)
	
$(BINARY): $(OBJS) $(MAIN_OBJ)
	$(CXX) -o $(BINARY) $(OBJS) $(MAIN_OBJ) $(LIBS)

$(SHARED_LIB): $(OBJS)
	$(CXX) -shared $^ -o $@ $(LIBS)

createlib: $(SHARED_LIB)
	
run:
	@echo "----------------------------"
	@echo "Running the programme. "
	@echo "----------------------------"
	@echo "----------------------------"
	$(BINARY)
	@echo "----------------------------"

clean:
	rm -rf $(BINARY) $(SHARED_LIB) *.o *.log
