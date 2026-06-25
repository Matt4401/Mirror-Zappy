class BroadcastMessageManager:
    def __init__(self, player_state):
        self.player_state = player_state
        self.team = player_state.team_name
        self.key = sum(ord(letter) for letter in self.team)

    def create_message(self, command):
        content = f"{self.player_state.level} {command}"
        encrypted_content = self.code(content)
        return f"{self.team}_{encrypted_content}"

    def code(self, message):
        result = []
        for l in message:
            value = (ord(l) + self.key) % 256
            result.append(str(value))
        return "-".join(result)

    def read_broadcast(self, raw_message):
        try:
            if "_" not in raw_message:
                return None
            team_name, encrypted_content = raw_message.split("_", 1)
            if team_name != self.team:
                return None
            clear_text = self.decode(encrypted_content)
            if clear_text:
                parts = clear_text.split(" ", 1)
                if len(parts) == 2:
                    return int(parts[0]), parts[1]
        except Exception:
            pass
        return None

    def decode(self, encrypted_message):
        try:
            result = []
            parts = encrypted_message.split("-")
            for part in parts:
                if not part:
                    continue
                crypto_value = int(part)
                value = (crypto_value - self.key + 256) % 256
                result.append(chr(value))
            return "".join(result)
        except Exception:
            return None
