#!/usr/bin/env python3
"""
Generate a simple WAV audio file
"""
import wave
import struct
import math

# Audio parameters
sample_rate = 44100  # Hz
duration = 10  # seconds
frequency = 440  # Hz (A4 note)

# Generate audio data
frames = []
for i in range(duration * sample_rate):
    # Generate a simple sine wave
    value = int(32767 * 0.5 * math.sin(2 * math.pi * frequency * i / sample_rate))
    frames.append(struct.pack('<h', value))

# Write WAV file
wav_path = "assets/music/theme.wav"
with wave.open(wav_path, 'wb') as wav_file:
    wav_file.setnchannels(1)  # Mono
    wav_file.setsampwidth(2)   # 2 bytes per sample
    wav_file.setframerate(sample_rate)
    wav_file.writeframes(b''.join(frames))

print(f"✓ Created {wav_path}")
