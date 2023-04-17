# If the first argument is "run"...
COMPILEFILE := stringbuf.c compile.c lexer.c parser.c
OUTPUTFILE := compiler
all:
	gcc $(COMPILEFILE) -g -o $(OUTPUTFILE)


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
	gcc $(COMPILEFILE) -g -o $(OUTPUTFILE)
	gdb ./$(OUTPUTFILE)
clean:
	rm $(OUTPUTFILE)
