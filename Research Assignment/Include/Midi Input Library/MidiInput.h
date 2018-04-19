// PuppetMaster Input Library (MIDI)
// Copyright 2001 by Jon Parise <jparise@cmu.edu>

#ifndef __MIDIINPUT_H__
#define __MIDIINPUT_H__

#include <windows.h>

#include "MidiDevice.h"

#define MIDI_POLL_INTERVAL  10      // milliseconds
#define MAX_MIDI_EVENTS     8192

struct MidiEvent {
    int type;
    int channel;
    int value;
};



class MidiInput {
private:
    unsigned int    device_id;

    MidiEvent       events[MAX_MIDI_EVENTS];
    unsigned int    events_read_pos;
    unsigned int    events_write_pos;

    // Threading stuff
    HANDLE          threadHandle;
    DWORD           threadID;
    bool            isRunning;

    void decode(unsigned short *buf, int size);

public:
	MidiInput(unsigned int device_id);

    unsigned int poll(MidiEvent* buf);

    // Thread Interface
    DWORD threadProc();
    void threadBegin();
    void threadEnd();

    // Device information
    static void deviceName(unsigned int num, char *name);
    static UINT deviceCount();
};

static DWORD WINAPI midiThreadProc(MidiInput *instance);

#endif /* __MIDI_H__ */