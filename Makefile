.PHONY: clean run

SRC ?= day1/day1.c
OUT := $(SRC).out

$(OUT): $(SRC)
	@mkdir -p $(dir $(OUT))
	gcc -o $(OUT) $(SRC)

run: $(OUT)
	cd $(dir $(OUT)); ./$(notdir $(OUT))

clean:
	rm **/*.out

