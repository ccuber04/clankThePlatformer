# clankThePlatformer
CSC 340 - Game Development project, inspired by the deck-building, dungeon-crawling board game Clank!

## Assets
### Player Asset
Player graphics created by Penzilla Design.

Download from this [link](https://penzilla.itch.io/hooded-protagonist/download/eyJpZCI6MTc0MjEzMCwiZXhwaXJlcyI6MTc3Mzg3NTgxN30%3d%2eLwfEbX0i9ghVSekBZfDsgFCYx3I%3d).
Edit the graphics following these instruction:
#### Idle
1. Double the idle graphics
2. Move blinking graphics to after the doubled idle graphics
3. Put half blink graphic after full blink graphic
4. Add first idle graphic to the end

#### Kneel/Duck
1. Place the last two graphics on a new line

### Tileset
#### Castle Tiles
Tileset graphics created by Szadi art. Named the castle set as `szadi_main_lev_build.png`. Named the diamond tile as `diamond_big.png`
and laid the diamond images into one flat image to match other animation sprites.

Download castle tileset from this [link](https://szadiart.itch.io/pixel-platformer-castle/download/eyJleHBpcmVzIjoxNzc0Mzg5NzE5LCJpZCI6NDI2NDg5fQ%3d%3d%2eb%2fqLDj2CNRrU3P4cShvIiUCr4ek%3d)
Download big diamond tile from this [link](https://szadiart.itch.io/rocky-world-platformer-set/download/eyJpZCI6NDMxNDYyLCJleHBpcmVzIjoxNzc3ODQ0MTUyfQ%3d%3d%2ewMTxOug8FwlVMDvV0BpxZJsIJX8%3d)

### Background 
Background images are created by Szadi art.

Named the six background images as follows:
- szadi_BG1.png for first layer
- szadi_BG2.png for second layer
- szadi_BG3A.png for third layer mountain selection
- szadi_BG3B.png for third layer city selection
- szadi_BG4.png for fourth layer
- szadi_BG5.png for fifth layer

Download from this [link](https://szadiart.itch.io/pixel-platformer-castle/download/eyJleHBpcmVzIjoxNzc0Mzg5NzE5LCJpZCI6NDI2NDg5fQ%3d%3d%2eb%2fqLDj2CNRrU3P4cShvIiUCr4ek%3d)

### Enemy Assets
There are four assets used for enemies: Mushroom, Flying Monster, Bat, and Golem. All assets were created by MonoPixelArt.
Flip horizontally the individual sprite frames for each enemy asset to be facing the right direction.

Download from the following links:
- [Mushroom](https://monopixelart.itch.io/forest-monsters-pixel-art/download/eyJleHBpcmVzIjoxNzc3NzYzNTg0LCJpZCI6MjY3MjQzMH0%3d%2eqo9axJrmVlEZiAXtgHd%2f1LndJ1g%3d)
- [Flying Monster](https://monopixelart.itch.io/flying-enemies/download/eyJleHBpcmVzIjoxNzc3NzY0MTAzLCJpZCI6MzE1Njc2OX0%3d%2eB%2b7W4Z0wL0A%2fBzj7Unsgtd3O0%2fw%3d)
- [Bat](https://monopixelart.itch.io/dark-fantasy-enemies-asset-pack/download/eyJleHBpcmVzIjoxNzc3NzY0MDcwLCJpZCI6Mjc1NzE4NH0%3d%2eUnUFZGiUqk1HW9mt345PKKBeoUU%3d)
- [Golem](https://monopixelart.itch.io/golems-pack/download/eyJleHBpcmVzIjoxNzc3NzY0MTMxLCJpZCI6MjQ2MzExNH0%3d%2eJZgn5CFWsCBh8jeRc%2f7pbNKtam8%3d)

#### Mushroom Enemy
Consolidate the idle and running files into one file and name that file `Mushroom-enemy.png`. Adjust the distance between
sprite frames so that each sprite is in a 48 pixel box instead of the original 80x64 box

#### Flying Monster
Consolidate the idle and running files into one file and name that file `mysticflyer-enemy.png`. Sprite frames are at
64x64 pixel boxes so keep as is.

#### Bat
Not currently used. Plan to use in deeper levels

#### Golem
Not currently used. Plan to use in deeper levels

### Audio Assets
#### Background Audio
Background audio is created by alkakrab. Download from this [link](https://alkakrab.itch.io/free-25-fantasy-rpg-game-tracks-no-copyright-vol-2)
Files being used are `Action 1 (Loop).mp3`, `Death.mp3`, and `Victory.mp3`, and they are renamed to `alkakrab_to_the_end.mp3`,
`alkakrab_death.mp3`, and `alkakrab_victory.mp3`.

#### Sound Effect Audio
Sound effect audio is created by Leohpaz. Download from this [link](https://leohpaz.itch.io/rpg-essentials-sfx-free/download/eyJpZCI6MTYxNTQzMiwiZXhwaXJlcyI6MTc3NzkwNTEzN30%3d%2e1M2VBZ9N3PWhpbqlxgFeBjyOu9M%3d)
Files being used are `69_Enemy_death_01.wav` and `61_Hit_03.wav`, which are renamed to `rpg_se_death.wav` and `rpg_se_hit.wav`