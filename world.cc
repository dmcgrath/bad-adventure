/* 
 * File:   world.cc
 * Author: itcmcgrath
 * 
 * Created on February 16, 2014, 10:21 PM
 * 
 * A quick overview of all the classes can be found in the header file.
 * 
 */

#include <string.h>

#include "world.h"

Direction::Direction() {
}

Direction::Direction(char direction) {
  this->direction = direction;
  switch (direction) {
    case 'n':
      description = "A path to the North";
      break;
    case 's':
      description = "A path to the South";
      break;
    case 'e':
      description = "A path to the East";
      break;
    case 'w':
      description = "A path to the West";
      break;
    case 'u':
      description = "A ladder headed up";
      break;
    case 'd':
      description = "A man-sized hole in the ground";
  }
}

Direction::Direction(char direction, std::string description) {
  this->direction = direction;
  this->description = description;
}

Direction::~Direction() {

}

void Direction::SetDescription(std::string description) {
  this->description = description;
}

std::string Direction::GetDescription() const {
  return description;
}

void Direction::SetDirection(char direction) {
  this->direction = direction;
}

char Direction::GetDirection() const {
  return direction;
}

Room::Room() {
  name = "Empty Room";
  description = "Just an empty, uninitialized room.";
}

Room::Room(std::string name, std::string description, int id) {
  this->name = name;
  this->description = description;
  this->id = id;
}

void Room::SetDescription(std::string description) {
  this->description = description;
}

std::string Room::GetDescription() const {
  return description;
}

void Room::SetName(std::string name) {
  this->name = name;
}

std::string Room::GetName() const {
  return name;
}

Room* Room::GetDirection(char direction) {
  if (links.find(direction) == links.end()) {
    error_reason = "Hmm, there doesn't appear to be a path in that direction.";
    return NULL;
  } else
    return links[direction];
}

void Room::SetId(int id) {
  this->id = id;
}

int Room::GetId() const {
  return id;
}

bool Room::CheckMove(char direction) {
  error_reason = "";
  if (links.find(direction) == links.end()) {
    error_reason = "There is no path in that direction";
    return false;
  }
  return true;
}

void Room::link(char direction, Room &room) {
  std::map<char, Direction*>::iterator it = directions.find(direction);
  if (it == directions.end())
    directions[direction] = new Direction(direction);

  links[direction] = &room;
}

void Room::link(char direction, std::string description, Room &room) {
  std::map<char, Direction*>::iterator it = directions.find(direction);
  if (it == directions.end())
    directions[direction] = new Direction(direction, description);

  links[direction] = &room;
}

/* Briefly described in the header file. */
std::vector<std::string> Room::ListDirections() {
  std::vector<std::string> list_directions;
  for (std::map<char, Direction*>::iterator it = directions.begin(); it != directions.end(); it++) {
    std::stringstream temp;
    temp << "(" << it->first << ") " << it->second->GetDescription();
    list_directions.push_back(temp.str());
  }
  return list_directions;
}

Room::~Room() {
  for (std::map<char, Direction*>::iterator it = directions.begin(); it != directions.end(); it++) {
    delete it->second;
  }
}

Player::Player() {
  health = 1000;
  strength = 10;
}

Player::~Player() {

}

std::string Room::GetErrorReason() const {
  return error_reason;
}

void Player::SetStrength(int strength) {
  this->strength = strength;
}

int Player::GetStrength() const {
  return strength;
}

void Player::SetHealth(int health) {
  this->health = health;
}

int Player::GetHealth() const {
  return health;
}

/* 
 * This default constructor will set up a very basic dummy world.
 * Generally, you will want to use the World(const char*) constructor as it
 * will parse an XML file to generate the world data.
 */
World::World() {
  rooms[0] = new Room("Spawn Room", "Home, sweet, home!", 0);
  rooms[1] = new Room("North Chamber", "Very northern", 1);
  rooms[2] = new Room("South Chamber", "A chamber that is decidedly south of the Spawn Room", 2);

  rooms[0]->link('n', *rooms[1]);
  rooms[1]->link('s', *rooms[0]);
  rooms[0]->link('s', *rooms[2]);
  rooms[2]->link('n', *rooms[0]);

  current_room = rooms[0];
  player = new Player();
}

/* 
 * Generate the world from an XML configuration file. See world.xml for an
 * example.
 */
World::World(const char* filename) {
  world_file = new rapidxml::file<>(filename);
  world_doc.parse<0>(world_file->data());

  rapidxml::xml_node<> *room_node, *link_node;
  rapidxml::xml_node<> *base_node = world_doc.first_node("world");
  base_node = base_node->first_node("rooms");

  for (room_node = base_node->first_node("room"); room_node; room_node = room_node->next_sibling("room")) {
    add_room(room_node);
  }

  base_node = base_node->parent();
  base_node = base_node->first_node("links");
  for (link_node = base_node->first_node("link"); link_node; link_node = link_node->next_sibling("link")) {
    add_link(link_node);
  }

  current_room = rooms[0];
  player = new Player();
}

/*
 * A private method used to add a room from an XML definition.
 */
void World::add_room(rapidxml::xml_node<> *room_node) {
  int id = atoi(room_node->first_attribute("id")->value());
  rooms[id] = new Room(room_node->first_attribute("name")->value(),
          room_node->first_attribute("desc")->value(),
          id);
}

/*
 * A private method used to add a link between rooms from an XML definition.
 */
void World::add_link(rapidxml::xml_node<> *link_node) {
  int from_id, to_id;
  rapidxml::xml_attribute<> *desc = NULL;

  from_id = atoi(link_node->first_attribute("from")->value());
  to_id = atoi(link_node->first_attribute("to")->value());

  desc = link_node->first_attribute("desc");
  if(desc == NULL) {
    rooms[from_id]->link(link_node->first_attribute("dir")->value()[0], *rooms[to_id]);
  }
  else {
    rooms[from_id]->link(link_node->first_attribute("dir")->value()[0], desc->value(), *rooms[to_id]);
  }
    
}

World::World(const World & orig) {
}

World::~World() {
  for (std::map<int, Room*>::iterator it = rooms.begin(); it != rooms.end(); it++) {
    delete it->second;
  }
  delete player;
  delete world_file;
}

std::string World::GetRoomName() const {
  return current_room->GetName();
}

std::string World::GetRoomDescription() const {
  return current_room->GetDescription();
}

bool World::MoveRoom(char direction) {
  if (!current_room->CheckMove(direction)) {
    error_reason = current_room->GetErrorReason();
    return false;
  }
  error_reason = "";

  current_room = current_room->GetDirection(direction);
  return true;
}

/*
 * Move commands are inputted by the user in the form of:
 * m:<direction> where <direction> is the char to denote a direction. E.g.:
 *    m:n
 */
bool World::DoCommand(std::string command) {
  std::vector<std::string> elems = split(command, ';');

  if (elems[0].compare("m") == 0)
    return MoveRoom(elems[1][0]);
}

/*
 * Used by DoCommand to separate out the command type from the command parameter
 */
std::vector<std::string> World::split(const std::string &s, char delim) {
  std::vector<std::string> elems;
  std::stringstream ss(s);
  std::string item;
  while (std::getline(ss, item, delim)) {
    elems.push_back(item);
  }
  return elems;
}

std::string World::GetErrorReason() const {
  return error_reason;
}

std::vector<std::string> World::ListDirections() {
  return current_room->ListDirections();
}
