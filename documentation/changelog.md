# Version 0.7

## Added

- Teach Mode
- Pose recording
- Replay engine
- Pose structure
- SAVE command
- PLAY command
- UNDO command
- CLEAR command
- Replay state machine using playing flag
- Pose indexing using playIndex

## Improved

- Firmware architecture
- Command parser
- Motion control organization

## Known Limitations

- Replay uses blocking delay() in motion engine.
- Poses are stored only in RAM.
- Replay speed is fixed by Vxx command.