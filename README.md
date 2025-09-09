# Multiplayer Trivia (Linux)

A server‑authoritative trivia game that runs on Linux where players connect over sockets and answer questions in real time. The server broadcasts each question to all connected players. Players submit their answers at the same time.

## How it works
- Questions and multiple choice options are loaded from a plain text file
- Each question is displayed to every player
- Players enter the number of their chosen option
- Only the first player to submit an option earns the points
- The game advances for everyone whether they answered or not
- The server uses multiplexed I O to handle many clients at once

## Game end
When all questions have been asked, the server announces the winner, ends the match, and closes all socket connections.
