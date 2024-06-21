# boggle-cli
Word boggle in the terminal -- currently only supported on UNIX systems. Made as a school project.

![20240621_13:51:48_screenshot](https://github.com/yazoink/boggle-cli/assets/98802603/2d9c8a4d-89af-4b85-98d1-ad8817e863c0)

```
$ git clone https://github.com/yazoink/boggle-cli
$ make
$ ./boggle
```
## How to play
There is a timer set for each game, the length can be set in "config.h" under "TIMER_SECONDS" but the default is 60 seconds. The objective is to find as many words as possible using only the letters in the grid. Your score is increased by the amount of letters in each word you find.

Your score is exported to a text file under the name <name>test.txt every time a game has been completed. You can set your name in "config.h" under "NAME".
