# Build directory (where executables reside), must expand immediately
BLDDIR := ../bin

TARGETS := $(addprefix $(BLDDIR)/, $(TARGETNAMES))

all: $(TARGETS)

# Order-only prerequisite (denoted by |): 
# BLDDIR must exist before any targets are built.
# However, the timestamp of BLDDIR is not checked.
$(TARGETS): | $(BLDDIR)

# This is invoked only if BLDDIR doesn't exist, 
# so theoretically, there is no need for -p.
# However, -p might prevent a race condition in some rare cases.
$(BLDDIR):
	mkdir -p $(BLDDIR)

# Include directories
INCLUDE_DIRS := ../include
INCLUDES = $(addprefix -I, $(INCLUDE_DIRS))

STD = 99

ifeq "$(STD)" "99"
     CFLAGS += -std=gnu99
endif

# Compile/link flags
CFLAGS += -g -Wall 
CPPFLAGS += $(INCLUDES)

# Library framework
LIBDIR=../lib
LIBSOCK = $(LIBDIR)/libsock.a
#LIBTHREAD = $(LIBDIR)/libthread.a

#LIBS = $(LIBSOCK)

#LIBSOCK_OBJ := $(subst .c,.o,$(wildcard $(LIBDIR)/*.c))
#LIBTHREAD_OBJ := $(LIBDIR)/wrapthread.o

#LIBSOCK_OBJ := $(filter-out $(LIBTHREAD_OBJ), $(LIBSOCK_OBJ))

#$(LIBSOCK): $(LIBSOCK_OBJ)
#	$(AR) r $@ $?

#$(LIBTHREAD): $(LIBTHREAD_OBJ)
#	$(AR) r $@ $?

.PHONY: clean

clean:
	$(QUIET)rm -f $(TARGETS)
	$(QUIET)rm -f *.o *.d *.tmp

distclean: clean
	$(QUIET)rm -rf $(BLDDIR)
#	$(QUIET)rm -f $(LIBS)
#	$(QUIET)rm -f $(LIBOBJ)
