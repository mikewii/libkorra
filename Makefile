.SUFFIXES:

ifeq ($(strip $(DEVKITARM)),)
$(error "Please set DEVKITARM in your environment. export DEVKITARM=<path to>devkitARM")
endif

include $(DEVKITARM)/base_rules
include $(DEVKITARM)/3ds_rules

TARGET		:=	korra
INCLUDES	:=	include
SOURCES		:=	src \
                        src/MH4U \
                        src/MH4U/Extentions \
                        src/MHXX \
                        src/MHXX/Enemy \
                        src/MHXX/Extentions \
                        src/MHXX/Quest

ARCH		:=	-march=armv6k -mlittle-endian -mtune=mpcore -mfloat-abi=hard -mtp=soft -mfpu=vfpv2
CFLAGS		:=	-g -Wall -Werror -Wno-format \
						-mword-relocations \
                        -ffunction-sections \
                        -fdata-sections \
                        $(ARCH) \
                        $(BUILD_CFLAGS)

CFLAGS		+=	$(INCLUDE) -D__3DS__ -DN3DS
CXXFLAGS	:=	$(CFLAGS) -fno-rtti -fno-exceptions -std=gnu++11
ASFLAGS		:=	-g $(ARCH) $(INCLUDE)

#---------------------------------------------------------------------------------
# list of directories containing libraries, this must be the top level containing
# include and lib
#---------------------------------------------------------------------------------
LIBS		:=	-lz
LIBDIRS		:=	$(CTRULIB) $(PORTLIBS)
#---------------------------------------------------------------------------------
# no real need to edit anything past this point unless you need to add additional
# rules for different file extensions
#---------------------------------------------------------------------------------
ifneq ($(BUILD),$(notdir $(CURDIR)))
#---------------------------------------------------------------------------------

export VPATH	:=	$(foreach dir,$(SOURCES),$(CURDIR)/$(dir)) \
                        $(foreach dir,$(DATA),$(CURDIR)/$(dir))

CFILES		:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.c)))
CPPFILES	:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.cpp)))
SFILES		:=	$(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.s)))
BINFILES	:=	$(foreach dir,$(DATA),$(notdir $(wildcard $(dir)/*.*)))

#---------------------------------------------------------------------------------
# use CXX for linking C++ projects, CC for standard C
#---------------------------------------------------------------------------------
ifeq ($(strip $(CPPFILES)),)
#---------------------------------------------------------------------------------
        export LD	:=	$(CC)
#---------------------------------------------------------------------------------
else
#---------------------------------------------------------------------------------
        export LD	:=	$(CXX)
#---------------------------------------------------------------------------------
endif
#---------------------------------------------------------------------------------

export OFILES_BIN	:=	$(addsuffix .o,$(BINFILES))
export OFILES_SRC	:=	$(CPPFILES:.cpp=.o) $(CFILES:.c=.o) $(SFILES:.s=.o)

export HFILES_BIN	:=	$(addsuffix .h,$(subst .,_,$(BINFILES)))

export OFILES		:=	$(OFILES_BIN) $(OFILES_SRC)

export INCLUDE		:=	$(foreach dir,$(INCLUDES),-I$(CURDIR)/$(dir)) \
                        $(foreach dir,$(LIBDIRS),-I$(dir)/include) \
                        -I.

.PHONY: clean all

#---------------------------------------------------------------------------------
all: lib/libkorra.a lib/libkorrad.a

install: all
	@mkdir -p $(DEVKITPRO)/libkorra
	@cp -r include $(DEVKITPRO)/libkorra
	@cp -r lib $(DEVKITPRO)/libkorra

lib:
	@[ -d $@ ] || mkdir -p $@

release:
	@[ -d $@ ] || mkdir -p $@

debug:
	@[ -d $@ ] || mkdir -p $@

lib/libkorra.a : lib release $(SOURCES) $(INCLUDES)
	@$(MAKE) BUILD=release OUTPUT=$(CURDIR)/$@ \
	BUILD_CFLAGS="-DNDEBUG=1 -O2 -fomit-frame-pointer" \
	DEPSDIR=$(CURDIR)/release \
 	--no-print-directory -C release \
	-f $(CURDIR)/Makefile

lib/libkorrad.a : lib debug $(SOURCES) $(INCLUDES)
	@$(MAKE) BUILD=debug OUTPUT=$(CURDIR)/$@ \
	BUILD_CFLAGS="-DDEBUG=1 -Og" \
	DEPSDIR=$(CURDIR)/debug \
	--no-print-directory -C debug \
	-f $(CURDIR)/Makefile

#---------------------------------------------------------------------------------
clean:
	@echo clean ...
	@-rm -rf release debug lib

#---------------------------------------------------------------------------------
else

DEPENDS	:=	$(OFILES:.o=.d)

#---------------------------------------------------------------------------------
# main targets
#---------------------------------------------------------------------------------
$(OUTPUT)	:	$(OFILES)

$(OFILES_SRC) : $(HFILES_BIN)

#---------------------------------------------------------------------------------
%.bin.o	%_bin.h :	%.bin
#---------------------------------------------------------------------------------
	@echo $(notdir $<)
	@$(bin2o)


-include $(DEPENDS)

#---------------------------------------------------------------------------------------
endif
#---------------------------------------------------------------------------------------
