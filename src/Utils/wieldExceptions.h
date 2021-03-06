#ifndef WIELD_EXCEPTION_H
#define WIELD_EXCEPTION_H
#include <stdexcept>
#include <sstream>
#include <string>
#include <iostream>

#include "Utils/wieldColor.h"

#define WIELD_EXCEPTION(MESSAGE)					\
  {									\
    std::cout << WIELD_COLOR_B_ON;						\
    std::cout << "Exception " << WIELD_COLOR_FG_GREEN << "caught";		\
    std::cout << WIELD_COLOR_FG_DEFAULT << ": " << MESSAGE << std::endl ;	\
    std::cout << "\t" << WIELD_COLOR_B_ON << WIELD_COLOR_FG_BLUE << __FILE__;	\
    std::cout << WIELD_COLOR_FG_DEFAULT << ":";				\
    std::cout << WIELD_COLOR_FG_BLUE << __LINE__ <<WIELD_COLOR_FG_DEFAULT << std::endl; \
    std::cout << "\t" << WIELD_COLOR_FG_YELLOW << __func__;		\
    std::cout << WIELD_COLOR_RESET << std::endl;				\
  }
#define WIELD_EXCEPTION_NEW(MESSAGE)					\
  {									\
    std::cout << std::endl;						\
    std::cout << WIELD_COLOR_B_ON << "Exception " <<WIELD_COLOR_FG_RED<< "thrown"; \
    std::cout << WIELD_COLOR_FG_DEFAULT << ": " << MESSAGE << std::endl ; \
    std::cout << "\t" << WIELD_COLOR_B_ON << WIELD_COLOR_FG_BLUE<<__FILE__; \
    std::cout << WIELD_COLOR_FG_DEFAULT << ":";				\
    std::cout << WIELD_COLOR_FG_BLUE <<__LINE__ <<WIELD_COLOR_FG_DEFAULT << std::endl; \
    std::cout << "\t" << WIELD_COLOR_FG_YELLOW << __func__;		\
    std::cout << WIELD_COLOR_RESET<<std::endl;				\
    std::stringstream ss; ss << MESSAGE; throw std::runtime_error(ss.str()); \
  }

#define WIELD_EXCEPTION_TRY try {
#define WIELD_EXCEPTION_CATCH } catch (std::exception &e) {WIELD_EXCEPTION("");throw;} catch (int i) {WIELD_EXCEPTION("");throw;}
#define WIELD_EXCEPTION_CATCH_MSG(MESSAGE) } catch (std::exception &e) {WIELD_EXCEPTION(MESSAGE);throw;} catch (int i) {WIELD_EXCEPTION(MESSAGE);throw;}
#define WIELD_EXCEPTION_CATCH_FINAL }					\
    catch (std::exception &e) {WIELD_EXCEPTION("");std::cout << "\tWhat: " << e.what() << std::endl;} \
    catch (std::runtime_error *e) {WIELD_EXCEPTION("");std::cout << "\tWhat: " << e->what() << std::endl;} \
    catch (int i) {WIELD_EXCEPTION(""); std::cout << "\tError code = " << i << std::endl;}

#endif // WIELD_EXCEPTION_H
