# If this the food is under this threshold, survive state start
SURVIVAL_THRESHOLD = 5

# Number of member for reproduce state to start
# NUMBER_OF_MEMBER_FOR_REPRODUCE = 0
# Number of member for attack state to start
# NUMBER_OF_MEMBER_FOR_ATTACK = 0

# Interval between auto command
INVENTORY_INTERVAL = 20
LOOK_INTERVAL = 7
BROADCAST_INTERVAL = 50

DISTRIBUTION_VALUE = 50
PRIME_NUMBER_COMBINATION = 7

ATTACK_SIZE_RATIO = 1.5
ATTACK_DIST_MAX = 4
MIN_TEAM_SIZE = 6
WORLD_MODEL_TTL = 100

ELEVATION_REQUIREMENTS = {
    2: {
        "player": 1,
        "linemate": 1,
        "deraumere": 0,
        "sibur": 0,
        "mendiane": 0,
        "phiras": 0,
        "thystame": 0,
    },
    3: {
        "player": 2,
        "linemate": 1,
        "deraumere": 1,
        "sibur": 1,
        "mendiane": 0,
        "phiras": 0,
        "thystame": 0,
    },
    4: {
        "player": 2,
        "linemate": 2,
        "deraumere": 0,
        "sibur": 1,
        "mendiane": 0,
        "phiras": 2,
        "thystame": 0,
    },
    5: {
        "player": 4,
        "linemate": 1,
        "deraumere": 1,
        "sibur": 2,
        "mendiane": 0,
        "phiras": 1,
        "thystame": 0,
    },
    6: {
        "player": 4,
        "linemate": 1,
        "deraumere": 2,
        "sibur": 1,
        "mendiane": 3,
        "phiras": 0,
        "thystame": 0,
    },
    7: {
        "player": 6,
        "linemate": 1,
        "deraumere": 2,
        "sibur": 3,
        "mendiane": 0,
        "phiras": 1,
        "thystame": 0,
    },
    8: {
        "player": 6,
        "linemate": 2,
        "deraumere": 2,
        "sibur": 2,
        "mendiane": 2,
        "phiras": 2,
        "thystame": 1,
    },
}
