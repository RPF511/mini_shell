CC= gcc
CFLAGS= -g -Wall -O2
SRC_DIR= ./src
OBJ_DIR= ./obj
LIB_DIR= lib
INCLUDE= -I./includes
LDFLAGS=

TARGET=minishell_rpf511

LIBS_C = $(notdir $(wildcard $(SRC_DIR)/$(LIB_DIR)/*.c))
LIBS = $(patsubst %.c,$(LIB_DIR)/%.c,$(LIBS_C))
SRCS = $(notdir $(wildcard $(SRC_DIR)/*.c))

OBJS = \
	$(LIBS:.c=.o) \
	$(SRCS:.c=.o)
# DEPS = \
# 	$(LIBS_LIB:.c=.d) \
# 	$(SRCS_LIB:.c=.d)
OBJ_LIST = $(patsubst %.o,$(OBJ_DIR)/%.o,$(OBJS))
DEP_LIST = $(OBJ_LIST:.o=.d)
$(info $$var is [${OBJ_LIST}])

all : $(TARGET)

$(OBJ_DIR)/%.o : $(SRC_DIR)/%.c
	mkdir -p $(@D)
	$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@ -MD $(LDFLAGS)
$(TARGET) : $(OBJ_LIST)
	$(CC) $(CFLAGS) $(OBJ_LIST) -o $(TARGET) $(LDFLAGS)

.PHONY: clean all
clean:
	rm -f $(OBJ_LIST) $(DEP_LIST) $(TARGET)

-include $(DEP_LIST)