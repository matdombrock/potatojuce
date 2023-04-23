Synth Engines will be built from text files
- These text files will be loaded in real-time (on startup)
- This allows us to quickly setup new patches
- This will let us write a software solution for end-users to create patches

Limitations:
- Only one wave type per patch
- Patches cant be created in real-time on the device


```
name: Some Synth
description: A demo synth
wave: square
p1: detune
p2: noise
p3: lowpass1
p1b: glide
```

```
name: Other Synth
description: Another demo synth
wave: tri
p1: downsample
p1_rng: 100 127 # midi range 100->127
p2: bitcrush
p3: lowpass1
p1b: attack
p2b: decay
p3b: release
polyphony: true
```