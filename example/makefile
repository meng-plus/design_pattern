# ./demo/Makefile

CC = gcc
CFLAGS = -Wall -Wextra -O2 -g -fPIC
INCLUDES = -I../ -I./

# 目标文件路径
BUILD_DIR = ./build
SRCDIR = .
OBJDIR = $(BUILD_DIR)/obj

# 确保目标文件夹存在
$(shell mkdir -p $(OBJDIR))

# 源文件
SRCS = $(wildcard ../*.c) ./test-factory-observer.c

# 目标文件
TARGET = $(BUILD_DIR)/demo_app.out
# 生成的可执行文件
OBJS = $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(SRCS))

all: $(TARGET)

# 链接 demo_app 可执行文件
$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $@ $(LDFLAGS) $(INCLUDES)

# 编译目标文件
$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR) $(TARGET) $(OBJDIR)/*.o

test: $(TARGET)
	./$(TARGET)
run:
	$(TARGET)

.PHONY: clean all run

# end of Makefile
