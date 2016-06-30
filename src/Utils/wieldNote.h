#ifndef WIELD_NOTE_H
#define WIELD_NOTE_H

#include "Utils/wieldColor.h"

#define WIELD_NOTE(MESSAGE)						\
  {									\
    std::cout << WIELD_COLOR_B_ON;					\
    std::cout << WIELD_COLOR_FG_BLUE;					\
    std::cout << "Note at " << __FILE__ << ":" << __LINE__ << ": ";	\
    std::cout << WIELD_COLOR_RESET;					\
    std::cout << MESSAGE;						\
    std::cout << std::endl;						\
  }

#endif // WIELD_NOTE_H
