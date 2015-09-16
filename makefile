######################################################################
#                        xBlog Makefile                              #
######################################################################
CC          := g++
SRC_DIR     := *.cpp ./common/*.cpp ./common/json/*.cpp
#INC_DIR中，非默认路径，添加自己的对应路径，比如webscalesql：/usr/local/mysql/include
INC_DIR     := /usr/include/mysql
INC_DIR     += ./include 
INC_DIR     += ./common
#自定义webscalesqlclient_r等的路径，加入-Wl,-rpath保证程序运行时也能找到
#LIB_DIR     += -L/usr/local/mysql/lib -Wl,-rpath /usr/local/mysql/lib

INSTALL_PATH := .

SRC_FILES   := $(wildcard $(SRC_DIR))
OBJ_FILES   := $(SRC_FILES:.cpp=.o)
#如果使用webscalesql改为：webscalesqlclient_r z m event
LD_LIBS     := mysqlclient_r z m event

APP_NAME    := xblog
APP_SUFFIX  := 
APP_TARGET  := $(APP_NAME)$(APP_SUFFIX)

.PHONY: all clean

LDFLAGS     := $(LIB_DIR) $(addprefix -l,$(LD_LIBS)) `mysql_config --cflags --libs`
CFLAGS      := -Wall -O2  $(addprefix -I,$(INC_DIR))  
CFLAGS      += -ggdb2


all: $(APP_TARGET);

$(APP_TARGET): $(OBJ_FILES)
	$(CC) -o $(APP_TARGET) $(CFLAGS) $(OBJ_FILES) $(LDFLAGS)
	@echo *********Build $(APP_TARGET:.a=.so) Successful*********


%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

install:
	@echo install...

clean:
	@echo clean...
	@rm -rf $(OBJ_FILES)

