venv:
	sudo apt-get update
	sudo apt-get install portaudio19-dev
	python3 -m venv .venv && \
	. .venv/bin/activate && \
	.venv/bin/pip install adafruit-blinka --force-reinstall
	.venv/bin/pip install -r requirements.txt

run:
	.venv/bin/python3 read_audio.py