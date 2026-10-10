PYTHON ?= python3
CXX ?= g++
.PHONY: check pdf format hash bundle pack
check:
	$(PYTHON) scripts/verify.py --compiler "$(CXX)"
pdf:
	$(PYTHON) scripts/build.py
format:
	$(PYTHON) scripts/sync_metadata.py
	$(PYTHON) scripts/style.py
hash:
	$(PYTHON) scripts/hash.py $(FILE)
bundle:
	$(PYTHON) scripts/bundle.py $(HEADERS) --output build/bundle.cpp
pack:
	$(PYTHON) scripts/pack.py
