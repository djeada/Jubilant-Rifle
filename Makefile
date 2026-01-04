BUILD_DIR ?= build
BUILD_TYPE ?= Debug
CMAKE ?= cmake
RUN_ARGS ?=
CONFIGURE_FLAGS ?=

.PHONY: all configure build run install test clean distclean

all: build

configure:
	$(CMAKE) -S . -B $(BUILD_DIR) -DCMAKE_BUILD_TYPE=$(BUILD_TYPE) $(CONFIGURE_FLAGS)

build: configure
	$(CMAKE) --build $(BUILD_DIR)

run: build
	$(BUILD_DIR)/jubilant-rifle $(RUN_ARGS)

install: configure
	$(CMAKE) --build $(BUILD_DIR) --target install

test: build
	ctest --test-dir $(BUILD_DIR)

clean:
	$(CMAKE) --build $(BUILD_DIR) --target clean

distclean:
	rm -rf $(BUILD_DIR)
