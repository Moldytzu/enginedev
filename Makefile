
all:
	$(MAKE) -C engine -j$(shell nproc)
	$(MAKE) -C sandbox

clean:
	$(MAKE) -C engine clean
	$(MAKE) -C sandbox clean