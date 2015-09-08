# Configuring input #
An InputManager must have a key configuration in order to work properly. Sonetto will attempt to automatically load such configurations from a configuration file when it's time to create this Manager. But if you want to create it manually, you will need to supply InputManager::initialise() with an Ogre::RenderWindow and an array of KeyConfig structs. The RenderWindow is needed because the InputManager needs to listen only to inputs sent to the game window, and not simply start to globally detect key strikes and button presses. The KeyConfig array describes the joystick configurations and key mappings of each player's input. The array size must be the maximum number of players accepted by the engine, which is originally four, but you may change this if you want so. Changing of the maximum number of players is a little hard for beginners, as it needs Sonetto to be recompiled, but this will be more carefuly explained later. The key configuration includes the input device, whether to enable the controller's rumble, the number of present(or emulated) analog inputs, and general key mappings. The input device describes where to get player's input. Any input device may access the keyboard keys(if you find your controller doesn't have sufficient buttons for the game), but the first input device(device 0) exclusively uses the keyboard. You may have players sharing the same input devices, and that makes sense sometimes, with keyboards. It most likely won't make much sense with joysticks, but it's not prohibited. Input devices greater than 0 stands for the joystick devices. For example, if you want player 0 to use a nice, enhanced joystick, which is the third joystick plugged in your PC, and player 1(your "friend") to use that screwed old crappy simple keyboard, you would configure player 0 to input device 3, and player 1 to input device 0. Simple as that.
After the InputManager being initialised, you may still change the key configurations through InputManager::getKeyConfig() and InputManager::setKeyConfig() methods. Whether the supplied input device is or isn't plugged in can be checked by means of InputManager::playerPlugged().

# Accessing of actual input #
Well, enough preparations, we are in the middle of the game and want to know the states of our just-configured buttons. That's an easy task. From anywhere in your code, you may access the Kernel using the singleton pointer, and from there on access the InputManager. Here's a small example showing some simple input accesses:
```
// The namespace MyGame is ficticious, and used only for example purposes

MyGame::Enemy          enemy;  // Big Boss
MyGame::Protagonist    me;     // Game Protagonist
MyGame::World          world;  // The Game Environment

Sonetto::InputManager *inputMan = Sonetto::Kernel::getSingleton()->getInputMan();

if(inputMan->getButtonState(0,Sonetto::BTN_CROSS) == Sonetto::KS_RELEASE) {
  // Cast a magic spell if enough focused
  if(me.mentalFocus > 32) {
    enemy.castSpell(me.activeSpell,me.mentalFocus);
    me.mentalFocus = 0;
  }
}

if(inputMan->getButtonState(0,Sonetto::BTN_CROSS) == Sonetto::KS_HOLD) {
  // Charge mental focus while Cross is being held
  me.mentalFocus++;
}

// Walk based on the left analog stick
me.walk(inputMan->getAxis(0,AX_LEFT));

// Rotate the camera based on the right one
world.cameraRotate(inputMan->getAxis(0,AX_RIGHT));
```

Ok, here you see many "ficticious ornaments", but what matters is: You may access the controller input from everywhere, and the InputManager keeps track of the button states for you, so you may not only rely on information about whether a button is or not currently pressed, but also know its transitions(pressing and releasing), which will keep you knowning about what really matters. Also, you may have noticed the InputManager::getAxis() calls. These returns an Ogre::Vector2, with values ranging from -1.0f to +1.0f, even when you had configured the axis' input to be taken from raw keyboard keys or even digital joystick buttons. The precision of these values, however, depends on the quality of the input device you've choosen to use.