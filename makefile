APP     := server
TEST    := test_server
CXX := g++

# Project paths
OBJDIR   := obj
SRC_DIR  := src
INC_DIR  := inc
TEST_DIR := test

# Library flags and paths
ASIO_INCLUDES  := lib/asio/include/
ASIO_FLAGS     := -DASIO_STANDALONE
GTEST_INCLUDES := usr/include/
GTEST_LIBS     := -lgtest -lgtest_main
GMOCK_INCLUDES := usr/include/
GMOCK_LIBS     := -lgmock
PTHREAD_LIBS   := -lpthread
LIBUV_LIBS     := -luv

# Source files names and rules
SRCS    := $(shell find $(SRC_DIR) -name '*.cpp')
SRCDIRS := $(shell find $(SRC_DIR) -name '*.cpp' -exec dirname {} \; | uniq)
OBJS    := $(patsubst %.cpp,$(OBJDIR)/%.o,$(SRCS))
DEPS    := $(patsubst %.cpp,$(OBJDIR)/%.d,$(SRCS))

TEST_SRCS    := $(filter-out src/server.cpp, $(SRCS))
TEST_OBJS    := $(filter-out obj/src/server.o, $(OBJS))
TEST_DEPS    := $(filter-out obj/src/server.d, $(DEPS))

TEST_SRCS    += $(shell find $(TEST_DIR) -name '*.cpp')
TEST_SRCDIRS := $(shell find $(TEST_DIR) -name '*.cpp' -exec dirname {} \; | uniq)
test: SRCDIRS += $(TEST_SRCDIRS)
TEST_OBJS    += $(patsubst %.cpp,$(OBJDIR)/%.o,$(TEST_SRCS))
TEST_DEPS    += $(patsubst %.cpp,$(OBJDIR)/%.d,$(TEST_SRCS))

# Profiling and debug flags
DEBUG   := -g
GPROF   := -pg
GTEST_FILTER  := --gtest_filter=
FILTER := *

INCLUDES       := -I./$(INC_DIR) -I./$(ASIO_INCLUDES) 
test: INCLUDES += -I./$(GTEST_INCLUDES) -I./$(GMOCK_INCLUDES)
CPPFLAGS       := $(DEBUG) -Wall -Wpedantic $(INCLUDES) -c $(ASIO_FLAGS)
test: CPPFLAGS = $(DEBUG) -Wall -Wpedantic $(INCLUDES) $(TEST_INCLUDES) -c $(ASIO_FLAGS)
#LDFLAGS  :=
# Libraries
LIBS     := $(PTHREAD_LIBS) $(LIBUV_LIBS)
test: LIBS = $(PTHREAD_LIBS) $(GMOCK_LIBS) $(GTEST_LIBS)

DEPENDS  = -MT $@ -MD -MP -MF $(subst .o,.d,$@)

SHELL    = /bin/sh

.PHONY: all clean distclean

all: buildrepo $(APP)

print-%  : ; @echo $* = $($*)

test: buildrepo $(TEST)

$(APP): $(OBJS)
	$(CXX) $(LDFLAGS) $^ $(LIBS) -o $@

$(TEST): $(TEST_OBJS)
	$(CXX) $(LDFLAGS) $^ $(LIBS) -o $@
	./$@ $(GTEST_FILTER)$(FILTER)

$(OBJDIR)/%.o: %.cpp
	$(CXX) $(CPPFLAGS) $(DEPENDS) $< -o $@

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

ifeq "$(MAKECMDGOALS)" "all"
-include $(DEPS)
endif

ifeq "$(MAKECMDGOALS)" "test"
-include $(TEST_DEPS)
endif
