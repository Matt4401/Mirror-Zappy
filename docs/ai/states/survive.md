## SurviveState

### The SurviveState is an emergency behavior aimed exclusively at preventing the Trantorian's death.  
- The system analyzes the player's vision to identify the tile index containing the closest food.  
- If food is visible, the Trantorian moves to that tile, takes the object, and updates its inventory.  
- If no food is initially visible, it updates its vision; if it remains unfound, it moves forward one tile and updates its vision again.

```python
class SurviveState(AState):
    def execute(self):
        self.maybe_eat_food()
        self.trantorian.logger.info("===========Entering Survive state===========")
        closest_food_idx = self.trantorian.player_state.vision.get_tile_index_of("food")

        if closest_food_idx is not None:
            self.trantorian.logger.info(
                "[Survive]: Food visible, go to the tile and take it"
            )
            self.trantorian.move_to_tile(closest_food_idx)
            self.trantorian.take_object("food")
            self.trantorian.inventory()
            self.trantorian.look()
            return

        self.trantorian.logger.info(
            "[Survive]: No food visible, look then move forward"
        )
        self.trantorian.look()
        closest_food_idx = self.trantorian.player_state.vision.get_tile_index_of("food")
        if closest_food_idx is None:
            self.trantorian.forward()
        self.trantorian.look()
```