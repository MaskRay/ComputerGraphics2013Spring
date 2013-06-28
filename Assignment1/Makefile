.SUFFIXES:
.PHONY: all dist clean release

CXXFLAGS += -Wall -Wextra -Weffc++ -I. -std=c++11
P := hw1
BUILD := build

DIST_SRC := src README.md Makefile report/css/*.styl report/vendor report/*.jade report/package.json report/Gruntfile.coffee
RELEASE_SRC := $(DIST_SRC) build/hw1 report/css/*.css report/*.html

SRC := $(wildcard src/*.cc)
OBJ := $(addprefix $(BUILD)/,$(notdir $(SRC:.cc=.o)))

all: $(BUILD)/$(P)

$(BUILD)/$(P): $(OBJ) | $(BUILD)
	$(LINK.cc) $^ $(LOADLIBES) $(LDLIBS) $$(pkg-config --libs opencv) -o $@

$(BUILD):
	mkdir $@

sinclude $(OBJ:.o=.d)

$(BUILD)/%.o: src/%.cc
	g++ -std=c++11 -MM -MP -MT $@ -MF $(@:.o=.d) $<
	$(COMPILE.cc) $(OUTPUT_OPTION) $<

clean:
	-$(RM) -r $(BUILD)/* *.tar.gz

dist:
	tar zcf /tmp/dist.tar.gz --transform 's,^,$P/,' $(DIST_SRC)

release: all
	tar zcf /tmp/release.tar.gz --transform 's,^,$P/,' $(RELEASE_SRC)
