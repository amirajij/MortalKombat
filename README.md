# Mortal Kombat Clone - C

This project is simple a clone of the Mortal Kombat game, developed as part of my C programming coursework. It uses linked lists to manage game data, ensuring memory is handled efficiently to prevent leaks. The game includes attacks, combos, and cheat codes, offering a dynamic and engaging gameplay experience.

## Special Features

- Linked Lists: Used for managing players' health and stamina, as well as keeping a history of moves.
- Memory Management: Careful allocation and deallocation of memory to prevent leaks.
- Attacks and Combos: Implement various attacks and powerful combos for strategic gameplay.
- Cheat Codes: Special codes that can alter game mechanics and provide advantages.

**Rules**:
- A player cannot use attack combos; each player is allowed up to 4 attacks per round.
- A player can only use a combo when they have more than 750 stamina.
- The damage dealt from the simple attacks (ataques) is based on a table.


**Combos**:

| Combo Name        | Letter Sequence     | Health Points Taken from Opponent | Stamina Lost |
|-------------------|---------------------|-------------------------------|------------------|
| Arrozão           | ARROZAO             | 500                           | 500              |
| Dad Bad           | DADBAD              | 400                           | 400              |
| Bife Wellington   | STTEACC             | 300                           | 300              |
| Furacão Thiago    | TATAPAAA            | 200                           | 200              |

**Cheat codes**:
- Jesus-Mode: Resets the game, both players return to 1000 stamina and 1000 health, with no moves in the history.
- Alt-F4X: Player 1 - Restores stamina to X points. X must be a positive number.
- KebabX: Player 2 - Restores stamina to X points. X must be a positive number.
- HiroshimaX: Player 1 - Restores health to X points. X must be a positive number.
- Nood-ModeX: Player 2 - Restores health to X points. X must be a positive number.

## Game Examples

<div style="text-align: center;">
    <img src="Images/a.png" alt="Game Example 1" width="400"/>
    <p><em>Definition of the board</em></p>
</div>

<div style="text-align: center;">
    <img src="Images/b.png" alt="Game Example 2" width="400"/>
    <p><em>Shooting and game winner</em></p>
</div>

## Combos Examples

<div style="text-align: center;">
    <img src="Images/e.png" alt="Save and Load Example 1" width="400"/>
    <p><em>Saving the game after defining the board</em></p>
</div>

<div style="text-align: center;">
    <img src="Images/f.png" alt="Save and Load Example 2" width="400"/>
    <p><em>Loading the game saved earlier</em></p>
</div>

## Cheat Codes Examples

<div style="text-align: center;">
    <img src="Images/e.png" alt="Save and Load Example 1" width="400"/>
    <p><em>Saving the game after defining the board</em></p>
</div>

<div style="text-align: center;">
    <img src="Images/f.png" alt="Save and Load Example 2" width="400"/>
    <p><em>Loading the game saved earlier</em></p>
</div>

## Author

- <a href="https://github.com/amirajij" target="_blank">Amir Irshad</a>
- <a href="https://amirajij.github.io/" target="_blank">Personal Website</a>
