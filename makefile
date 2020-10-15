BUILD_DIR = ./build
CC = gcc
LD = ld
LIB = -I lex/
CFLAGS = -c $(LIB)
# LDFLAGS = -Map $(BUILD_DIR)/scc.map

OBJS = $(BUILD_DIR)/scc.o $(BUILD_DIR)/lex.o $(BUILD_DIR)/dynarray.o \
		$(BUILD_DIR)/dynstring.o

###### c代码编译 ######
$(BUILD_DIR)/scc.o: lex/scc.c lex/scc.h \
					lex/lex.h
	$(CC) $(CFLAGS) $< -o $@

$(BUILD_DIR)/lex.o: lex/lex.c lex/lex.h \
					lex/dynstring.h lex/dynarray.h
	$(CC) $(CFLAGS) $< -o $@

$(BUILD_DIR)/dynarray.o: lex/dynarray.c lex/dynarray.h
	$(CC) $(CFLAGS) $< -o $@

$(BUILD_DIR)/dynstring.o: lex/dynstring.c lex/dynstring.h
	$(CC) $(CFLAGS) $< -o $@

###### 链接文件 ######
scc: $(OBJS)
	$(LD) $^ -o $@

.PHONT : clean mk_dir all

mk_dir:
	if [ ! -d $(BUILD_DIR) ]; then mkdir $(BUILD_DIR);fi

clean:
	cd $(BUILD_DIR) && rm -r ./*

build: scc

all: mk_dir build