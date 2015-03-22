build: clean
	python setup.py -L/home/aviral/OpenSceneGraph-3.2.1-rc2/build/lib build

install:
	python setup.py -L/home/aviral/OpenSceneGraph-3.2.1-rc2/build/lib install

develop:
	python setup.py -L/home/aviral/OpenSceneGraph-3.2.1-rc2/build/lib develop

undevelop:
	python setup.py -L/home/aviral/OpenSceneGraph-3.2.1-rc2/build/lib develop --uninstall

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
