# Makefile
SHELL := /bin/bash

.DEFAULT_GOAL := all

.PHONY: all clean

PROJECT_NAME := crawl

help: ## Show help message
	@awk 'BEGIN {FS = ":.*##"; printf "\nUsage:\n make \033[36m\033[0m\n"} /^[$$()% 0-9a-zA-Z_-]+:.*?##/ { printf " \033[36m%-15s\033[0m %s\n", $$1, $$2 } /^##@/ { printf "\n\033[1m%s\033[0m\n", substr($$0, 5) } ' $(MAKEFILE_LIST)

venv:
	virtualenv -p python3 venv/
	source venv/bin/activate && pip3 install conan

build: venv
	source venv/bin/activate && conan profile detect --force
	source venv/bin/activate && conan install . --output-folder=build --build=missing
	cd build/ && cmake .. -DCMAKE_TOOLCHAIN_FILE=conan_toolchain.cmake -DCMAKE_BUILD_TYPE=Release

build/$(PROJECT_NAME): build
	cd build/ && make

clean: ## Clean all build artifacts
	rm -rf build/
	rm -rf venv/

all: ## Build the whole thing
	$(MAKE) build/$(PROJECT_NAME)

