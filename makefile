APP     := server
TEST    := test_server
CLIENT  := client
CXX := g++

# Project paths
OBJDIR   := obj
SRC_DIR  := src/server src/server/tcp
INC_DIR  := -I./inc -I./inc/tcp
TEST_DIR := test

OBJDIR_CLIENT   := obj
SRC_DIR_CLIENT  := src/client
INC_DIR_CLIENT  := inc inc/tcp
TEST_DIR_CLIENT := test

# Library flags and paths
ASIO_INCLUDES  := lib/asio/include/
ASIO_FLAGS     := -DASIO_STANDALONE
GTEST_INCLUDES := usr/include/
GTEST_LIBS     := -lgtest -lgtest_main
GMOCK_INCLUDES := usr/include/
GMOCK_LIBS     := -lgmock
PTHREAD_LIBS   := -lpthread

# Source files names and rules
SRCS    := $(shell find $(SRC_DIR) -maxdepth 1 -name '*.cpp')
SRCDIRS := $(shell find $(SRC_DIR) -maxdepth 1 -name '*.cpp' -exec dirname {} \; | uniq)
OBJS    := $(patsubst %.cpp,$(OBJDIR)/%.o,$(SRCS))
DEPS    := $(patsubst %.cpp,$(OBJDIR)/%.d,$(SRCS))

# Client source file names and variables
SRCS_CLIENT    := $(shell find $(SRC_DIR_CLIENT) -name '*.cpp')
SRCDIRS_CLIENT := $(shell find $(SRC_DIR_CLIENT) -name '*.cpp' -exec dirname {} \; | uniq)
OBJS_CLIENT    := $(patsubst %.cpp,$(OBJDIR_CLIENT)/%.o,$(SRCS_CLIENT))
DEPS_CLIENT    := $(patsubst %.cpp,$(OBJDIR_CLIENT)/%.d,$(SRCS_CLIENT))

# Target test variables
TEST_SRCS    := $(filter-out src/server/server.cpp, $(SRCS))
TEST_OBJS    := $(filter-out obj/server/server.o, $(OBJS))
TEST_DEPS    := $(filter-out obj/server/server.d, $(DEPS))
TEST_SRCS    := $(filter-out src/client/client.cpp, $(SRCS_CLIENT))
TEST_OBJS    := $(filter-out obj/client/client.o, $(OBJS_CLIENT))
TEST_DEPS    := $(filter-out obj/client/client.d, $(DEPS_CLIENT))

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

INCLUDES       := $(INC_DIR) -I./$(ASIO_INCLUDES)
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

client: buildrepo-client client_compile

test: buildrepo $(TEST)

$(APP): $(OBJS)
	$(CXX) $(LDFLAGS) $^ $(LIBS) -o $@

client_compile: $(OBJS_CLIENT)
	$(CXX) $(LDFLAGS) $^ $(LIBS) -o $(CLIENT)

$(TEST): $(TEST_OBJS)
	$(CXX) $(LDFLAGS) $^ $(LIBS) -o $@
	./$@ $(GTEST_FILTER)$(FILTER)

$(OBJDIR)/%.o: %.cpp
	$(CXX) $(CPPFLAGS) $(DEPENDS) $< -o $@

clean:
	$(RM) -r $(OBJDIR)
	$(RM) $(APP)
	$(RM) $(CLIENT)

distclean: clean
	$(RM) $(APP)

buildrepo:
	@$(call make-repo)
buildrepo-client:
	@$(call make-repo-client)

define make-repo
   for dir in $(SRCDIRS); \
   do \
    mkdir -p $(OBJDIR)/$$dir; \
   done
endef

define make-repo-client
   for dir in $(SRCDIRS_CLIENT); \
   do \
    mkdir -p $(OBJDIR)/$$dir; \
   done
endef

ifeq "$(MAKECMDGOALS)" "all"
-include $(DEPS)
endif

ifeq "$(MAKECMDGOALS)" "client"
-include $(DEPS_CLIENT)
endif

ifeq "$(MAKECMDGOALS)" "test"
-include $(TEST_DEPS)
endif
