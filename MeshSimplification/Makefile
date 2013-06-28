.PHONY: all clean

P := MeshSimplification
BUILD := build
DIST_SRC := src Makefile

dist:
	tar zcf /tmp/dist.tar.gz --transform 's,^,$P/,' $(DIST_SRC)

release: all
	tar zcf /tmp/release.tar.gz --transform 's,^,$P/,' $(RELEASE_SRC)
