# binary name
APP = main

# all source are stored in SRCS-y
#SRCS-y := hexstr2bin.c main.c
OBJS = hexstr2bin.o #.o文件与.cpp文件同名
LIB = libutils.a # 目标文件名

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
CFLAGS += -g -Wall -O3
CC=gcc


# 2. 生成.o文件
%.o : %.c
	$(CC) $(CFLAGS) -c $< -o $@ 

$(LIB) : $(OBJS)
	rm -f $@
	ar cr $@ $(OBJS)
	rm -f $(OBJS)

#$(APP): $(SRCS-y) Makefile 
#	$(CC) $(CFLAGS)  $(SRCS-y) -o $@ 


ifeq ($(shell uname),Linux)
.PHONY: clean
clean:
	echo "Linux rm"
	rm -f *.o
	rm -f *.exe
	rm -f *.a
	rm -f *.so
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
