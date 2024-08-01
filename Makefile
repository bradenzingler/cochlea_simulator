venv:
	python3 -m venv .venv && \
	source .venv/bin/activate && \
	.venv/bin/pip install adafruit-blinka --force-reinstall
	.venv/bin/pip install -r requirements.txt
