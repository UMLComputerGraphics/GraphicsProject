.PHONY: cmake
cmake:
	mkdir -p build
	cd build; cmake -DDBUILD_SHARED_LIBS=ON -DBUILD_STATIC_LIBS=ON -DFREEGLUT_BUILD_DEMOS=OFF -DCMAKE_INSTALL_PREFIX=../../expensiveglut ..

.PHONY: compile
compile:
	cd build; make; make install

clean:
	rm -Rf build
