/* 
 * File:   main.cpp
 * Author: itcmcgrath
 *
 * Created on February 13, 2014, 12:09 PM
 */

#include <iostream>
#include <fstream>
#include <cstdlib>
#include "world.h"

/*
 * 
 */
int main(int argc, char** argv) {
  World *world = new World("world.xml");
  bool active = true, done;
  std::string input;

  std::cout << "You awake in: " << world->GetRoomName() << std::endl;
  std::cout << world->GetRoomDescription() << std::endl;


  while (active) {
    done = false;
    std::cin >> input;
    if (input.compare("quit") == 0)
      active = false;
    else {
      if (input.compare("look") == 0) {
        done = true;
        std::vector<std::string> directions = world->ListDirections();
        for (unsigned long int direction = 0; direction < directions.size(); ++direction) {
          std::cout << directions[direction] << std::endl;
        }
      }
      if (input.compare("search") == 0) {
        done = true;
        std::vector<std::string> items = world->ListRoomItems();
        for (unsigned long int index = 0; index < items.size(); ++index) {
          std::cout << items[index] << std::endl;
        }
      }
      if (input.compare("review") == 0) {
        done = true;
        std::vector<std::string> details = world->ListPlayerDetails();
        for (unsigned long int index = 0; index < details.size(); ++index) {
          std::cout << details[index] << std::endl;
        }
      }
      if (!done) {
        done = true;
        if (world->DoCommand(input)) {
          std::cout << "You are now in " << world->GetRoomName() << std::endl;
          std::cout << world->GetRoomDescription() << std::endl;

        } else
          std::cout << world->GetErrorReason() << std::endl;
      }
    }
  }

  return 0;
}

