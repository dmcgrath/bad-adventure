/* 
 * File:   world.h
 * Author: itcmcgrath
 *
 * Created on February 16, 2014, 10:21 PM
 * 
 * Contained are the various classes required to represent the complete world
 * in Bad Adventure.
 * 
 * Class Direction
 * 
 * A simple class that represents a possible direction that a player can move
 * from one room to another. It stores both the char that the movement system
 * uses to identify it as well as a description to be displayed.
 * 
 * TODO: Add a lock system that optionally requires the player to possess a
 *       specific item to move in that direction. Add this after Items are
 *       implemented.
 * 
 * Class Item
 * 
 * Item stores attributes for a single item in the game.
 * 
 * Class Room
 * 
 * This class stores all the aspects that comprise a room in Bad Adventure.
 * 
 * This includes name, description and possible directions to move to other
 * rooms.
 * 
 * The Room class handles verification of the ability to move in a certain
 * direction.
 * 
 * IMPORTANT: The RemoveItem() method transfers ownership of the item being
 * removed to whatever called the method. This means the callee must handle
 * deleting the memory that item once it has finished with it.
 * 
 * Class Player
 * 
 * Player keeps track of player specific information such as health.
 * 
 * Since this in not multi-player, it doesn't store the current room. Should
 * this change, the current room handling should be moved from the World class
 * into the Player Class.
 * 
 * TODO: Keep track of items that have been picked up. Add this after
 *       the Item class
 * 
 * Class World
 * 
 * Keep track of the entire world and handle all interactions with it.
 * 
 * The World class is also responsible for reading in XML configuration data.
 */

#ifndef WORLD_H
#define	WORLD_H

#include <stdlib.h>
#include <string>
#include <sstream>
#include <vector>
#include <map>
#include "rapidxml_utils.hpp"

/*
 * See above for a description of the Direction class
 * 
 * Note: Several directions (n/s/e/w/u/p) will pre-populate the description
 *       property when using the Direction(char) constructor. See world.cc
 *       for details.
 */
class Direction {
public:

  Direction();
  Direction(char direction);
  Direction(Direction &direction);
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

/*
 * 
 * 
 * 
 * 
 */
class Item {
public:
  Item();
  Item(std::string name, std::string description, int health, int strength, int secret);
  virtual ~Item();

  void SetSecret(int secret);
  int GetSecret() const;
  void SetStrength(int strength);
  int GetStrength() const;
  void SetHealth(int health);
  int GetHealth() const;
  void SetDescription(std::string description);
  std::string GetDescription() const;
  void SetName(std::string name);
  std::string GetName() const;
private:
  std::string name, description;
  int health, strength, secret;
};

/*
 * See above for a description of the Room class
 * 
 * Note: The Room() constructor will populate name & description with generic
 *       empty room values.
 * 
 * ListDirection returns a string for each direction in the format of:
 * "(direction_char) description"
 * 
 * ListItems returns a string for each item in the format of:
 * "(number) description"
 * 
 * GetDirection & CheckMove will set an error string on when the request fails.
 * It can be retrieved with GetErrorReason
 */
class Room {
public:
  Room();
  Room(std::string name, std::string description, int id);
  virtual ~Room();

  void SetDescription(std::string description);
  void SetName(std::string name);
  void LinkRooms(Direction &dir, Room &room);

  std::string GetDescription() const;
  std::string GetName() const;
  Room* GetDirection(char direction);

  bool CheckMove(char direction);
  std::vector<std::string> ListDirections();

  void AddItem(Item &item);
  Item *ViewItem(unsigned long id);
  Item *RetrieveItem(unsigned long id);
  std::vector<std::string> ListItems();
  
  
  void SetId(int id);
  int GetId() const;

  std::string GetErrorReason() const;
private:
  int id;
  std::map<char, Room*> links;
  std::map<char, Direction*> directions;
  std::vector<Item*> items;
  std::string name, description, error_reason;
};

/*
 * See above for a description of the Player class
 */
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
  std::vector<Item*> items;
};

/*
 * See above for a description of the World class
 * 
 * Note: The World() constructor contains minimal dummy world data
 *       initialization. World(cons char*) will parse an XML file to initialize
 *       the world and is generally how you would initialize it.
 * 
 *       If MoveRoom or DoCommand fails, an error reason string can be retrieved
 *       with the GetErrorReason method.
 */
class World {
public:
  World();
  World(const char *filename);
  World(const World& orig);
  virtual ~World();

  std::string GetRoomName() const;
  std::string GetRoomDescription() const;
  std::vector<std::string> ListDirections();
  std::vector<std::string> ListRoomItems();

  bool MoveRoom(char direction);
  bool DoCommand(std::string command);

  std::string GetErrorReason() const;
private:
  rapidxml::file<> *world_file;
  rapidxml::xml_document<> world_doc;
  void AddRoom(rapidxml::xml_node<> *room_node);
  void AddLink(rapidxml::xml_node<> *link_node);
  void AddItem(rapidxml::xml_node<> *item_node);
  
  std::vector<std::string> split(const std::string &s, char delim);

  std::map<int, Room*> rooms;
  Room *current_room;
  Player *player;

  std::string error_reason;
};

#endif	/* WORLD_H */

