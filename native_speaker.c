#include "native_speaker.h"

#include <stdio.h>

#ifdef _WIN32
#include <windows.h>

typedef struct {
	const int16_t *pcm;
	size_t num_samples;
	int sample_rate;
} NativeSpeakerRequest;

static HANDLE speaker_event;
static HANDLE speaker_thread;
static CRITICAL_SECTION speaker_lock;
static int speaker_lock_initialized;
static int speaker_stop;
static NativeSpeakerRequest pending_request;

static DWORD WINAPI speaker_worker(LPVOID unused)
{
	(void)unused;
	for (;;) {
		NativeSpeakerRequest request;
		int should_stop;
		size_t i;
		size_t transitions = 0;
		double seconds;
		double frequency;
		DWORD duration;

		if (WaitForSingleObject(speaker_event, INFINITE) != WAIT_OBJECT_0) {
			fprintf(stderr, "Waiting for native speaker event failed: %lu\n",
				(unsigned long)GetLastError());
			return 1;
		}

		EnterCriticalSection(&speaker_lock);
		should_stop = speaker_stop;
		request = pending_request;
		pending_request.pcm = NULL;
		pending_request.num_samples = 0;
		pending_request.sample_rate = 0;
		LeaveCriticalSection(&speaker_lock);
		if (should_stop)
			return 0;
		if (!request.pcm || request.num_samples == 0 || request.sample_rate <= 0)
			continue;

		for (i = 1; i < request.num_samples; i++) {
			if ((request.pcm[i - 1] < 0) != (request.pcm[i] < 0))
				transitions++;
		}
		seconds = (double)request.num_samples / request.sample_rate;
		frequency = seconds > 0.0 ? transitions / (2.0 * seconds) : 440.0;
		if (frequency < 37.0)
			frequency = 37.0;
		if (frequency > 32767.0)
			frequency = 32767.0;

		duration = (DWORD)(seconds * 1000.0 + 0.5);
		if (duration == 0)
			duration = 1;
		if (!Beep((DWORD)(frequency + 0.5), duration))
			fprintf(stderr, "Windows Beep failed: %lu\n", (unsigned long)GetLastError());
	}
}

int native_speaker_open(void)
{
	if (speaker_thread)
		return 1;

	speaker_stop = 0;
	pending_request.pcm = NULL;
	pending_request.num_samples = 0;
	pending_request.sample_rate = 0;
	InitializeCriticalSection(&speaker_lock);
	speaker_lock_initialized = 1;
	speaker_event = CreateEvent(NULL, FALSE, FALSE, NULL);
	if (!speaker_event) {
		fprintf(stderr, "Could not create native speaker event: %lu\n",
			(unsigned long)GetLastError());
		DeleteCriticalSection(&speaker_lock);
		speaker_lock_initialized = 0;
		return 0;
	}
	speaker_thread = CreateThread(NULL, 0, speaker_worker, NULL, 0, NULL);
	if (!speaker_thread) {
		fprintf(stderr, "Could not create native speaker thread: %lu\n",
			(unsigned long)GetLastError());
		CloseHandle(speaker_event);
		speaker_event = NULL;
		DeleteCriticalSection(&speaker_lock);
		speaker_lock_initialized = 0;
		return 0;
	}
	return 1;
}

void native_speaker_play(const int16_t *pcm, size_t num_samples, int sample_rate)
{
	if (!speaker_thread || !pcm || num_samples == 0 || sample_rate <= 0)
		return;

	EnterCriticalSection(&speaker_lock);
	pending_request.pcm = pcm;
	pending_request.num_samples = num_samples;
	pending_request.sample_rate = sample_rate;
	LeaveCriticalSection(&speaker_lock);
	if (!SetEvent(speaker_event))
		fprintf(stderr, "Could not signal native speaker thread: %lu\n",
			(unsigned long)GetLastError());
}

void native_speaker_close(void)
{
	if (speaker_thread) {
		EnterCriticalSection(&speaker_lock);
		speaker_stop = 1;
		LeaveCriticalSection(&speaker_lock);
		if (!SetEvent(speaker_event))
			fprintf(stderr, "Could not stop native speaker thread: %lu\n",
				(unsigned long)GetLastError());
		WaitForSingleObject(speaker_thread, INFINITE);
		CloseHandle(speaker_thread);
		speaker_thread = NULL;
	}
	if (speaker_event) {
		CloseHandle(speaker_event);
		speaker_event = NULL;
	}
	if (speaker_lock_initialized) {
		DeleteCriticalSection(&speaker_lock);
		speaker_lock_initialized = 0;
	}
	speaker_stop = 0;
	pending_request.pcm = NULL;
	pending_request.num_samples = 0;
	pending_request.sample_rate = 0;
}

#else

int native_speaker_open(void)
{
	fprintf(stderr, "Native PC speaker mode is only available on Windows\n");
	return 0;
}

void native_speaker_play(const int16_t *pcm, size_t num_samples, int sample_rate)
{
	(void)pcm;
	(void)num_samples;
	(void)sample_rate;
}

void native_speaker_close(void)
{
}

#endif
