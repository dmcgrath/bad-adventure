/* 
 * File:   world.h
 * Author: itcmcgrath
 *
 * Created on February 16, 2014, 10:21 PM
 */

#ifndef WORLD_H
#define	WORLD_H

#include <stdlib.h>
#include <string>
#include <sstream>
#include <vector>
#include <map>
#include "rapidxml_utils.hpp"

class Direction {
public:

  Direction();
  Direction(char direction);
  Direction(char direction, std::string description);
  virtual ~Direction();

  void SetDescription(std::string description);
  std::string GetDescription() const;
  void SetDirection(char direction);
  char GetDirection() const;

private:
  char direction;
  std::string description;
};

class Room {
public:
  Room();
  Room(std::string name, std::string description, int id);
  virtual ~Room();

  void SetDescription(std::string description);
  void SetName(std::string name);
  void link(char direction, Room &room);
  void link(Direction &direction, Room &room);

  std::string GetDescription() const;
  std::string GetName() const;
  Room* GetDirection(char direction);

  bool CheckMove(char direction);
  std::vector<std::string> ListDirections();

  void SetId(int id);
  int GetId() const;

  std::string GetErrorReason() const;
private:
  int id;
  std::map<char, Room*> links;
  std::map<char, Direction*> directions;
  std::string name, description, error_reason;
};

class Player {
public:
  Player();
  virtual ~Player();
  void SetStrength(int strength);
  int GetStrength() const;
  void SetHealth(int health);
  int GetHealth() const;
private:
  int health, strength;
};

class World {
public:
  World();
  World(const char *filename);
  World(const World& orig);
  virtual ~World();

  std::string GetRoomName() const;
  std::string GetRoomDescription() const;
  std::vector<std::string> ListDirections();

  bool MoveRoom(char direction);
  bool DoCommand(std::string command);

  std::string GetErrorReason() const;
private:
  rapidxml::file<> *world_file;
  rapidxml::xml_document<> world_doc;
  void add_room(rapidxml::xml_node<> *room_node);
  void add_link(rapidxml::xml_node<> *link_node);
  std::vector<std::string> split(const std::string &s, char delim);

  std::map<int, Room*> rooms;
  Room *current_room;
  Player *player;

  std::string error_reason;
};

#endif	/* WORLD_H */

