.SUFFIXES:
.PHONY: all dist clean release

CXXFLAGS += -g -Wall -Wextra -Weffc++ -Iinclude -std=c++11 -D'PACKAGE="ray-tracing"' -DVERSION=0.1
#CXX := clang++
TEX := xelatex -shell-escape -interaction=nonstopmode
P := ray-tracing
BUILD := build

DIST_SRC := src Makefile

SRC := $(shell find src -name '*.cc') src/cmdline.cc
OBJ := $(addprefix $(BUILD)/,$(subst src/,,$(SRC:.cc=.o))) build/cmdline.o

all: src/cmdline.cc $(BUILD)/$(P) 

src/cmdline.cc include/cmdline.h: src/cmdline.ggo
	gengetopt --header-output-dir=include --output-dir=src --c-extension=cc < $<

$(BUILD)/$(P): $(OBJ) | $(BUILD)
	$(LINK.cc) $^ $(LOADLIBES) $(LDLIBS) $$(pkg-config --libs opencv) -o $@

ifneq ($(MAKECMDGOALS),clean)
$(shell mkdir -p $(shell find src -type d -printf '$(BUILD)/%P '))
endif

sinclude $(OBJ:.o=.d)

$(BUILD)/%.o: src/%.cc
	g++ -std=c++11 -Iinclude -MM -MP -MT $@ -MF $(@:.o=.d) $<
	$(COMPILE.cc) $(OUTPUT_OPTION) $<

clean:
	-$(RM) -r $(BUILD) *.tar.gz

dist:
	tar zcf /tmp/dist.tar.gz --transform 's,^,$P/,' $(DIST_SRC)
