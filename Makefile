.PHONY: clean dist

ARCHIVE = ../jaccard-distance.tar.gz

dist: clean
	tar -czf $(ARCHIVE) \
		jaccard/ \
		doc/ \
		Makefile

clean:
	$(MAKE) -C jaccard -f makefile clean
	rm -f $(ARCHIVE)
