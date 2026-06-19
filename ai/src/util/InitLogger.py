import logging.config
from pathlib import Path
import yaml


def setup_logging():
    RACINE_PROJECT = Path(__file__).resolve().parent
    DOSSIER_LOGS = RACINE_PROJECT / "logs"
    DOSSIER_LOGS.mkdir(parents=True, exist_ok=True)

    with open(RACINE_PROJECT / "config.yml", "r") as f:
        config = yaml.safe_load(f)

    config['handlers']['network_file']['filename'] = str(DOSSIER_LOGS / "server.log")

    logging.config.dictConfig(config)
