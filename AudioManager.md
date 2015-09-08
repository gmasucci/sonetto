# Introduction #
Sonetto has an audio manager that is responsible for managing audio resources and abstracting the underlying audio libraries used to decompress and play audio data.

# Audio Concepts #
This audio manager has three audio concepts: Audio Listeners, Musics and Sounds. Each of these concepts will be explained before we get into the "how to use" part of this wiki page.

## Audio Listeners ##
An Audio Listener, for those not used to 3D audio, is a point in 3D space used to calculate each audio source's 2D channel panning. This panning is used by our brain to figure from where the sound is coming. They can be attached to events or to the current camera.

## Musics ##
Musics are divided into two types: Background Musics and Music Effects.

Background Musics are musics that loop indefinitely until you want them to stop. They have a loop point from which the music will start playing when the stream ends. This is used to make seamless looping possible without letting go of music introductions, which are needed to make it easier to loop and still make the music sound good. Background Musics can fade in and out when you ask them to start and stop playing.

Music Effects, contrary to Background Musics, don't use the music loop point and never fade. They are played only once. If there is a background music playing when a Music Effect is asked to play, this background music will be faded out and faded back in from the point it stopped when the Music Effect ends.

It is important to note that fading is always optional. If you supply a fading speed of zero to any method that uses fading, the proposed changes will be taken instantly, without fading. Also keep in mind that musics are always following the audio listener, so they seem like they don't have a position in space.

From here on, we will use the term BGM to refer to Background Musics and ME to Music Effects.

## Sounds ##
Sounds are short pieces of audio data, such as attack, door/chest opening, footstep sounds, etc. They are often called _Sound Effects_ (or SEs), and they're generally loaded and unloaded upon module and map changing. A _sound effect_, in Sonetto, refers only to the audio data, and not to its instantiation. Therefore, SEs do not have a position in 3D space until a _sound source_ with its ID is created. These sound sources can be attached to Ogre::Nodes, so that their positions are inherited from them. This way you can attach sound sources to your Ogre::SceneNodes or even to your meshes' bones, if needed. By default, sound sources are attached to the audio listener (i.e. they will sound the same everywhere, like musics).

Various sound sources can use the same sound effect ID, in a way that the same resource (audio data) can be reused without doubling memory usage.

# Using the AudioManager #
The AudioManager can be accessed through `Kernel::get()->getAudioMan()`, and the MusicStream can be accessed through `Kernel::get()->getAudioMan()->getMusicStream()`.

## Changing settings ##
You can attach the audio listener to an Ogre::Node by using `void AudioManager::setListenerNode(Ogre::Node *node)`, and retrieve this value using `Ogre::Node *AudioManager::getListenerNode()`. The supplied `node` can be NULL, in which case the listener will inherit the positions of the current module's camera.

You can configure the master music and sound volume using:
```
void AudioManager::setMasterMusicVolume(float volume);
float AudioManager::getMasterMusicVolume() const;
void AudioManager::setMasterSoundVolume(float volume);
float AudioManager::getMasterSoundVolume() const;
```

Beware that changing these values manually during gameplay is highly discouraged. They are chosen by the user in the configuration file, and _must_ be respected. They are the absolute maximum volume the music and the sounds will reach.

There is another maximum volume, however, that is not user-configurable, and is intended to be changed internally by the game. This maximum volume of the musics can be accessed through:
```
void MusicStream::setMaxVolume(float vol);
float MusicStream::getMaxVolume() const;
```

And the maximum volume of the sounds is local. That is, there is a maximum volume for each sound source you create. They can be accessed through:
```
void SoundSource::setMaxVolume(float maxVolume);
float SoundSource::getMaxVolume() const;
```

Of course, these maximum volumes are subject to their respective master volumes.

## Playing musics ##
The music IDs are indexes inside the Database.

BGMs can be played using `void AudioManager::playBGM(size_t id,float aFadeOut,float aFadeIn)`. The `aFadeOut` argument determines the speed in which the current BGM should be faded out before starting the desired music, and the `aFadeIn` argument determines the speed in which the desired BGM will fade in.

MEs can be played using `void AudioManager::playME(size_t id,float aFadeOut,float aFadeIn)`. Both fading values refer to the current BGM fading out and in, before the ME starts playing and after stopping, respectively.

Whatever music type is currently playing, there are a few methods useful to control them:
```
// The fading arguments only apply when a BGM is currently playing
// These methods should be self-explanatory
void AudioManager::stopMusic(float fadeOut);
void AudioManager::pauseMusic(float fadeOut);
void AudioManager::resumeMusic(float fadeIn);
```

That's all about music playing.

## Playing sounds ##
The sound IDs are indexes inside the Database.

Sounds need to be loaded prior to creating a sound source that uses its audio data. They can be loaded using `void AudioManager::loadSound(size_t id);` and disposed using `void AudioManager::unloadSound(size_t id);`.

When a sound is loaded, a sound source using it can be created by using one of the following methods:
```
SoundSourcePtr AudioManager::createSound(size_t id,Ogre::Node *node = NULL);
void AudioManager::playSound(size_t id,float aMaxVolume = 1.0f,Ogre::Node *node = NULL);
```

The second method is the easiest one. It will create a sound source, set its maximum volume and node, and then start playing it. The sound source will play the sound once and then will be deleted. This is useful for things like menu sound playing, where you don't need much control over the source. The first method will just create the sound source and return a pointer to it. This is useful if you want more control over the sound source (you can play it as many times as you want, stop it, and pause it). When it is not needed anymore, you can simply let the returned SoundSourcePtr run out of scope. The AudioManager will take care of destroying it when it stops playing.