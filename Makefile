MAKEFILES = $(shell find . -maxdepth 2 -mindepth 2 -name Makefile)

default:
	for m in $(MAKEFILES); do make -C $$(dirname $$m); done

clean:
	for m in $(MAKEFILES); do make -C $$(dirname $$m) clean; done

distclean:
	for m in $(MAKEFILES); do make -C $$(dirname $$m) distclean; done
