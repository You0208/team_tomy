#include "AudioManager.h"

namespace Lemur::Audio
{

    void AudioManager::load_audio()
    {
        {
            bgm[static_cast<int>(BGM::BOSS)] = std::make_unique<Lemur::Audio::audio>(xAudio2.Get(), L"./resources/Sound/BGM/Boss.wav");
            bgm[static_cast<int>(BGM::GAMBLE)] = std::make_unique<Lemur::Audio::audio>(xAudio2.Get(), L"./resources/Sound/BGM/Gambling.wav");
            bgm[static_cast<int>(BGM::CLEAR)] = std::make_unique<Lemur::Audio::audio>(xAudio2.Get(), L"./resources/Sound/BGM/GameClear.wav");
            bgm[static_cast<int>(BGM::OVER)] = std::make_unique<Lemur::Audio::audio>(xAudio2.Get(), L"./resources/Sound/BGM/GameOver.wav");
            bgm[static_cast<int>(BGM::PLAY)] = std::make_unique<Lemur::Audio::audio>(xAudio2.Get(), L"./resources/Sound/BGM/Play.wav");
            bgm[static_cast<int>(BGM::TITLE)] = std::make_unique<Lemur::Audio::audio>(xAudio2.Get(), L"./resources/Sound/BGM/Title.wav");

            se[static_cast<int>(SE::ARROW)] = std::make_unique<Lemur::Audio::audio>(xAudio2.Get(), L"./resources/Sound/SE/arrow_se.wav");
            se[static_cast<int>(SE::AVOID)] = std::make_unique<Lemur::Audio::audio>(xAudio2.Get(), L"./resources/Sound/SE/avoid.wav");
            se[static_cast<int>(SE::CONTER)] = std::make_unique<Lemur::Audio::audio>(xAudio2.Get(), L"./resources/Sound/SE/counter.wav");
            se[static_cast<int>(SE::DAMAGE)] = std::make_unique<Lemur::Audio::audio>(xAudio2.Get(), L"./resources/Sound/SE/damage.wav");
            se[static_cast<int>(SE::DECISION)] = std::make_unique<Lemur::Audio::audio>(xAudio2.Get(), L"./resources/Sound/SE/decision.wav");
            se[static_cast<int>(SE::HIT)] = std::make_unique<Lemur::Audio::audio>(xAudio2.Get(), L"./resources/Sound/SE/Hit.wav");
            se[static_cast<int>(SE::SENI)] = std::make_unique<Lemur::Audio::audio>(xAudio2.Get(), L"./resources/Sound/SE/Seni.wav");
            se[static_cast<int>(SE::SWING)] = std::make_unique<Lemur::Audio::audio>(xAudio2.Get(), L"./resources/Sound/SE/swing.wav");
        }
    
    }
    void AudioManager::play_bgm(const BGM& bgm_, const bool isloop)
    {
        bgm[static_cast<int>(bgm_)]->play(isloop);
    }

    void AudioManager::play_se(const SE& se_, const bool isLoop)
    {
        se[static_cast<int>(se_)]->play(isLoop);
    }

    void AudioManager::stop_BGM(const BGM& bgm_)
    {
        bgm[static_cast<int>(bgm_)]->stop();
    }

    void AudioManager::stop_SE(const SE& se_)
    {
        se[static_cast<int>(se_)]->stop();
    }

    void AudioManager::stop_AllBGM()
    {
        for (std::unique_ptr<Lemur::Audio::audio>& bgm_ : bgm)
        {
            bgm_->stop();
        }
    }

    void AudioManager::stop_AllSE()
    {
        for (std::unique_ptr<Lemur::Audio::audio>& se_ : bgm)
        {
            se_->stop();
        }
    }

    void AudioManager::stop_AllAudio()
    {
        for (std::unique_ptr<Lemur::Audio::audio>& bgm_ : bgm)
        {
            bgm_->stop();
        }
        for (std::unique_ptr<Lemur::Audio::audio>& se_ : bgm)
        {
            se_->stop();
        }
    }

    std::unique_ptr<Lemur::Audio::audio>& AudioManager::GetBGM(const BGM& bgm_)
    {
        return bgm[static_cast<int>(bgm_)];
    }

    std::unique_ptr<Lemur::Audio::audio>& AudioManager::GetSE(const SE& se_)
    {
        return se[static_cast<int>(se_)];
    }

}