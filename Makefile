CC      = gcc
CXX     = g++
LINKC    = gcc
LINKCXX  = g++
CFLAGS  = -g -Wall -O2

TOP_DIR     := $(shell pwd)
SRC_DIRS    := $(shell find $(TOP_DIR) -maxdepth 3 -type d)
LIB         := -lpthread
TARGET      := $(TOP_DIR)/bin/MutexDemo

INC_PATH    = -I$(TOP_DIR)/src/thread \
              -I$(TOP_DIR)/src/mutex 

LIB_PATH    = -L$(TOP_DIR)/lib
EXT_LIB     = -Wl,-rpath,$(TOP_DIR)/lib -lcsay

#$(foreach <var>,<list>,<text>)
#把参数<list>;中的单词逐一取出放到参数<var>;所指定的变量中，然后再执行<text>;所包含的表达式。
#每一次<text>;会返回一个字符串，循环过程中，<text>;的所返回的每个字符串会以空格分隔，最后当整个循环结束时，<text>;
#所返回的每个字符串所组成的整个字符串（以空格分隔）将会是foreach函数的返回值。
#wildcard把 指定目录 dir 下的所有后缀是cpp的文件全部展开(包含路径)。
CXX_SRCS    = $(foreach dir, $(SRC_DIRS), $(wildcard $(dir)/*.cpp))

#patsubst把$(CXX_SRCS)中的变量符合后缀是.cpp的全部替换成.o
CXX_OBJS    = $(patsubst %.cpp, %.o, $(CXX_SRCS))

C_SRCS  = $(foreach dir, $(SRC_DIRS), $(wildcard $(dir)/*.c))
C_OBJS  = $(patsubst %.c, %.o, $(C_SRCS))

all:$(TARGET)

#创建bin文件夹
$(TARGET):$(CXX_OBJS) $(C_OBJS)
	$(shell if [ ! -d $(TOP_DIR)/bin ]; then mkdir $(TOP_DIR)/bin; fi)

ifeq ($(CXX_SRCS),)
	$(LINKC) $(CFLAGS) -o $@ $^ $(EXT_LIB) $(LIB_PATH) $(LIB)
else
	$(LINKCXX) $(CFLAGS) -o $@ $^ $(LIB_PATH) $(LIB)
endif

%.o:%.cpp
	$(CXX) -c -o $@ $< $(INC_PATH) $(CFLAGS)
%.o:%.c
	$(CC) -c -o $@ $< $(INC_PATH) $(CFLAGS)

.PHONY:clean
clean:
	rm -rf $(TARGET) $(CXX_OBJS) $(C_OBJS)