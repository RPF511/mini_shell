CC= gcc
CFLAGS= -g -Wall -O2
SRC_DIR= ./src
OBJ_DIR= ./obj
INCLUDE= -I./includes
LDFLAGS=

TARGET=minishell_rpf511


SRCS = $(notdir $(wildcard $(SRC_DIR)/*.c))

OBJS = $(SRCS:.c=.o)
DEPS = $(SRCS:.c=.d)
OBJ_LIST = $(patsubst %.o,$(OBJ_DIR)/%.o,$(OBJS))
DEP_LIST = $(OBJECTS:.o=.d)


all : $(TARGET)

$(OBJ_DIR)/%.o : $(SRC_DIR)/%.c
	mkdir -p $(@D)
	$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@ -MD $(LDFLAGS)
$(TARGET) : $(OBJ_LIST)
	$(CC) $(CFLAGS) $(OBJ_LIST) -o $(TARGET) $(LDFLAGS)

.PHONY: clean all
clean:
	rm -f $(OBJECTS) $(DEP_LIST) $(TARGET)

-include $(DEP_LIST)