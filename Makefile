# If the first argument is "run"...
all:
	gcc *.c -o compiler


ifeq (run,$(firstword $(MAKECMDGOALS)))

  # use the rest as arguments for "run"
  RUN_ARGS := $(wordlist 2,$(words $(MAKECMDGOALS)),$(MAKECMDGOALS))

  # ...and turn them into do-nothing targets
  $(eval $(RUN_ARGS):;@:)
endif

.PHONY: run
run: 
	./compiler $(RUN_ARGS)



debug:
	gcc *.c -g -o compiler
	gdb ./compiler 
clean:
	rm compiler
