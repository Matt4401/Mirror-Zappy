import copy
import yaml
import logging.config
from pathlib import Path


class PlayerLogger:
    @staticmethod
    def setup_player_logging(player_id: str, config, DOSSIER_LOGS):
        name_handler = f"player_{player_id}_file"
        name_logger = f"player_{player_id}"

        dossier_players = DOSSIER_LOGS / "player"
        dossier_players.mkdir(parents=True, exist_ok=True)
        path_file_log = dossier_players / f"player_{player_id}.log"

        template_handler = config["handlers"].pop("player_file_template", None)
        if template_handler:
            config["handlers"][name_handler] = copy.deepcopy(template_handler)
            config["handlers"][name_handler]["class"] = (
                "logging.handlers.RotatingFileHandler"
            )
            config["handlers"][name_handler]["filename"] = str(path_file_log)

        config["loggers"][name_logger] = copy.deepcopy(
            config["loggers"]["player_template"]
        )
        config["loggers"][name_logger]["handlers"].append(name_handler)

    @staticmethod
    def setup_logging(player_id: str):
        PATH_UTIL = Path(__file__).resolve().parent
        ROOT_PROJECT = PATH_UTIL.parent.parent.parent
        FOLDER_LOGS = ROOT_PROJECT / "logs"
        FOLDER_LOGS.mkdir(parents=True, exist_ok=True)

        with open(PATH_UTIL / "logger_config.yaml", "r") as f:
            config = yaml.safe_load(f)

        PlayerLogger.setup_player_logging(player_id, config, FOLDER_LOGS)

        config["handlers"]["network_file"]["filename"] = str(FOLDER_LOGS / "server.log")
        config["handlers"]["command_file"]["filename"] = str(
            FOLDER_LOGS / "commands.log"
        )

        logging.config.dictConfig(config)
