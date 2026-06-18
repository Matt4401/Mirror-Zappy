import pytest
import yaml
import re
from conftest import ZappyClient

with open("scenarios.yaml", "r") as f:
    SCENARIOS = yaml.safe_load(f)

@pytest.mark.parametrize("scenario", SCENARIOS, ids=[s["name"] for s in SCENARIOS])
def test_yaml_scenario(server, scenario):
    port, binary_name = server
    client = ZappyClient(port)
    
    assert client.read_until() == "WELCOME\n"
    
    if scenario["client_type"] == "GUI":
        client.send("GRAPHIC\n")
    elif scenario["client_type"] == "AI":
        client.send(f"{scenario['team']}\n")
        client.read_until()
        client.read_until()
    
    for step in scenario["steps"]:
        if "send" in step:
            client.send(step["send"])
        if "expect" in step:
            response = client.read_until()
            match = re.search(step["expect"], response)
            assert match is not None, f"[{binary_name}] Expected regex '{step['expect']}' but got '{response}'"
            
    client.close()
