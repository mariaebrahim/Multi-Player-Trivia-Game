This project is designed to simulate a trivia game where the server sends a question to all players who have joined the game through a socket connection and the players can send an answer simultanouesly.

This project is designed to run on a linux operating system.

The question are read from a plain text file along with the options that the user can select. This is then displayed to every player who can input the number associated with the choice. However, only the first player to input the option will be assigned the points and the game will move forward for every player regardless of whether they answered the question. This is done through a multiplexed I/O.

Once all the questions are answered the winner is displayed, the game ends, and all socket connections are closed.
