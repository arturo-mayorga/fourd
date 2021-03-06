#pragma once

#include "../common/component.h"

namespace fd {

class InputHandler {
public:
  typedef std::vector<ComponentBus*> BusList;
  BusList m_inputTargets;
  //ComponentBus* m_inputTarget;

  struct JoystickBinding {
    std::string m_command;
    std::string m_releaseCommand;
    //std::string m_joystickType;
    float m_deadzone;
    bool m_isButton;
    bool m_spamRepeats;
    int m_buttonIndex;
    bool m_isInverted;
    bool m_isButtonContinuous;
    // sensitivity?
    // button press/repeat?
    JoystickBinding() : m_deadzone(0.2f), m_isButton(false), m_spamRepeats(false),
        m_buttonIndex(0), m_isInverted(false), m_isButtonContinuous(false) {}
  };
  typedef std::vector<JoystickBinding> JoyBindingList;
  JoyBindingList m_joyBindings;

  struct Joystick {
    std::string m_name;
    bool m_isPresent;

    typedef std::vector<unsigned char> ButtonList;
    typedef std::vector<float> AxisList;
    ButtonList m_buttons;
    ButtonList m_lastButtons;

    AxisList m_axes;

    Joystick() : m_isPresent(false) {}
  };
  typedef std::vector<Joystick> JoystickList;
  JoystickList m_joysticks;
  const int m_startExtraJoysticks; // non-glfw joysticks essentially

  InputHandler();

  Joystick& TickleExtraJoystick(int index); // don't keep this more than a frame, allocates on call potentially

  // weirdly, I feel like this is still under-abstracted,
  // like there should be yet another intermediate that holds state
  void AddDefaultBindings();
  void PollJoysticks();
  void ApplyJoystickInput(float frameTime);

  void DoCommand(const std::string& command, float frameTime);

  void AddInputTarget(ComponentBus* bus);

  void SendAnyInputSignal(ComponentBus* target);
  void SendDiscreteSignal(const std::string& signal, float frameTime);
  void SendContinuousInputSignal(const std::string& signal, float frameTime, float value);
};

} // namespace fd


// how to structure key input code?

// would be nice to have key binding support

// joysticks seem to be polling based, keys are event based
// key events should write to input state structure?
// joystick polling should write to input state structure?
// input state is looked at each frame, iterated over movement, then applied
//   

// use a component bus pointer for input targets
// so input handler lives on main,
// but the existing key handler takes input and still does the easy keybindings
// input handler has mappings for movement keys, up/down state
