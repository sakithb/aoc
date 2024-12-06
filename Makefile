.PHONY: clean run

SRC ?= day1/day1.c
OUT := $(SRC).out

%.c.out: $(SRC)
	@mkdir -p $(dir $(OUT))
	gcc -o $(OUT) $(SRC)

%.rs.out: $(SRC)
	@mkdir -p $(dir $(OUT))
	rustc -o $(OUT) $(SRC)

run: $(OUT)
	cd $(dir $(OUT)); ./$(notdir $(OUT))

clean:
	rm **/*.out

