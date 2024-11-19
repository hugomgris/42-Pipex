# 42-Pipex
Pipex project at 42 Barcelona

I didn't evaluate this project because I previously did Minitalk, but I wanted to do it so I didn't go into Minishell without knowing how to handle pipes. In other words, this never went through the filter of peer-to-peer evals, so make of it what you want (i.e., double check the code if you use it as reference).

The repo has two versions of Pipex. The first one is my own approach, which doesn't handle the bonus parts. 

The second one is a full bonus pipex that handles a lot of extreme cases, which I wrote following someone elses code (link below). I did this second version after runing that same person's tests, which had one case in which the infile was set to the urandom number generator and made the program, if the case was not properly handled, get stuck in an infinite void of virutal desperation. I could've tried to fix it using timetout alarms, but I felt that it would've been just a patch in a code that would have been far from the best possible version from pipex, which is unacceptable (note: read this last word as the lemon guy from Adventure Time would). So, while researching the code related to the tests, I realized my pipex stank, and I firmly believe that when you come across a valuable lesson, you should take advantage of it. All of is is to say that my second code is pretty similar to the one that can be found in the repo linked below. I tried, nevertheless, make a combined version, but apparently my parsing was too condensed and was not "easy to debug and not too scalable", so I just replicated the parsing of the over-mentioned code, etc, etc.

Anyhow, I hope that this repo finds you in a good place <3

link to the repo related to my second pipex version, which includes the megatester: https://github.com/mcombeau/pipex/tree/main
