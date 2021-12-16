# binary name
APP = getimsi

# all source are stored in SRCS-y
SRCS-y := main.cpp msg.cpp SccpAddr.cpp common.cpp \
		  tcuser.cpp getimsi.cpp sccpuser.cpp

ifeq ($(OS),Windows_NT)
		PLATFORM="Windows"
		SHELL=powershell.exe
else 
ifeq ($(shell uname),Darwin)
		PLATFORM="MacOS"
else 
ifeq ($(shell uname),Linux)
		PLATFORM="Unix-Like"
endif
endif
endif

# Add flag 
CPPFLAGS += -g -Wall -std=c++11
INCLUDE = -I../
LIB = -L../
LOADPATH=-Wl,-rpath=../
CC=g++


$(APP): $(SRCS-y) Makefile 
	$(CC) $(CPPFLAGS) $(SRCS-y) $(INCLUDE) $(LIB) -lutils $(LOADPATH) -o $@ 


ifeq ($(shell uname),Linux)
.PHONY: clean
clean:
	echo "Linux rm"
	rm -f *.o
	rm -f *.exe
	rm -f $(APP)
else 
ifeq ($(OS),Windows_NT)
SHELL=powershell.exe
.PHONY: clean
clean:
	echo "Windows rm"
	rm -Force *.o
	rm -Force *.exe
	rm -Force $(APP)
endif
endif
