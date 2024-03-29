// PuppetMaster Input Library (MIDI)
// Copyright 2001 by Jon Parise <jparise@cmu.edu>

#include "MidiInput.h"

// Disable Warning About strcpy
#pragma warning (disable : 4996)


// Global MIDI device handle
MidiDevice *midi = NULL;


MidiInput::MidiInput(unsigned int device_id)
{
    threadID = 0;
    threadHandle = NULL;
    isRunning = false;

    events_read_pos = 0;
    events_write_pos = 0;

    this->device_id = device_id;
}

void
MidiInput::decode(unsigned short *buf, int size)
{
    int length;

    for (int i = 0; i < size;) {
        length = (int)((buf[i] & 0xff00) >> 8);

        if ((byte)(buf[i] & 0x00ff) != 63) {
            continue;
        }

        MidiEvent event;

        // Type
        event.type = (int)((buf[i + 1] & 0x00ff));

        // Channel
        event.channel = (int)((buf[i + 1] & 0xff00) >> 8);

        // Value
        event.value = (int)((buf[i + 2] & 0x00ff));

        // Add this new event to the events list
        events[events_write_pos] = event;
        events_write_pos++;
        if (events_write_pos == MAX_MIDI_EVENTS) {
            events_write_pos = 0;
        }

        i += length;
    }
}

unsigned int
MidiInput::poll(MidiEvent* buf)
{
    int count = 0;

    while ((events_read_pos != events_write_pos) && (count < MAX_MIDI_EVENTS)) {
        buf[count] = events[events_read_pos];
        ++events_read_pos;
        if (events_read_pos == MAX_MIDI_EVENTS) {
            events_read_pos = 0;
        }
        count++;
    }

    return count;
}

DWORD
MidiInput::threadProc()
{
    unsigned short midibuf[MIDI_BUFFERSIZE];

    ZeroMemory(midibuf, MIDI_BUFFERSIZE);

    midi->open(this->device_id);

    do {
        int size = midi->poll(midibuf);
        decode(midibuf, size);

        // Sleep for one hundredth of a second
        Sleep(MIDI_POLL_INTERVAL);
    } while (isRunning);

    midi->close();

    return 0;
}

void
MidiInput::threadBegin()
{
    if (threadHandle) {
        threadEnd();  // just to be safe.
    }

    // Start the thread.
    threadHandle = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)midiThreadProc,
                                this, 0, (LPDWORD)&threadID);

    // Check for errors
    if (threadHandle == NULL) {
        return;
    }

    isRunning = true;
}

void
MidiInput::threadEnd()
{
    if (threadHandle != NULL) {
        isRunning = false;
        WaitForSingleObject(threadHandle, INFINITE);
        CloseHandle(threadHandle);
        threadHandle = NULL;
    }
}

static DWORD WINAPI
midiThreadProc(MidiInput *instance)
{
    return instance->threadProc();
}

void
MidiInput::deviceName(unsigned int num, char* name)
{
    // Ensure the requested device number is within the range of known devices
    if (num > midiInGetNumDevs()) {
        return;
    }

    MIDIINCAPS capabilities;

    if (midiInGetDevCaps(num, &capabilities, sizeof(MIDIINCAPS)) != MMSYSERR_NOERROR) {
        return;
    }

    strcpy(name, (const char*)capabilities.szPname);
}

UINT
MidiInput::deviceCount()
{
    return midiInGetNumDevs();
}