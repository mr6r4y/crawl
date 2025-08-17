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

CMakeUserPresets.json: venv
	source venv/bin/activate && conan profile detect --force

build/Release: CMakeUserPresets.json venv
	source venv/bin/activate && conan install . --settings=build_type=Release --build=missing

build/Debug: CMakeUserPresets.json venv
	source venv/bin/activate && conan install . --settings=build_type=Debug --build=missing

build/Release/Makefile: build/Release
	cd build/Release && cmake ../.. -DCMAKE_TOOLCHAIN_FILE=generators/conan_toolchain.cmake -DCMAKE_BUILD_TYPE=Release

build/Debug/Makefile: build/Debug
	cd build/Debug && cmake ../.. -DCMAKE_TOOLCHAIN_FILE=generators/conan_toolchain.cmake -DCMAKE_BUILD_TYPE=Debug


build/Debug/$(PROJECT_NAME): build/Debug/Makefile
	cd build/Debug && make

build/Release/$(PROJECT_NAME): build/Release/Makefile
	cd build/Release && make

clean: ## Clean all build artifacts
	rm -rf build/
	rm -rf venv/

all: ## Build the whole thing
	$(MAKE) build/Release/$(PROJECT_NAME)
	$(MAKE) build/Debug/$(PROJECT_NAME)

