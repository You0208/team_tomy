#pragma once
// XAUDIO2

#include <xaudio2.h>
#include <mmreg.h>

#include "misc.h"

namespace Lemur::Audio
{
	class audio
	{
		WAVEFORMATEXTENSIBLE wfx = { 0 };
		XAUDIO2_BUFFER buffer = { 0 };

		IXAudio2SourceVoice* source_voice;

	public:
		audio(IXAudio2* xaudio2, const wchar_t* filename);
		virtual ~audio();
		void play(int loop_count = 0/*255 : XAUDIO2_LOOP_INFINITE*/);
		void stop(bool play_tails = true, size_t after_samples_played = 0);
		void volume(float volume);
		bool queuing();
	};

}