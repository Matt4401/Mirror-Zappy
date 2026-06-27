from datetime import datetime
import random

""" Example : Team1_2026-06-19_130522-123456_842 """


def generate_id(team_name: str) -> str:
    team_clean = team_name.replace(" ", "_")
    now = datetime.now().strftime("%Y-%m-%d_%H%M%S-%f")
    id_aleatoire = random.randint(1, 9999)

    return f"{team_clean}_{now}_{id_aleatoire}"
