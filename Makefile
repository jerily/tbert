ifndef NAVISERVER
    NAVISERVER  = /usr/local/ns
endif

#
# Module name
#
MOD      =  libtbert.so

#
# Objects to build.
#
MODOBJS     = library.o

MODLIBS  += -lbert -lggml

include  $(NAVISERVER)/include/Makefile.module