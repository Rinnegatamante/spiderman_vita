#include <vitasdk.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "soloud.h"
#include "soloud_wavstream.h"

#define MAX_SOUNDS_NUM (128)
#define MAX_MUSICS_NUM (32)

typedef struct {
	int handle;
	SoLoud::WavStream source;
	bool valid;
} audio_instance;

SoLoud::Soloud soloud;
audio_instance snd[MAX_SOUNDS_NUM];
int curr_snd = 0;

audio_instance snd_loop[MAX_MUSICS_NUM];
int curr_snd_loop = 0;

extern "C" {

void audio_player_init() {
	soloud.init();
}

void audio_player_set_volume(void *m, float vol) {
	audio_instance *mus = (audio_instance *)m;
	mus->source.setVolume(vol);
	soloud.setVolume(mus->handle, vol);
}

void *audio_player_play(char *path, uint8_t loop, float vol) {
	if (loop) {
		//sceClibPrintf("Loading %s in music slot %d\n", path, curr_snd_loop);
		snd_loop[curr_snd_loop].valid = true;
		snd_loop[curr_snd_loop].source.load(path);
		snd_loop[curr_snd_loop].source.setVolume(vol);
		snd_loop[curr_snd_loop].source.setLooping(true);
		snd_loop[curr_snd_loop].source.setSingleInstance(true);
		snd_loop[curr_snd_loop].handle = soloud.playBackground(snd_loop[curr_snd_loop].source);
		void *r = (void *)&snd_loop[curr_snd_loop];
		curr_snd_loop = (curr_snd_loop + 1) % MAX_MUSICS_NUM;
		return r;
	} else {
		//sceClibPrintf("Loading %s in sound slot %d\n", path, curr_snd);
		snd[curr_snd].valid = true;
		snd[curr_snd].source.load(path);
		snd[curr_snd].source.setVolume(vol);
		snd[curr_snd].source.setLooping(false);
		snd[curr_snd].source.setSingleInstance(true);
		snd[curr_snd].handle = soloud.play(snd[curr_snd].source);
		void *r = (void *)&snd[curr_snd];
		curr_snd = (curr_snd + 1) % MAX_SOUNDS_NUM;
		return r;
	}
}

void audio_player_instance(void *m, uint8_t loop, float vol) {
	audio_instance *mus = (audio_instance *)m;
	if (loop) {
		mus->handle = soloud.playBackground(mus->source);
	} else {
		mus->handle = soloud.play(mus->source);
	}
	audio_player_set_volume(m, vol);
}

int audio_player_is_playing(void *m) {
	audio_instance *mus = (audio_instance *)m;
	return (soloud.isValidVoiceHandle(mus->handle) && !soloud.getPause(mus->handle));
}

void audio_player_stop(void *m) {
	audio_instance *mus = (audio_instance *)m;
	mus->source.stop();
	mus->valid = false;
}

void audio_player_set_pause(void *m, uint8_t val) {
	audio_instance *mus = (audio_instance *)m;
	soloud.setPause(mus->handle, val);
}

void audio_player_stop_all_sounds() {
	soloud.stopAll();
	for (int i = 0; i < MAX_MUSICS_NUM; i++) {
		snd_loop[i].valid = false;
	}
	for (int i = 0; i < MAX_SOUNDS_NUM; i++) {
		snd[i].valid = false;
	}
}

void audio_player_set_pause_all_sounds(uint8_t val) {
	soloud.setPauseAll(val);
}

void audio_player_change_bgm_volume(float vol) {
	for (int i = 0; i < MAX_MUSICS_NUM; i++) {
		if (snd_loop[i].valid) {
			audio_player_set_volume((void *)&snd_loop[i], vol);
		}
	}
}

void audio_player_change_sfx_volume(float vol) {
	for (int i = 0; i < MAX_SOUNDS_NUM; i++) {
		if (snd[i].valid) {
			audio_player_set_volume((void *)&snd[i], vol);
		}
	}
}
};