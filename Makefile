.DEFAULT_GOAL := release

QPROJECT=object-labeller.pro

release:
	qmake -o build.make $(QPROJECT)
	$(MAKE) -f build.make

.PHONY: clean

clean:
	@rm -f moc_* *.o object-labeller build.make
