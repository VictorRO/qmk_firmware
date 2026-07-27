# Unicorne

## Compile and flash the keyboard

Use the bootmagic reset and then run the next command
> Bootmagic reset: Hold down the key at (0,0) in the matrix (the top left key) and plug in the keyboard

```
make boardsource/unicorne:VictorRO:flash
```

## This is a 42 keymap layout with home row mods no mod-tap
```
qmk c2json -kb boardsource/unicorne -km VictorRO --no-cpp | keymap parse -c 12 -q - >keyboards/boardsource/unicorne/keymaps/VictorRO/sweep_keymap.yaml
keymap draw keyboards/boardsource/unicorne/keymaps/VictorRO/sweep_keymap.yaml > keyboards/boardsource/unicorne/keymaps/VictorRO/viz/sweep_keymap.unicorn.svg
```

![Layout](viz/sweep_keymap.unicorn.svg)

