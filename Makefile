all:
	@$(MAKE) -C examples/fn_01
	@$(MAKE) -C examples/fn_02
	@$(MAKE) -C examples/fn_03
	@$(MAKE) -C examples/fn_04
	@$(MAKE) -C examples/fn_05

clean:
	@$(MAKE) -C examples/fn_01 clean
	@$(MAKE) -C examples/fn_02 clean
	@$(MAKE) -C examples/fn_03 clean
	@$(MAKE) -C examples/fn_04 clean
	@$(MAKE) -C examples/fn_05 clean
