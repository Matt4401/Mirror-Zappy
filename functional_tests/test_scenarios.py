import pytest
import yaml
import re
import time
import socket
from conftest import ZappyClient

with open("scenarios.yaml", "r") as f:
    SCENARIOS = yaml.safe_load(f)

def flush_gui_handshake(client: ZappyClient):
    """Reads and discards the massive burst of data sent to a new GUI."""
    client.sock.settimeout(0.5)
    try:
        while True:
            client.sock.recv(8192)
    except socket.timeout:
        pass
    client.buffer = ""

@pytest.mark.parametrize("scenario", SCENARIOS, ids=[s["name"] for s in SCENARIOS])
def test_yaml_scenario(server, scenario):
    port, binary_name = server
    client = ZappyClient(port)
    
    assert client.read_until() == "WELCOME\n"
    
    if scenario["client_type"] == "GUI":
        client.send("GRAPHIC\n")
        flush_gui_handshake(client)
        
    elif scenario["client_type"] == "AI":
        client.send(f"{scenario['team']}\n")
        client.read_until()
        client.read_until()
    
    for step in scenario["steps"]:
        if "send" in step:
            client.send(step["send"])
        if "sleep" in step:
            time.sleep(step["sleep"])
        if "expect" in step:
            response = client.read_until(timeout=3.0)
            match = re.search(step["expect"], response)
            assert match is not None, f"[{binary_name}] Expected regex '{step['expect']}' but got '{response}'"
            
    client.close()

def test_integration_team_limit_exhaustion(server):
    port, binary_name = server
    clients = []
    
    for _ in range(5):
        c = ZappyClient(port)
        c.read_until()
        c.send("team1\n")
        c.read_until()
        c.read_until()
        clients.append(c)
        
    c6 = ZappyClient(port)
    c6.read_until()
    c6.send("team1\n")
    
    response = c6.read_until()
    assert response.strip() == "ko", f"[{binary_name}] Expected 'ko', got '{response}'"
    
    for c in clients:
        c.close()
    c6.close()

def test_integration_gui_observes_starvation(server):
    port, binary_name = server
    gui = ZappyClient(port)
    ai = ZappyClient(port)
    
    gui.read_until()
    gui.send("GRAPHIC\n")
    
    ai.read_until()
    ai.send("team1\n")
    ai.read_until()
    ai.read_until()
    
    player_id = None
    while True:
        msg = gui.read_until()
        if msg.startswith("pnw "):
            match = re.search(r"^pnw #?(\d+)", msg) 
            player_id = match.group(1)
            break

    time.sleep(1.5) 
    
    assert ai.read_until(timeout=5.0) == "dead\n"
    
    found_death = False
    while True:
        try:
            msg = gui.read_until(timeout=1.0)
            if re.search(rf"^pdi #?{player_id}\n", msg):
                found_death = True
                break
        except TimeoutError:
            break
            
    assert found_death, f"[{binary_name}] GUI never received pdi {player_id}"
    
    gui.close()
    ai.close()
