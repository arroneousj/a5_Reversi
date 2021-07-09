# a5_Reversi

A very rudimentary human vs computer Reversi game that was done for a first year school assignment. \
The computer player is extremely rudimentary.

The game operates inside the terminal. \
The player inputs their move and the computer excecutes their move. \
All moves are displayed within the terminal.

LEGEND:
1) "." - Empty Space
2) "0" - Computer's Piece (white piece)
3) "X" - Player's Piece (black piece)
4) "#" - Valid Space Location

Code was originally written to compile and run in Linux. Output to terminal was coloured (using ANSI codes with ESC codes) for easier interpretation.

A) The "reversi_a5" file (non .cpp) is an executable for Linux
   - In console navigated to the directory in which the file is in, enter: `./reversi_a5`

B) The "reversi_a5_win.exe" is an executable for Windows
   - In console navigated to the directory in which the file is in, enter: `reversi_a5_win.exe`
   - Colour functionality returned as method for displaying colour discovered for Windows CMD

- If colour does not appear and text appears as something like `\033[38;5;206m.\033[0m` do the following:
  - As per [this guide](https://www.codeproject.com/Tips/5255355/How-to-Put-Color-on-Windows-Console):
    - Run the command in CMD `reg add HKEY_CURRENT_USER\Console /v VirtualTerminalLevel /t REG_DWORD /d 0x00000001 /f` 
    - This allows Windows to interpret the ESC code of `\033`
    - Restart the console and navigate back to the directory and run the excecutable
    - If desired, to undo, run `reg add HKEY_CURRENT_USER\Console /v VirtualTerminalLevel /t REG_DWORD /d 0x00000000 /f`
- ~~The coloured display to console of the original code does not work so it currently operates without colour~~
- ~~Game otherwise works but is harder to see compared to its Linux counterpart~~
- ~~May look on how to implement the colour functionality for windows in the future~~ 

