# AudioGraphBug

This is a repro app to demonstrait that `AudioGraph` runs fast: well at least `AudioFrameOutputNode`s do.

The actual sample rate of AudioFrameOutputNode is around 44102 when the real sample rate is 44100.

The quantum size is 441 which is 10 ms of the real sample rate. However the quantums are very silghtly too fast 9999.5 ish rather than 10 000 microseconds appart (sometimes it starts higher thant 10 000 and takes some time to reach an 9999.5 average).

This probably means that AudioGraph is distorting Audio from Devices (well at least for microphones that don't oversample), and thus may be distorting audio from Files as well.

-----------------------

I found this bug by reading data from a fixed sized between swapchain present calls (QuantumStarted handler would write the data). The buffer eventually overflows, and my timer indicates Audiograph is at fault not the VSync.

There are two cases for `AudioFileInputNode` because there is a bug with the configuration in my main project, but this didn't occur with this projects configuration (AudioGraph ignores the playback speed and just dumps the audio as fast as it can).

Commented/Uncomment the lines below to switch between timing the sample rate and the quantum inteval: 
```
  m_timer.timeAverage();
  //m_timer.timeCumulative(value_size / sizeof(float));
```
