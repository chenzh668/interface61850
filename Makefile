ifndef CROSS_COMPILE
#CROSS_COMPILE	:= arm-none-linux-gnueabihf-
CROSS_COMPILE	:= arm-linux-gnueabihf-
endif
#测试提交
CC			:=	${CROSS_COMPILE}gcc
CPP			:=	${CROSS_COMPILE}cpp
AS			:=	${CROSS_COMPILE}gcc
AR			:=	${CROSS_COMPILE}ar
LINKER			:=	${CROSS_COMPILE}ld
OC			:=	${CROSS_COMPILE}objcopy
OD			:=	${CROSS_COMPILE}objdump
NM			:=	${CROSS_COMPILE}nm
PP			:=	${CROSS_COMPILE}gcc -E
#测试提交
BUILD_DIR := $(PWD)/build
LIB_DIR := -L$(PWD)/src/lib
MAKE_DEP = -Wp,-MD,$(DEP) -MT $$@ -MP

INCLUDES		+=	-I./src				\
                    -I./src/include     \
	                -I./src/Threads \
	                -I./src/interface
_SOURCES_ += ./src/lib61850_main.c  
_SOURCES_ += $(wildcard ./src/Threads/*.c) 
_SOURCES_ += $(wildcard ./src/interface/*.c) 


$(eval OBJS1 := $(patsubst %.c,%.o,$(notdir $(_SOURCES_)))) 
$(info OBJS1 = $(OBJS1))
$(eval OBJS = $(addprefix $(BUILD_DIR)/,$(OBJS1)))
$(info OBJS= $(OBJS))


all:$(OBJS) libiec61850.so
	@echo "12345 ssdlh"
	-cp libiec61850.so  /mnt/hgfs/Tcu_lib/libiec61850_1.so;

libiec61850.so: $(OBJS)
	@echo "54321 xxxxx"
	$(LINKER)   $(LIB_DIR) -fmessage-length=0 -shared $^ -lDllCommon -lpthread -lrt  -o $@ 
define MAKE_C

$(eval OBJ = $(addprefix $(1)/,$(patsubst %.c,%.o,$(notdir $(2)))))
$(info OBJ= $(OBJ))
$(eval DEP := $(patsubst %.o,%.d,$(OBJ)))
$(OBJ): $(2)
	-mkdir -p $(BUILD_DIR)
	@echo "  CC      $$<"
	$(CC) -Wall -shared -fPIC -nostartfiles -c  $(INCLUDES)  $(MAKE_DEP)  $$< -o $$@
endef

#$(eval $(call MAKE_C,$(BUILD_DIR),$(_SOURCES_)))
#gcc -shared -fPIC -nostartfiles -o mylib.so mylib.c

$(eval $(foreach obj,$(_SOURCES_),$(call MAKE_C,$(BUILD_DIR),$(obj))))

.PHONY: clean
clean:
	@echo "clean"
	-rm *.so
	-rm -rf 	$(BUILD_DIR)
