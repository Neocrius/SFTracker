SoundFont-based tracker developed with the Clockwork uConsole (and possibly other Raspberry Pi devices) in mind.
It uses FluidSynth for audio and RayLib/RayGui for the UI.

Bulding on the Pi (will be simplified later):
```
git clone https://github.com/Neocrius/SFTracker.git SFTracker
cd SFTracker
git submodule init
git submodule update
cd deps/raylib/src
make PLATFORM=PLATFORM_DESKTOP GRAPHICS=GRAPHICS_API_OPENGL_ES2
cd ../../fluidsynth
cmake -DCMAKE_BUILD_TYPE=Release -S . -B ./build
cmake --build ./build
cd ../../src
make
```

This is a work in progress and not working at all yet, check back in a couple of months.