venv:
	python3 -m venv .venv
	. .venv/bin/activate
	.venv/bin/pip install -r requirements.txt