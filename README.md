# uxn-chess
## introduction
I have started this project as a fun way to try out [chibicc](https://github.com/lynn/chibicc) - "a small c compiler for [uxn](https://wiki.xxiivv.com/site/uxn.html)".

Therefore the base code is written in C with mappings to uxn provided by chibicc such as `set_screen_addr`.


## how to run
To run this app you need [chibicc](https://github.com/lynn/chibicc), from there you can run
```
./run.sh PATH_TO_YOUR_CHESS_UXN/chess.c
```

## what is implemented
- [x] board drawn
- [x] pieces drawn
- [x] keep order - first white pieces, then black, and so on
- [ ] standard moves - IN PROGRESS
- [ ] special moves - en passant, pawn promotion, castles
- [ ] check
- [ ] check mate
- [ ] game restart

## screenshots

![uxn-chess preview](screenshot.png)

