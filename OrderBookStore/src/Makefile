###############
## Makefile
###############

BINARY=../bin/OrderBookStoreBinary
BOOST_BASE=${BOOST_INSTALL_ROOT}
SPARSE_HASHMAP_BASE=${SPARSE_HASHMAP_ROOT}
TBB_BASE=${TBB_BASE_ROOT}
#SGI_STL_BASE=/Users/ankithbti/Desktop/AlphaMineTrading/sgi_stl

ARCH = $(shell uname -m)	# ARCH becomes the output from the uname -m command.
CXX=/usr/bin/g++
CXXFLAGS=-g -O2 -std=c++11 -Wall $(INCS)

SRCS = \
	OrderBookImproved.cpp \
	TcpClientSession.cpp \

MAIN_SRC = Main.cpp

OBJS = $(patsubst %.cpp, %.o, $(SRCS))
MAIN_OBJ = $(patsubst %.cpp, %.o, $(MAIN_SRC))

INCS = -I../include \
	   -I../../Common/include \
	   -I../../third_party/spdlog/include \
       -I$(BOOST_BASE)/include \
       -I${SPARSE_HASHMAP_BASE}/include \
       -I${TBB_BASE_ROOT}/include \

LIBS = \
        -L$(BOOST_BASE)/lib -lboost_thread \
        -L$(BOOST_BASE)/lib -lboost_system \
        -L$(BOOST_BASE)/lib -lboost_iostreams \
        -L$(BOOST_BASE)/lib -lboost_chrono \
        -L$(BOOST_BASE)/lib -lboost_date_time \
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
