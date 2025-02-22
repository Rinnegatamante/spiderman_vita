#include <vitasdk.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_SOUNDS_NUM (128)
#define MAX_MUSICS_NUM (32)

#define USE_SDL_MIXER_EXT
//#define USE_SDL_MIXER
//#define USE_SOLOUD

#if defined(USE_SDL_MIXER_EXT) || defined(USE_SDL_MIXER)
#include <SDL2/SDL.h>
#ifdef USE_SDL_MIXER_EXT
#include <SDL2/SDL_mixer_ext.h>
#else
#include <SDL2/SDL_mixer.h>
#endif

#ifdef USE_SDL_MIXER
typedef struct {
	int handle;
	void *buffer;
	void *source;
	bool valid;
} audio_instance;

audio_instance snd[MAX_SOUNDS_NUM];
int curr_snd = 0;

audio_instance snd_loop[MAX_MUSICS_NUM];
int curr_snd_loop = 0;
#endif

extern "C" {

void audio_player_init() {
	SDL_Init(SDL_INIT_AUDIO);
#ifdef USE_SDL_MIXER_EXT
	Mix_Init(MIX_INIT_OGG);
#endif
	Mix_OpenAudio(44100, AUDIO_S16SYS, 2, 1024);
}

void audio_player_set_volume(void *m, float vol) {
#ifdef USE_SDL_MIXER
	audio_instance *mus = (audio_instance *)m;
	if (mus == &bgm && mus->valid) {
		Mix_VolumeMusic((int)(128.0f * vol));
	} else if (mus->valid) {
		Mix_VolumeChunk((Mix_Chunk *)mus->source, (int)(128.0f * vol));
		Mix_Volume(mus->handle, (int)(128.0f * vol));
	}
#else
	Mix_VolumeMusicStream((Mix_Music *)m, (int)(128.0f * vol));
#endif
}

void *audio_player_play(char *path, uint8_t loop, float vol) {
#ifdef USE_SDL_MIXER_EXT
	Mix_Music *ret = Mix_LoadMUS(path);
	Mix_VolumeMusicStream(ret, (int)(128.0f * vol));
	Mix_PlayMusicStream(ret, loop ? -1 : 0);
	return (void *)ret;
#else
	if (loop) {
		//sceClibPrintf("Loading %s in music slot %d\n", path, curr_snd_loop);
		Mix_Music *ret = Mix_LoadMUS(path);
		bgm.valid = true;
		bgm.source = (void *)ret;
		Mix_VolumeMusic((int)(128.0f * vol));
		Mix_PlayMusic(ret, -1);
		return &bgm;
	} else {
		//sceClibPrintf("Loading %s in sound slot %d\n", path, curr_snd);
		snd[curr_snd].valid = true;
		SceUID fd = sceIoOpen(path, SCE_O_RDONLY, 0777);
		size_t sz = sceIoLseek32(fd, 0, SCE_SEEK_END);
		snd[curr_snd].buffer = malloc(sz);
		sceIoLseek32(fd, 0, SCE_SEEK_SET);
		sceIoRead(fd, snd[curr_snd].buffer, sz);
		sceIoClose(fd);
		Mix_Chunk *_snd = Mix_LoadWAV_RW(SDL_RWFromMem(snd[curr_snd].buffer, sz), 1);
		snd[curr_snd].source = (void *)_snd;
		Mix_VolumeChunk(_snd, (int)(128.0f * vol));
		snd[curr_snd].handle = Mix_PlayChannel(-1, _snd, 0);
		void *r = (void *)&snd[curr_snd];
		curr_snd = (curr_snd + 1) % MAX_SOUNDS_NUM;
		return r;
	}
#endif
}

void audio_player_instance(void *m, uint8_t loop, float vol) {
#ifdef USE_SDL_MIXER
	audio_instance *mus = (audio_instance *)m;
	if (loop) {
		Mix_PlayMusic((Mix_Music *)mus->source, -1);
	} else {
		mus->handle = Mix_PlayChannel(-1, (Mix_Chunk *)mus->source, 0);
	}
	audio_player_set_volume(m, vol);
#else
	Mix_Music *mus = (Mix_Music *)m;
	Mix_VolumeMusicStream(mus, (int)(128.0f * vol));
	Mix_PlayMusicStream(mus, loop ? -1 : 0);
#endif
}

int audio_player_is_playing(void *m) {
#ifdef USE_SDL_MIXER
	audio_instance *mus = (audio_instance *)m;
	if (mus == &bgm) {
		return Mix_PlayingMusic();
	} else if (mus->valid) {
		return Mix_Playing(mus->handle);
	}
	return 0;
#else
	return Mix_PlayingMusicStream((Mix_Music *)m);
#endif
}

void audio_player_stop(void *m) {
#ifdef USE_SDL_MIXER
	audio_instance *mus = (audio_instance *)m;
	if (mus == &bgm && mus->valid) {
		Mix_HaltMusic();
		Mix_FreeMusic((Mix_Music *)mus->source);
	} else if (mus->valid) {
		Mix_HaltChannel(mus->handle);
		Mix_FreeChunk((Mix_Chunk *)mus->source);
		free(mus->buffer);
	}
	mus->valid = false;
#else
	Mix_Music *mus = (Mix_Music *)m;
	Mix_HaltMusicStream(mus);
	Mix_FreeMusic(mus);
#endif
}

void audio_player_set_pause(void *m, uint8_t val) {
#ifdef USE_SDL_MIXER
	audio_instance *mus = (audio_instance *)m;
	if (mus == &bgm && mus->valid) {
		val ? Mix_PauseMusic() : Mix_ResumeMusic();
	} else if (mus->valid) {
		val ? Mix_Pause(mus->handle) : Mix_Resume(mus->handle);
	}
#else
	val ? Mix_PauseMusicStream((Mix_Music *)m) : Mix_ResumeMusicStream((Mix_Music *)m);		
#endif
}

void audio_player_stop_all_sounds() {
#ifdef USE_SDL_MIXER
	audio_player_stop((void *)&bgm);
	for (int i = 0; i < MAX_SOUNDS_NUM; i++) {
		audio_player_stop((void *)&snd[i]);
	}
#endif
}

void audio_player_set_pause_all_sounds(uint8_t val) {
#ifdef USE_SDL_MIXER
	audio_player_set_pause((void *)&bgm, val);
	for (int i = 0; i < MAX_SOUNDS_NUM; i++) {
		audio_player_set_pause((void *)&snd[i], val);
	}
#else
	val ? Mix_PauseMusicStreamAll() : Mix_ResumeMusicStreamAll();
#endif
}

void audio_player_change_bgm_volume(float vol) {
#ifdef USE_SDL_MIXER
	audio_player_set_volume((void *)&bgm, vol);
#endif
}

void audio_player_change_sfx_volume(float vol) {
#ifdef USE_SDL_MIXER
	for (int i = 0; i < MAX_SOUNDS_NUM; i++) {
		audio_player_set_volume((void *)&snd[i], vol);
	}
#endif
}
};
#endif

#ifdef USE_SOLOUD
#include "soloud.h"
#include "soloud_wavstream.h"

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
#endif
