APP     := server

CC := g++
OBJDIR  := obj
SRC_DIR := src
INC_DIR := inc
GTEST_INCLUDES := /usr/include/gtest/
GTEST_LIBS := -lgtest
GMOCK_INCLUDES := /usr/include/gmock/
GMOCK_LIBS := -lgmock
PTHREAD_LIBS = -lpthread
SRCS    := $(shell find $(SRC_DIR) -name '*.cpp')
SRCDIRS := $(shell find $(SRC_DIR) -name '*.cpp' -exec dirname {} \; | uniq)
OBJS    := $(patsubst %.cpp,$(OBJDIR)/%.o,$(SRCS))
DEPS    := $(patsubst %.cpp,$(OBJDIR)/%.d,$(SRCS))

DEBUG    := -g

INCLUDES := -I./$(INC_DIR)
CPPFLAGS   := $(DEBUG) -Wall -Wpedantic $(INCLUDES) -c
# LDFLAGS  := $(QLIBC_RPATH) $(QLIBC_LIBRARY_PATH) $(YAJL_RPATH) $(YAJL_LIBRARY_PATH)
LIBS     := $(PTHREAD_LIBS)

DEPENDS  = -MT $@ -MD -MP -MF $(subst .o,.d,$@)

SHELL    = /bin/sh

.PHONY: all clean distclean


all: buildrepo $(APP)

$(APP): $(OBJS)
	$(CC) $(LDFLAGS) $^ $(LIBS) -o $@

$(OBJDIR)/%.o: %.cpp
	$(CC) $(CPPFLAGS) $(DEPENDS) $< -o $@

clean:
	$(RM) -r $(OBJDIR)
	$(RM) $(APP)

distclean: clean
	$(RM) $(APP)

buildrepo:
	@$(call make-repo)

define make-repo
   for dir in $(SRCDIRS); \
   do \
    mkdir -p $(OBJDIR)/$$dir; \
   done
endef

ifneq "$(MAKECMDGOALS)" "distclean"
ifneq "$(MAKECMDGOALS)" "clean"
-include $(DEPS)
endif
endif
