# If this the food is under this threshold, survive state start
SURVIVAL_THRESHOLD = 8
FOOD_SAFE_FOR_INCANTATION = 13
FOOD_SAFE_FOR_FORK = 12
FOOD_OPPORTUNISTIC = 18

# Number of member for reproduce state to start
# NUMBER_OF_MEMBER_FOR_REPRODUCE = 0
# Number of member for attack state to start
# NUMBER_OF_MEMBER_FOR_ATTACK = 0

# Interval between auto command
INVENTORY_INTERVAL = 10
LOOK_INTERVAL = 5
BROADCAST_INTERVAL = 50

DISTRIBUTION_VALUE = 50
PRIME_NUMBER_COMBINATION = 7

# All of fork process constants
MAX_TEAM_EGGS = 2
FORK_COOLDOWN_TICKS = 600 #300
FORK_COOLDOWN_SECONDS = 8.0
MAX_REPRODUCE_GEN = 6
LIFETIME_FORK_CAP = 1

ATTACK_SIZE_RATIO = 1.5
ATTACK_DIST_MAX = 4
MIN_TEAM_SIZE = 6
WORLD_MODEL_TTL = 100

# All for incantation and evolution
# INCANTATION_WAIT_TICKS = 150
# BROADCAST_INCANTATION_READY = "INC_READY"
# BROADCAST_INCANTATION_JOIN = "INC_JOIN"
# BROADCAST_INCANTATION_DONE = "INC_DONE" # not used for the moment

#"{team}|EVO_PROCESS:{level}:{leader_id}" debut
#"{team}|DONE:{level}:{leader_id}" fin
BROADCAST_EVO_PROCESS = "EVO_PROCESS"
BROADCAST_DONE = "DONE"

INCANT_FOOD_ABORT = SURVIVAL_THRESHOLD + 2
EVO_PROCESS_BROADCAST_PERIOD = 2 # for loop broadcast

FORK_WHILE_WAITING_TICKS = 40
FORK_WHILE_WAITING_FOOD = SURVIVAL_THRESHOLD + 2
JOIN_SIGNAL_TIMEOUT = 500

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
