######################################################################
#                         Makefile                                   #
######################################################################
CC          := ccache g++
SRC_DIR     := ./src/xBlogMain.cpp  ./src/xBlog.cpp ./src/xConfig.cpp ./src/xBlogApi.cpp
SRC_DIR     += ./src/common/jsoncpp.cpp ./src/common/xIniFile.cpp ./src/common/xLog.cpp ./src/MultipartParase.cpp 
SRC_DIR     += ./src/common/xMysqlPool.cpp ./src/common/xUntil.cpp ./src/common/multipart_parser.cpp

INC_DIR     := /usr/local/include
INC_DIR     += ./src/common
INC_DIR     += ./src
INC_DIR     += ./
LIB_DIR     := ./lib 
LIB_DIR     += /usr/local/lib

INSTALL_PATH := .

SRC_FILES   := $(wildcard $(SRC_DIR))
OBJ_FILES   := $(SRC_FILES:.cpp=.o)
LD_LIBS     := rt pthread
AR_LIBS     := event
STATIC_LIBS := -Wl,-Bstatic $(addprefix -l,$(AR_LIBS)) -Wl,-Bdynamic

APP_NAME    := xweb
APP_SUFFIX  := 
APP_TARGET  := $(APP_NAME)$(APP_SUFFIX)

.PHONY: all clean

LDFLAGS     := $(addprefix -L, $(LIB_DIR)) $(addprefix -l,$(LD_LIBS)) $(STATIC_LIBS) `mysql_config --libs `
CFLAGS      := -Wall -g -Werror -O2 $(addprefix -I,$(INC_DIR)) `mysql_config  --cflags`
CFLAGS      += -D_REENTRANTm -D__STDC_FORMAT_MACROS


all: $(APP_TARGET);

$(APP_TARGET): $(OBJ_FILES)
	$(CC) -o $(APP_TARGET) $(CFLAGS) $(OBJ_FILES) $(LDFLAGS)
	@echo *********Build $(APP_TARGET:.a=.so) Successful*********
	

%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

install:
	@echo install...
	cp $(APP_NAME) /home/xsky/$(APP_NAME)
	@scp $(APP_NAME) tongtong@221.228.215.38:/data/immserver/pushserver/test

yun:
	@echo install...
	@ssh xsky@121.42.10.151 'cd /home/xsky/xdb/ &&./xapp.sh stop'
	@scp $(APP_NAME) xsky@121.42.10.151:/home/xsky/xdb
	@ssh xsky@121.42.10.151 'cd /home/xsky/xdb/ &&./xapp.sh start'

clean:
	@echo clean...
	@rm -rf $(OBJ_FILES)
	@rm -rf *.orig

    
    

