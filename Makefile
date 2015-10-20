build: clean
	python setup.py build

install:
	python setup.py install

develop:
	python setup.py develop

undevelop:
	python setup.py develop --uninstall

register-test:
	python setup.py register -r pypitest

register-release:
	python setup.py register -r pypi

upload-test:
	python setup.py sdist upload -r pypitest

upload-release:
	python setup.py sdist upload -r pypi

clean:
	rm -rf moogli.egg-info
	rm -rf dist
	rm -rf build
