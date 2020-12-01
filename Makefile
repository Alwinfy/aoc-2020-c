SOURCES = $(shell find -name '*.cpp' -or -name '*.c')
ALL     = $(basename $(SOURCES))
TARGETS = $(basename $(shell grep -zl 'int\s\+main' $(SOURCES)))
-include 

default: all
all: $(TARGETS)

CCFLAGS  = -Wall -Wextra -pipe -pedantic
CXXFLAGS = $(CCFLAGS) -std=c++17
CFLAGS   = $(CCFLAGS) -ansi
LDFLAGS  =

ifdef FLAGS
	CCFLAGS += $(FLAGS)
endif

DEBUG ?= 0
ifneq ($(DEBUG),0)
	CCFLAGS += -g -O0 -DDEBUG -fsanitize=address
	LDFLAGS += -fsanitize=address
else
	CCFLAGS += -O2
endif

DEPDIR   = .deps
DEPFLAGS = $(CCFLAGS) -MM -MG
MKDIR = mkdir -p 

%.o: %.cpp $(DEPDIR)/%.d
	$(CXX) -c $(CXXFLAGS) $< -o $@

%.o: %.c $(DEPDIR)/%.d
	$(CC) -c $(CFLAGS) $< -o $@

%: %.o
	$(CXX) -o $@ $^ $(LDFLAGS)

.PHONY: deps
deps: $(patsubst %,$(DEPDIR)/%.d,$(ALL))
$(DEPDIR)/%.d: %.cpp
	@$(MKDIR) $(DEPDIR)/$(dir $<)
	$(CXX) $(DEPFLAGS) -MF $@ $<

$(DEPDIR)/%.d: %.c
	@$(MKDIR) $(DEPDIR)/$(dir $<)
	$(CC) $(DEPFLAGS) -MF $@ $<

#############################
### Put linker rules here ###
#############################

day01: arraylist.o
arraylist_test: arraylist.o



clean: clean-obj clean-deps
	$(RM) $(TARGETS)
clean-obj:
	$(RM) *.o
clean-deps:
	$(RM) -r $(DEPDIR)

ifeq (,$(filter clean%,$(MAKECMDGOALS)))
-include $(ALL:%=$(DEPDIR)/%.d)
endif

.PHONY: default clean clean-obj clean-deps deps all
.PRECIOUS: $(DEPDIR)/%.d
.SUFFIXES:
