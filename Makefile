# This is just a stub makefile used for travis builds
# to keep things all compiling. Normally you'd use
# one of the makefiles directly.

# CPPFLAGS+=./libs/libopencm3/include/
# LDFLAGS+=./libs/libopencm3/lib/stm32/f1/

# These hoops are to enable parallel make correctly.
GZ_ALL := $(wildcard Makefile.*)

all: $(GZ_ALL:=.all)
clean: $(GZ_ALL:=.clean)

%.all:
	$(MAKE) -f $* all
%.clean:
	$(MAKE) -f $* clean
	
