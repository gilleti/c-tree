all:
	python3 setup.py build_ext --inplace

clean:
	rm *.so
	rm -r build/*
