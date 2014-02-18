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
  bool active = true;
  std::string input;

  std::cout << "You awake in: " << world->GetRoomName() << std::endl;
  std::cout << world->GetRoomDescription() << std::endl;


  while (active) {
    std::cin >> input;
    if (input.compare("quit") == 0)
      active = false;
    else {
      if (input.compare("look") == 0) {
        std::vector<std::string> directions = world->ListDirections();
        for (unsigned long int direction = 0; direction < directions.size(); direction++) {
          std::cout << directions[direction] << std::endl;
        }
      } else {
        if (world->DoCommand(input)) {
          std::cout << "You are now in: " << world->GetRoomName() << std::endl;
          std::cout << world->GetRoomDescription() << std::endl;

        } else
          std::cout << world->GetErrorReason() << std::endl;
      }
    }
  }

  return 0;
}

