.PHONY: clean

clean:
	rm */*.out

%:
	gcc -o ./$@.out ./$(@D)/$(@F).c
	cd ./$(@D); ./$(@F).out
