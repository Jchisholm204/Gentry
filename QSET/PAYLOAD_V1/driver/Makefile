
BUILD_DIR := build
BUILD_TYPE ?= Debug

all: build

${BUILD_DIR}/Makefile:
	cmake \
		-B${BUILD_DIR} \
		-DCMAKE_BUILD_TYPE=${BUILD_TYPE} \
		-DCMAKE_EXPORT_COMPILE_COMMANDS=ON \

cmake: ${BUILD_DIR}/Makefile

build: cmake
	$(MAKE) -C ${BUILD_DIR} --no-print-directory -j4

run: build
	./build/QSET_USB_DRIVER

clean:
	rm -r build
